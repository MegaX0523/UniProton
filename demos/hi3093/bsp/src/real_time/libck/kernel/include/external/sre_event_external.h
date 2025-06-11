/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 获取PID模块的模块间头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01, LibCK项目组,  华为规范整改
 */
#ifndef SRE_EVENT_EXTERNAL_H
#define SRE_EVENT_EXTERNAL_H

#include "sre_task_external.h"
#include "sre_event.h"
#include "sre_task_sched_external.h"

OS_SEC_ALW_INLINE INLINE bool OsIsEventNotMatch(U32 flags, U32 event, U32 eventMask, struct TagTskCB *runTsk)
{
    /* 读事件非期望获得所有事件 */
    if ((flags & OS_EVENT_ALL) != 0) {
        TSK_STATUS_SET(runTsk, OS_TSK_EVENT_TYPE);
        if ((eventMask != (event & eventMask))) {
            return TRUE;
        }
    } else {
        TSK_STATUS_CLEAR(runTsk, OS_TSK_EVENT_TYPE);
        if ((event & eventMask) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

#if defined(OS_OPTION_VOS_IPC)

// 需确保与业务保持一致
#define VOS_EVENT_NOWAIT 0x00000001 /* 不等待事件       */
#define VOS_EVENT_WAIT   0x00000000 /* 等待事件         */
#define VOS_EVENT_ANY    0x00000002 /* 等待期望的任意位 */
#define VOS_EVENT_ALL    0x00000000 /* 等待期望的所有位 */

#define VOS_MSG_SYNC_EVENT 0x200
#define MILLISECONDS_PER_TICK 10
#endif

#endif /* SRE_EVENT_EXTERNAL_H */
