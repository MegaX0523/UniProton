/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Tick模块的头文件, 用户不能直接包含。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_TICK_H
#define OS_TICK_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_tick
 * Tick中断错误码：tick在申请硬件定时器物理资源时资源不足，申请不到。
 *
 * 值: 0x02000b02
 *
 * 解决方案:确保有足够的定时器物理资源
 */
#define OS_ERRNO_TICK_NO_HWTIMER OS_ERRNO_BUILD_ERROR(OS_MID_TICK, 0x02)

/*
 * @ingroup OS_tick
 * Tick中断错误码：Cortex-AX平台在调整Tick计数值时，输入的补偿值总大小为负值且绝对值大于正常的Tick计数值。
 *
 * 值: 0x02000b03
 *
 * 解决方案:确保调整Tick计数值时，输入的补偿值总大小为负值时且它的绝对值不能大于正常的Tick计数值
 */
#define OS_ERRNO_TICK_ADJUST_VALUE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_TICK, 0x03)

/*
 * @ingroup OS_tick
 * Tick中断错误码：Cortex-MX tick的周期错误
 *
 * 值: 0x02000b04
 *
 * 解决方案:确认经过转换后的tick周期(多少cycle/tick)是否区间(0,0x00FFFFFF]内。
 */
#define OS_ERRNO_TICK_PERIOD OS_ERRNO_BUILD_ERROR(OS_MID_TICK, 0x04)

/*
 * @ingroup OS_tick
 * Tick中断错误码：每秒的Tick中断计数配置不合法。
 *
 * 值: 0x02000b05
 *
 * 解决方案:确保配置的每秒Tick中断计数小于系统时钟数，并且不等于0。
 */
#define OS_ERRNO_TICK_PER_SECOND_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_TICK, 0x05)

/*
 * @ingroup OS_tick
 * tick补偿错误码：tick补偿模式不对。
 *
 * 值: 0x02000b06
 *
 * 解决方案:检查tick补偿模式是否是浅睡或深睡模式
 */
#define OS_ERRNO_TICK_ADJUST_MODE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_TICK, 0x06)

/*
 * @ingroup OS_tick
 * Tick中断错误码：用户注册的Tick中断钩子为空。
 *
 * 值: 0x02000b07
 *
 * 解决方案: 请保证用户注册的Tick中断钩子不能为空。
 */
#define OS_ERRNO_TICK_REG_HOOK_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_TICK, 0x07)

/*
 * @ingroup OS_tick
 * Tick补偿模式。
 *
 */
enum SleepMode {
    OS_TYPE_LIGHT_SLEEP = 0, /* 浅睡唤醒补偿方式 */
    OS_TYPE_DEEP_SLEEP, /* 深睡唤醒补偿方式 */
    OS_TYPE_INVALID_SLEEP, /* 无效补偿方式 */
};

/*
 * @ingroup OS_tick
 * Tick中断模块配置信息的结构体定义。
 *
 * 保存Tick中断模块的配置项信息。
 */
struct TickModInfo {
    /* Tick中断的优先级配置，仅SD6185 Device核配置支持 */
    U32 tickPriority;
    /* 每秒产生的TICK中断数 */
    U32 tickPerSecond;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_TICK_H */
