/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: 低功耗头文件。
 * Author: LibCK项目组
 * Create: 2019-12-18
 * History: 2019-12-18 LibCK项目组  创建
 */
#ifndef OS_LOWPOWER_H
#define OS_LOWPOWER_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_lowpower
 * 硬件相关信息错误码：低功耗保存或者恢复输入的类型不正确。
 *
 * 值: 0x02001a01
 *
 * 解决方案: 请确保传入的类型合法。
 */
#define OS_ERRNO_LOWPOWER_TYPE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LOWPOWER, 0x01)

/*
 * @ingroup os_lowpower
 * 硬件相关信息错误码：低功耗suspend或者Resume GIC时，GIC未初始化。
 *
 * 值: 0x02001a02
 *
 * 解决方案: 低功耗suspend或者Resume GIC时，请保证GIC已经初始化。
 */
#define OS_ERRNO_LOWPOWER_GIC_NOT_INIT OS_ERRNO_BUILD_ERROR(OS_MID_LOWPOWER, 0x02)

/*
 * @ingroup os_lowpower
 * 硬件相关信息错误码：低功耗Suspend GIC时，系统已经Suspend。
 *
 * 值: 0x02001a03
 *
 * 解决方案: 低功耗Suspend GIC时，请保证系统未Suspend。
 */
#define OS_ERRNO_LOWPOWER_GIC_ALREADY_SUSPENDED OS_ERRNO_BUILD_ERROR(OS_MID_LOWPOWER, 0x03)

/*
 * @ingroup os_lowpower
 * 硬件相关信息错误码：低功耗Resume GIC时，系统未suspend。
 *
 * 值: 0x02001a04
 *
 * 解决方案: 低功耗Resume GIC时，请保证系统已经suspend。
 */
#define OS_ERRNO_LOWPOWER_GIC_NOT_SUSPENDED OS_ERRNO_BUILD_ERROR(OS_MID_LOWPOWER, 0x04)

/*
 * @ingroup os_lowpower
 * 硬件相关信息错误码：低功耗Suspend GIC时，存在未处理的中断。
 *
 * 值: 0x02001a05
 *
 * 解决方案: 低功耗Suspend GIC时，请保证系统所有中断已经被处理。
 */
#define OS_ERRNO_LOWPOWER_GIC_PENDING_INTERRUPT OS_ERRNO_BUILD_ERROR(OS_MID_LOWPOWER, 0x05)

/*
 * @ingroup os_lowpower
 * 任务错误码：中断内做cpu hotplug。
 *
 * 值: 0x02004300
 *
 * 解决方案: 不要在中断内做cpu hotplug。
 */
#define OS_ERRNO_CPUHP_INT_ACTIVE OS_ERRNO_BUILD_ERROR(OS_MID_CPUHP, 0x00)

/*
 * @ingroup os_lowpower
 * 任务错误码：不允许执行hotplug。
 *
 * 值: 0x02004301
 *
 * 解决方案: 确认当前没有除能hotplug功能，并且没有hotplug正在执行。
 */
#define OS_ERRNO_CPUHP_NOT_ALLOWED OS_ERRNO_BUILD_ERROR(OS_MID_CPUHP, 0x01)

/*
 * @ingroup os_lowpower
 * 任务错误码：cpu已下线。
 *
 * 值: 0x02004302
 *
 * 解决方案: 确认CPU在线再尝试下电。
 */
#define OS_ERRNO_CPUHP_ALREADY_OFFLINE OS_ERRNO_BUILD_ERROR(OS_MID_CPUHP, 0x02)

/*
 * @ingroup os_lowpower
 * 任务错误码：cpu已在线
 *
 * 值: 0x02004303
 *
 * 解决方案: 确认CPU已下线再尝试上电。
 */
#define OS_ERRNO_CPUHP_ALREADY_ONLINE OS_ERRNO_BUILD_ERROR(OS_MID_CPUHP, 0x03)

/*
 * @ingroup os_lowpower
 * 任务错误码：任务锁调度
 *
 * 值: 0x02004304
 *
 * 解决方案: 确认待下电的CPU没有锁任务调度。
 */
#define OS_ERRNO_CPUHP_TASK_LOCKED OS_ERRNO_BUILD_ERROR(OS_MID_CPUHP, 0x04)

/*
 * @ingroup os_lowpower
 * 任务错误码：无效的核ID
 *
 * 值: 0x02004305
 *
 * 解决方案: 确认拔插的核号在有效范围内。
 */
#define OS_ERRNO_CPUHP_INVALID_CORE_ID OS_ERRNO_BUILD_ERROR(OS_MID_CPUHP, 0x05)

/*
 * @ingroup os_lowpower
 * 任务错误码：无效的核ID
 *
 * 值: 0x02004306
 *
 * 解决方案: 最后一个在线核，不允许下电。
 */
#define OS_ERRNO_CPUHP_LAST_ONLINE_CORE OS_ERRNO_BUILD_ERROR(OS_MID_CPUHP, 0x06)

/*
 * @ingroup os_lowpower
 * 低功耗suspend和resume模块编号。
 */
enum OsLowPowerType {
    LOW_POWER_TYPE_L2CACHE = 0, /* L2 Cache模块ID */
    LOW_POWER_TYPE_GIC, /* GIC模块ID */
    LOW_POWER_TYPE_ALL,
    LOW_POWER_TYPE_BUIT
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_LOWPOWER_H */
