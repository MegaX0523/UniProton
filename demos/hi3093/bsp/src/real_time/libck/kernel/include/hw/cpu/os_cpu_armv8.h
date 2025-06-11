/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: cpu架构相关的外部头文件
 * Author: LibCK项目组
 * Create: 2020-06-16
 * History: 2020-06-16 LibCK项目组 创建文件
 */
#ifndef OS_CPU_ARMV8_H
#define OS_CPU_ARMV8_H

#include "sre_typedef.h"
#include "sre_buildef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

// CurrentEl等级
#define CURRENT_EL_2       0x8
#define CURRENT_EL_1       0x4
#define CURRENT_EL_0       0x0

#define DAIF_DBG_BIT      (1U << 3)
#define DAIF_ABT_BIT      (1U << 2)
#define DAIF_IRQ_BIT      (1U << 1)
#define DAIF_FIQ_BIT      (1U << 0)

#define INT_MASK          (1U << 7)

#define OS_CORE_ID_MASK          0xFFU
#define OS_CLUSTER_ID_MASK       0x7U

/*
 *  描述: 根据mpidr寄存器计算逻辑核号。
 *  armv8平台一个簇最多有4个核，因此当最大核数大于4时，计算逻辑核号需要考虑簇号（bit16-23）
 */
#if (OS_MAX_CORE_NUM > 4)
#define OS_CORE_MPID_CPUID(mpid) (((((mpid) >> 16) & OS_CLUSTER_ID_MASK) * 4) + \
    (((mpid) | ((mpid) >> 8)) & OS_CORE_ID_MASK))
#else
#define OS_CORE_MPID_CPUID(mpid) (((mpid) | ((mpid) >> 8)) & OS_CORE_ID_MASK)
#endif
#define OS_FPU_CONTEXT_REG_NUM 32

/*
 * @ingroup SRE_task
 * 任务上下文的结构体定义。
 */
struct TskContext {
#if defined(OS_OPTION_HAVE_FPU)
    __uint128_t q[OS_FPU_CONTEXT_REG_NUM];
    uintptr_t fpcr;
    uintptr_t fpsr;
#endif
    uintptr_t elr;               // 返回地址
    uintptr_t spsr;
    uintptr_t far;
    uintptr_t esr;
    uintptr_t xzr;
    uintptr_t x30;
    uintptr_t x29;
    uintptr_t x28;
    uintptr_t x27;
    uintptr_t x26;
    uintptr_t x25;
    uintptr_t x24;
    uintptr_t x23;
    uintptr_t x22;
    uintptr_t x21;
    uintptr_t x20;
    uintptr_t x19;
    uintptr_t x18;
    uintptr_t x17;
    uintptr_t x16;
    uintptr_t x15;
    uintptr_t x14;
    uintptr_t x13;
    uintptr_t x12;
    uintptr_t x11;
    uintptr_t x10;
    uintptr_t x09;
    uintptr_t x08;
    uintptr_t x07;
    uintptr_t x06;
    uintptr_t x05;
    uintptr_t x04;
    uintptr_t x03;
    uintptr_t x02;
    uintptr_t x01;
    uintptr_t x00;
};

/*
 *  描述: 读取当前核号
 *        使用mpidr 寄存器 (64bit) 根据核的线程模式获取核号
 *        bit 63-40 39-32   31  30 29~25  24 23-16  15-8   7~0
 *             res0  aff3  res1  u  res0  mt  aff2  aff1  aff0
 */
OS_SEC_ALW_INLINE INLINE U32 OsGetCoreID(void)
{
    U64 mpid;
    OS_EMBED_ASM("MRS  %0, MPIDR_EL1" : "=r"(mpid)::"memory", "cc");
    /* single-thread 模式下，核号取AFF0 AF1为0 */
    /* muti-thread 模式下，核号取AFF1 AF0为0 */
    /* 综上核号计算采用AFF0 + AFF1 */
    return OS_CORE_MPID_CPUID(mpid);
}

/*
 * @ingroup  SRE_sys
 * @brief 获取当前核ID。
 *
 * @par 描述: 获取当前核ID。
 *
 * @attention
 * <ul>
 * <li>获取的核ID为硬件寄存器中的ID号。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 物理核ID。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_GetNumberOfCores | SRE_GetPrimaryCore
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_GetCoreID(void)
{
    return OsGetCoreID();
}

/*
 * @ingroup  SRE_sys
 * @brief 开中断。
 *
 * @par 描述: 开启全局可屏蔽中断。
 *
 * @attention
 * <ul><li>中断服务函数里慎用该接口，会引起中断优先级反转。</li></ul>
 *
 * @param 无
 *
 * @retval 开启全局中断前的中断状态值。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_IntLock | SRE_IntRestore
 */
OS_SEC_ALW_INLINE INLINE uintptr_t SRE_IntUnLock(void)
{
    uintptr_t state = 0;

    OS_EMBED_ASM(
        "mrs %0, DAIF      \n"
        "msr DAIFClr, %1   \n"
        : "=r"(state)
        : "i"(DAIF_IRQ_BIT)
        : "memory", "cc");

    return state & INT_MASK;
}

/*
 * @ingroup  SRE_sys
 * @brief 关中断。
 *
 * @par 描述: 关闭全局可屏蔽中断。
 *
 * @attention 在关全局中断后，禁止调用引起内核调度的相关接口，如SRE_TaskDelay接口
 *
 * @param 无
 *
 * @retval 关闭全局中断前的中断状态值。
 * @par 依赖: sre_base.h：该接口声明所在的头文件。
 * @see SRE_IntUnLock | SRE_IntRestore
 */
OS_SEC_ALW_INLINE INLINE uintptr_t SRE_IntLock(void)
{
    uintptr_t state = 0;
    OS_EMBED_ASM(
        "mrs %0, DAIF      \n"
        "msr DAIFSet, %1   \n"
        : "=r"(state)
        : "i"(DAIF_IRQ_BIT)
        : "memory", "cc");
    return state & INT_MASK;
}

/*
 * @ingroup  SRE_sys
 * @brief 恢复中断状态接口。
 *
 * @par 描述: 恢复原中断状态寄存器。
 *
 * @attention
 * <ul>
 * <li>该接口必须和关闭全局中断或者是开启全局中断接口成对使用，以关全局中断或者开全局中断操作的返回值为入参</li>
 * <li>以保证中断可以恢复到关全局中断或者开全局中断操作前的状态</li>
 * </ul>
 * @param  intSave [IN]类型#UINTPTR，关全局中断SRE_IntLock和开全局中断SRE_IntUnLock的返回值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_IntUnLock | SRE_IntLock
 */
OS_SEC_ALW_INLINE INLINE void SRE_IntRestore(uintptr_t intSave)
{
    if ((intSave & INT_MASK) == 0) {
        OS_EMBED_ASM(
            "msr DAIFClr, %0\n"
            :
            : "i"(DAIF_IRQ_BIT)
            : "memory", "cc");
    } else {
        OS_EMBED_ASM(
            "msr DAIFSet, %0\n"
            :
            : "i"(DAIF_IRQ_BIT)
            : "memory", "cc");
    }
    return;
}

/*
 * @ingroup  SRE_sys
 * @brief 等待内存操作完成。
 *
 * @par 描述: 等待操作完成。
 *
 * @attention
 * <ul>
 * <li>多核之间操作内存，建议使用这个接口保证数据的一致性。</li>
 * </ul>
 *
 * @param 无
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
OS_SEC_ALW_INLINE INLINE void SRE_MemWait(void)
{
    OS_EMBED_ASM("DSB sy" : : : "memory");
}

/*
 * @ingroup  SRE_cpu
 * @brief ARM的DSB功能
 *
 * @par 描述
 * ARM的DSB功能，等待内存操作完毕，内存操作未结束之前，CPU不会继续操作内存和寄存器。
 *
 * @attention
 * <ul>
 * <li>只支持Cortex-AX平台。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_DMB
 */
#define SRE_DSB() OS_EMBED_ASM("DSB sy" : : : "memory")

/*
 * @ingroup  SRE_cpu
 * @brief ARM的DMB功能
 *
 * @par 描述
 * ARM的DMB功能，等待内存操作完毕，内存操作未结束之前，CPU不会继续操作内存。
 *
 * @attention
 * <ul>
 * <li>只支持Cortex-AX平台。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖: sre_hw.h：该接口声明所在的头文件。
 * @see SRE_DSB
 */
#define SRE_DMB() OS_EMBED_ASM("DMB sy" : : : "memory")

/*
 * @ingroup  SRE_cpu
 * @brief ARM的ISB功能
 *
 * @par 描述
 * ARM的ISB功能。
 *
 * @attention
 * <ul>
 * <li>只支持Cortex-AX平台。</li>
 * <li>最严格：它会清洗流水线。</li>
 * <li>以保证所有它前面的指令都执行完毕之后，才执行它后面的指令。</li>
 * </ul>
 *
 * @param  无
 *
 * @retval 无
 * @par 依赖: sre_hw.h：该接口声明所在的头文件。
 * @see SRE_ISB
 */
#define SRE_ISB() OS_EMBED_ASM("ISB" : : : "memory")

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_CPU_ARMV8_H */
