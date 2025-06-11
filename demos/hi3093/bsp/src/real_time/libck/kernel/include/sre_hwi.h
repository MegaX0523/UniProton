/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 硬中断模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * * @defgroup SRE_hwi 硬中断
 * @ingroup SRE_interrupt
 */

#ifndef SRE_HWI_H
#define SRE_HWI_H

#include "sre_base.h"
#include "sre_hook.h"
#include "./common/os_hwi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_hwi
 * 组合型硬中断。
 */
#define OS_HWI_MODE_COMBINE 0x8000

/*
 * @ingroup OS_hwi
 * 独立型硬中断。
 */
#define OS_HWI_MODE_ENGROSS 0x4000

/*
 * @ingroup SRE_hwi
 * 服务体快速硬中断。
 */
#define OS_HWI_TYPE_FAST 0x01

/*
 * @ingroup SRE_hwi
 * 普通硬中断。
 */
#define OS_HWI_TYPE_NORMAL 0x00

/*
 * @ingroup SRE_hwi
 * 中断属性组装宏(共8bit)。
 */
#define OS_HWI_ATTR(mode, type) (HwiMode)((U32)(mode) | (U32)(type))

/*
 * @ingroup OS_hwi
 * 缺省硬中断模式。
 */
#define OS_HWI_MODE_DEFAULT OS_HWI_MODE_ENGROSS

/*
 * @ingroup SRE_hwi
 * 作为使能或屏蔽所有硬中断函数的入参，仅Tensilica平台有效。
 */
#define OS_HWI_ALL 0x7fff

enum OsHwiStatus {
    OS_HWI_UNCREATED = 0, /* 未创建 */
    OS_HWI_CREATED  = 1, /* 已创建 */
};

/*
 * @ingroup OS_hwi
 * IPI触发类型。
 */
enum OsHwiIpiType {
    OS_TYPE_TRIGGER_BY_MASK = 0, /* 通过mask确定需要触发的目标核 */
    OS_TYPE_TRIGGER_TO_OTHER, /* 触发除本核外的其他核 */
    OS_TYPE_TRIGGER_TO_SELF, /* 触发本核 */
    OS_TYPE_TRIGGER_BUTT /* 非法 */
};

#if (OS_HARDWARE_PLATFORM == OS_CORTEX_RX || OS_HARDWARE_PLATFORM == OS_CORTEX_AX || OS_HARDWARE_PLATFORM == OS_ARMV8)
/*
 * @ingroup OS_hwi
 * 支持的SGI中断编号为[0,15]
 * 可用的核间中断号定义。
 */
#define OS_HWI_IPI_NO_00                                  0

/*
 * @ingroup OS_hwi
 * 可用的核间中断号定义[OS占用1号:触发它核响应一次调度的IPI中断号]。
 */
#define OS_HWI_IPI_NO_01                                  1

/*
 * @ingroup OS_hwi
 * 可用的核间中断号定义[OS占用2号:一个核异常后将其它核停住的IPI中断号]。
 */
#define OS_HWI_IPI_NO_02                                  2

/*
 * @ingroup OS_hwi
 * 可用的核间中断号定义[OS占用3号:响应tick中断的核触发它核的模拟tickIPI中断号]。
 */
#define OS_HWI_IPI_NO_03                                  3

/*
 * @ingroup OS_hwi
 * 可用的核间中断号定义[OS占用4号:核间通信用于核间通知的中断号]。
 */
#define OS_HWI_IPI_NO_04                                  4

/*
 * @ingroup OS_hwi
 * 可用的核间中断号定义。
 */
#define OS_HWI_IPI_NO_05                                  5
#define OS_HWI_IPI_NO_06                                  6
#define OS_HWI_IPI_NO_07                                  7
#define OS_HWI_IPI_NO_08                                  8
#define OS_HWI_IPI_NO_09                                  9
#define OS_HWI_IPI_NO_10                                  10
#define OS_HWI_IPI_NO_011                                 11
#define OS_HWI_IPI_NO_012                                 12
#define OS_HWI_IPI_NO_013                                 13
#define OS_HWI_IPI_NO_014                                 14
#define OS_HWI_IPI_NO_015                                 15

#endif

/*
 * @ingroup SRE_hwi
 * 可用的硬中断优先级的宏定义。
 *
 * Cortex-RX的优先级为0-14，SD6183x的优先级为1-7。
 */
#define SRE_HWI_PRIORITY0 OS_HWI_PRIORITY0
#define SRE_HWI_PRIORITY1 OS_HWI_PRIORITY1
#define SRE_HWI_PRIORITY2 OS_HWI_PRIORITY2
#define SRE_HWI_PRIORITY3 OS_HWI_PRIORITY3
#define SRE_HWI_PRIORITY4 OS_HWI_PRIORITY4
#define SRE_HWI_PRIORITY5 OS_HWI_PRIORITY5
#define SRE_HWI_PRIORITY6 OS_HWI_PRIORITY6
#define SRE_HWI_PRIORITY7 OS_HWI_PRIORITY7
#define SRE_HWI_PRIORITY8 OS_HWI_PRIORITY8
#define SRE_HWI_PRIORITY9 OS_HWI_PRIORITY9
#define SRE_HWI_PRIORITY10 OS_HWI_PRIORITY10
#define SRE_HWI_PRIORITY11 OS_HWI_PRIORITY11
#define SRE_HWI_PRIORITY12 OS_HWI_PRIORITY12
#define SRE_HWI_PRIORITY13 OS_HWI_PRIORITY13
#define SRE_HWI_PRIORITY14 OS_HWI_PRIORITY14

/*
 * @ingroup SRE_hwi
 * 硬中断优先级的类型定义。
 */
typedef U16 HwiPrior;

/*
 * @ingroup SRE_hwi
 * 硬中断模式配置信息的类型定义。
 */
typedef U16 HwiMode;

/*
 * @ingroup  SRE_hwi
 * @brief 设置硬中断属性接口。
 *
 * @par 描述
 * 在创建硬中断前，必须要配置好硬中断的优先级和模式，包括独立型（#OS_HWI_MODE_ENGROSS）和
 * 组合型（#OS_HWI_MODE_COMBINE）两种配置模式。
 *
 * @attention
 * <ul>
 * <li>OS已经占用的不能被使用。</li>
 * <li>Cortex-RX平台下0-511，优先级都0-14。</li>
 * <li>SD6183平台下可用硬中断号为34-51,55-58,64-103,124-127，优先级1-7，数字越大优先级越高。</li>
 * <li>对于SD6185x平台，可用硬中断号为34-44,48-50(HIDSPV200  49不可用，HIAVPV200  51不可用),</li>
 * <li>54,56-58,64-127，优先级1-7，数字越大优先级越高。</li>
 * <li>HI1171平台下可用硬中断优先级1-7，数字越大优先级越高，由于唤醒中断的限制，强烈不建议用户创建优先级为7的中断。</li>
 * <li>8191平台：非安全世界仅支持偶数优先级，即0，2，4...12。</li>
 * </ul>
 *
 * @param hwiNum  [IN]  类型#HwiHandle，硬中断号。
 * @param hwiPrio [IN]  类型#HwiPrior，硬中断优先级。
 * @param mode    [IN]  类型#HwiMode，设置的中断模式，为独立型(#OS_HWI_MODE_ENGROSS)或者组合型(#OS_HWI_MODE_COMBINE)。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID                     0x02001400，中断号非法。
 * @retval #OS_ERRNO_HWI_PRI_ERROR                       0x02001401，优先级非法。
 * @retval #OS_ERRNO_HWI_MODE_ERROR                      0x0200140a，创建的中断函数既不是独立型，也不是组合型。
 * @retval #OS_ERRNO_HWI_ATTR_CONFLICTED                 0x02001406，重复设置时属性冲突。
 * @retval #SRE_OK                                       0x00000000，硬中断创建成功。
 * @par 依赖
 * <ul><li>SRE_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiCreate
 */
extern U32 SRE_HwiSetAttr(HwiHandle hwiNum, HwiPrior hwiPrio, HwiMode mode);

/*
 * @ingroup  SRE_hwi
 * @brief 创建硬中断函数。
 *
 * @par 描述
 * 注册硬中断的处理函数。
 *
 * @attention
 * <ul>
 * <li>在调用该函数之前，请先确保已经设置了中断属性。</li>
 * <li>硬中断创建成功后，并不使能相应向量的中断，需要显式调用#SRE_HwiEnable单独使能。</li>
 * </ul>
 *
 * @param hwiNum  [IN]  类型#HwiHandle，硬中断号。
 * @param handler [IN]  类型#HwiProcFunc，硬中断触发时的处理函数。
 * @param arg     [IN]  类型#HwiArg，调用硬中断处理函数时传递的参数。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID                     0x02001400，中断号非法。
 * @retval #OS_ERRNO_HWI_PROC_FUNC_NULL                  0x02001403，硬中断处理函数为空。
 * @retval #OS_ERRNO_HWI_MODE_UNSET                      0x0200140c，未进行硬中断模式设置。
 * @retval #OS_ERRNO_HWI_MEMORY_ALLOC_FAILED             0x02001408，组合型中断节点或中断描述符申请私有静态内存失败
 * @retval #OS_ERRNO_HWI_COMBINEHOOK_ALREADY_CREATED     0x02001409，组合型中断已创建了相同的中断处理函数。
 * @retval #OS_ERRNO_HWI_ALREADY_CREATED                 0x02001402，硬中断已被创建或相应中断向量号已被其它中断占用。
 * @retval #SRE_OK                                       0x00000000，硬中断创建成功。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiDelete
 */
extern U32 SRE_HwiCreate(HwiHandle hwiNum, HwiProcFunc handler, HwiArg arg);

/*
 * @ingroup  SRE_hwi
 * @brief 删除硬中断函数。
 *
 * @par 描述
 * 屏蔽相应硬中断或事件，取消硬中断处理函数的注册。
 *
 * @attention
 * <ul>
 * <li>不能删除OS占用的中断号。</li>
 * </ul>
 *
 * @param hwiNum [IN]  类型#HwiHandle，硬中断号。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID            0x02001400，中断号非法。
 * @retval #OS_ERRNO_HWI_DELETE_INT             0x02001405，删除os内部硬中断。
 * @retval #OS_ERRNO_HWI_DELETED                0x0200140b，删除未创建或已删除的硬中断。
 * @retval #SRE_OK                              0x00000000，硬中断删除成功。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiCreate
 */
extern U32 SRE_HwiDelete(HwiHandle hwiNum);

/*
 * @ingroup  SRE_hwi
 * @brief 激活指定核号内的硬中断。
 *
 * @par 描述
 * 激活指定核号内的软件可触发的硬中断
 *
 * @attention
 * <ul>
 * <li>支持CORTEX-M4/SD61xx等平台。</li>
 * </ul>
 *
 * @param dstCore [IN]  类型#U32，目标核号。目前只支持指定为本核。
 * @param hwiNum  [IN]  类型#HwiHandle，硬中断号，只支持软件可触发的中断号 （SD6182平台为7和11号中断，
 * SD6183、SD6185平台为124、125、126和127号中断）。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID                0x02001400，中断号非法。
 * @retval #OS_ERRNO_HWI_CORE_ID_INVALID            0x0200140d，输入无效的核号。
 * @retval #OS_ERRNO_HWI_NUM_NOT_SOFT_TRIGGERD      0x0200140e，输入无效的中断号，软件不可触发。
 * @retval #SRE_OK                                  0x00000000，硬中断请求位清除成功。
 * @par 依赖
 * <ul><li>SRE_hwi.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_HwiTrigger(U32 dstCore, HwiHandle hwiNum);

/*
 * @ingroup  SRE_hwi
 * @brief 清空中断请求位。
 *
 * @par 描述
 * 清除所有的中断请求位。即放弃当前已触发中断的的响应。
 *
 * @attention
 * <ul>
 * <li>支持CORTEX-M4/SD61xx等平台，清除所有的中断请求位(对于NMI中断无效)。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>SRE_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiClearPendingBit
 */
extern void SRE_ClearAllPending(void);

/*
 * @ingroup  SRE_hwi
 * @brief 清除硬中断的Pending位。
 *
 * @par 描述
 * 显式清除硬中断或事件的请求位，因为有的硬件响应中断后不会自动清Pending位。
 *
 * @attention
 * <ul>
 * <li>支持CORTEX-M4/SD61xx等平台。</li>
 * </ul>
 *
 * @param hwiNum [IN]  类型#HwiHandle，硬中断号。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID            0x02001400，中断号非法。
 * @retval #SRE_OK                              0x00000000，硬中断请求位清除成功。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiCreate
 */
extern U32 SRE_HwiClearPendingBit(HwiHandle hwiNum);

/*
 * @ingroup  SRE_hwi
 * @brief 查询中断状态。
 *
 * @par 描述
 * 查询中断状态。
 *
 * @attention
 * <ul>
 * <li>只支持Hidsp核。</li>
 * <li>读出的是当前中断的中断状态。</li>
 * </ul>
 *
 * @param hwiNum        [IN]  类型#HwiHandle，依据不同的芯片，硬中断号或中断向量号，见注意事项
 * @param hwiPendingReg [OUT] 类型#U32 *，对应的中断状态1:有中断，0无中断
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID           0x02001400，中断号非法。(针对SD6183、SD6185)
 * @retval #OS_ERRNO_HWI_PARAM_NULL            0x02001411，入参非法。
 * @retval #SRE_OK                             0x00000000，查询成功。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_HwiPendingQuery(HwiHandle hwiNum, U32 *hwiPendingReg);

/*
 * @ingroup  SRE_hwi
 * @brief 屏蔽指定的硬中断。
 *
 * @par 描述
 * 禁止核响应指定硬中断的请求。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param hwiNum [IN]  类型#HwiHandle，依据不同的芯片，硬中断号或中断向量号，见注意事项。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID           0x02001400，中断号非法。(针对Cortex-AX/SD6183/SD6185/SD6186/SD6186L)
 * @retval #SRE_OK                             0x00000000，硬中断去使能成功。(针对Cortex-RX)。
 * @retval #0                                  屏蔽前的中断禁止状态值。(SD6183/SD6185/SD6186/SD6186L)
 * @retval #1                                  屏蔽前的中断使能状态值。(SD6183/SD6185/SD6186/SD6186L)
 * @retval #任意值                             屏蔽前的中断使能寄存器的值。(针对Tensilica)
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiEnable
 */
extern uintptr_t SRE_HwiDisable(HwiHandle hwiNum);

/*
 * @ingroup  SRE_hwi
 * @brief 使能指定的硬中断。
 *
 * @par 描述
 * 允许核响应指定硬中断的请求。
 *
 * @attention
 * <ul>
 * <li>对于不同芯片，此返回值代表的意义有所差异，差异细节见下面返回值说明</li>
 * </ul>
 *
 * @param hwiNum [IN]  类型#HwiHandle，依据不同的芯片，硬中断号或中断向量号，见注意事项。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID           0x02001400，中断号非法(针对Cortex-RX、SD6183、SD6185)。
 * @retval #SRE_OK                             0x00000000，硬中断使能成功。(针对Cortex-RX)。
 * @retval #0                                  屏蔽前的中断禁止状态值(针对SD6183、SD6185)。
 * @retval #1                                  屏蔽前的中断使能状态值(针对SD6183、SD6185)。
 * @retval #任意值                             屏蔽前的中断使能寄存器的值(针对Tensilica)。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiDisable
 */
extern uintptr_t SRE_HwiEnable(HwiHandle hwiNum);

/*
 * @ingroup  SRE_hwi
 * @brief 添加硬中断进入钩子
 *
 * @par 描述
 * 添加硬中断进入钩子。该钩子函数在进入硬中断ISR前被调用。
 *
 * @attention
 * <ul>
 * <li>不同钩子函数间执行的先后顺序，不应当存在依赖关系。</li>
 * <li>不应在钩子函数里调用可能引起线程调度或阻塞的OS接口。</li>
 * <li>最大支持钩子数需静态配置</li>
 * </ul>
 *
 * @param hook [IN]  类型#HwiEntryHook，中断进入钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，添加成功。
 * @retval 其他                          添加失败。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiEntryHookDel | SRE_HookAdd | SRE_HwiExitHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_HwiEntryHookAdd(HwiEntryHook hook)
{
    return SRE_HookAdd(OS_HOOK_HWI_ENTRY, (OsVoidFunc)hook);
}

/*
 * @ingroup  SRE_hwi
 * @brief 删除硬中断进入钩子
 *
 * @par 描述
 * 删除硬中断进入钩子。该钩子函数将停止在进入硬中断ISR前的调用。
 *
 * @attention
 * <ul>
 * <li>不同钩子函数间执行的先后顺序，不应当存在依赖关系。</li>
 * <li>不应在钩子函数里调用可能引起线程调度或阻塞的OS接口。</li>
 * <li>最大支持钩子数需静态配置</li>
 * </ul>
 *
 * @param hook [IN]  类型#HwiEntryHook，中断进入钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，删除成功。
 * @retval 其他                          删除失败。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiEntryHookAdd | SRE_HookDel
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_HwiEntryHookDel(HwiEntryHook hook)
{
    return SRE_HookDel(OS_HOOK_HWI_ENTRY, (OsVoidFunc)hook);
}

/*
 * @ingroup  SRE_hwi
 * @brief 添加硬中断退出钩子
 *
 * @par 描述
 * 添加硬中断退出钩子。该钩子函数在退出硬中断ISR后被调用。
 *
 * @attention
 * <ul>
 * <li>不同钩子函数间执行的先后顺序，不应当存在依赖关系。</li>
 * <li>不应在钩子函数里调用可能引起线程调度或阻塞的OS接口。</li>
 * <li>最大支持钩子数需静态配置</li>
 * </ul>
 *
 * @param hook [IN]  类型#HwiExitHook，中断退出钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，添加成功。
 * @retval 其他                          添加失败。。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiExitHookDel | SRE_HookAdd | SRE_HwiEntryHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_HwiExitHookAdd(HwiExitHook hook)
{
    return SRE_HookAdd(OS_HOOK_HWI_EXIT, (OsVoidFunc)hook);
}

/*
 * @ingroup  SRE_hwi
 * @brief 删除硬中断退出钩子
 *
 * @par 描述
 * 删除硬中断退出钩子。该钩子函数将停止在退出硬中断ISR后的调用。
 *
 * @attention
 * <ul>
 * <li>不同钩子函数间执行的先后顺序，不应当存在依赖关系。</li>
 * <li>不应在钩子函数里调用可能引起线程调度或阻塞的OS接口。</li>
 * <li>最大支持钩子数需静态配置</li>
 * </ul>
 *
 * @param hook [IN]  类型#HwiExitHook，中断退出钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，删除成功。
 * @retval 其他                          删除失败。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiExitHookAdd | SRE_HookDel
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_HwiExitHookDel(HwiExitHook hook)
{
    return SRE_HookDel(OS_HOOK_HWI_EXIT, (OsVoidFunc)hook);
}

/*
 * @ingroup  SRE_hwi
 * @brief 注册硬中断处理程序地址校验钩子
 *
 * @par 描述
 * 硬中断处理程序地址校验钩子。在硬中断发生时首先会调用该钩子函数，该钩子函数返回
 * 成功(TRUE)时才会去执行中断处理程序。
 *
 * @attention
 * <ul>
 * <li>由于钩子函数返回成功时才会执行中断处理程序，所以要保证钩子函数对中断处理程序地址校验的准确性。</li>
 * <li>支持重复注册，以最后一次钩子注册为准。</li>
 * </ul>
 *
 * @param hook [IN]  类型#OsHwiIsrCheckHook，硬中断处理程序地址校验钩子函数。
 *
 * @retval #OS_ERRNO_HOOK_PTR_NULL       0x02001602，钩子指针为空。
 * @retval #SRE_OK                       0x00000000，添加钩子成功。
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_HwiSetIsrCheckHook(OsHwiIsrCheckHook hook);

/*
 * @ingroup  SRE_hwi
 * @brief 触发核间IPI(Inter-Processor Interrupt)中断
 *
 * @par 描述
 * 核间通信用于触发IPI中断使目标核响应中断的接口。
 *
 * @attention
 * <ul>
 * <li>枚举方式列出了CORTEX-RX平台的所有中断，OS占用的作了特殊说明，不支持重复使用。</li>
 * <li>系统开始任务调度后才能调用该接口。</li>
 * </ul>
 *
 * @param type     [IN]  类型#enum OsHwiIpiType，IPI触发类型。
 * @param coreMask [IN]  类型#U32，要求响应的目标核掩码。注意每bit位表示一个对应核号，
 * 例如0核和1核的核掩码为二进制0b'11，即0x3.
 * @param hwiNum   [IN]  类型#U32，IPI触发中断号，RX平台共16个，编号[0,15]。
 *
 * @retval #OS_ERRNO_HWI_TRIGGER_TYPE_INVALID           0x02001414，ARM核间触发中断类型非法。
 * @retval #OS_ERRNO_HWI_TRIGGER_HWINUM_NOT_SGI         0x02001413，ARM核间触发中断号非法。
 * @retval #OS_ERRNO_HWI_TRIGGER_MASK_INVALID           0x02001415，ARM核间触发中断核掩码非法。
 * @retval #SRE_OK                                      0x00000000，触发成功。
 *
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiCreate
 */
extern U32 SRE_HwiMcTrigger(enum OsHwiIpiType type, U32 coreMask, U32 hwiNum);

#if defined(OS_OPTION_SMP)
/*
 * @ingroup  SRE_hwi
 * @brief 中断绑定
 *
 * @par 描述
 * 设置某个中断的响应目标核掩码。
 *
 * @attention
 * <ul>
 * <li>只有全局中断能配置生效，依赖硬件是否支持该中断的路由。</li>
 * </ul>
 *
 * @param hwiNum   [IN]  类型#U32，绑定的中断号。
 * @param coreMask [IN]  类型#U32，要求响应的目标核掩码。注意每bit位表示一个对应核号，
 * 例如绑定0核和1核的核掩码为二进制0b'11，即0x3.
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID                  0x02001400，中断号非法。
 * @retval #OS_ERRNO_HWI_AFFINITY_MASK_INVALID        0x02001416，中断绑定的核掩码非法。
 * @retval #OS_ERRNO_HWI_AFFINITY_HWINUM_SGI          0x02001418，中断绑定的中断号是SGI。
 * @retval #SRE_OK                                    0x00000000，触发成功。
 *
 * @par 依赖
 * <ul><li>sre_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiCreate
 */
extern U32 SRE_HwiSetAffinity(U32 hwiNum, U32 coreMask);
#endif

/*
 * @ingroup  SRE_hwi
 * @brief 获取中断状态接口
 *
 * @par 描述
 * 获取中断状态，独立型中断，直接通过中断号获取；非独立新中断需要通过中断号和回调函数来获取中断状态。
 *
 * @attention
 * <ul>
 * <li>当前本接口仅支持独立型中断状态的获取(创建或未创建)。
 * <li>仅支持ARM7/ARM9。</li>
 * </ul>
 *
 * @param hwiNum  [IN]  类型#HwiHandle，硬中断号。
 * @param handler [IN]  类型#HwiProcFunc，回调函数(无效)。
 * @param status  [OUT] 类型#enum OsHwiStatus *，中断状态。
 *
 * @retval #OS_ERRNO_HWI_NUM_INVALID                     0x02001400，中断号非法。
 * @retval #SRE_OK                                       0x00000000，获取状态成功。
 * @par 依赖
 * <ul><li>SRE_hwi.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwiCreate
 */
extern U32 SRE_HwiGetStatus(HwiHandle hwiNum, HwiProcFunc handler, enum OsHwiStatus *status);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_HWI_H */
