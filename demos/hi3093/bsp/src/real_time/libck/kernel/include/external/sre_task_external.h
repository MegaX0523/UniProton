/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 任务模块的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_TASK_EXTERNAL_H
#define SRE_TASK_EXTERNAL_H

#include "sre_lib_external.h"
#include "sre_list_external.h"
#include "sre_sys_external.h"
#include "sre_hw_external.h"
#include "sre_err_external.h"
#include "sre_plist_external.h"
#include "sre_tick_external.h"
#include "sre_cpu_external.h"
#if defined(OS_OPTION_SMP)
#include "sre_sched_external.h"
#endif
#include "sre_mem_external.h"

#if defined(OS_OPTION_POSIX)
#include "pthread.h"
#define PTHREAD_KEYS_MAX 32
#endif

#if !defined(OS_OPTION_SMP)  // AMP
struct TagOsRunQue {
    U32 taskReadyListBitMap;
#if (OS_TSK_NUM_OF_PRIORITIES >= 32)
    /* 优先级bit位表 */
    U32 tskChildBitMap[OS_GET_WORD_NUM_BY_PRIONUM(OS_TSK_NUM_OF_PRIORITIES)];
#endif
    struct TagListObject readyList[OS_TSK_NUM_OF_PRIORITIES];
};

struct TagOsTskSortedDelayList {
    /* 延时任务链表 */
    struct TagListObject tskList;
};
#else
struct TagOsTskSortedDelayList {
    /* 超时链涉及核间操作，必须有锁 */
    volatile uintptr_t spinLock;

    U32 reserved;
    /* 延时任务链表 */
    struct TagListObject tskList;
    /* 延时链上最近超时的tick刻度 */
    U64 nearestTicks;
#if defined(OS_OPTION_HOTPLUG_CPU)
    /* 链入任务延时链的托管链内，详见插拔核的设计方案 */
    struct TagListObject agentList;
    /* 指向本任务延时链的托管链，未托管时=NULL */
    struct TagListAgent *tskDlyAgent;
    /* 任务延时链托管迁移时，可能漏掉一次处理，新增字段避免该可能，详见插拔核的设计方案 */
    U32 nrNeedProc;
#endif
};

struct TagTskCB;
struct TagOsRunQue;

/* 调度类 */
struct TagScheduleClass {
    void (*osEnqueueTask)(struct TagOsRunQue *runQue, struct TagTskCB *tsk, U32 flags);
    void (*osDequeueTask)(struct TagOsRunQue *runQue, struct TagTskCB *tsk, U32 flags);
    void (*osPutPrevTask)(struct TagOsRunQue *runQue, struct TagTskCB *prevTsk);
    struct TagTskCB *(*osPickNextTask)(struct TagOsRunQue *runQue);
    struct TagTskCB *(*osNextReadyTask)(struct TagOsRunQue *runQue);
};
#endif

struct MsgQHead {
    /* 消息队列中的消息数 */
    U32 msgNum;
    /* 消息队列头 */
    struct TagListObject msgList;
};

/*
 * 任务线程及进程控制块的结构体统一定义, 64字节对齐。
 *
 * stackPointer、taskStatus、priority、topOfStack、taskPID及
 * cpSaveAreaA、cpSaveAreaB、mmuBase、usp、cidr的位置不能变动，汇编中已写死。
 */
struct TagTskCB {
    /* 当前任务的SP */
    void *stackPointer;
    /* 任务状态,后续内部全改成U32 */
    U32 taskStatus;
    /* 任务的运行优先级 */
    TskPrior priority;
    /* 保留字段 */
    U16 reserved2;
#if defined(OS_OPTION_SMP)
    /* 任务正在进行的操作类型 */
    volatile U32 taskOperating;
    /* 用于任务做关键不可打断的操作时不被打断如绑核，挂起，删任务，被删，0可以做这些事，为1不能做 */
    U32 opBusy;
#endif

    /* 任务栈大小 */
    U32 stackSize;
    /* 任务PID：SMP下为任务OS内部索引,AMP下为核号 | 任务索引 */
    TskHandle taskPID;

    /* 任务栈顶 */
    uintptr_t topOfStack;

    /* 任务入口函数 */
    TskEntryFunc taskEntry;
    /* 任务Pend的信号量指针 */
    void *taskSem;
    /* 快速信号量计数 */
    S32 fsemCount;
    /* 任务的参数 */
    uintptr_t args[4];
#if defined(OS_OPTION_VEC_LAZY_SAVE) /* 只有Tensilica平台才有该功能 */
    /* 矢量寄存器缓存地址A */
    void *cpSaveAreaA;
    /* 矢量寄存器缓存地址B */
    void *cpSaveAreaB;
#endif
#if (!defined(OS_OPTION_HUNT) && defined(OS_OPTION_TASK_INFO))
    /* 存放任务名 */
    char name[OS_TSK_NAME_LEN];
#endif
    /* 任务的原始优先级 */
    TskPrior origPriority;
    /* 任务栈配置标记 */
    U16 stackCfgFlg;
    /* 消息队列数 */
    U16 queNum;
    /* 期望接收消息的QID */
    U8 recvQID;
    U8 timeSlice;
    /* 私有数据 */
    uintptr_t privateData;
    /* 指向消息队列数组 */
    struct MsgQHead *msgQ;
    /* 信号量链表指针 */
    struct TagListObject pendList;
    /* 任务延时链表指针 */
    struct TagListObject timerList;
    /* 持有互斥信号量链表 */
    struct TagListObject semBList;
    /* 记录条件变量的等待线程 */
    struct TagListObject condNode;
#if defined(OS_OPTION_EVENT)
    /* 任务事件 */
    U32 event;
    /* 任务事件掩码 */
    U32 eventMask;
#endif

    /* 任务记录的最后一个错误码 */
    U32 lastErr;
#if defined(OS_OPTION_SMP)
    /* reserved */
    U32 smpReserve;
    /* 可push队列链表 */
    struct PushablTskList pushAbleList;
    U32 timeCoreID;
#if defined(OS_OPTION_HOTPLUG_CPU)
    /* 任务只绑一个核时，记录核号 */
    U32 bindCoreID;
    /* 任务可运行的原始核掩码，参见插拔核设计文档 */
    U32 origAllowedMask;
#endif
    /* 该任务可以执行的核bitmap */
    OS_CORE_MASK coreAllowedMask;
    /* 该任务可以执行的核个数 */
    U32 nrCoresAllowed;
    /* 该任务所处的rq挂接的核号 */
    U32 coreID;
    /* 该任务是否在运行队列上(ready) */
    bool isOnRq;
    /* 该任务使用的调度类 */
    struct TagScheduleClass *scheClass;
#endif
#if defined(OS_OPTION_CRE)
    struct TagCrgCtrl *crgCtrl;
#endif

    /* 任务恢复的时间点(单位Tick) */
    U64 expirationTick;
#if defined(OS_OPTION_FUSION_INTEGRATE_API)
#if defined(OS_OPTION_MMU)
    uintptr_t mmuBase;
    U32 contextId; /* context id */
#endif
#endif
#if defined(OS_OPTION_TSK_STACK_TRACE)
    uintptr_t stackPeak;
    U32 reserved1;
#endif
#if defined(OS_OPTION_POSIX)
    /* 当前任务状态 */
    U8 state;
    /* pthread cancel */
    U8 cancelState;
    U8 cancelType;
    U8 cancelPending;
    struct __ptcb *cancelBuf;
    /* exit status */
    void *retval;
    /* count for thread join */
    U16 joinCount;
    /* semaphore for thread join */
    SemHandle joinableSem;
    /* pthread key */
    void *tsd[PTHREAD_KEYS_MAX];
    U32 tsdUsed;
#endif
};

/*
 * 任务信息表节点数据结构
 */
struct TagTskMonNode {
    /* 撑死/饿死的时间点(tick) */
    U64 expiredTick;
    /* 撑死/饿死标记 */
    U32 flag;
    /* 撑死/饿死检测标记 */
    U32 ckFlag;
    /* 检测类型 */
    U32 ckStyles;
    /* 操作该TskMon节点使用的锁,仅对SMP生效；AMP平台 该字段为 保留(对齐)用 */
    volatile U32 spinLock;
};

typedef void (*TskCoresleep)(void);
typedef void (*TaskNameDelFunc)(U32 taskID);
typedef void (*TaskFirstSwitchFunc)(void);
typedef void (*TaskMsgDelFunc)(struct TagTskCB *tskCB);
typedef void (*TaskNameGetFunc)(U32 taskID, char **taskName);
typedef U32 (*TaskNameAddFunc)(U32 taskID, const char *name);
typedef U32 (*SetPriorityCheck)(struct TagTskCB *taskCB, TskPrior taskPrio);
typedef U32 (*OsMsgRecvFromTaskFunc)(U8 queId, U32 timeout, struct TagListObject **list, bool vosFlag);
typedef U32 (*OsMsgSubmitToTaskFunc)(struct TagListObject *list, U32 threadID, U8 queId, U32 flag);

#if !defined(OS_OPTION_SMP)
extern U16 g_uniTaskLock;
extern TskHandle g_idleTaskID;
extern struct TagOsRunQue g_runQueue;
extern struct TagTskCB *g_runningTask;
extern struct TagTskCB *g_highestTask;
extern struct TagOsTskSortedDelayList g_tskSortedDelay;
#else
extern volatile uintptr_t g_createTskLock;
extern struct TagOsTskSortedDelayList g_tskSortedDelay[OS_MAX_CORE_NUM];
extern struct TagListObject g_tskPercpuRecycleList[OS_MAX_CORE_NUM];
#endif

extern U32 g_tskMaxNum;
extern U32 g_tskBaseID;
extern struct TagTskCB *g_tskCBArray;
extern struct TskModInfo g_tskModInfo;
extern struct TagTskMonNode *g_tskMonList;

extern TaskMsgDelFunc g_taskMsgDel; /* 强制释放未接收消息内存钩子，消息初始化时赋初值 */
extern TaskNameAddFunc g_taskNameAdd;
extern TaskNameDelFunc g_taskNameDel;
extern TaskNameGetFunc g_taskNameGet;
extern OsMsgSubmitToTaskFunc g_msgSubmitToTask;
extern OsMsgRecvFromTaskFunc g_msgRecvFromTask;

#if defined(OS_OPTION_MODULARIZATION_LOAD)
extern bool g_taskFirstSwitchFlag;
extern TaskFirstSwitchFunc g_taskFirstSwitch;
#endif

#if defined(OS_OPTION_SEM_PRIO_INVERSION)
extern SetPriorityCheck g_setPriorityCheck;
#endif
#if defined(OS_OPTION_INIT)
#define WORD_PACK(val) (((val) << 24) | ((val) << 16) | ((val) << 8) | (val))
#endif
#define OS_TSK_STACK_CFG_BY_USER 1
#define OS_TSK_STACK_CFG_BY_SYS  0

#define OS_TSK_PARA_0   0
#define OS_TSK_PARA_1   1
#define OS_TSK_PARA_2   2
#define OS_TSK_PARA_3   3

#define OS_TSK_STACK_TOP_MAGIC  0xAAAAAAAA

#define OS_TSK_OP_FREE       0
#define OS_TSK_OP_MIGRATING  (1UL << 0)   // 任务在迁移中，看到delete立即返回
#define OS_TSK_OP_SUSPENDING (1UL << 2)   // 任务在挂起中，看到delete立即返回
#define OS_TSK_OP_RESUMING   (1UL << 3)   // 任务在恢复中，看到delete立即返回
#define OS_TSK_OP_DELETING   (1UL << 9)   // 任务在删除中，必须等待上述迁移、挂起操作完
#define OS_TSK_OP_MOVING     (1UL << 10)  // 任务在迁移/回迁过程中置该标志，确保当前不会被他人操作该任务

#if defined(OS_OPTION_SMP)
#define CPU_TSK_DELAY_BASE(cpu)    (&g_tskSortedDelay[(cpu)])
#define TSK_IDX2PID(taskID)        (taskID)    // ((taskID) + g_tskBaseID)
#define TSK_GET_INDEX(taskID)      (taskID)    // ((taskID) - g_tskBaseID)
#define OS_TASK_LOCK_DATA          (THIS_RUNQ()->uniTaskLock)
#define IDLE_TASK_ID               (GET_RUNQ(THIS_CORE())->tskIdlePID)
#define RUNNING_TASK               (GET_RUNQ(THIS_CORE())->tskCurr)
#define TSK_CORE_GET(taskPID)      (GET_TCB_HANDLE(taskPID)->coreID)
#define TSK_CORE_SET(tsk, coreId)  ((tsk)->coreID = (coreId))
#define TSK_CREATE_DEL_LOCK()      OS_MCMUTEX_LOCK(0, &g_createTskLock)
#define TSK_CREATE_DEL_UNLOCK()    OS_MCMUTEX_UNLOCK(0, &g_createTskLock)
#define OS_MAX_TCB_NUM             (g_tskMaxNum + 1 + g_maxNumOfCores)  // 1个给无效线程，其余的为每个核的预留idle
#define OS_TSK_SUSPEND_READY_BLOCK (OS_TSK_SUSPEND | OS_TSK_CRG_IDLE_SUSPEND | OS_TSK_DELETING | OS_TSK_READY)
#define OS_TSK_UNLOCK()            OsTskUnlock()
#define GET_TASK_RQ(task)          (GET_RUNQ((task)->coreID))
#define OS_TASK_GET_PRI_LIST(prio) (&((struct TagOsRunQue *)THIS_RUNQ())->rtRq.activeTsk.readyList[(prio)])

#define CHECK_TSK_PID_OVERFLOW(taskID) (TSK_GET_INDEX(taskID) >= (OS_MAX_TCB_NUM - 1))
#define CHECK_TSK_PID(taskID)          (TSK_GET_INDEX(taskID) >= g_tskMaxNum)

#define CPU_OVERTIME_SORT_LIST_LOCK(tskDlyBase) \
    OS_MCMUTEX_LOCK(0, &(tskDlyBase)->spinLock)  // 提高性能专用锁，避免多次cpu偏移计算地址
#define CPU_OVERTIME_SORT_LIST_UNLOCK(tskDlyBase) OS_MCMUTEX_UNLOCK(0, &(tskDlyBase)->spinLock)  // 提高性能专用锁

#define OS_TSK_DELAY_LOCKED_DETACH(task)                                                    \
    do {                                                                                    \
        struct TagOsTskSortedDelayList *tmpDlyBase = &g_tskSortedDelay[(task)->timeCoreID]; \
        OS_MCMUTEX_LOCK(0, &tmpDlyBase->spinLock);                                          \
        ListDelete(&(task)->timerList);                                                     \
        OS_MCMUTEX_UNLOCK(0, &tmpDlyBase->spinLock);                                        \
    } while (0)

#if (OS_MAX_CORE_NUM > 1)
#define OS_TSK_OP_SET(taskCB, operate) ((taskCB)->taskOperating |= (operate))
#define OS_TSK_OP_CLR(taskCB, operate) ((taskCB)->taskOperating &= ~(operate))
#else
#define OS_TSK_OP_SET(taskCB, operate) ((void)(taskCB), (void)(operate))
#define OS_TSK_OP_CLR(taskCB, operate) ((void)(taskCB), (void)(operate))
#endif
#if defined(OS_OPTION_HOTPLUG_CPU)
OS_PER_CPU_DECLARE(struct TagListAgent, g_tskDelayAgent);
#define CPU_TSK_DELAY_AGENT(cpu) OS_OTHER_PER_CPU_PTR((cpu), g_tskDelayAgent)
#endif

#else
#define OS_TSK_SUSPEND_READY_BLOCK (OS_TSK_SUSPEND)
#define OS_TASK_LOCK_DATA          g_uniTaskLock
#define IDLE_TASK_ID               g_idleTaskID
#define RUNNING_TASK               g_runningTask
#define GET_RUNQ(core)             (&g_runQueue)
#define CPU_TSK_DELAY_BASE(cpu)    (&g_tskSortedDelay)
#define TSK_IDX2PID(index)         ((index) + g_tskBaseID)
#define TSK_GET_INDEX(taskID)      ((taskID) - g_tskBaseID)
#define TSK_CORE_GET(taskPID)      (GET_COREID(taskPID))
#define TSK_CORE_SET(tsk, coreId)  (void)(tsk)

#define TSK_CREATE_DEL_LOCK()      (void)NULL
#define TSK_CREATE_DEL_UNLOCK()    (void)NULL
#define OS_TSK_UNLOCK()            (OS_TASK_LOCK_DATA--)

/* 内核进程的进程及线程调度控制块使用同一类型 */
#define OS_MAX_TCB_NUM             (g_tskMaxNum + 1 + 1)  // 1个IDLE，1个无效任务

#define CPU_OVERTIME_SORT_LIST_LOCK(tskDlyBase)
#define CPU_OVERTIME_SORT_LIST_UNLOCK(tskDlyBase)
#define OS_TSK_DELAY_LOCKED_DETACH(task)            ListDelete(&(task)->timerList)
#define CHECK_TSK_PID_OVERFLOW(taskID)              (TSK_GET_INDEX(taskID) >= (g_tskMaxNum + 1))
#define CHECK_TSK_PID(taskID)                       (CHECK_TSK_PID_OVERFLOW(taskID) || ((taskID) == IDLE_TASK_ID))
#define TSK_ON_THIS_CORE(taskPID)                   (GET_COREID(taskPID) == OsGetHwThreadId())
#define OS_TASK_GET_PRI_LIST(prio)                  (&g_runQueue.readyList[(prio)])
#endif

/* 定义任务的缺省任务栈大小 */
#define OS_TSK_DEFAULE_STACK_SIZE      g_tskModInfo.defaultSize
#define OS_PST_ZOMBIE_TASK             (&g_tskCBArray[OS_MAX_TCB_NUM - 1])
#define TSK_IS_UNUSED(tsk)             ((tsk)->taskStatus == OS_TSK_UNUSED)
#define TSK_STATUS_TST(tsk, statBit)   (((tsk)->taskStatus & (statBit)) != 0)
#define TSK_STATUS_CLEAR(tsk, statBit) ((tsk)->taskStatus &= ~((U32)(statBit)))
#define TSK_STATUS_SET(tsk, statBit)   ((tsk)->taskStatus |= (statBit))
#define OS_TSK_LOCK()                  (OS_TASK_LOCK_DATA++)
#define GET_TCB_PEND(ptr)              LIST_COMPONENT(ptr, struct TagTskCB, pendList)
#define OS_TCB_CHECK()                 (g_tskCBArray == NULL)
#define GET_TCB_HANDLE(taskPid)        (((struct TagTskCB *)g_tskCBArray) + TSK_GET_INDEX((uintptr_t)(taskPid)))
// 保留一个idle task。最大任务handle为FE，FF表示硬中断线程。
#define MAX_TASK_NUM                   ((1U << OS_TSK_TCB_INDEX_BITS) - 2)  // 254
#define OS_TSK_BLOCK                                                                                            \
        (OS_TSK_DELAY | OS_TSK_PEND | OS_TSK_SUSPEND | OS_TSK_FSEM_PEND | OS_TSK_MSG_PEND | OS_TSK_QUEUE_PEND | \
         OS_TSK_EVENT_PEND | OS_TSK_DELETING)

#if defined(OS_OPTION_SMP)
#define OS_TSK_SPINLOCK() OS_TSK_LOCK()
#define OS_TSK_SPINUNLOCK() OS_TSK_UNLOCK()
#else
#define OS_TSK_SPINLOCK()
#define OS_TSK_SPINUNLOCK()
#endif

extern void OsTaskScan(void);
extern void OsTimeSliceScan(void);
extern void OsTskUnlock(void);
extern void OsTskSchedule(void);
extern void OsTskEntry(TskHandle taskID);
extern void OsTaskExit(struct TagTskCB *tsk);
extern void OsTaskMsgDel(struct TagTskCB *tskCB);
extern void OsTskReadyAdd(struct TagTskCB *task);
extern void OsTskReadyDel(struct TagTskCB *taskCB);
extern void OsTskReadyAddHead(struct TagTskCB *taskCB);
extern void OsTskRqReAddTail(struct TagTskCB *task);
extern void OsTskSwitchHookCaller(U32 prevPID, U32 nextPID);
extern void OsContextSave(struct TagTskCB *taskCB);
extern void OsTskTimerAdd(struct TagTskCB *taskCB, uintptr_t timeout);

extern U32 OsIdleTskAMPCreate(void);
extern U32 OsTskMaxNumGet(void);
extern U32 OsTaskDelete(TskHandle taskPID);
extern U32 OsTskMonSwitchProc(U32 lastTaskID, U32 nextTaskID);
extern U32 OsTaskPrivateDataGetById(TskHandle taskPID, uintptr_t *privateData);
extern U32 OsTaskCreateOnly(TskHandle *taskPID, struct TskInitParam *initParam, bool isSmpIdle);
extern U32 OsMsgSubmitToTask(struct TagListObject *list, U32 threadID, U8 queId, U32 flag);
extern U32 OsMsgQRecvFromTask(U8 queId, U32 timeout, struct TagListObject **list, bool vosFlag);
extern void OsTaskStackPeakCal(struct TagTskCB *taskCB, struct StackInfo *stackInfo);

#if defined(OS_OPTION_MEM_USAGE_DETAIL_GET)
extern void OsGetMemTaskForTask(U32 threadNum, struct TagMemPtInfo *memPt, U16 *maxNum, struct MemTask *memThread);
#endif

#if defined(OS_OPTION_SMP)
#if (OS_MAX_CORE_NUM > 1)
extern U32 OsTaskOperaBegin(struct TagTskCB *runTask, struct TagTskCB *tgtTask);
extern void OsTaskOperaEnd(struct TagTskCB *runTask, struct TagTskCB *tgtTask);
#endif
#else
extern void OsTaskFirstTimeSwitch(void);

/* 如果支持超快速切换 */
#if defined(OS_OPTION_SUP_FAST_SWITCH)
extern void OsTskScheduleFast(void);
extern void OsTskScheduleFastPS(uintptr_t intSave);
#else
OS_SEC_ALW_INLINE INLINE void OsTskScheduleFast(void)
{
    OsTskSchedule();
}

OS_SEC_ALW_INLINE INLINE void OsTskScheduleFastPS(uintptr_t intSave)
{
    (void)intSave;
    OsTskSchedule();
}
#endif

#endif

#if defined(OS_OPTION_SHELL)
extern U32 OsTskPeakNumGet(void);
extern U32 OsTskCurrentNumGet(void);
#endif

/*
 * 模块内内联函数定义
 */
#if defined(OS_OPTION_SMP)
OS_SEC_ALW_INLINE INLINE void OsTskScheduleFast(void)
{
    OsTskSchedule();
}
OS_SEC_ALW_INLINE INLINE void OsTskScheduleFastPS(uintptr_t intSave)
{
    (void)(intSave);
    OsTskSchedule();
}
OS_SEC_ALW_INLINE INLINE bool IsIdleTask(TskHandle task)
{
    return (((task) >= g_tskMaxNum) && ((task) < (g_tskMaxNum + g_maxNumOfCores)));
}
#if (OS_MAX_CORE_NUM == 1)
OS_SEC_ALW_INLINE INLINE U32 OsTaskOperaBegin(struct TagTskCB *runTask, struct TagTskCB *tgtTask)
{
    (void)runTask;
    (void)tgtTask;
    return SRE_OK;
}
OS_SEC_ALW_INLINE INLINE void OsTaskOperaEnd(struct TagTskCB *runTask, struct TagTskCB *tgtTask)
{
    (void)runTask;
    (void)tgtTask;
}
#endif

OS_SEC_ALW_INLINE INLINE void OsTskSetCoreAllowed(U32 coreAllowed, struct TagTskCB *tsk)
{
    tsk->coreAllowedMask = coreAllowed;
    tsk->nrCoresAllowed = OsCoreMaskGetNum(coreAllowed);
}

OS_SEC_ALW_INLINE INLINE void OsSetTaskCoreId(U32 newCoreId, struct TagTskCB *tskCB)
{
    tskCB->coreID = newCoreId;
}

#if defined(OS_OPTION_FIBER)
OS_SEC_ALW_INLINE INLINE bool OsTskUnlockIsNeedResched(const struct TagOsRunQue *rq)
{
    return ((rq->needReschedule) || ((rq->uniFlag & OS_FLG_FIBER_REQ) != 0));
}
#else
OS_SEC_ALW_INLINE INLINE bool OsTskUnlockIsNeedResched(const struct TagOsRunQue *rq)
{
    return rq->needReschedule;
}
#endif

#else
#if (OS_TSK_NUM_OF_PRIORITIES < 32)
OS_SEC_ALW_INLINE INLINE void OsTskHighestSet(void)
{
    U32 rdyListIdx = OsGetLMB1(g_runQueue.taskReadyListBitMap);
    struct TagListObject *readyList = &g_runQueue.readyList[rdyListIdx];

    g_highestTask = GET_TCB_PEND(OS_LIST_FIRST(readyList));
#ifdef OS_OPTION_TASK_TIMESLICE
    if (g_highestTask != RUNNING_TASK && g_highestTask->timeSlice == 0) {
        g_highestTask->timeSlice = OS_TSK_TIMESLICE_TIMEOUT;
    }
#endif
}
#else
OS_SEC_ALW_INLINE INLINE void OsTskHighestSet(void)
{
    U32 rdyListIdx;
    struct TagListObject *readyList;
    U32 childBitMapIdx;

    /* find the highest priority */
    /* get valid Child BitMap according to the ReadyListBitMap */
    childBitMapIdx = OsGetLMB1(g_runQueue.taskReadyListBitMap);

    /* get the ready list task priority idx in the Child BitMap */
    rdyListIdx = OsGetLMB1(g_runQueue.tskChildBitMap[childBitMapIdx]);

    /* get task ready list according to the task priority */
    readyList = &(g_runQueue.readyList[OS_GET_32BIT_ARRAY_BASE(childBitMapIdx) + rdyListIdx]);

    g_highestTask = GET_TCB_PEND(OS_LIST_FIRST(readyList));
#ifdef OS_OPTION_TASK_TIMESLICE
    if (g_highestTask != RUNNING_TASK && g_highestTask->timeSlice == 0) {
        g_highestTask->timeSlice = OS_TSK_TIMESLICE_TIMEOUT;
    }
#endif
}
#endif
OS_SEC_ALW_INLINE INLINE bool IsIdleTask(TskHandle task)
{
    return (task == IDLE_TASK_ID);
}
#endif
#if defined(OS_OPTION_HOTPLUG_CPU)
extern void OsTskReadyAddBGD(struct TagTskCB *task);
#else
OS_SEC_ALW_INLINE INLINE void OsTskReadyAddBGD(struct TagTskCB *task)
{
    OsTskReadyAdd(task);
}
#endif

#if (defined(OS_OPTION_SMP) && (OS_MAX_CORE_NUM > 1))
// 必须是多核SMP场景才有根据flag激活任务、唤醒低功耗核的情形
extern void OsTskReadyAddNoWakeUpIpc(struct TagTskCB *task);
#else
OS_SEC_ALW_INLINE INLINE void OsTskReadyAddNoWakeUpIpc(struct TagTskCB *task)
{
    OsTskReadyAddBGD(task);
}
#endif

/* 当处在任务context时，返回任务的ID; 否则返回-1，表示系统 */
OS_SEC_ALW_INLINE INLINE U32 OsGetCurPid(void)
{
    U32 uniFlag = UNI_FLAG;
    // 不能使用intactive的判断，须强判当前处在任务上下文，否则内存初始化到调度之间的内存申请会空指针引用
    if (LIKELY((uniFlag & OS_THREAD_FLAG_MASK) == OS_FLG_BGD_ACTIVE)) {
        return RUNNING_TASK->taskPID;
    }

#if defined(OS_OPTION_CRE)
    if ((uniFlag & OS_THREAD_FLAG_MASK) == (OS_FLG_BGD_ACTIVE | OS_FLG_CRE_ACTIVE)) {
        return RUNNING_TASK->taskPID;
    }
#endif

    return (U32)(-1);
}

#if defined(OS_OPTION_SMP)
/* Get task's coremask */
OS_SEC_ALW_INLINE INLINE U32 OsTskGetCoreAllowed(TskHandle taskPID)
{
    return GET_TCB_HANDLE(taskPID)->coreAllowedMask;
}
#endif

#if defined(OS_OPTION_CRE)
/* 根据切出的任务类型更新uniflag的OS_FLG_CRE_ACTIVE */
OS_SEC_ALW_INLINE INLINE void OsUpdateCreActiveFlag(struct TagTskCB *next)
{
    struct TagOsRunQue *rq = THIS_RUNQ();
    /* 普通任务 */
    if (next->crgCtrl == NULL) {
        rq->uniFlag &= ~OS_FLG_CRE_ACTIVE;
    } else {   /* CRE */
        rq->uniFlag |= OS_FLG_CRE_ACTIVE;
    }
}
#else
OS_SEC_ALW_INLINE INLINE void OsUpdateCreActiveFlag(struct TagTskCB *next)
{
    (void)next;
}
#endif

#if defined(OS_OPTION_MMU) && defined(OS_OPTION_FUSION_INTEGRATE_API)
/* 任务运行空间切换 */
OS_SEC_ALW_INLINE INLINE void OsSchedSwitchTgrpSpace(struct TagTskCB *next)
{
    /* 目标不是背景实例并且实际context不一样时，才发生切换 */
    if (UNLIKELY(next->contextId != 0) && UNLIKELY(next->contextId != OsGetCurCntxId())) {
        OsSwitchTgrpSpace(next->mmuBase, next->contextId);
    }
}
#else
OS_SEC_ALW_INLINE INLINE void OsSchedSwitchTgrpSpace(struct TagTskCB *next)
{
    (void)next;
}
#endif

#if defined(OS_OPTION_SMP)
OS_SEC_ALW_INLINE INLINE bool OsTaskStackIsSave(const struct TagTskCB *taskCB)
{
    // 如果是用户配置的栈，有可能是私有内存，所以跨核访问具有不确定性
    return !((taskCB->coreID != THIS_CORE()) && (taskCB->stackCfgFlg == OS_TSK_STACK_CFG_BY_USER));
}

OS_SEC_ALW_INLINE INLINE bool OsTaskSpPcInfoReady(const struct TagTskCB *taskCB)
{
    return ((((taskCB->taskStatus & OS_TSK_RUNNING) == 0) || (OS_INT_ACTIVE) || (taskCB->coreID != THIS_CORE())) &&
            OsTaskStackIsSave(taskCB));
}
#else
#define THIS_RUNQ() ((struct TagOsRunQue *)&g_runQueue)
OS_SEC_ALW_INLINE INLINE void OsRescheduleCurr(struct TagOsRunQue *runQue)
{
    (void)runQue;
}

OS_SEC_ALW_INLINE INLINE bool OsTaskSpPcInfoReady(const struct TagTskCB *taskCB)
{
    return (((taskCB->taskStatus & OS_TSK_RUNNING) == 0) || (OS_INT_ACTIVE));
}
#endif

#if defined(OS_OPTION_TICK)
OS_SEC_ALW_INLINE INLINE void OsTaskAttachToTimerList(struct TagTskCB *task, U32 timeOut)
{
    if (timeOut == OS_WAIT_FOREVER) {
        return;
    }

    TSK_STATUS_SET(task, OS_TSK_TIMEOUT);
    OsTskTimerAdd(task, timeOut);
}

OS_SEC_ALW_INLINE INLINE void OsTaskDetachFromTimerList(struct TagTskCB *task)
{
    if (!TSK_STATUS_TST(task, OS_TSK_TIMEOUT)) {
        return;
    }

    OS_TSK_DELAY_LOCKED_DETACH(task);
    TSK_STATUS_CLEAR(task, OS_TSK_TIMEOUT);
}
#else
OS_SEC_ALW_INLINE INLINE void OsTaskAttachToTimerList(struct TagTskCB *task, U32 timeOut)
{
    (void)task;
    (void)timeOut;
}

OS_SEC_ALW_INLINE INLINE void OsTaskDetachFromTimerList(struct TagTskCB *task)
{
    (void)task;
}
#endif

#endif /* SRE_TASK_EXTERNAL_H */
