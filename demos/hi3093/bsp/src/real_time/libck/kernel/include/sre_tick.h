/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Tick中断模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_TICK_H
#define SRE_TICK_H

#include "sre_base.h"
#include "./common/os_tick.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup  SRE_tick
 * @brief Tick中断用户钩子函数。
 *
 * @par 描述
 * Tick中断处理函数。
 *
 * @attention
 * <ul>
 * <li>当前仅SD6185平台有效。</li>
 * <li>用户调用的Tick中断处理钩子。</li>
 * <li>Tick模块裁剪开关未打开，调用此接口无效。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_tick.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
typedef void (*TickHandleFunc)(void);

/*
 * @ingroup  SRE_tick
 * @brief Tick中断处理函数。
 *
 * @par 描述
 * Tick中断处理函数。
 *
 * @attention
 * <ul>
 * <li>只有在Tick中断源由用户提供，并在Tick中断处理中调用有效。若在非tick中断处理函数中调用，可能引发tick时钟不准确</li>
 * <li>Tick模块裁剪开关未打开，调用此接口无效。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_tick.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern void SRE_TickISR(void);

#if !defined(OS_OPTION_TICKLESS)
/*
 * @ingroup SRE_tick
 * @brief 用户注册Tick处理钩子。
 *
 * @par 描述
 * 注册用户Tick处理钩子。
 * @attention
 * <ul>
 * <li>仅支持SD6185x之后无线平台。</li>
 * <li>当多次注册该钩子时，最后一次注册的钩子生效。</li>
 * <li>注册的钩子函数不能为空，即一旦注册钩子函数，不能通过注册空函数将其取消。</li>
 * </ul>
 *
 * @param hook [IN]  类型#TickHandleFunc，钩子函数。
 *
 * @retval #OS_ERRNO_TICK_REG_HOOK_PTR_NULL 0x02000b07，钩子函数为空
 * @retval #SRE_OK 0x00000000，成功返回。
 *
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件</li></ul>
 * @see 无
 */
extern U32 SRE_TickRegHook(TickHandleFunc hook);
#endif

/*
 * @ingroup  SRE_tick
 * @brief 获取每秒钟对应的Tick数。
 *
 * @par 描述
 * 获取每秒钟时间内触发的Tick中断次数。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param 无。
 *
 * @retval #[0,0xFFFFFFFF]  每秒钟对应的Tick数。
 * @par 依赖
 * <ul><li>sre_tick.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TickCountGet
 */
extern U32 SRE_TickPerSecondGet(void);

/*
 * @ingroup  SRE_tick
 * @brief 调整Tick计数值。
 *
 * @par 描述
 * 调整Tick计数值。
 *
 * @attention
 * <ul><li>该功能接口仅作统计使用（只影响SRE_TickCountGet返回值），不做调度行为控制</li></ul>
 * <ul><li>（即不影响Task Delay，软件定时器，信号量/消息/时间延时等待）。</li></ul>
 *
 * @param tick [IN]  类型#S32，Tick计数补偿值，取值范围[-2^31,2^31 - 1]。
 *
 * @retval #OS_ERRNO_TICK_ADJUST_VALUE_INVALID  0x02000b03    Tick计数总补偿值为负数，并且绝对值大于实际的Tick计数值。
 * @retval #SRE_OK                              0x00000000    调整Tick计数值成功。
 * @par 依赖
 * <ul><li>sre_tick.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TickCountGet
 */
extern U32 SRE_TickCountAdjust(S32 tick);

/*
 * @ingroup  SRE_tick
 * @brief    获取可休眠的Tick数
 *
 * @par 描述
 * 获取可休眠的Tick数。注意，调用者确保不会换核(调用前关中断等方式)
 *
 * @attention
 * <ul><li>8191平台不支持该接口功能</li></ul>
 *
 * @param minTicks [OUT] 类型#U32 *，输出获取到的可休眠的Tick数
 * @param coreId   [OUT] 类型#U32 *，输出最近需要处理tick事务的核号
 *
 * @retval #OS_FAIL  0x00000001    无事务需要响应tick，表示可无限休眠。
 * @retval #SRE_OK   0x00000000    成功获取。
 * @par 依赖
 * <ul><li>sre_tick.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TickCountUpdate
 */
extern U32 SRE_TickLessCountGet(U32 *minTicks, U32 *coreId);

/*
 * @ingroup  SRE_tick
 * @brief 更新Tick计数值。
 *
 * @par 描述
 * 更新Tick计数值。
 *
 * @attention
 * <ul><li>深睡唤醒补偿时，sleepMode为OS_TYPE_DEEP_SLEEP，影响范围同SRE_TickCountAdjust</li></ul>
 * <ul><li>浅睡唤醒补偿时，sleepMode为OS_TYPE_LIGHT_SLEEP，影响SRE_TickCountGet返回值, Task</li></ul>
 * <ul><li>Delay，软件定时器，信号量/消息/时间延时等待。</li></ul>
 * <ul><li>8191平台不支持该接口功能</li></ul>
 *
 * @param  sleepMode [IN] 类型#enum SleepMode，Tick补偿方式。
 * @param  tick      [IN] 类型#S32，Tick计数补偿值，取值范围[-2^31,2^31 - 1]。
 *
 * @retval #OS_ERRNO_TICK_ADJUST_MODE_INVALID   0x02000b06    Tick补偿模式不对，见OS_SLEEP_MODE_E。
 * @retval #OS_ERRNO_TICK_ADJUST_VALUE_INVALID  0x02000b03    Tick计数总补偿值为负数，并且绝对值大于实际的Tick计数值。
 * @retval #SRE_OK                              0x00000000    调整Tick计数值成功。
 * @par 依赖
 * <ul><li>sre_tick.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TickLessCountGet
 */
extern U32 SRE_TickCountUpdate(enum SleepMode sleepMode, S32 ticks);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_TICK_H */
