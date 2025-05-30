#include "test.h"
#include "prt_task.h"
#include "rpmsg_backend.h"
#include "stdio.h"
#include "spi.h"
#include "prt_timer.h"
#include "prt_sem.h"
#include "ctltask.h"
#include "gpio_def.h"

volatile double ref_signal = 0;
volatile double err_signal = 0;
volatile int counts = 0;
static U8 task_flag = 0;
static U32 spitmierID;
static SemHandle taskstart_sem;
// static SemHandle taskend_sem;

extern TskHandle g_testTskHandle;
extern TskHandle g_CtlTskHandel;
extern struct rpmsg_endpoint tty_ept;
extern double *InputArray;

void timer_init(void);

static void Timer_Func(TimerHandle tmrHandle, U32 arg1, U32 arg2, U32 arg3, U32 arg4)
{
    PRT_SemPost(taskstart_sem);
    return;
}

void ControlTaskEntry()
{
    int i = 0;
    int ret;
    U8 txbuff[4] = {0x00, 0x55, 0xAA, 0xFF};
    U8 rxbuff[4] = {0};
    U8 dummy[4] = {0x11, 0x22, 0x33, 0x44};
    U8 tx_buff[BUFF_LEN];
    volatile double output;

    timer_init();
    FilterInit();
    SPI0_Init();
    GPIO_init(CONVST_PIN, GPIO_OUTPUT); // Initialize CONVST pin as output


    task_flag = 0;
    while (1)
    {
        PRT_SemPend(taskstart_sem, OS_WAIT_FOREVER);
        if (task_flag)
        {
            // spi0_transfer(SPI0_CE0, dummy, rxbuff, 4);
            // output = outputget(ref_signal, err_signal);
            // i++;
            // ret = sprintf(tx_buff, "%d %lf %lf\n", counts, err_signal, output);
            // send_message(tx_buff, ret);
            SPI0_TransferBuffer(txbuff, rxbuff, 4); 
        }
    }
}

void timer_init(void)
{
    struct TimerCreatePara spitimer = {0};
    spitimer.type = OS_TIMER_SOFTWARE;
    spitimer.mode = OS_TIMER_LOOP;
    spitimer.interval = 10;
    spitimer.timerGroupId = 0;
    spitimer.callBackFunc = Timer_Func;
    spitimer.arg1 = 0;

    PRT_SemCreate(0, &taskstart_sem);
    PRT_TimerCreate(&spitimer, &spitmierID);
    PRT_TimerStart(0, spitmierID);
}

int rec_msg_proc(void *data, int len)
{
    char tx_buff[BUFF_LEN];
    int ret;
    int id;
    int count;
    double data1;
    double data2;
    // PRT_Printf("rec_msg_proc: %s\n\0", (char *)data);
    memcpy(tx_buff, data, len);
    tx_buff[len] = '\0';
    ret = sscanf(tx_buff, MSG_FORMAT, &id, &count, &data1, &data2);
    if (ret != 4)
    {
        return -1;
    }
    PRT_Printf("rec_msg_proc: id=%d, count=%d, data1=%lf, data2=%lf\n", id, count, data1, data2);
    switch (id)
    {
    case MSG_CONTROL_START:
        task_flag = 1;
        PRT_TimerStart(0, spitmierID);
        set_gpio(CONVST_PIN, 1); // Set CONVST high to prepare for conversion
        break;
    case MSG_CONTROL_STOP:
        PRT_TimerStop(0, spitmierID);
        task_flag = 0;
        set_gpio(CONVST_PIN, 0); // Set CONVST low to stop conversion
        break;
    case MSG_CONTROL_EXIT:
        PRT_TimerStop(0, spitmierID);
        task_flag = 0;
        clear_env();
        return 1;
    case MSG_INPUTDATA:
        ref_signal = data1;
        err_signal = data2;
        counts = count;
        break;
    default:
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
