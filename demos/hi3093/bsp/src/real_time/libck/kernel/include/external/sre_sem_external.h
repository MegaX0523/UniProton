/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 信号量模块内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01, LibCK项目组,  华为规范整改
 */
#ifndef SRE_SEM_EXTERNAL_H
#define SRE_SEM_EXTERNAL_H

#include "sre_task_external.h"
#include "sre_sem.h"
#if defined(OS_OPTION_SMP)
#include "sre_raw_spinlock_external.h"
#endif
#if defined(OS_OPTION_POSIX)
#include "bits/semaphore_types.h"
#endif
/* 模块间宏定义 */
#if defined(OS_OPTION_SMP)
extern volatile uintptr_t g_semPrioLock;
#define SEM_CB_LOCK(sem) OS_MCMUTEX_LOCK(0, &(sem)->semLock)
#define SEM_CB_UNLOCK(sem) OS_MCMUTEX_UNLOCK(0, &(sem)->semLock)
#define SEM_CB_IRQ_LOCK(sem, intSave) \
    do {                              \
        (intSave) = OsIntLock();      \
        SEM_CB_LOCK(sem);             \
    } while (0)
#define SEM_CB_IRQ_UNLOCK(sem, intSave) \
    do {                                \
        SEM_CB_UNLOCK(sem);             \
        OsIntRestore(intSave);          \
    } while (0)
#define SEM_INIT_IRQ_LOCK(intSave) OS_MCMUTEX_IRQ_LOCK(0, &g_mcInitGuard, (intSave))
#define SEM_INIT_IRQ_UNLOCK(intSave) OS_MCMUTEX_IRQ_UNLOCK(0, &g_mcInitGuard, (intSave))
#define SEM_INIT_LOCK() OS_MCMUTEX_LOCK(0, &g_mcInitGuard)
#define SEM_INIT_UNLOCK() OS_MCMUTEX_UNLOCK(0, &g_mcInitGuard)
OS_SEC_ALW_INLINE INLINE void OsSemPrioLock(void)
{
    OS_MCMUTEX_LOCK(0, &g_semPrioLock);
}
OS_SEC_ALW_INLINE INLINE void OsSemPrioUnLock(void)
{
    OS_MCMUTEX_UNLOCK(0, &g_semPrioLock);
}
#define SEM_IF_PRIO_LOCK(sem)                   \
    do {                                        \
        if ((sem)->semMode == SEM_MODE_PRIOR) { \
            OsSemPrioLock();                    \
        }                                       \
    } while (0)
#define SEM_IF_PRIO_UNLOCK(sem)                 \
    do {                                        \
        if ((sem)->semMode == SEM_MODE_PRIOR) { \
            OsSemPrioUnLock();                  \
        }                                       \
    } while (0)
#else
#define SEM_CB_LOCK(sem) (void)(sem)
#define SEM_CB_UNLOCK(sem) (void)(sem)
#define SEM_CB_IRQ_LOCK(sem, intSave) \
    do {                              \
        (void)(sem);                  \
        ((intSave) = OsIntLock());    \
    } while (0)
#define SEM_CB_IRQ_UNLOCK(sem, intSave) \
    do {                                \
        (void)(sem);                    \
        (OsIntRestore(intSave));        \
    } while (0)
#define SEM_INIT_IRQ_LOCK(intSave) ((intSave) = OsIntLock())
#define SEM_INIT_IRQ_UNLOCK(intSave) (OsIntRestore(intSave))
#define SEM_INIT_LOCK() (void)NULL
#define SEM_INIT_UNLOCK() (void)NULL
OS_SEC_ALW_INLINE INLINE void OsSemPrioLock(void)
{
}
OS_SEC_ALW_INLINE INLINE void OsSemPrioUnLock(void)
{
}
#define SEM_IF_PRIO_LOCK(sem)
#define SEM_IF_PRIO_UNLOCK(sem)
#endif

#define OS_SEM_MAX_U64 0xFFFFFFFFFFFFFFFFULL
#define MAX_POSIX_SEMAPHORE_NAME_LEN 31
#define OS_SEM_UNUSED 0
#define OS_SEM_USED 1

#define OS_SEM_WITH_LOCK_FLAG 1
#define OS_SEM_WITHOUT_LOCK_FLAG 0

#define GET_SEM_LIST(ptr) LIST_COMPONENT(ptr, struct TagSemCB, semList)
#define GET_SEM(semid) (((struct TagSemCB *)g_allSem) + (semid))
#define GET_SEM_TSK(semid) (((SEM_TSK_S *)g_semTsk) + (semid))
#define GET_TSK_SEM(tskid) (((TSK_SEM_S *)g_tskSem) + (tskid))

/* 模块间数据结构定义 */
enum OsSemTypeEnum {
    SEM_T_COUNT = 0, /* 信号量为计数模式 */
    SEM_T_BIN_SYNC = 1, /* 信号量为二进制同步模式 */
    SEM_T_BIN_MUTEX = 2 /* 信号量为二进制互斥模式 */
};

struct TagSemCB {
    // 支持SMP
#if defined(OS_OPTION_SMP)
    /* 信号量控制块的锁，放第一个不能动 */
    volatile uintptr_t semLock;
#endif
    /* 是否使用 OS_SEM_UNUSED/OS_SEM_USED */
    U16 semStat;
    /* 核内信号量索引号 */
    U16 semID;
#if defined(OS_OPTION_SEM_RECUR_PV)
    /* 二进制互斥信号量递归P计数，计数型信号量和二进制同步模式信号量无效 */
    U32 recurCount;
#endif
    /* 当该信号量已用时，其信号量计数 */
    U32 semCount;
    /* 挂接阻塞于该信号量的任务 */
    struct TagListObject semList;
    union {
        /* 计数型信号量最大计数，二进制信号量无效 */
        U32 maxSemCount;
        /* 挂接任务持有的互斥信号量，计数型信号量和二进制同步模式信号量无效 */
        struct TagListObject semBList;
    };
    /* Pend到该信号量的线程ID */
    U32 semOwner;
    /* 信号量唤醒阻塞任务的方式 */
    enum SemMode semMode;
    /* 信号量，计数型或二进制 */
    enum OsSemTypeEnum semType;
#if defined(OS_OPTION_POSIX)
    /* 信号量名称 */
    char name[MAX_POSIX_SEMAPHORE_NAME_LEN + 1]; // + \0
    /* sem_open 句柄 */
    sem_t handle;
#endif
};

#if defined(OS_OPTION_SEM_DBG)
struct TagSemOwner {
    // 记录信号量的创建者
    U32 semCreaterPID;
    // 记录信号量创建时传入的cookie(R5版本由OS记录LR，R6版本由用户传入cookie)
    U32 semCreaterLR;
};

extern U16 OsSemGetPeakNum(void);
extern U16 OsSemGetCurrentNum(void);
#endif

/* 模块间全局变量声明 */
extern U16 g_maxSem;

/* 指向核内信号量控制块 */
extern struct TagSemCB *g_allSem;

/* 模块间函数声明 */
extern U32 OsSemCMaxCountSet(SemHandle semHandle, U32 maxCount);

extern U32 OsSemPend(SemHandle semHandle, U64 timeout);
OS_SEC_ALW_INLINE INLINE U16 OsSemGetMaxSemNum(void)
{
    return g_maxSem;
}

extern U32 OsSemBCreate(U32 count, SemHandle *semHandle, enum SemMode mode, U32 cookie);
extern U32 OsSemCCreate(U32 count, SemHandle *semHandle, enum SemMode mode, U32 cookie);

#endif /* SRE_SEM_EXTERNAL_H */
