/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 硬中断模块的基础头文件，用户不能直接引用
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_HWI_H
#define OS_HWI_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_hwi
 * 硬中断错误码：中断号非法。
 *
 * 值: 0x02001400
 *
 * 解决方案：确保中断号合法，中断号请查看硬件手册。
 */
#define OS_ERRNO_HWI_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x00)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：优先级非法。
 *
 * 值: 0x02001401
 *
 * 解决方案：确保优先级合法。
 */
#define OS_ERRNO_HWI_PRI_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x01)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：硬中断已被创建或相应中断向量号已被其它中断占用。
 *
 * 值: 0x02001402
 *
 * 解决方案：更换中断号
 */
#define OS_ERRNO_HWI_ALREADY_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x02)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：硬中断处理函数为空。
 *
 * 值: 0x02001403
 *
 * 解决方案：传入非空的有效处理函数
 */
#define OS_ERRNO_HWI_PROC_FUNC_NULL OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x03)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：未创建的硬中断被响应。
 *
 * 值: 0x03001404
 *
 * 解决方案：先创建硬中断，然后使能并触发该中断使其得到响应
 */
#define OS_ERRNO_HWI_UNCREATED OS_ERRNO_BUILD_FATAL(OS_MID_HWI, 0x04)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：删除os内部硬中断。
 *
 * 值: 0x02001405
 *
 * 解决方案：不允许删除os内部硬中断
 */
#define OS_ERRNO_HWI_DELETE_INT OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x05)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：设置硬中断属性时，属性设置与之前设置值不一致
 *
 * 值: 0x02001406
 *
 * 解决方案：确认当前设置属性值与之前是否一致。如果确需修改属性，请先删除该硬中断。
 */
#define OS_ERRNO_HWI_ATTR_CONFLICTED OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x06)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：组合型中断创建失败，为组合型中断节点申请系统默认私有FSC内存失败，或申请中断描述信息失败。
 *
 * 值: 0x02001408
 *
 * 解决方案: 增大系统默认私有FSC分区大小
 */
#define OS_ERRNO_HWI_MEMORY_ALLOC_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x08)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：组合型中断函数注册失败，该组合型中断已创建了相同的中断处理函数。
 *
 * 值: 0x02001409
 *
 * 解决方案: 更换中断处理函数
 */
#define OS_ERRNO_HWI_COMBINEHOOK_ALREADY_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x09)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：创建的中断函数即不是独立型，也不是组合型，或者SD6183/SD6185平台配置的中断模式既不是边沿型也不是电平型。
 *
 * 值: 0x0200140a
 *
 * 解决方案: 硬中断模式只能设置为独立型或组合型，另外SD6183/SD6185平台配置的中断模式必须是边沿型或者电平型
 */
#define OS_ERRNO_HWI_MODE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x0a)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：删除未创建或者已经被删除的硬中断。
 *
 * 值: 0x0200140b
 *
 * 解决方案: 删除已创建并且未被删除的硬中断
 */
#define OS_ERRNO_HWI_DELETED OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x0b)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：未进行硬中断模式设置。
 *
 * 值: 0x0200140c
 *
 * 解决方案: 调用中断创建函数前，需要先调用中断模式设置函数，进行模式参数设置
 */
#define OS_ERRNO_HWI_MODE_UNSET OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x0c)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：硬中断触发接口入参错误，输入无效的核号。
 *
 * 值: 0x0200140d
 *
 * 解决方案: 输入本核核号
 */
#define OS_ERRNO_HWI_CORE_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x0d)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：硬中断软件不可触发。
 *
 * 值: 0x0200140e
 *
 * 解决方案: 软件可触发中断号(SD6155:1或18, SD6182/SD6157:7, SD6183x:[124,127])。
 */
#define OS_ERRNO_HWI_NUM_NOT_SOFT_TRIGGERD OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x0e)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：Cortex-RX硬件上报错误中断，上报的中断号大于0x1FF。
 *
 * 值: 0x02001410
 *
 * 解决方案：无。
 */
#define OS_ERRNO_HWI_HW_REPORT_HWINO_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x10)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：入参为空
 *
 * 值: 0x02001411
 *
 * 解决方案：检查入参是的正确性。
 */
#define OS_ERRNO_HWI_PARAM_NULL OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x11)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：核号非法。
 *
 * 值: 0x02001412
 *
 * 解决方案：确保软件触发中断对应的核号合法。
 */
#define OS_ERRNO_HWI_TRIGGER_CORENUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x12)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：ARM核间触发中断号非法。
 *
 * 值: 0x02001413
 *
 * 解决方案：ARM平台确保软件触发中断为SGI中断。
 */
#define OS_ERRNO_HWI_TRIGGER_HWINUM_NOT_SGI OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x13)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：ARM核间触发中断类型非法。
 *
 * 值: 0x02001414
 *
 * 解决方案：ARM平台确保软件触发中断传入的触发类型正确。
 */
#define OS_ERRNO_HWI_TRIGGER_TYPE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x14)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：ARM核间触发中断核掩码非法。
 *
 * 值: 0x02001415
 *
 * 解决方案：ARM平台确保软件触发中断传入的触发核掩码正确。
 */
#define OS_ERRNO_HWI_TRIGGER_MASK_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x15)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：中断绑定的核掩码非法。
 *
 * 值: 0x02001416
 *
 * 解决方案：确保中断绑定传入的核掩码正确。
 */
#define OS_ERRNO_HWI_AFFINITY_MASK_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x16)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：中断绑定的中断号是SGI。
 *
 * 值: 0x02001418
 *
 * 解决方案：确保中断绑定的中断号不为SGI。
 */
#define OS_ERRNO_HWI_AFFINITY_HWINUM_SGI OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x18)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：设置的硬中断已经被其他进程独占
 *
 * 值: 0x02001419
 *
 * 解决方案：独占该中断的进程需要删除该硬中断后，才能设置。
 */
#define OS_ERRNO_HWI_PROCESS_OWN OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x19)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：该硬中断的拥有权已经被进程释放
 *
 * 值: 0x0200141a
 *
 * 解决方案：不能释放已经无进程拥有权的硬中断。
 */
#define OS_ERRNO_HWI_PROCESS_FREE OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x1a)

/*
 * @ingroup OS_hwi
 * 系统基本功能错误码：关中断超时注册形参非法
 *
 * 值: 0x0200141b
 *
 * 解决方案: 请确保传入的形参非空
 */
#define OS_ERRNO_HWI_INT_LOCK_REG_PARA_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x1b)

/*
 * @ingroup OS_hwi
 * 系统基本功能错误码：注册不可删除的中断失败
 *
 * 值: 0x0300141c
 *
 * 解决方案: 请确保传是独立型中断，或者修改OS_HWI_INTERNAL_NUM值
 */
#define OS_ERROR_HWI_INT_REGISTER_FAILED OS_ERRNO_BUILD_FATAL(OS_MID_HWI, 0x1c)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：配置触发方式时，检查到与硬件规格不一致
 *
 * 值: 0x0200141d
 *
 * 解决方案: 请检查配置模式是否与硬件规格一致
 */
#define OS_ERRNO_HWI_TRIGGER_MODE_INVALID  OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x1d)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：修改触发方式时，中断源依然active
 *
 * 值: 0x0200141e
 *
 * 解决方案: 请在修改触发方式之前，clear中断源
 */
#define OS_ERRNO_HWI_SOURCE_STILL_ACTIVE  OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x1e)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：不支持中断嵌套平台检测到中断嵌套了
 *
 * 值: 0x0300141f
 *
 * 解决方案: 检查中断服务程序是否打开了核中断
 */
#define OS_ERRNO_HWI_NESTED  OS_ERRNO_BUILD_FATAL(OS_MID_HWI, 0x1F)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：调用中断使能接口时检测到不允许操作
 *
 * 值: 0x02001420
 *
 * 解决方案: tsp上检查该中断是否只绑到在已下电DSS了
 */
#define OS_ERRNO_HWI_ENABLE_NOT_PERMITED  OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x20)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：调用绑核接口时检测到不允许操作
 *
 * 值: 0x02001421
 *
 * 解决方案: tsp上检查该中断是否要只绑到已下电DSS了
 */
#define OS_ERRNO_HWI_CORE_BIND_NOT_PERMITED  OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x21)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：中断内存资源申请失败
 *
 * 值: 0x02001422
 *
 * 解决方案: 检查默认分区大小配置是否正确
 */
#define OS_ERRNO_HWI_RESOURCE_ALLOC_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x22)

/*
 * 核间中断错误码：核间中断触发的目的核号无效
 *
 * 值: 0x02001480
 *
 * 解决方案：目的核号非本核，且在有效范围内
 */
#define OS_ERRNO_MCHWI_DST_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x80)

/*
 * 核间中断错误码：中断目标核不支持1-N
 *
 * 值: 0x02001423
 *
 * 解决方案：目标核掩码只描述1个目标核，不能描述多个目标核
 */
#define OS_ERRNO_MULTI_TARGET_CORE OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x23)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：硬中断调度模式设置的调度模式值非法
 *
 * 值: 0x02001424
 *
 * 解决方案: 检查硬中断调度模式设置的调度模式参数设置的合理性
 */
#define OS_ERROR_HWI_SCHED_MODE_SET_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x24)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：硬中断调度模式设置的硬中断号非法
 *
 * 值: 0x02001425
 *
 * 解决方案: 硬中断调度模式设置的硬中断号只能是MSGQ1/MSGQ2对应的中断号
 */
#define OS_ERROR_HWI_SCHED_SET_HWI_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x25)

/*
 * @ingroup OS_hwi
 * 硬中断错误码：硬中断地址信息配置错误
 *
 * 值: 0x02001426
 *
 * 解决方案: 根据核手册正确配置OS_GIC_BASE_ADDR/OS_GICR_OFFSET/OS_GICR_STRIDE
 */
#define OS_ERROR_HWI_BASE_ADDR_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HWI, 0x26)

/*
 * @ingroup OS_hwi
 * 可用的硬中断优先级的宏定义。
 *
 * Cortex-RX的优先级为0-14
 * SD6183x的优先级为1-7
 * HITSP合法的优先级为1-30
 * HITVP合法的优先级为1-14
 *
 */
/* ************************************************* */
#define OS_HWI_PRIORITY0  0
#define OS_HWI_PRIORITY1  1
#define OS_HWI_PRIORITY2  2
#define OS_HWI_PRIORITY3  3
#define OS_HWI_PRIORITY4  4
#define OS_HWI_PRIORITY5  5
#define OS_HWI_PRIORITY6  6
#define OS_HWI_PRIORITY7  7
#define OS_HWI_PRIORITY8  8
#define OS_HWI_PRIORITY9  9
#define OS_HWI_PRIORITY10 10
#define OS_HWI_PRIORITY11 11
#define OS_HWI_PRIORITY12 12
#define OS_HWI_PRIORITY13 13
#define OS_HWI_PRIORITY14 14
#define OS_HWI_PRIORITY15 15
#define OS_HWI_PRIORITY16 16
#define OS_HWI_PRIORITY17 17
#define OS_HWI_PRIORITY18 18
#define OS_HWI_PRIORITY19 19
#define OS_HWI_PRIORITY20 20
#define OS_HWI_PRIORITY21 21
#define OS_HWI_PRIORITY22 22
#define OS_HWI_PRIORITY23 23
#define OS_HWI_PRIORITY24 24
#define OS_HWI_PRIORITY25 25
#define OS_HWI_PRIORITY26 26
#define OS_HWI_PRIORITY27 27
#define OS_HWI_PRIORITY28 28
#define OS_HWI_PRIORITY29 29
#define OS_HWI_PRIORITY30 30

/*
 * @ingroup os_hwi
 * 硬中断处理函数的参数类型定义。
 */
typedef uintptr_t HwiArg;

/*
 * @ingroup os_hwi
 * 硬中断号的类型定义。
 */
typedef U32 HwiHandle;

/*
 * @ingroup os_hwi
 * @brief 硬中断处理函数的类型定义。
 *
 * @par 描述
 * 通过硬中断处理函数的类型定义硬中断处理函数，在硬中断触发时调用该中断处理函数。
 *
 * @attention 无。
 *
 * @param  param1 [IN] 类型#HwiArg，硬中断处理函数的参数。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>os_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef void (*HwiProcFunc)(HwiArg);

/*
 * @ingroup  os_hwi
 * @brief 硬中断调用处理函数钩子函数类型定义。
 *
 * @par 描述
 * 用户通过硬中断调用钩子处理函数类型定义硬中断调用处理函数钩子，在硬中断调用处理函数时，调用该钩子。
 * @attention 无。
 *
 * @param hwiNum [IN]  类型#U32，硬中断号。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>os_hwi.h：该接口声明所在的头文件。</li></ul>
 */
typedef void (*HwiEntryHook)(U32 hwiNum);

/*
 * @ingroup  os_hwi
 * @brief 硬中断退出处理函数钩子函数类型定义。
 *
 * @par 描述
 * 用户通过硬中断退出钩子处理函数类型定义硬中断退出处理函数钩子，在硬中断退出处理函数时，调用该钩子。
 * @attention 无。
 *
 * @param hwiNum [IN]  类型#U32，硬中断号。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>os_hwi.h：该接口声明所在的头文件。</li></ul>
 */
typedef void (*HwiExitHook)(U32 hwiNum);

/* 硬中断处理程序地址校验钩子 */
typedef bool (*OsHwiIsrCheckHook)(HwiProcFunc isr);

/* 中断绑核钩子, coreMask为虚拟核掩码 */
typedef U32 (*OsHwiCoreBindHook)(U32 hwiNum, U32 coreMask);

/* 中断使能、去使能钩子，flag为1表示使能接口时调用，为0时表示去使能接口时调用 */
typedef U32 (*OsHwiEnableHook)(U32 hwiNum, U32 flag);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_HWI_H */
