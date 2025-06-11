/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Call stack implementation
 * Author: LibCK项目组
 * Create: 2009-07-16
 */
#ifndef SRE_CALLSTACK_EXTERNAL_H
#define SRE_CALLSTACK_EXTERNAL_H

#include "sre_callstack.h"
#include "sre_task_external.h"
#include "sre_irq_external.h"
#include "sre_exc_external.h"
#include "sre_sys_external.h"

/* 异常时获取发生异常的PID */
OS_SEC_ALW_INLINE INLINE U32 OsCallStackGetCurPid(void)
{
    if (CUR_NEST_COUNT > 0) {
        /* 中断时发生异常 */
        if (OS_INT_COUNT > CUR_NEST_COUNT) {
            return (U32)(-1);
        }
        /* 使用系统栈的场景，注意，异常的时候会将OS_FLG_HWI_ACTIVE和OS_FLG_EXC_ACTIVE置起来 */
        if ((UNI_FLAG & (OS_INT_ACTIVE_MASK & ~(OS_FLG_HWI_ACTIVE | OS_FLG_EXC_ACTIVE))) != 0) {
            return (U32)(-1);
        }
        /* 任务已开始调度 */
        if ((UNI_FLAG & OS_FLG_BGD_ACTIVE) != 0) {
            return RUNNING_TASK->taskPID;
        } else {
            // 任务模块还没开始调度，RUNNING_TASK为NULL
            return (U32)(-1);
        }
    } else {
        return OsGetCurPid();
    }
}

OS_SEC_ALW_INLINE INLINE U32 OsGetCallStackByIdParaCheck(const U32 *maxDepth, const uintptr_t *list, U32 taskId,
                                                         struct TagTskCB **taskCb)
{
    if ((maxDepth == NULL) || (list == NULL)) {
        return OS_ERRNO_CALLSTACK_PTR_NULL;
    }

    if ((*maxDepth == 0) || (*maxDepth > CALLSTACK_DEPTH_MAX_NUM)) {
        return OS_ERRNO_CALLSTACK_MAX_NUM_INVALID;
    }

    if (CHECK_TSK_PID_OVERFLOW(taskId)) {
        return OS_ERRNO_CALLSTACK_TSK_ID_INVALID;
    }

    *taskCb = GET_TCB_HANDLE(taskId);
    if (TSK_IS_UNUSED(*taskCb)) {
        return OS_ERRNO_CALLSTACK_TSK_UNUSED;
    }

#if defined(OS_OPTION_SMP)
    // 业务传入的栈有可能是私有的，不进一步推调用栈
    if (OsTaskStackIsSave(*taskCb) == FALSE) {
        return OS_ERRNO_CALLSTACK_CROSS_CORE_AND_STACK_USER;
    }
#endif
    return SRE_OK;
}

/*
 * 描述: 检查是否就是当前正在运行的任务
 */
OS_SEC_ALW_INLINE INLINE U32 OsCallStackCheckIsSelf(const struct TagTskCB *taskCb, bool *isSelf)
{
#if defined(OS_OPTION_SMP)
    uintptr_t intSave = OsIntLock();
#endif
    bool excFlag = (CUR_NEST_COUNT > 0) ? TRUE : FALSE;
    struct TagTskCB *runningTask = RUNNING_TASK;
    U32 uniFlag = UNI_FLAG;
#if defined(OS_OPTION_SMP)
    OsIntRestore(intSave);
#endif

    // 区分调用场景进行处理
    if (!excFlag) {  // 非异常场景
        if ((taskCb == runningTask) && ((uniFlag & OS_FLG_HWI_ACTIVE) == 0)) {
            *isSelf = TRUE;
            return SRE_OK;
        }
    }

    if ((taskCb->taskStatus & OS_TSK_RUNNING) != 0) {
        return OS_ERRNO_CALLSTACK_TSK_RUNNING;
    }
    *isSelf = FALSE;
    return SRE_OK;
}

#endif /* SRE_CALLSTACK_EXTERNAL_H */
