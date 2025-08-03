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
volatile int counts = 0;
static int task_flag = 0;
// static int ctlstart_flag = 0;
static U32 spitmierID;
static SemHandle taskstart_sem;
uint8_t AD7606_Data[16] = {0x0}; // Buffer to hold AD7606 data
// static SemHandle taskend_sem;

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
    phase += 2 * PI * CONT_Hz / 1000.0;
    // 归一化相位到0~2π之间，避免溢出
    while (phase >= 2 * PI)
    {
        phase -= 2 * PI;
    }
    return value;
}

void Timer_Init(void)
{
    struct TimerCreatePara spitimer = {0};
    spitimer.type = OS_TIMER_SOFTWARE;
    spitimer.mode = OS_TIMER_LOOP;
    spitimer.interval = 1;
    spitimer.timerGroupId = 0;
    spitimer.callBackFunc = Timer_Func;
    spitimer.arg1 = 0;

    PRT_SemCreate(0, &taskstart_sem);
    PRT_TimerCreate(&spitimer, &spitmierID);
    PRT_TimerStart(0, spitmierID);
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

    task_flag = 0;
    while (1)
    {
        PRT_SemPend(taskstart_sem, OS_WAIT_FOREVER);
        if (task_flag)
        {
            // send_message(tx_buff, ret);
            if (task_flag > 1)
            {
                static double i = 0;
                i += 0.1;
                tmp_vol = next_sine_sample_static();
                // DAC8563_SetVoltage(1, 4 * tmp_vol + 5.0);
                DAC8563_SetVoltage(1, i);
            }

            ret = AD7606_ReadAllChannels(AD7606_Data);
            AD_CH0 = (int16_t)((AD7606_Data[0] << 8) | AD7606_Data[1]);
            AD_CH1 = (int16_t)((AD7606_Data[2] << 8) | AD7606_Data[3]);
            votlage0 = (double)AD_CH0 * 10 / 0x7fff; // Assuming 16-bit ADC and 10.0V reference
            votlage1 = (double)AD_CH1 * 10 / 0x7fff; // Assuming 16-bit ADC and 10.0V reference

            if (task_flag > 2)
            {
                output = outputget(votlage0, votlage1);
                // DAC8563_SetVoltage(0, (-1.0 * output) + 5.0);
                DAC8563_SetVoltage(1, i);
            }

            // PRT_Printf("$%.4f %.4f %.4f;", votlage0, votlage1, (-1.0 * output));
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
        if (len >= sizeof(uint16_t) + sizeof(uint16_t))
        {
            PRT_Printf("Received command: 0x%02X, len = %d\n", packet->payload.command, len);
        }
        if (packet->payload.command == START_DAMPING)
        {
            PRT_Printf("Sending array.\n", packet->payload.command);

            rpmsg_packet pkt = {
                .msg_type = MSG_SENSOR_ARRAY};
            for (uint16_t i = 0; i < SENSOR_ARRAY_SIZE; i++)
            {
                pkt.payload.array[i] = i; // 示例数据
                if ((pkt.payload.array[i] & 0xFF00) == 0x0A00)
                {
                    pkt.payload.array[i] = i - 1;
                }
                if ((pkt.payload.array[i] & 0x00FF) == 0x000A)
                {
                    pkt.payload.array[i] = i - 1;
                }
            }
            PRT_Printf("msg_type: %d\n", pkt.msg_type);
            PRT_Printf("%d,%d,%d,%d\n", pkt.payload.array[8], pkt.payload.array[9], pkt.payload.array[10], pkt.payload.array[11]);
            send_message(((uint8_t *)&pkt), sizeof(pkt.msg_type) + sizeof(SensorArray));
            for (int i = 0; i < sizeof(pkt); i++)
            {
                PRT_Printf("%02X ", ((uint8_t *)&pkt)[i]);
                if ((i + 1) % 16 == 0)
                    PRT_Printf("\n");
            }
        }
        break;

    case MSG_SET_PARAM:
        if (len >= sizeof(uint16_t) + sizeof(ParamPayload))
        {
            PRT_Printf("Received param: id=0x%02X, value=%.2f, len=%d\n",
                       packet->payload.param.param_id,
                       packet->payload.param.param_value,
                       len);
        }
        break;
    case MSG_SENSOR_ARRAY:

        break;

    default:
        PRT_Printf("Unknown message type: 0x%02X\n", packet->msg_type);
        break;
    }
    return 0;
}

void clear_env()
{
    PRT_TimerStop(0, spitmierID);
    PRT_TimerDelete(0, spitmierID);
    PRT_SemDelete(taskstart_sem);
    PRT_TaskDelete(g_CtlTskHandel);
    rpmsg_backend_remove();
}
