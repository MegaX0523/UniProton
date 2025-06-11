/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 低功耗相关模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * Notes: 当前仅Balong(Coretex R8,tvp)支持。
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_LOWPOWER_H
#define SRE_LOWPOWER_H

#include "sre_base.h"
#include "./common/os_lowpower.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup  SRE_lowpower
 * @brief 低功耗Suspend。
 *
 * @par 描述
 * 根据lpType的类型，进行低功耗Suspend，以保存相关信息。
 *
 * @attention
 * <ul>
 * <li>仅Cortex-R8平台支持此功能。</li>
 * <li>需要在进入低功耗前调用此接口。</li>
 * </ul>
 *
 * @param lowPowerType [IN]  类型#enum OsLowPowerType，低功耗suspend和resume模块类型。
 *
 * @retval #OS_ERRNO_LOWPOWER_TYPE_INVALID            0x02004001，传入的低功耗suspend模块类型不合法。
 * @retval #OS_ERRNO_LOWPOWER_GIC_NOT_INIT            0x02004002，低功耗Suspend GIC时，GIC未初始化。
 * @retval #OS_ERRNO_LOWPOWER_GIC_ALREADY_SUSPENDED   0x02004003，低功耗Suspend GIC时，GIC已经被Suspended。
 * @retval #OS_ERRNO_LOWPOWER_GIC_PENDING_INTERRUPT   0x02004005，低功耗Suspend GIC时，系统存在未处理的中断。
 * @retval #OS_ERRNO_L2_CACHE_NOT_INIT                0x02001114，必须在L2 Cache初始化后调用此接口。
 * @retval #OS_ERRNO_L2_CACHE_ALREADY_SUSPENDED       0x02001116，L2 Cache已经被Suspend。
 * @retval #SRE_OK                                    0x00000000，低功耗Suspend成功。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_LowPowerSuspend(enum OsLowPowerType lowPowerType);

/*
 * @ingroup  SRE_lowpower
 * @brief 低功耗Resume。
 *
 * @par 描述
 * 根据lpType的类型，进行低功耗Resume，以恢复相关信息。
 *
 * @attention
 * <ul>
 * <li>仅Cortex-R8平台支持此功能。</li>
 * <li>需要在唤醒低功耗后调用此接口。</li>
 * </ul>
 *
 * @param lowPowerType [IN]  类型#enum OsLowPowerType，低功耗suspend和resume模块类型。
 *
 * @retval #OS_ERRNO_LOWPOWER_TYPE_INVALID            0x02004001，传入的低功耗Resume模块类型不合法。
 * @retval #OS_ERRNO_LOWPOWER_GIC_NOT_INIT            0x02004002，低功耗Resume GIC时，GIC未初始化。
 * @retval #OS_ERRNO_LOWPOWER_GIC_NOT_SUSPENDED       0x02004004，低功耗Resume GIC时，GIC未Suspended。
 * @retval #OS_ERRNO_L2_CACHE_NOT_INIT                0x02001114，必须在L2 Cache初始化后调用此接口。
 * @retval #OS_ERRNO_L2_CACHE_NOT_SUSPENDED           0x02001117，L2 Cache未Suspend。
 * @retval #SRE_OK                                    0x00000000，低功耗Suspend成功。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_LowPowerResume(enum OsLowPowerType lowPowerType);

#if defined(OS_OPTION_HOTPLUG_CPU)
/*
 * @ingroup  SRE_lowpower
 * @brief 插拔核钩子函数类型定义。
 *
 * @par 描述
 * 用户通过插拔核钩子函数类型分别定义拔除和插入钩子函数，分别在下地、上电流程中调用。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param coreId [IN]  类型#U32，待上电/下电的核号。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef void (*CpuHotplugHook)(U32 coreId);

/*
 * @ingroup  SRE_lowpower
 * @brief 核下电接口。
 *
 * @par 描述
 * 核下电会将中断，定时业务、任务迁移到其他核，下电核不再响应中断，不参与任务调度
 *
 * @attention
 * <ul>
 * <li>函数返回SRE_OK，只是把中断、定时器、任务迁移走了，调用者还需自己等待拔除钩子的调用。</li>
 * </ul>
 *
 * @param coreId [IN]  类型#U32，待下电核的核ID。
 *
 * @retval #OS_ERRNO_CPUHP_INVALID_CORE_ID      0x02004305，无效的核ID；
 * @retval #OS_ERRNO_CPUHP_NOT_ALLOWED          0x02004301，当前不允许做下电操作；
 * @retval #OS_ERRNO_CPUHP_ALREADY_OFFLINE      0x02004302，核已下电；
 * @retval #OS_ERRNO_CPUHP_INT_ACTIVE           0x02004300，中断内调用下电；
 * @retval #OS_ERRNO_CPUHP_TASK_LOCKED          0x02004304，任务锁调度，不能下电
 * @retval #SRE_OK                              0x00000000，下电成功。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpuUp
 */
extern U32 SRE_CpuDown(U32 coreId);

/*
 * @ingroup  SRE_lowpower
 * @brief 核上电接口。
 *
 * @par 描述
 * 核上电会将原来在核上的中断、定时业务回迁，并开始参与任务调度
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param coreId [IN]  类型#U32，待上电核的核ID。
 *
 * @retval #OS_ERRNO_CPUHP_INVALID_CORE_ID      0x02004305，无效的核ID；
 * @retval #OS_ERRNO_CPUHP_NOT_ALLOWED          0x02004301，当前不允许做上电操作；
 * @retval #OS_ERRNO_CPUHP_ALREADY_ONLINE       0x02004303，核已上电；
 * @retval #OS_ERRNO_CPUHP_INT_ACTIVE           0x02004300，中断内调用上电；
 * @retval #OS_ERRNO_CPUHP_TASK_LOCKED          0x02004304，任务锁调度，不能上电
 * @retval #SRE_OK                              0x00000000，上电成功。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpuDown
 */
extern U32 SRE_CpuUp(U32 coreId);

/*
 * @ingroup  SRE_lowpower
 * @brief 插拔核使能/除能接口。
 *
 * @par 描述
 * 控制插拔核功能的使能/除能，默认使能
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param enable [IN]  类型#bool，使能(TRUE) 或 除能(FALSE)。
 *
 * @retval #SRE_OK                              0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_CpuhpEnable(bool enable);

/*
 * @ingroup  SRE_lowpower
 * @brief 注册插拔核钩子。
 *
 * @par 描述
 * 控制插拔核功能的使能/除能，默认使能
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param plugIn [IN]  类型#CpuHotplugHook，插入钩子。
 *
 * @retval #SRE_OK                              0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_CpuhpHookReg(CpuHotplugHook plugIn);

/*
 * @ingroup  SRE_lowpower
 * @brief hotplug的上电后资源回迁。
 *
 * @par 描述
 * hotplug的上电后资源回迁，调用者确保关中断。
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern void SRE_CpuhpRetrieve(void);

/*
 * @ingroup  SRE_lowpower
 * @brief cpu hotplug 判断核是否还在线。
 *
 * @par 描述
 * cpu hotplug 判断核是否还在线。
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param coreId [IN]  类型#U32，待判断核的核ID。
 *
 * @retval #TRUE     0x00000001，在线。
 * @retval #FALSE    0x00000000，不在线。
 * @par 依赖
 * <ul><li>sre_lowpower.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern bool SRE_CpuhpIsCoreOnline(U32 coreId);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_LOWPOWER_H */
