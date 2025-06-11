/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 实时调度公共头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_RT_EXTERNAL_H
#define SRE_RT_EXTERNAL_H

#include "sre_plist_external.h"
#include "sre_cpu_pri_external.h"

/*
 * 模块间宏定义
 */
// pendList复用作为就绪链表节点
#define GET_TCB_READY(ptr) LIST_COMPONENT(ptr, struct TagTskCB, pendList)

/*
 * 模块间结构体定义
 */
/*
 * 就绪队列链表
 */
struct RtActiveTskList {
    /* 优先级bit位表 */
    U32 readyPrioBit[OS_GET_WORD_NUM_BY_PRIONUM(OS_TSK_NUM_OF_PRIORITIES)];
    struct TagListObject readyList[OS_TSK_NUM_OF_PRIORITIES];
};

/*
 * 实时运行队列
 */
struct RtRq {
    /* 该队列是否overload */
    bool isOverload;
    /* pushable链表中最高的优先级 */
    U32 nextPrio;
    /* 运行队列上任务的个数 */
    U32 nrRunning;
    /* 运行队列可迁移任务个数 */
    U32 nrMigratory;
    /* ready链表 */
    struct RtActiveTskList activeTsk;
    /* 可push链表 */
    struct PushablTskListHead pushAblList;
};

/*
 * 模块间全局变量声明
 */
/*
 * 模块间函数声明
 */
struct TagOsRunQue;
struct TagTskCB;

extern void OsEnqueueTaskRt(struct TagOsRunQue *runQue, struct TagTskCB *task, U32 flags);
extern void OsDequeueTaskRt(struct TagOsRunQue *runQue, struct TagTskCB *task, U32 flags);
extern void OsPutPrevTaskRt(struct TagOsRunQue *runQue, struct TagTskCB *prevTskCB);
extern struct TagTskCB *OsPickNextTaskRt(struct TagOsRunQue *runQue);
extern struct TagTskCB *OsNextReadyRtTask(struct TagOsRunQue *runQue);

extern void OsEnqueueTaskRtSingle(struct TagOsRunQue *runQue, struct TagTskCB *task, U32 flags);
extern void OsDequeueTaskRtSingle(struct TagOsRunQue *runQue, struct TagTskCB *task, U32 flags);
extern void OsPutPrevTaskRtSingle(struct TagOsRunQue *runQue, struct TagTskCB *prevTskCB);
extern struct TagTskCB *OsPickNextTaskRtSingle(struct TagOsRunQue *runQue);
extern struct TagTskCB *OsNextReadyRtTaskSingle(struct TagOsRunQue *runQue);
#endif /* SRE_RT_EXTERNAL_H */
