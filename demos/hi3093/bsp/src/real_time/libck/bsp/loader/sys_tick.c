/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "sre_sys.h"
#include "sre_tick.h"
#include "sre_config.h"
#include "sre_task.h"
#include "sre_hwi.h"
#include "securec.h"
#include "sre_common.h"

U64 g_timerFrequency;
#define PMU_TIMER_FREQUENCY g_timerFrequency

static U64 GetGenericTimerFreq(void)
{
    U64 freq;

    OS_EMBED_ASM("MRS %0, CNTFRQ_EL0" : "=r"(freq) : : "memory", "cc");

    return freq;
}

static void TimerIsr(uintptr_t para)
{
    (void)para;
    U32 cfgMask = 0x0;
    U64 cycle = PMU_TIMER_FREQUENCY / OS_TICK_PER_SECOND;

    OS_EMBED_ASM("MSR CNTP_CTL_EL0, %0" : : "r"(cfgMask) : "memory");
    SRE_ISB();
    OS_EMBED_ASM("MSR CNTP_TVAL_EL0, %0" : : "r"(cycle) : "memory", "cc");

    cfgMask = 0x1;
    OS_EMBED_ASM("MSR CNTP_CTL_EL0, %0" : : "r"(cfgMask) : "memory");

    SRE_TickISR();
    SRE_ISB();
}

static void CoreTimerInit(void)
{
    U32 cfgMask = 0x0;
    U64 cycle = PMU_TIMER_FREQUENCY / OS_TICK_PER_SECOND;

    OS_EMBED_ASM("MSR CNTP_CTL_EL0, %0" : : "r"(cfgMask) : "memory");
    SRE_ISB();
    OS_EMBED_ASM("MSR CNTP_TVAL_EL0, %0" : : "r"(cycle) : "memory", "cc");

    cfgMask = 0x1;
    OS_EMBED_ASM("MSR CNTP_CTL_EL0, %0" : : "r"(cfgMask) : "memory");
}

static U32 CoreTimerStart(void)
{
    g_timerFrequency = GetGenericTimerFreq();
    CoreTimerInit();

    return OS_OK;
}

U32 TaskClkStart(void);
U32 TaskClkStart(void)
{
    U32 ret;

    ret = SRE_HwiSetAttr(TASK_CLK_INT, 10, OS_HWI_MODE_ENGROSS); // priority 10
    if (ret != OS_OK) {
        return ret;
    }

    ret = SRE_HwiCreate(TASK_CLK_INT, (HwiProcFunc)TimerIsr, 0);
    if (ret != OS_OK) {
        return ret;
    }
#if defined(OS_GIC_VER) && (OS_GIC_VER == 3)
    ret = SRE_HwiEnable(TASK_CLK_INT);
    if (ret != OS_OK) {
        return ret;
    }
#elif defined(OS_GIC_VER) && (OS_GIC_VER == 2)
    IsrRegister(TASK_CLK_INT, 0xaU, (0x1U << LIBCK_CORE_ID));
#endif

    ret = CoreTimerStart();
    if (ret != OS_OK) {
        return ret;
    }

    return OS_OK;
}
