/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 事件模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_EVENT_H
#define OS_EVENT_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_event
 * 事件等待时间设定：事件最大等待超时值设置。
 */
#define OS_EVENT_WAIT_FOREVER OS_WAIT_FOREVER

/*
 * @ingroup OS_event
 * 事件读取模式：表示期望eventMask中的任何一个事件。
 */
#define OS_EVENT_ANY 0x00000001

/*
 * @ingroup OS_event
 * 事件读取模式：表示期望接收eventMask中的所有事件。
 */
#define OS_EVENT_ALL 0x00000010

/*
 * @ingroup OS_event
 * 事件读取模式：表示等待接收事件。
 */
#define OS_EVENT_WAIT 0x00010000

/*
 * @ingroup OS_event
 * 事件读取模式：表示不等待接收事件。
 */
#define OS_EVENT_NOWAIT 0x00100000

/*
 * @ingroup OS_event
 * 事件错误码：事件读取失败，期望事件没有发生。
 *
 * 值: 0x02002600
 *
 * 解决方案：可使用等待读取事件。
 */
#define OS_ERRNO_EVENT_READ_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x00)

/*
 * @ingroup OS_event
 * 事件错误码：读取事件超时。
 *
 * 值: 0x02002601
 *
 * 解决方案：增大事件读取等待时间，或其他任务给该任务写事件操作。
 */
#define OS_ERRNO_EVENT_READ_TIMEOUT OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x01)

/*
 * @ingroup OS_event
 * 事件错误码：写事件时入参任务ID非法。
 *
 * 值: 0x02002602
 *
 * 解决方案: 请输入合法任务ID。
 */
#define OS_ERRNO_EVENT_TASKID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x02)

/*
 * @ingroup OS_event
 * 事件错误码：写事件时入参任务未创建。
 *
 * 值: 0x02002603
 *
 * 解决方案: 请输入合法任务ID，或先创建任务再对其写事件操作。
 */
#define OS_ERRNO_EVENT_TSK_NOT_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x03)

/*
 * @ingroup OS_event
 * 事件错误码：读事件时EVENTMASK入参非法，入参不能为0。
 *
 * 值: 0x02002604
 *
 * 解决方案: 请输入合法值。
 */
#define OS_ERRNO_EVENT_EVENTMASK_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x04)

/*
 * @ingroup OS_event
 * 事件错误码：只能在任务中进行读事件操作。
 *
 * 值: 0x02002605
 *
 * 解决方案: 请在任务中进行读事件操作。
 */
#define OS_ERRNO_EVENT_READ_NOT_IN_TASK OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x05)

/*
 * @ingroup OS_event
 * 事件错误码
 * 读事件接口中flags入参非法，该入参为（OS_EVENT_ANY，OS_EVENT_ALL）中一个和
 * （OS_EVENT_WAIT，OS_EVENT_NOWAIT）中的一个标识或的结果;
 * OS_EVENT_WAIT模式下，等待时间必须非零。
 *
 * 值: 0x02002606
 *
 * 解决方案: 请输入合法的入参。
 */
#define OS_ERRNO_EVENT_FLAGS_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x06)

/*
 * @ingroup OS_event
 * 事件错误码：在锁任务调度状态下，禁止任务阻塞于读事件。
 *
 * 值: 0x02002607
 *
 * 解决方案: 请解锁任务调度后，再进行读事件。
 */
#define OS_ERRNO_EVENT_READ_IN_LOCK OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x07)

/*
 * @ingroup OS_event
 * 事件错误码：写事件时EVENT入参非法，入参不能为0。
 *
 * 值: 0x02002608
 *
 * 解决方案: 请输入合法值。
 */
#define OS_ERRNO_EVENT_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x08)

/*
 * @ingroup OS_event
 * 事件错误码：只能在任务/CR中进行读事件操作。
 *
 * 值: 0x02002609
 *
 * 解决方案: 请在任务/CR中进行读事件操作。
 */
#define OS_ERRNO_EVENT_RECV_THREAD_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_EVENT, 0x09)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_EVENT_H */
