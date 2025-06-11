/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: 调用栈模块头文件, 用户不能直接包含
 * Author: LibCK项目组
 * Create: 2020-12-12
 */
#ifndef OS_CALLSTACK_H
#define OS_CALLSTACK_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_callstack
 * 调用栈相关信息错误码：获取函数调用栈传入的参数为NULL
 *
 * 值: 0x02003501
 *
 * 解决方案: 请确保传入的参数不为NULL
 */
#define OS_ERRNO_CALLSTACK_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x01)

/*
 * @ingroup os_callstack
 * 硬件相关信息错误码：获取函数调用栈传入的最大调用层数非法
 *
 * 值: 0x02003502
 *
 * 解决方案: 请确保传入的最大调用层数大于0且小于10
 */
#define OS_ERRNO_CALLSTACK_MAX_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x02)

/*
 * @ingroup os_callstack
 * 调用栈相关信息错误码：添加overlay段的unwind信息时，段的起始或结束地址非法
 *
 * 值: 0x02003503
 *
 * 解决方案: 请确保传入的段结束地址大于起始地址
 */
#define OS_ERRNO_CALLSTACK_PARA_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x03)

/*
 * @ingroup os_callstack
 * 调用栈相关信息错误码：传入的任务ID非法
 *
 * 值: 0x02003504
 *
 * 解决方案: 请确保传入的任务ID合法
 */
#define OS_ERRNO_CALLSTACK_TSK_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x04)

/*
 * @ingroup os_callstack
 * 调用栈相关信息错误码：传入的任务ID未创建
 *
 * 值: 0x02003505
 *
 * 解决方案: 请确保传入的任务ID是已经创建了的任务
 */
#define OS_ERRNO_CALLSTACK_TSK_UNUSED OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x05)

/*
 * @ingroup os_callstack
 * 调用栈相关信息错误码：获取调用栈时暂停任务失败。
 *
 * 值: 0x02003506
 *
 * 解决方案: 获取的过程中任务被删除或者是它核正在运行idle任务。
 */
#define OS_ERRNO_CALLSTACK_TSK_PAUSE_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x06)

/*
 * @ingroup os_callstack
 * 调用栈相关信息错误码：系统内存不足。
 *
 * 值: 0x02003507
 *
 * 解决方案: 增大系统内存配置。
 */
#define OS_ERRNO_CALLSTACK_NO_MEM OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x07)

/*
 * @ingroup os_callstack
 * 调用栈相关信息错误码：传入的任务ID正在运行。
 *
 * 值: 0x02003508
 *
 * 解决方案: 确保任务不在RUNNING态。
 */
#define OS_ERRNO_CALLSTACK_TSK_RUNNING OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x08)

/*
 * @ingroup os_callstack
 * 调用栈相关信息错误码：不支持跨核对由用户配置栈空间的任务进行调用栈推导
 *
 * 值: 0x02003509
 *
 * 解决方案: 在目标任务的运行核上进行调用栈推导
 */
#define OS_ERRNO_CALLSTACK_CROSS_CORE_AND_STACK_USER OS_ERRNO_BUILD_ERROR(OS_MID_CALLSTACK, 0x09)

/* 调用栈解析最大层数 */
#define CALLSTACK_DEPTH_MAX_NUM 10

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_CALLSTACK_H */
