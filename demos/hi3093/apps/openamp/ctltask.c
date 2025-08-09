#include <math.h>
#include "test.h"
#include "prt_task.h"
#include "rpmsg_backend.h"
#include "stdio.h"
#include "hi_spi.h"
#include "prt_timer.h"
#include "prt_sem.h"
#include "ctltask.h"
#include "gpio_def.h"
#include "rpmsg_protocol.h"

volatile double ref_signal = 0;
volatile double err_signal = 0;
static U32 spi_timer_id;
static SemHandle taskstart_sem;
static int timer_interval = 500;
uint8_t AD7606_data1[2] = {0x0}; // Buffer to hold AD7606 data
uint8_t AD7606_data2[2] = {0x0}; // Buffer to hold AD7606 data

static bool state_idle_flag = 0; // 空闲状态
static bool state_excitation_flag = 0; // 激励状态
static bool state_control_flag = 0;

extern TskHandle g_testTskHandle;
extern TskHandle g_CtlTskHandel;
extern double *InputArray;

void Timer_Init(void);
void UART_Init(int bard_rate);

static void Timer_Func(TimerHandle tmrHandle, U32 arg1, U32 arg2, U32 arg3, U32 arg4)
{
    PRT_SemPost(taskstart_sem);
    return;
}

static double next_sine_sample_static(void)
{
    static double phase = 0.0;
    double value = sin(phase);
    phase += 2 * PI * CONT_Hz / timer_interval;
    while (phase >= 2 * PI)
    {
        phase -= 2 * PI;
    }
    return value;
}

void Timer_Init(void)
{
    struct TimerCreatePara spi_timer = {0};
    spi_timer.type = OS_TIMER_SOFTWARE;
    spi_timer.mode = OS_TIMER_LOOP;
    spi_timer.interval = 500;
    spi_timer.timerGroupId = 0;
    spi_timer.callBackFunc = Timer_Func;
    spi_timer.arg1 = 0;

    PRT_SemCreate(0, &taskstart_sem);
    PRT_TimerCreate(&spi_timer, &spi_timer_id);
    PRT_TimerStart(0, spi_timer_id);
}

void ControlTaskEntry()
{
    int i = 0;
    int ret;
    U8 txbuff[4] = {0x00, 0x55, 0xAA, 0xFF};
    U8 rxbuff[4] = {0};
    U8 dummy[4] = {0x11, 0x22, 0x33, 0x44};
    U8 tx_buff[BUFF_LEN];
    int16_t AD_CH0, AD_CH1;
    double output = 0;
    double votlage0 = 0;
    double votlage1 = 0;
    double tmp_vol = 0;

    Timer_Init();
    FilterInit();
    SPI0_Init();
    AD7606_Init();
    DAC8563_Init();

    while (1)
    {
        PRT_SemPend(taskstart_sem, OS_WAIT_FOREVER);
        if (state_idle_flag)
        {
            if (state_excitation_flag)
            {
                static double i = 0;
                i += 0.1;
                tmp_vol = next_sine_sample_static();
                DAC8563_SetVoltage(EXCITATION_CHANNEL, i);
            }

            AD7606_read_channel_1(AD7606_data1);
            AD_CH0 = (int16_t)((AD7606_data1[0] << 8) | AD7606_data1[1]);
            votlage0 = (double)AD_CH0 * 10 / 0x7fff;

            if (state_control_flag)
            {
                output = output_get(votlage0);
                DAC8563_SetVoltage(CONTROL_CHANNEL, output + 5.0);

                AD7606_read_channel_2(AD7606_data2);
                AD_CH1 = (int16_t)((AD7606_data2[0] << 8) | AD7606_data2[1]);
                votlage1 = (double)AD_CH1 * 10 / 0x7fff;
                W_update(votlage1);
            }
        }
    }
}

int rec_msg_proc(void *data, int len)
{
    static int array[200];
    rpmsg_packet *packet = (rpmsg_packet *)data;

    // 解析数据包（注意边界检查）
    // 1. 检查最小长度（消息类型）
    if (len < sizeof(uint16_t))
    {
        PRT_Printf("Invalid packet: too short");
        return -1;
    }
    // 2. 解析消息类型
    switch (packet->msg_type)
    {
    case MSG_COMMAND:
        if (len = sizeof(uint16_t) + sizeof(uint16_t))
        {
            switch (packet->payload.command)
            {
            case START_EXCITATION:
                PRT_Printf("Received command: START_EXCITATION\n");
                state_excitation_flag = 1; // 开始激励
                break;
            case STOP_EXCITATION:
                PRT_Printf("Received command: STOP_EXCITATION\n");
                state_excitation_flag = 0; // 停止激励
                break;
            case START_DAMPING:
                PRT_Printf("Sending array.\n", packet->payload.command);

                rpmsg_packet pkt = {
                    .msg_type = MSG_SENSOR_ARRAY};
                for (uint16_t i = 0; i < SENSOR_ARRAY_SIZE; i++)
                {
                    pkt.payload.array[i] = i * 2 + 3; // 示例数据
                    if ((pkt.payload.array[i] & 0xFF00) == 0x0A00)
                    {
                        pkt.payload.array[i] = i - 1;
                    }
                    if ((pkt.payload.array[i] & 0x00FF) == 0x000A)
                    {
                        pkt.payload.array[i] = i - 1;
                    }
                }
                PRT_Printf("%d,%d,%d,%d\n", pkt.payload.array[8], pkt.payload.array[9], pkt.payload.array[10], pkt.payload.array[11]);
                send_message(((uint8_t *)&pkt), sizeof(pkt.msg_type) + sizeof(SensorArray));
                for (int i = 0; i < sizeof(pkt); i++)
                {
                    PRT_Printf("%02X ", ((uint8_t *)&pkt)[i]);
                    if ((i + 1) % 16 == 0)
                        PRT_Printf("\n");
                }
                PRT_Printf("\n");
            }
        }
    case MSG_SET_PARAM:
        if (len >= sizeof(uint16_t) + sizeof(ParamPayload))
        {
            PRT_Printf("Received param: id=0x%02X, value=%.8f, len=%d\n",
                       packet->payload.param.param_id,
                       packet->payload.param.param_value,
                       len);
        }
        break;
    default:
        PRT_Printf("Unknown message type: 0x%02X\n", packet->msg_type);
        break;
    }
    return 0;
}

void clear_env()
{
    PRT_TimerStop(0, spi_timer_id);
    PRT_TimerDelete(0, spi_timer_id);
    PRT_SemDelete(taskstart_sem);
    PRT_TaskDelete(g_CtlTskHandel);
    rpmsg_backend_remove();
}
