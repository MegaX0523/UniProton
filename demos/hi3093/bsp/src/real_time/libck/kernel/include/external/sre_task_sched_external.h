/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 任务和调度的公共函数实现私有头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_TASK_SCHED_EXTERNAL_H
#define SRE_TASK_SCHED_EXTERNAL_H

#include "sre_buildef.h"
#include "sre_typedef.h"
#include "sre_task_external.h"
#include "sre_cpu_external.h"

#if defined(OS_OPTION_SMP)
#include "sre_sched_external.h"
#endif

#if defined(OS_OPTION_SMP) && (OS_MAX_CORE_NUM > 1)
// 尝试根据PID获取任务所属RQ锁直到获取为止，涉及任务态的迁移，都使用RQ锁保证互斥
extern void OsSpinLockTaskRq(struct TagTskCB *taskCB);

extern bool OsTrySpinLockTaskRq(struct TagTskCB *taskCB);

/* running任务持有自己的rq锁无需判断，中途不可能换核 */
extern struct TagOsRunQue *OsSpinLockRunTaskRq(void);

extern void OsSpinUnLockRunTaskRq(struct TagOsRunQue *runQue);

// 释放PID所属RQ锁
extern void OsSpinUnlockTaskRq(struct TagTskCB *taskCB);

extern U32 OsTryLockTaskOperating(U32 operate, struct TagTskCB *taskCB, uintptr_t *intSave);

#else

OS_SEC_ALW_INLINE INLINE void OsSpinLockTaskRq(struct TagTskCB *taskCB)
{
    (void)taskCB;
}

OS_SEC_ALW_INLINE INLINE struct TagOsRunQue *OsSpinLockRunTaskRq(void)
{
    return NULL;
}

OS_SEC_ALW_INLINE INLINE void OsSpinUnlockTaskRq(struct TagTskCB *taskCB)
{
    (void)taskCB;
}

OS_SEC_ALW_INLINE INLINE void OsSpinUnLockRunTaskRq(struct TagOsRunQue *runQue)
{
    (void)runQue;
}

OS_SEC_ALW_INLINE INLINE U32 OsTryLockTaskOperating(U32 operate, struct TagTskCB *taskCB, uintptr_t *intSave)
{
    (void)operate;
    (void)taskCB;
    *intSave = OsIntLock();
    return SRE_OK;
}
#endif

#endif /* SRE_TASK_SCHED_EXTERNAL_H */
