/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 队列模块内部头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01, LibCK项目组,  华为规范整改
 */
#ifndef SRE_QUEUE_EXTERNAL_H
#define SRE_QUEUE_EXTERNAL_H

#include "sre_queue.h"
#include "sre_list_external.h"
#if defined(OS_OPTION_SMP)
#include "sre_raw_spinlock_external.h"
#else
#include "sre_cpu_external.h"
#endif

/* 模块间宏定义 */
#define OS_QUEUE_NODE_HEAD_LEN (sizeof(struct QueNode) - 2)

/* 队列状态 */
#define OS_QUEUE_UNUSED 0 /* 队列没有使用 */
#define OS_QUEUE_USED 1 /* 已经被使用 */

#define OS_QUEUE_PID_INVALID 0xFFFF /* 节点未使用 */
#define OS_QUEUE_UINT16_MAX_VALUE 0xFFFF /* 无符号16位数最大值 */

#define GET_QUEUE_HANDLE(queueId) (((struct TagQueCB *)g_allQueue) + (uintptr_t)(queueId))

#if defined(OS_OPTION_SMP)
#define QUEUE_CB_LOCK(queue) OS_MCMUTEX_LOCK(0, &(queue)->queueLock)
#define QUEUE_CB_UNLOCK(queue) OS_MCMUTEX_UNLOCK(0, &(queue)->queueLock)
#define QUEUE_CB_IRQ_LOCK(queue, intSave) OS_MCMUTEX_IRQ_LOCK(0, &(queue)->queueLock, (intSave))
#define QUEUE_CB_IRQ_UNLOCK(queue, intSave) OS_MCMUTEX_IRQ_UNLOCK(0, &(queue)->queueLock, (intSave))
#define QUEUE_INIT_IRQ_LOCK(intSave) OS_MCMUTEX_IRQ_LOCK(0, &g_mcInitGuard, (intSave))
#define QUEUE_INIT_IRQ_UNLOCK(intSave) OS_MCMUTEX_IRQ_UNLOCK(0, &g_mcInitGuard, (intSave))
#define QUEUE_INIT_LOCK() OS_MCMUTEX_LOCK(0, &g_mcInitGuard)
#define QUEUE_INIT_UNLOCK() OS_MCMUTEX_UNLOCK(0, &g_mcInitGuard)
#else
#define QUEUE_CB_LOCK(queue) (void)(queue)
#define QUEUE_CB_UNLOCK(queue) (void)(queue)
#define QUEUE_CB_IRQ_LOCK(queue, intSave) ((intSave) = OsIntLock())
#define QUEUE_CB_IRQ_UNLOCK(queue, intSave) (OsIntRestore(intSave))
#define QUEUE_INIT_IRQ_LOCK(intSave) ((intSave) = OsIntLock())
#define QUEUE_INIT_IRQ_UNLOCK(intSave) (OsIntRestore(intSave))
#define QUEUE_INIT_LOCK() (void)NULL
#define QUEUE_INIT_UNLOCK() (void)NULL
#endif

#define OS_QUEUE_INNER_ID(queueId)  ((queueId) - 1)
#define OS_QUEUE_ID(innerId)  ((innerId) + 1)

#define OS_QUEUE_NAME_LEN   16

/* 队列控制块结构体 */
struct TagQueCB {
#if defined(OS_OPTION_SMP)
    /* 队列锁，放第一个不能动 */
    uintptr_t queueLock;
#endif
#if defined(OS_OPTION_QUEUE_NAME)
    char name[OS_QUEUE_NAME_LEN];
#endif
    /* 队列起始地址指针 */
    U8 *queue;
    /* 队列状态 */
    U16 queueState;
    /* 队列长度(队列节点个数) */
    U16 nodeNum;
    /* 每个节点长度，单位word */
    U16 nodeSize;
    /* 当前队列头下标 */
    U16 queueHead;
    /* 当前队列尾下标 */
    U16 queueTail;
    /* 队列节点使用的峰值 */
    U16 nodePeak;
    /* 队列写资源计数器 */
    U16 writableCnt;
    /* 队列读资源计数器 */
    U16 readableCnt;
    /* 写队列超时LIST */
    struct TagListObject writeList;
    /* 读队列超时LIST */
    struct TagListObject readList;
};

/* 队列节点的数据结构 */
struct QueNode {
    /* 源PID */
    U32 srcPid;
    /* 节点长度 */
    U16 size;
    /* buf是初始化申请的，长度>=1 */
    U8 buf[2];
};

/* 队列最大个数 */
extern U16 g_maxQueue;
extern struct TagQueCB *g_allQueue;
OS_SEC_ALW_INLINE INLINE U16 OsQueGetMaxQueNum(void)
{
    return g_maxQueue;
}
#endif /* SRE_QUEUE_EXTERNAL_H */
