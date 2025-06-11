/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Tick私有头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_TICK_EXTERNAL_H
#define SRE_TICK_EXTERNAL_H

#include "sre_tick.h"
#if defined(OS_OPTION_TICK_TRIGGERED_BY_SWI)
#include "sre_swi.h"
#endif

/*
 * 模块间宏定义
 */
#define OS_TICKLESS_FOREVER ((U64)-1)

/*
 * 模块间typedef声明
 */
typedef void(*SwitchScanFunc)(void);
typedef void (*TskmonTickHook)(void);

#if defined(OS_OPTION_TICKLESS)

typedef U64(*TickNearestGetFunc)(U32 coreID);
typedef bool (*TickTargetCheckFunc)(U32 coreID);

extern TickNearestGetFunc g_tskDlyNearestTicksGet;
extern TickNearestGetFunc g_swtmrNearestTicksGet;
extern TickNearestGetFunc g_cpupNearestTicksGet;

/* 按核号获取最短任务延时的tick刻度 */
extern TickTargetCheckFunc g_tskDlyTargetCheck;
extern TickTargetCheckFunc g_swtmrTargetCheck;
extern TickTargetCheckFunc g_cpupTargetCheck;

#endif

extern SwitchScanFunc g_swtmrScanHook;
#if defined(OS_OPTION_SWTMR_STAGGER)
extern SwitchScanFunc g_swtmrStaggerScanHook;
#endif
/* 任务检测Tick中断调用钩子 */
extern TskmonTickHook g_tskMonHook;

extern U32 OsTickGetMs(void);
extern void OsTickDispatcher(void);
#if defined(OS_OPTION_SMP)
extern void OsTickForward(U32 coreMask);
extern void OsTickForwardISR(void);
#endif

#if defined(OS_OPTION_TICK_TRIGGERED_BY_SWI)
extern bool OsTickSwiCheck(SwiHandle swiID);
#endif
extern U32 g_cyclePerTick;
OS_SEC_ALW_INLINE INLINE U32 OsGetCyclePerTick(void)
{
    return g_cyclePerTick;
}
#endif /* SRE_TICK_EXTERNAL_H */
