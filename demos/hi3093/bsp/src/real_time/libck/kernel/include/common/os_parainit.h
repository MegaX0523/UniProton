/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 初始化模块的基础头文件，用户不能直接包含
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_PARAINIT_H
#define OS_PARAINIT_H

#include "sre_buildef.h"
#include "os_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_sys
 * 系统模块注册信息的结构体定义。
 *
 * 保存系统模块的注册项信息。
 */
struct OsSysRegInfo {
    U32 systemClock;
    U32 sysRunNum;
    U32 sysMaxNum;
    U32 sysRunCoremask;
    U32 sysCoreStart;
    U32 sysPrimary;
    uintptr_t sysMpRegBase;
    uintptr_t sysrebootHook;
    U32 sysMinnumImgOption;
    uintptr_t sysTimeHook;
    U32 sysImageNum;
    U32 sysImageId;
    uintptr_t sysStackStartAddr;
    U32 sysStackAreaSize;
};

/*
 * @ingroup OS_timer
 * 定时器注册信息的结构体定义。
 *
 * 定时器模块的注册项信息。
 */
struct OsHwtimerRegInfo {
    U32 hwtimerOption;
    U32 mcHwtimerOption;
    U32 mcHwtmrClock;
    U32 tickSwtimerOption;
    U32 tickSwtimerMaxNum;
    U32 tringtimerOption;
};

/*
 * @ingroup OS_tick
 * tick中断注册信息的结构体定义。
 *
 * tick中断模块的注册项信息。
 */
struct OsTickRegInfo {
    U32 tickOption;
    U32 tickPriority;
    U32 tickPerSecond;
};

/*
 * @ingroup OS_swi
 * 软中断注册信息的结构体定义。
 *
 * 软中断模块的注册项信息。
 */
struct OsSwiRegInfo {
    U32 swiOption;
    U32 mcswiOption;
    U32 swiMaxSupportNum;
};

/*
 * @ingroup OS_task
 * 任务注册信息的结构体定义。
 *
 * 任务模块的注册项信息。
 */
struct OsTaskRegInfo {
    U32 taskOption;
    U32 taskMaxSupportNum;
    U32 taskDefaultStackSize;
    U32 taskIdleStackSize;
    U32 taskStackMagicWord;
};

/*
 * @ingroup OS_sched
 * 调度域注册信息的结构体定义。
 *
 * 调度域模块的注册项信息。
 */
struct OsSchedDomainRegInfo {
    U32 schedDomainInfo;
    U32 schedDomainInfoNum;
};

/*
 * @ingroup OS_taskmon
 * 任务监控注册信息的结构体定义。
 *
 * 任务监控模块的注册项信息。
 */
struct OsTaskmomRegInfo {
    U32 taskmonOption;
    U32 taskmonConfigOption;
    U32 taskmonOverOption;
    U32 taskmonOverThreshold;
    U32 taskmonDomode;
    U32 taskmonStarvation;
    U32 taskmonStarvationThreshold;
};

/*
 * @ingroup OS_tskstack
 * 任务栈越界检测注册信息的结构体定义。
 *
 * 任务栈越界检测模块的注册项信息。
 */
struct OsTskStackMonRegInfo {
    U32 tskStackOption;
    U32 tskStackConfigOption;
};

/*
 * @ingroup OS_exc
 * 异常模块注册信息的结构体定义。
 *
 * 保存异常模块的注册项信息。
 */
struct OsExceRegInfo {
    uintptr_t excProcHook;
    uintptr_t sysDoubleExcStackStartAddr;
    U32 sysDoubleExcStackAreaSize;
};

/*
 * @ingroup OS_mem
 * 内存模块注册信息的结构体定义。
 *
 * 保存内存模块的注册项信息。
 */
struct OsMemRegInfo {
    U32 maxPtNum;
    U32 memMcdynfscOption;
    U32 memPoolOption;
    U32 memMcpoolOption;
    U32 memblockOption;
    U32 memSlubfscOption;
    U32 memSscOption;
    U32 memMcMappoolOption;
    U32 memDsscOption;
    uintptr_t memFscPtAddr;
    U32 memFscPtSize;
    uintptr_t memMcdynFscPtAddr;
    U32 memMcdynFscPtSize;
    U32 memUncacheStaticOption;
    uintptr_t memUncchePtAddr;
    U32 memUncachePtSize;
};

/*
 * @ingroup OS_sem
 * 信号量注册信息的结构体定义。
 *
 * 信号量模块的注册项信息。
 */
struct OsSemRegInfo {
    U32 semOption;
    U32 semConfigOption;
    U32 semMaxSupportNum;
};

/*
 * @ingroup OS_mcfsem
 * 核间快速信号量注册信息的结构体定义。
 *
 * 核间快速信号量模块的注册项信息。
 */
struct OsMcfsemRegInfo {
    U32 mcfsemOption;
    U32 mcfsemConfigOption;
};

/*
 * @ingroup OS_sem
 * 消息注册信息的结构体定义。
 *
 * 消息模块的注册项信息。
 */
struct OsMsgRegInfo {
    U32 msgOption;
    U32 mcmsgOption;
    U32 msgMaxSupportNum;
    U32 msgMaxMulticoreNum;
};

/*
 * @ingroup OS_event
 * 可订阅事件注册信息的结构体定义。
 *
 * 可订阅事件模块的注册项信息。
 */
struct OsEventRegInfo {
    U32 eventOption;
    U32 eventMaxBookNum;
    U32 eventMaxEachBookNum;
};

/*
 * @ingroup OS_sync
 * 核间同步注册信息的结构体定义。
 *
 * 核间同步模块的注册项信息。
 */
struct OsSyncRegInfo {
    U32 syncOption;
};

/*
 * @ingroup OS_ecc
 * ECC注册信息的结构体定义。
 *
 * ECC模块的注册项信息。
 */
struct OsEccRegInfo {
    U32 eccOption;
};

/*
 * @ingroup OS_shell
 * shell注册信息的结构体定义。
 *
 * shell模块的注册项信息。
 */
struct OsShellRegInfo {
    U32 shellOption;
    U32 shellShtskPriority;
    U32 shellShtskStackSize;
};

/*
 * @ingroup OS_trace
 * trace注册信息的结构体定义。
 *
 * trace模块的注册项信息。
 */
struct OsTraceRegInfo {
    U32 traceOption;
    U32 traceConfigOption;
    U32 traceAreaAddr;
    U32 traceAreaSize;
};

/*
 * @ingroup OS_decive
 * 驱动注册信息的结构体定义。
 *
 * 驱动模块的注册项信息。
 */
struct OsDeviceRegInfo {
    U32 deviceOption;
};

/*
 * @ingroup OS_lp
 * lp注册信息的结构体定义。
 *
 * lp模块的注册项信息。
 */
struct OsLpRegInfo {
    U32 lpOption;
    U32 lpShareNcPtNo;
    U32 lpShareNcPtAddr;
    U32 lpShareNcPtSize;
    U32 lpMaxGroupNum;
    U32 lpMaxUnitNum;
};

/*
 * @ingroup OS_patch
 * 静态补丁信息的结构体定义。
 *
 * 静态补丁模块的注册项信息。
 */
struct OsPatchRegInfo {
    U32 patchOption;
};

/*
 * @ingroup OS_coresleep
 * 低功耗信息的结构体定义。
 *
 * 低功耗模块的注册项信息。
 */
struct OsCoresleepRegInfo {
    U32 coresleepOption;
};

/*
 * @ingroup OS_queue
 * 队列信息的结构体定义。
 *
 * 队列模块的注册项信息。
 */
struct OsQueueRegInfo {
    U32 queueOption;
    U32 queueMaxSupportNum;
};

/*
 * @ingroup OS_hook
 * 钩子模块注册信息的结构体定义。
 *
 * 钩子模块的注册项信息。
 */
struct OsHookRegInfo {
    U32 hookHwiEntryNum;
    U32 hookHwiExitNum;
    U32 hookSwiEntryNum;
    U32 hookSwiExitNum;
    U32 hookTaskCreateNum;
    U32 hookTaskDeleteNum;
    U32 hookTaskSwitchNum;
    U32 hookIdleNum;
    U32 hookLastWordNum;
    U32 hookDynpwrgtEntryNum;
    U32 hookDynpwrgtExitNum;
};

/*
 * @ingroup OS_symbol
 * 符号管理的结构体定义。
 *
 * 符号管理模块的注册项信息。
 */
struct OsSymbolRegInfo {
    U32 symbolOption;
    U32 symbolTablePtNo;
    U32 symbolTableNcPtAddr;
    U32 symbolTablePtSize;
};

/*
 * @ingroup OS_mng
 * 模拟加载信息的结构体定义。
 *
 * 模拟加载模块的注册项信息。
 */
struct OsMngRegInfo {
    U32 mngOption;
};

/*
 * @ingroup OS_mmu
 * mmu注册信息的结构体定义。
 *
 * mmu模块的注册项信息。
 */
struct OsMmuRegInfo {
    U32 mmuMapInfoOption;
    U32 mmuMapInfoAddr;
    U32 mmuMapInfoNum;
};

/*
 * @ingroup OS_excluive
 * excluive注册信息的结构体定义。
 *
 * excluive模块的注册项信息。
 */
struct OsExcluiveRegInfo {
    U32 excluiveRegionOption;
    uintptr_t excluiveRegionStart;
    U32 excluiveRegionSize;
};

/*
 * @ingroup OS_hwi
 * 硬中断模块注册信息的结构体定义。
 *
 * 硬中断模块的注册项信息。
 */
struct OsHwiRegInfo {
    U32 hwiMaxNum;
};

/*
 * @ingroup OS_gic
 * gic模块注册信息的结构体定义。
 *
 * gic模块的注册项信息。
 */
struct OsGicRegInfo {
    U32 gicBaseOption;
    uintptr_t gicBaseAddr;
    uintptr_t gicrOffset;
    uintptr_t gicrStride;
};
/*
 * @ingroup OS_cpup
 * cpup模块注册信息的结构体定义。
 *
 * cpuo模块的注册项信息。
 */
struct OsCpupRegInfo {
    U32 cpupOption;
    U32 cpupSampleRecordNum;
    U32 cpupSampleInterval;
    U32 cpupConfigWarnOption;
    U32 cpupShortWarn;
    U32 cpupshortResume;
};

struct OsInitRegPara {
    struct OsSysRegInfo sysRegInfo;
    struct OsHwtimerRegInfo hwtimerRegInfo;
    struct OsTickRegInfo tickRegInfo;
    struct OsSwiRegInfo swiRegInfo;
#if defined(OS_OPTION_TASK)
    struct OsTaskRegInfo taskRegInfo;
#endif
#if defined(OS_OPTION_SMP)
    struct OsSchedDomainRegInfo schedDomainRegInfo;
#endif
#if defined(OS_OPTION_TASK_MON)
    struct OsTaskmomRegInfo taskmomRegInfo;
#endif
#if defined(OS_OPTION_STKMON)
    struct OsTskStackMonRegInfo tskStackMonRegInfo;
#endif
    struct OsExceRegInfo exceRegInfo;
    struct OsCpupRegInfo cpupRegInfo;
    struct OsMemRegInfo memRegInfo;
    struct OsSemRegInfo semRegInfo;
    struct OsMcfsemRegInfo mcfsemRegInfo;
    struct OsMsgRegInfo msgRegInfo;
#if defined(OS_OPTION_FUSION_INTEGRATE_API)
    struct OsEventRegInfo eventRegInfo;
#endif
    struct OsSyncRegInfo syncRegInfo;
    struct OsEccRegInfo eccRegInfo;
#if defined(OS_OPTION_SHELL)
    struct OsShellRegInfo shellRegInfo;
#endif
#if defined(OS_OPTION_TRACE)
    struct OsTraceRegInfo traceRegInfo;
#endif
    struct OsDeviceRegInfo deviceRegInfo;
    struct OsLpRegInfo lpRegInfo;
    struct OsPatchRegInfo patchRegInfo;
#if defined(OS_OPTION_QUEUE)
    struct OsQueueRegInfo queueRegInfo;
#endif
    struct OsHookRegInfo hookRegInfo;
    struct OsSymbolRegInfo symbolRegInfo;
    struct OsMngRegInfo mngRegInfo;
#if defined(OS_OPTION_MMU)
    struct OsMmuRegInfo mmuRegInfo;
#endif
    struct OsExcluiveRegInfo excluiveRegInfo;
#if defined(OS_OPTION_HWI_MAX_NUM_CONFIG)
    struct OsHwiRegInfo hwiRegInfo;
#endif
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* OS_PARAINIT_H */
