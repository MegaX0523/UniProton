/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: swtmr模块的模块内头文件
 * Author: LibCK项目组
 * Create: 2021-3-4
 */
#ifndef SRE_SWTMR_STAGGER_EXTERNAL_H
#define SRE_SWTMR_STAGGER_EXTERNAL_H

#include "sre_swtmr_external.h"
#include "sre_tick_external.h"
#include "sre_atomic.h"

/*
 * 模块内宏定义
 */
#define OS_SWTMR_STAGGER_MAX_TOLERANCE 5
#define OS_MAX_SWTMR_STAGGER_TIMER_NUM 400

/* 最大支持软件错峰定时器个数 */
extern U32 g_swTmrStaggerMaxNum;
OS_SEC_ALW_INLINE INLINE U32 OsSwTmrStaggerMaxNum(void)
{
    return g_swTmrStaggerMaxNum;
}

#define OS_STAGGER_TIMER_MAX_NUM ((TIMER_TYPE_SWTMR_STAGGER << 28) + OsSwTmrStaggerMaxNum())
#define OS_SWTMR_STAGGER_MIN_NUM (TIMER_TYPE_SWTMR_STAGGER << 28)

#define OS_SWTMR_STAGGER_INDEX_2_ID(index) (((U32)(index)) + OS_SWTMR_STAGGER_MIN_NUM)
#define OS_SWTMR_STAGGER_ID_2_INDEX(timerId) ((timerId) - OS_SWTMR_STAGGER_MIN_NUM)

/*
 * 模块内全局变量声明
 */
/* 软件定时器Sortlink */
extern struct TagSwTmrSortLinkAttr g_tmrStaggerSortLink;
/* 软件定时器空闲链表 */
extern struct TagSwTmrCtrl *g_tmrStaggerFreeList;

struct SwtmrStaggerPara {
    /* 定时器工作模式 */
    U8 mode;
    /*
     * 表示当定时JOB分发碰撞达到阈值时，该定时周期JOB最大延迟分发的tick数，
     * 启动时设置为初始值，错峰处理时延迟tick数达到容忍度时则必须要执行分发该定时JOB。
     */
    U8 tolerance;
    /* 共享队列ID号 */
    U16 shqId;
    /* 定时周期，支持错峰的软件定时器，周期为ms，这里需要是1ms的倍数 */
    U32 interval;
    /* Qport2硬中断中，根据该字段查找对应WG的软中断线程 */
    U32 cpuId;
    /* WG对应软中断线程中通过本字段查找对应的RAT JOB入口地址，以便回调触发RAT任务 */
    U32 pid : 16;
    U32 rsv0 : 16;
    /* JsfJobData需要传递，这里传递类型为U32 */
    U32 *jobData;
    /* 定时器回调函数 */
    SreTmrProcFunc callBackFunc;
    /* 定时器用户参数1 */
    U32 arg1;
    /* 定时器用户参数2 */
    U32 arg2;
    /* 定时器用户参数3 */
    U32 arg3;
    /* 定时器用户参数4 */
    U32 arg4;
};

extern struct TagListObject **g_tmrStaggerToleranceList;
extern U32 g_tmrStaggerThreshold;
extern U32 g_tmrStaggerHostCoreId;
extern U32 g_tmrStaggerSetFlag;
extern struct TagSwTmrCtrl *g_swtmrStaggerCBArray;

/*
 * 模块内函数声明
 */
extern void OsSwTmrStaggerScan(void);
extern U32 OsSwTmrStaggerCreateTimer(struct SwtmrStaggerPara *createPara, TimerHandle *tmrHandle);
extern U32 OsSwTmrStaggerSetTimerPara(struct SwtmrStaggerPara *createPara, U32 tmrHandle);
extern U32 OsSwTmrStaggerStartTimer(TimerHandle tmrHandle);
extern U32 OsSwTmrStaggerDeleteTimer(TimerHandle tmrHandle);
extern void OsSwTmrStaggerStart(struct TagSwTmrCtrl *swtmr, U32 interval);
extern void OsSwTmrStaggerStop(struct TagSwTmrCtrl *swtmr);
extern void OsSwTmrStaggerDelete(struct TagSwTmrCtrl *swtmr);
extern U32 OsSwTmrStaggerInit(struct TagSwTmrCtrl *swtmrStaggerCB,
                              struct TagListObject **toleranceList,
                              struct TagListObject *sortLinkList);

#define OsSwtmrIqrSplLock(a) OsIntLock()
#define OsSwtmrIqrSplUnlock(a, b) OsIntRestore(b)

#endif /* SRE_SWTMR_STAGGER_EXTERNAL_H */
