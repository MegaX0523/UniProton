/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 硬件定时器模块的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_TIMER_EXTERNAL_H
#define SRE_TIMER_EXTERNAL_H

#include "sre_timer.h"
#include "sre_attr_external.h"

/*
 * 模块间typedef声明
 */
typedef U32(*TimerCreateFunc)(struct TimerCreatePara *createPara, TimerHandle *tmrHandle);

typedef U32(*TimerStartFunc)(TimerHandle timerHdl);

typedef U32(*TimerStopFunc)(TimerHandle timerHdl);

typedef U32(*TimerDeleteFunc)(TimerHandle timerHdl);

typedef U32(*TimerRestartFunc)(TimerHandle timerHdl);

typedef U32(*TimerSetIntervalFunc)(TimerHandle timerHdl, U32 interVal);

typedef U32(*TimerQueryFunc)(TimerHandle timerHdl, U32 *expireTime);

/* 定时器函数库 */
struct TagFuncsLibTimer {
    TimerCreateFunc createTimer;
    TimerStartFunc startTimer;
    TimerStopFunc stopTimer;
    TimerDeleteFunc deleteTimer;
    TimerRestartFunc restartTimer;
    TimerSetIntervalFunc setIntervalTimer;
    TimerQueryFunc timerQuery;
};

/* 定时器类型 */
enum {
    TIMER_TYPE_HWTMR, /* 核内私有硬件定时器 */
    TIMER_TYPE_MC_HWTMR, /* 硬件共享时钟源定时器 */
    TIMER_TYPE_SWTMR, /* 软件定时器 */
    TIMER_TYPE_TRING, /* TRING定时器 */
#if defined(OS_OPTION_SWTMR_STAGGER)
    TIMER_TYPE_SWTMR_STAGGER, /* 支持错峰机制的软件定时器 */
#endif
    TIMER_TYPE_INVALID
};

/*
 * 模块间全局变量声明
 */
extern struct TagFuncsLibTimer g_timerAPI[TIMER_TYPE_INVALID];

#define OS_SWTMR_PRE_FREE 0x40U /* 在软件定时器超时状态下进行删除操作，定时器进入预free态 */
#define OS_SWTMR_PRE_CREATED 0x80U /* 在软件定时器超时状态下进行停止操作，定时器进入预created态 */
#define OS_SWTMR_PRE_RUNNING 0xc0U /* 在软件定时器超时状态下进行删除操作，定时器进入预running态 */

#define OS_TIMER_PRE_FREE (U8)(OS_SWTMR_PRE_FREE | (U32)OS_TIMER_EXPIRED)
#define OS_TIMER_PRE_CREATED (U8)(OS_SWTMR_PRE_CREATED | (U32)OS_TIMER_EXPIRED)
#define OS_TIMER_PRE_RUNNING (U8)(OS_SWTMR_PRE_RUNNING | (U32)OS_TIMER_EXPIRED)

/*
 * timerHandle : timeType | timerIndex
 *               [31...28] | [27...28]
 * timeType : 0, TIMER_TYPE_HWTMR
 * timeType : 1, TIMER_TYPE_MC_HWTMR
 * timeType : 2, TIMER_TYPE_SWTMR
 * timeType : 3, TIMER_TYPE_TRING
 *
 */
#define OS_TIMER_GET_HANDLE(type, index) (((type) << 28) | (index))
#define OS_TIMER_GET_INDEX(handle) ((handle) & 0x0FFFFFFFU)
#define OS_TIMER_GET_TYPE(handle) ((handle) >> 28)

#endif /* SRE_TIMER_EXTERNAL_H */
