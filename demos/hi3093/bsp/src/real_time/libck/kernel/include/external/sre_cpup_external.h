/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: cpu占用率模块的内部公共头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组     华为规范整改
 */
#ifndef SRE_CPUP_EXTERNAL_H
#define SRE_CPUP_EXTERNAL_H

#include "sre_idle_external.h"
#include "sre_cpup.h"

/*
 * @ingroup  SRE_cpup
 * 设置cpu占用率的注册信息结构体。
 */
struct TagOsCpupWarnInfo {
    /* CPU占用率告警阈值 */
    U32 warn;
    /* CPU占用率告警恢复阈值 */
    U32 resume;
};

/*
 * 线程级CPU占用率结构体
 */
struct TagCpupThread {
    /* 运行总时间记录 */
    U64 allTime;
    /* 调用前时间记录 */
    U64 startTime;
    /* CPU占用率 */
    U16 usage;
    /* 保留 */
    U16 reserve;
    /* 保留，64位对齐(R8 cacheline)> */
    U32 reserve2;
#if defined(OS_OPTION_SMP)
    /* 锁保护，防止多核竞争修改switchCount */
    uintptr_t switchSpl;
    /* 任务 切入++,切出--,用于防止任务运行计时反转 */
    U32 switchCount;
#endif
};

#define OS_CPUP_INT_ID 0xffffffff /* 中断线程ID */
#define CPUP_USE_RATE 10000 /* CPUP使用比率 10000:万分比  1000:千分比  100:百分比 */
#define CPUP_INIT_VALUE 0xffffffff /* CPUP占用率初始值 */
#define OS_CPUP_CORE_NUM SRE_GetMaxNumberOfCores()

typedef void (*CpupWarnFunc)(void);
typedef U32(*CpupNowFunc)(void);

extern struct TagOsCpupWarnInfo g_cpupWarnInfo;
extern struct TagCpupThread *g_cpup;
extern U16 g_sysUsage;
extern U64 g_cpupCompen;

extern bool OsCpupInitIsDone(void);
extern U32 OsCpupLazyInit(void);

#if defined(OS_OPTION_INST_RESET)
extern void OsMcCpupClear(U32 coreIndex);
#endif
extern void OsCpupIdleNoSleep(void);

/* CPUP核休眠钩子注册 */
OS_SEC_ALW_INLINE INLINE void OsCpupCoreSleepHookSet(OsVoidFunc handle)
{
    OsIdleSetCoreSleepHook(handle);
    return;
}

OS_SEC_ALW_INLINE INLINE U64 OsCpupCompenGet(void)
{
    return g_cpupCompen;
}

#endif /* SRE_CPUP_EXTERNAL_H */
