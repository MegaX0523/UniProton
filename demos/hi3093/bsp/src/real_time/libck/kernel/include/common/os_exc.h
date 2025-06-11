/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: LibCK的基础头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_EXC_H
#define OS_EXC_H

#include "sre_buildef.h"
#include "os_errno.h"
#include "os_module.h"

#if ((OS_HARDWARE_PLATFORM == OS_HIDSP) || (OS_HARDWARE_PLATFORM == OS_HIDSPV200) ||     \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV200) || (OS_HARDWARE_PLATFORM == OS_HIAVPV280) || \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV300LITE) || (OS_HARDWARE_PLATFORM == OS_HIDSPV300LITE))
#include "./hw/cpu/os_exc_hidsp.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_RISC220) || (OS_HARDWARE_PLATFORM == OS_DSP220) || \
     (OS_HARDWARE_PLATFORM == OS_HACCC) || (OS_HARDWARE_PLATFORM == OS_DSP100))
#include "./hw/cpu/os_exc_xtensa.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_CORTEX_RX) || (OS_HARDWARE_PLATFORM == OS_CORTEX_AX))
#include "./hw/cpu/os_exc_armv7.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_CORTEX_M7) || (OS_HARDWARE_PLATFORM == OS_CORTEX_M4) || \
     (OS_HARDWARE_PLATFORM == OS_CORTEX_M3))
#include "./hw/cpu/os_exc_armv7_m.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_ARM7)
#include "./hw/cpu/os_exc_arm7_9.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_ARM9)
#include "./hw/cpu/os_exc_arm7_9.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_ARMV8)
#include "./hw/cpu/os_exc_armv8.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_RISCV)
#include "./hw/cpu/os_exc_riscv.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_HITSP_X)
#include "./hw/os_exc_hitsp.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_exc
 * 异常错误码: 获取异常缓冲区信息接口入参为空指针。
 *
 * 值：0x02001c02
 *
 * 解决方案：检查入参指针有效性。
 */
#define OS_ERRNO_EXC_PARA_INVLAID OS_ERRNO_BUILD_ERROR(OS_MID_EXC, 0x2)

/*
 * @ingroup os_exc
 * 异常错误码: 异常模块注册异常钩子函数为空。
 *
 * 值：0x02001c09
 *
 * 解决方案：查看注册异常钩子函数是否为空。
 */
#define OS_ERRNO_EXC_REG_HOOK_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_EXC, 0x9)

/*
 * @ingroup os_exc
 * 异常错误码: 异常注册函数中,double异常栈空间设置太小。
 *
 * 值：0x02001c0a
 *
 * 解决方案：请增大double异常栈空间的长度，使之不小于异常寄存器信息EXC_REGS_S的3倍(double异常,NMI异常,Dbg异常共用)。
 */
#define OS_ERRNO_EXC_REGISTER_DOUBLE_EXC_STACK_TOOSMALL OS_ERRNO_BUILD_ERROR(OS_MID_EXC, 0xa)

/*
 * @ingroup os_exc
 * 异常错误码: 中断优先级掩码被踩，导致系统异常重启。
 *
 * 值：0x03001c0b
 *
 * 解决方案：
 * 产品可以通过&g_intMask找到掩码存放地址，判断是否是以下的值
 * SD6182:0x2BFFB8C1 0xD400043C 0x00000302
 * 如果不是，说明掩码被踩，可在中断优先级掩码所在的内存中打数据断点，定位为何会出现中断优先级掩码被踩的错误。
 */
#define OS_ERRNO_EXC_INTERRUPT_LEVEL_MASK_ERROT OS_ERRNO_BUILD_FATAL(OS_MID_EXC, 0xb)

/*
 * @ingroup os_exc
 * 异常错误码: Hac超时处理钩子未注册。
 *
 * 值：0x03001c12
 *
 * 解决方案：进入此分支有两种情况，一种是Hac超时导致soc中的interrupt寄存器被清，导致中断调度到此异常分支，
 * 一种是中断优先级掩码被踩，导致进入这个分支，然而由于产品没有注册此类异常的钩子，因此进入致命错误处理。
 * 产品可以通过&g_intMask找到掩码存放地址，判断是否是以下的值
 * SD6182:0x2BFFB8C1 0xD400043C 0x00000302
 * 如果掩码都正确，则是hac出现超时自清了。
 */
#define OS_ERRNO_EXC_HAC_EXCEPTION_HOOK_NOREG OS_ERRNO_BUILD_FATAL(OS_MID_EXC, 0x12)

/*
 * @ingroup os_exc
 * 异常错误码: 进程异常信息保存结构体内存申请失败。
 *
 * 值：0x02001c13
 */
#define OS_ERRNO_EXC_INFO_STRUC_MALLOC_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_EXC, 0x13)

/*
 * @ingroup so_exc
 * 异常错误码: 发起单核复位时，OS内部维护的复位硬线程位表实例内有名空间地址未找到
 *
 * 值：0x02001c14
 *
 * 解决方案：确认有名空间配置是否正确，并确认该硬线程所在实例是否已加载完成
 */
#define OS_ERRNO_HW_THREAD_RESET_BITMAP_FIND_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_EXC, 0x14)

/*
 * @ingroup os_exc
 * 异常错误码: 消息白名单功能，本核挂死前给其它运行的核发消息，查找其它运行核有名空间初始化状态变量失败
 *
 * 值：0x02001c15
 *
 * 解决方案：确认有名空间配置是否正确
 */
#define OS_ERRNO_INIT_TRACE_FIND_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_EXC, 0x15)

/*
 * @ingroup os_exc
 * 异常错误码: 打印异常信息时传入的参数为NULL
 *
 * 值：0x02001c16
 *
 * 解决方案：确认参数是否不为NULL
 */
#define OS_ERRNO_EXC_INFO_PARA_NULL OS_ERRNO_BUILD_ERROR(OS_MID_EXC, 0x16)

/*
 * @ingroup os_exc
 * 异常错误码: 异常注册函数中,参数doubleExcStackSize、doubleExcStackStart之和过大，整数溢出。
 *
 * 值：0x02001c17
 *
 * 解决方案：请适当调整double异常栈空间的长度以及起始地址大小。
 */
#define OS_ERRNO_EXC_REGISTER_DOUBLE_EXC_STACK_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_EXC, 0x17)

/*
 * @ingroup os_exc
 * 异常处理函数返回类型:系统在发生异常后(并做完相关处理后)继续运行。
 */
#define OS_EXC_PROC_TYPE_RETURN 0

/*
 * @ingroup os_exc
 * 异常处理函数返回类型:系统在发生异常后(并做完相关处理后)进入死循环，等待重启。
 */
#define OS_EXC_PROC_TYPE_NO_RETURN 1

/*
 * @ingroup os_exc
 * 异常处理函数返回类型:系统在发生异常后(并做完相关处理后)跳过异常指令继续运行。
 */
#define OS_EXC_PROC_TYPE_RETURN_SKIP_INST 2

/*
 * @ingroup os_exc
 * 调用栈解析最大层数。
 */
#define EXC_CSTK_DEAPTH 10

struct ExcInfo;
/*
 * @ingroup os_exc
 * @brief 异常处理函数类型定义。
 *
 * @par 描述
 * 通过异常处理函数类型定义异常处理函数钩子，记录异常信息。
 * @attention
 *
 * @param excInfo [IN]  类型#struct ExcInfo *，异常时寄存器信息。
 *
 * @retval OS_EXC_PROC_TYPE_RETURN，系统在发生异常后(并做完相关处理后)继续运行。
 * @retval OS_EXC_PROC_TYPE_NO_RETURN，系统在发生异常后(并做完相关处理后)进入死循环，等待重启。
 * @retval OS_EXC_PROC_TYPE_RETURN_SKIP_INST，系统在发生异常后(并做完相关处理后)跳过异常指令继续运行。
 *
 * @par 依赖
 * os_exc.h：该接口声明所在的头文件。
 * @see 无
 */
typedef U32 (*ExcProcFunc)(struct ExcInfo *excInfo);

/*
 * @ingroup os_exc
 * @brief 保存异常的处理状态函数类型定义。
 *
 * @par 描述
 * 在单核复位时钩子执行失败的返回信息通过该钩子函数进行保存。
 * @attention 无。
 *
 * @param state [IN]  类型#U32，异常处理的出错信息。
 *
 * @retval 无。
 *
 * @see 无
 */
typedef void (*ExcStateTraceHook)(U32 state);

/*
 * @ingroup SRE_exc
 * @brief 临终遗言钩子函数类型定义。
 *
 * @par 描述
 * 通过该函数类型定义临终遗言钩子函数。
 * @attention 无。
 *
 * @param 无。
 *
 * @retval 无。
 *
 * @see 无
 */
typedef void (*SreLastwordsFunc)(void);

/*
 * @ingroup os_exc
 * @brief Hac超时自清异常处理函数类型定义。
 *
 * @par 描述
 * 通过Hac超时自清异常处理函数类型定义异常处理函数钩子，产品通过查看Hac超时寄存器判断是否存在超时，
 * 如果存在超时返回TRUE(1)，如果不存在超时，说明系统出现异常，返回FALSE(0),系统进入异常接管。
 * @attention 无。
 *
 * @param 无。
 *
 * @retval TRUE，系统检测到HAC超时，返回之后OS做中断丢弃处理。
 * @retval FALSE，系统未检测到HAC超时，返回之后OS做异常处理。
 *
 * @see 无
 */
typedef bool (*HacTimeoutProcFunc)(void);

/*
 * @ingroup os_exc
 * 模块配置信息结构体。
 */
struct ExcModInfo {
    /* Hac异常钩子 */
    HacTimeoutProcFunc hacExtHook;
    /* 异常时用户注册的函数钩子 */
    ExcProcFunc excepHook;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_EXC_H */
