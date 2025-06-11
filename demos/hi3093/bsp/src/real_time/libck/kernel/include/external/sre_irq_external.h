/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 中断模块内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_IRQ_EXTERNAL_H
#define SRE_IRQ_EXTERNAL_H

#include "sre_hwi_external.h"
#if defined(OS_OPTION_SMP)
#include "sre_sched_external.h"
#endif
#include "sre_cda_external.h"

/* 无效的线程控制块地址 */
#define OS_HWI_INVALID_TCB_ADDR 0xffffffff

/* 模块内全局变量声明 */
#if defined(OS_OPTION_SMP)
#define OS_INT_COUNT (THIS_RUNQ()->intCount)
#else
extern U32 g_intCount;
#define OS_INT_COUNT g_intCount
#endif

struct TagHwiHandleForm {
    union {
        HwiProcFunc hook;               // 非私有中断时为正常hook
        struct TagHwiHandleForm *form;  // 私有中断时为指向私有form数组的指针
    };

#if defined(OS_OPTION_HWI_ATTRIBUTE)
    HwiArg param;
#endif
};

struct TagHwiModeForm {
    HwiMode mode;
    HwiPrior prior;
#if defined(OS_OPTION_HWI_AFFINITY)
    U32 affinityMask;
#if defined(OS_OPTION_HOTPLUG_CPU)
    U32 origAffiMask;
#endif
#endif
};

/* Tick中断对应的硬件定时器ID */
extern U16 g_tickHwTimerIndex;

#if !defined(OS_OPTION_HWI_DYN_DESC)
#if defined(OS_OPTION_HWI_MAX_NUM_CONFIG)
extern struct TagHwiHandleForm *g_hwiForm;
extern struct TagHwiModeForm *g_hwiModeForm;
#else
extern struct TagHwiHandleForm g_hwiForm[OS_HWI_FORMARRAY_NUM];
extern struct TagHwiModeForm g_hwiModeForm[OS_HWI_MAX_NUM];
#endif
#endif

/* 模块内函数声明 */
extern void OsHwiDefaultHandler(HwiArg arg);
extern uintptr_t OsHwiModeFormAddrGet(void);

extern void OsHwiHookDispatcher(HwiHandle archHwi);
extern void OsHwiCombineDispatchHandler(HwiArg arg);
extern void OsHwiSetHandler(HwiHandle hwiNum, HwiProcFunc handler);
extern void OsHwiRecord(HwiHandle hwiNum);

extern void OsHwiDispatchTail(void);

/* 下列接口用于shell */
#if defined(OS_OPTION_SHELL)
extern HwiProcFunc OsHwiProcFuncGet(HwiHandle hwiNum);
extern HwiArg OsHwiArgGet(HwiHandle hwiNum);
extern HwiMode OsHwiModeGet(HwiHandle hwiNum);
extern bool OsHwiCreated(HwiHandle hwiNum);
#if defined(OS_OPTION_HWI_AFFINITY)
extern U32 OsHwiAffinityGet(HwiHandle hwiNum);
#endif
#endif

/*
 * 模块内内联函数定义
 */
#endif /* SRE_IRQ_EXTERNAL_H */
