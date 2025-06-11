/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 调度的公共函数实现私有头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_SCHED_EXTERNAL_H
#define SRE_SCHED_EXTERNAL_H

#include "sre_lib_external.h"
#include "sre_raw_spinlock_external.h"
#include "sre_err_external.h"
#include "sre_sys_external.h"
#include "sre_cpu_external.h"
#include "sre_board_external.h"
#include "sre_sched.h"
#include "sre_rt_external.h"
#include "sre_cpu_pri_external.h"
#include "sre_atomic.h"

#if defined(OS_OPTION_SMP)
/*
 * 模块间宏定义
 */
#define SMP_MC_SCHEDULE_TRIGGER(core) OsHwiTriggerByMask((U32)(1U << (core)), OS_HWI_IPI_NO_01)

/* 根据核号取对应的rq */
#define GET_RUNQ(core) ((struct TagOsRunQue *)&g_runQueue[(core)])
#define OS_RT_MAX_TRYS 3

#define THIS_RUNQ() ((struct TagOsRunQue *)&g_runQueue[THIS_CORE()])

#define OS_SCHED_CLASS(coreId)  (GET_RUNQ(coreId)->domain->schedClass)

#if defined(OS_OPTION_TASK_MIGRATE)
#define OS_DEFAULT_SCHED_TYPE OS_SCHEDULE_RT
#else
#define OS_DEFAULT_SCHED_TYPE OS_SCHEDULE_RT_SINGLE
#endif

/* *  入队出队的flags  * */
#define OS_ENQUEUE_HEAD 1U /* 入队加头 */

/*
 * 模块间结构体定义
 */
struct TagTskCB;
struct TagScheduleclass;
struct OsFiberMng;

struct TagOsSchedDomain {
    enum OsScheduleType schedType;          // 调度类型，记录
    U32    schedCoreMask;                   // 静态配置的核掩码
    U32    startCore;                       // 起始核
    U32    endCore;                         // 结束核
    U32    coreNum;                         // 核数
    struct TagCorePri         corePri;      // CPU优先级信息
    struct TagScheduleClass  *schedClass;   // 调度方法
};

/*
 * 每个核一份的running队列。
 * 局部队列方式，lock时需要两两核之间lock
 * spinLock/uniFlag/tskCurr 这几个变量位置不能动，汇编代码会使用
 */
struct TagOsRunQue {
    volatile uintptr_t spinLock;     // 操作该RQ使用的锁
    U32 uniFlag;
    struct TagTskCB *tskCurr;        // RQ中的当前运行任务
    bool needReschedule;             // 是否需要调度
    U32 rqCoreId;                    // 运行队列所属的核
    U32 tskIdlePID;                  // RQ中的idle任务
    U32 nrRunning;                   // RQ中的总运行任务个数
    U32 intCount;                    // 中断进入次数
    U32 tickNoRespondCnt;            // TICK待响应次数
    U16 uniTaskLock;                 // 锁任务计数
    U16 reserved;
    U32 shakeCount;                  // 核间握手计数
    bool online;                     // 队列是否还在线
    U32 currntPrio;                  // RQ中最高优先级任务的优先级
    struct RtRq rtRq;                // 实时优先级运行队列
    struct TagOsSchedDomain  *domain;    // 本核运行队列的根域
#if defined(OS_OPTION_FIBER)
    struct OsFiberMng *fiberMng;     // 纤程管理控制块
#endif
};

/*
 * 模块间全局变量
 */
extern struct TagOsRunQue g_runQueue[OS_MAX_CORE_NUM];

#define OS_SMP_HANDSHAKE_COUNT(core) (GET_RUNQ(core)->shakeCount)

/*
 * 模块间函数定义
 */
extern void OsReschedTask(struct TagTskCB *task);
extern void OsReschedTaskNoWakeIpc(struct TagTskCB *task);
extern void OsPutPrevTask(struct TagOsRunQue *runQue, struct TagTskCB *taskPrev);
extern void OsSmpSendReschedule(U32 coreID);

extern U32 g_tskMaxNum;

/* WARNING: 下面的几个函数一定都是在关中断下进行的，关中断由外部保证 */
OS_SEC_ALW_INLINE INLINE void OsDoubleRqLock(struct TagOsRunQue *thisRq, struct TagOsRunQue *busiestRq)
{
    if (thisRq < busiestRq) {
        OsSplLock(&thisRq->spinLock);
        OsSplLock(&busiestRq->spinLock);
    } else if (thisRq > busiestRq) {
        OsSplLock(&busiestRq->spinLock);
        OsSplLock(&thisRq->spinLock);
    } else {
        // equal
        OsSplLock(&thisRq->spinLock);
    }
    return;
}

/* 避免死锁，锁住两个rq */
OS_SEC_ALW_INLINE INLINE void OsDoubleLockBalance(struct TagOsRunQue *thisRq, struct TagOsRunQue *busiestRq)
{
    // 这里可以加个检测，如果thisrq还没关中断，报错 ...
    // 只解锁，不开中断
    OsSplUnlock(&thisRq->spinLock);
    OsDoubleRqLock(thisRq, busiestRq);
}

/* double解锁 */
OS_SEC_ALW_INLINE INLINE void OsDoubleUnLockBalance(struct TagOsRunQue *thisRq, struct TagOsRunQue *busiestRq)
{
    OsSplUnlock(&busiestRq->spinLock);
    if (thisRq != busiestRq) {
        OsSplUnlock(&thisRq->spinLock);
    }
}

/* double解锁，只解destRq */
OS_SEC_ALW_INLINE INLINE void OsDoubleUnLockDest(struct TagOsRunQue *thisRq, struct TagOsRunQue *destRq)
{
    if (thisRq != destRq) {
        OsSplUnlock(&destRq->spinLock);
    }
}

#if defined(OS_OPTION_HOTPLUG_CPU)
extern void OsCpuhpMigrateTasks(U32 coreId);
extern struct TagOsRunQue *OsLockOnlineRq(U32 coreMask, struct TagOsRunQue *runQue);
extern struct TagOsRunQue *OsLockAgentRq(U32 coreID, struct TagOsRunQue *runQue);
#else
#define CPU_AGENT_CPU(cpu) (cpu)
OS_SEC_ALW_INLINE INLINE void OsWorkHandler(void)
{
}
#endif

extern void OsContextSwitch(struct TagTskCB *prev, struct TagTskCB *next);
extern void OsDeactiveTask(struct TagOsRunQue *runQue, struct TagTskCB *tsk, U32 flags);
extern void OsActiveTask(struct TagOsRunQue *runQue, struct TagTskCB *tsk, U32 flags);
extern struct TagTskCB *OsPickNextTask(struct TagOsRunQue *runQue);
extern U32 OsSchedInit(void);
extern void OsSwitchTgrpSpace(uintptr_t mmuTbl, U32 contextId);
extern bool OsTskSchedNextRtTask(U32 core, TskHandle *tid, U32 *coreId);
extern void OsDequeueTask(struct TagOsRunQue *runQue, struct TagTskCB *tsk, U32 sleep);
extern void OsEnqueueTask(struct TagOsRunQue *runQue, struct TagTskCB *tsk, U32 flags);
extern void OsTskMoveDeque(struct TagOsRunQue *rq, struct TagTskCB *task);
extern void OsTskMoveEnque(struct TagOsRunQue *rq, struct TagTskCB *task);

// 后续用钩子
extern void OsMainSchedule(void);

OS_SEC_ALW_INLINE INLINE void OsRescheduleCurr(struct TagOsRunQue *runQue)
{
    runQue->needReschedule = TRUE;
}

/* 将运行队列的任务计数++ */
OS_SEC_ALW_INLINE INLINE void OsIncNrRunning(struct TagOsRunQue *runQue)
{
    runQue->nrRunning++;
}

/* 将运行队列的任务计数-- */
OS_SEC_ALW_INLINE INLINE void OsDecNrRunning(struct TagOsRunQue *runQue)
{
    // rq上的idle是永远存在的，nrrunning会大于1(至少一个idle)
    runQue->nrRunning--;
}

#if defined(OS_OPTION_TASK_MIGRATE)
OS_SEC_ALW_INLINE INLINE bool OsSchedIsDomainTypeValid(enum OsScheduleType type)
{
    return ((type == OS_SCHEDULE_RT) || (type == OS_SCHEDULE_RT_SINGLE));
}
#else
OS_SEC_ALW_INLINE INLINE bool OsSchedIsDomainTypeValid(enum OsScheduleType type)
{
    return (type == OS_SCHEDULE_RT_SINGLE);
}
#endif

#if defined(OS_OPTION_TASK_MIGRATE)
extern struct TagScheduleClass *OsGetRtSchedClass(void);
#endif
extern struct TagScheduleClass *OsGetRtSingleSchedClass(void);

OS_SEC_ALW_INLINE INLINE struct TagScheduleClass *OsSchedGetSchedClass(enum OsScheduleType type)
{
#if defined(OS_OPTION_TASK_MIGRATE)
    if (type == OS_SCHEDULE_RT) {
        return OsGetRtSchedClass();
    }
#endif
    (void)type;
    return OsGetRtSingleSchedClass();
}
#endif

#endif /* SRE_SCHED_EXTERNAL_H */
