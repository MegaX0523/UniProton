/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 原子运算操作模块外部头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_ATOMIC_H
#define SRE_ATOMIC_H

#include "sre_base.h"
#include "./common/os_atomic.h"

#if ((OS_HARDWARE_PLATFORM == OS_HIDSP) || (OS_HARDWARE_PLATFORM == OS_HIDSPV200) ||     \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV200) || (OS_HARDWARE_PLATFORM == OS_HIAVPV280) || \
     (OS_HARDWARE_PLATFORM == OS_HIDSPV300LITE) || (OS_HARDWARE_PLATFORM == OS_HIAVPV300LITE))
#include "./hw/cpu/os_atomic_hidsp.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_CORTEX_RX) || (OS_HARDWARE_PLATFORM == OS_CORTEX_AX))
#include "./hw/cpu/os_atomic_armv7.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_ARMV8)
#include "./hw/cpu/os_atomic_armv8.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * @ingroup SRE_atomic
 *
 * 带dbg信息的spinlock的锁结构。
 */
struct SreSpinLock {
    union {
        volatile uintptr_t rawLock;  /* 锁值 */
#if defined(OS_OPTION_SPINLOCK_OWNER_TRACE)
        struct {
            volatile uintptr_t rsvd0    : 8;  /* 为确保不与rawLock冲突，必须预留≥1 bit */
            volatile uintptr_t ownerCpu : 8;  /* 持有锁的核ID，一个核不应持有两次 */
            volatile uintptr_t ownerTid : 16; /* 持有锁的线程ID,一个线程不应持有两次 */
        };
#endif
    };
};

#if defined(OS_OPTION_SPINLOCK_TICKET)
/*
 * @ingroup SRE_atomic
 *
 * nextAddr和ownerAddr的共享属性必须与需要互斥的原子保护区配套，第一次使用前需要初始化。
 */
struct TicketSpinLock {
    U32 *nextAddr;  /* 地址必须为EMT等支持互斥操作的地址 */
    U32 *ownerAddr; /* 非EMT地址，出于性能考虑, 建议此地址为非DDR地址，若配置为DDR地址，对DDR流量有影响 */
};
#endif

#if defined(OS_OPTION_INT_LOCK_TC)
/*
 * @ingroup SRE_atomic
 *
 * SplIrqLock关中断阀值检测用户回调函数类型。
 */
typedef void (*SreSplTcFunc)(U32 counter, struct SreSpinLock *spinLock);

#if defined(OS_OPTION_SPINLOCK_TICKET)
typedef void (*TicketSplTcFunc)(U32 counter, struct TicketSpinLock *spinLock);
#endif
#endif

/*
 * @brief 无符号32位原子读
 *
 * @par 描述
 * 无符号32位原子读。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param addr [I/O] 类型#U32 *，要读取的地址。
 *
 * @retval 读取的数据
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicRead32 | SRE_AtomicReadU64
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicReadU32(U32 *addr)
{
    return (*addr);
}

/*
 * @brief 有符号32位原子读
 *
 * @par 描述
 * 有符号32位原子读。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param addr [I/O] 类型#S32 *，要读取的地址。
 *
 * @retval 读取的数据
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicReadU32 | SRE_AtomicReadU64
 */
OS_SEC_ALW_INLINE INLINE S32 SRE_AtomicRead32(S32 *addr)
{
    return (*addr);
}

/*
 * @brief 获取自旋锁。
 *
 * @par 描述
 * 获取自旋锁。
 *
 * @attention
 * <ul><li>该接口用于已经关闭中断的场景，否则需要特别留意获取锁而没有关中断。</li></ul>
 * <ul><li>该接口需要和#SRE_SplUnlock接口成对使用，先关中断在申请锁。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，自旋锁地址。
 *
 * @retval 无。
 *
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplUnlock
 */
extern void SRE_SplLock(struct SreSpinLock *spinLock);

/*
 * @brief 释放自旋锁。
 *
 * @par 描述
 * 释放自旋锁。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplLock接口成对使用，先释放锁再开中断。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，自旋锁地址。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplLock
 */
extern void SRE_SplUnlock(struct SreSpinLock *spinLock);

/*
 * @brief 获取自旋锁并关闭中断使能位。
 *
 * @par 描述
 * 获取自旋锁并关闭中断使能位。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplIrqUnlock接口成对使用。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，自旋锁地址。
 *
 * @retval #uintptr_t类型 中断状态寄存器
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplIrqUnlock
 */
extern uintptr_t SRE_SplIrqLock(struct SreSpinLock *spinLock);

/*
 * @brief 释放自旋锁并恢复中断使能位。
 *
 * @par 描述
 * 释放自旋锁并恢复中断使能位。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplIrqLock接口成对使用。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，自旋锁地址。
 * @param intSave  [IN]  类型#uintptr_t，中断句柄，自旋锁时的返回值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplIrqLock
 */
extern void SRE_SplIrqUnlock(struct SreSpinLock *spinLock, uintptr_t intSave);

/*
 * @brief 禁止本地中断并获取指定的读锁。
 *
 * @par 描述
 * 禁止本地中断并获取指定的读锁。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplIrqReadUnlock接口成对使用。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，读写锁地址。
 *
 * @retval #uintptr_t类型 中断状态寄存器，调用前的中断状态值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplIrqReadUnlock
 */
extern uintptr_t SRE_SplIrqReadLock(struct SreSpinLock *spinLock);

/*
 * @brief 释放读锁并恢复本地中断。
 *
 * @par 描述
 * 释放读锁并恢复本地中断。
 *
 * @attention 该接口需要和#SRE_SplIrqReadLock接口成对使用。
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，读写锁地址。
 * @param intSave  [IN]  类型#uintptr_t，原来的中断状态值，即锁时的返回值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplIrqReadLock
 */
extern void SRE_SplIrqReadUnlock(struct SreSpinLock *spinLock, uintptr_t intSave);

/*
 * @brief 禁止本地中断并获取指定的写锁。
 *
 * @par 描述
 * 禁止本地中断并获取指定的写锁。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplIrqWriteUnlock接口成对使用。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，读写锁地址。
 *
 * @retval #uintptr_t类型 中断状态寄存器，调用前的中断状态值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplIrqWriteUnlock
 */
extern uintptr_t SRE_SplIrqWriteLock(struct SreSpinLock *spinLock);

/*
 * @brief 释放写锁并恢复本地中断。
 *
 * @par 描述
 * 释放写锁并恢复本地中断。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplIrqWriteLock接口成对使用。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，读写锁地址。
 * @param intSave  [IN]  类型#uintptr_t，原来的中断状态值，即锁时的返回值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplIrqWriteLock
 */
extern void SRE_SplIrqWriteUnlock(struct SreSpinLock *spinLock, uintptr_t intSave);

#if defined(OS_OPTION_SPINLOCK_TICKET)
/*
 * @brief 获取自旋锁（ticket实现）。
 *
 * @par 描述
 * 获取自旋锁（ticket实现）。
 *
 * @attention
 * <ul><li>该接口用于已经关闭中断的场景，否则需要特别留意获取锁而没有关中断。</li></ul>
 * <ul><li>该接口需要和#SRE_TicketSplUnlock接口成对使用，先关中断在申请锁。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct TicketSpinLock *，自旋锁地址。
 * @param ticketId [OUT] 类型#U32 *，用于暂存ticketId值的地址。
 *
 * @retval 无
 *
 * @par 依赖
 * <ul><li>SRE_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TicketSplUnlock
 */
extern void SRE_TicketSplLock(struct TicketSpinLock *spinLock, U32 *ticketId);

/*
 * @brief 释放自旋锁（ticket实现）。
 *
 * @par 描述
 * 释放自旋锁（ticket实现）。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_TicketSplLock接口成对使用，先释放锁再开中断。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct TicketSpinLock *，自旋锁地址。
 * @param ticketId [IN]  类型#U32，加锁时获得的ticketId值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>SRE_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TicketSplLock
 */
extern void SRE_TicketSplUnlock(struct TicketSpinLock *spinLock, U32 ticketId);

/*
 * @brief 获取自旋锁（ticket实现）并关闭中断使能位。
 *
 * @par 描述
 * 获取自旋锁（ticket实现）并关闭中断使能位。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_TicketSplIrqUnlock接口成对使用。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct TicketSpinLock *，自旋锁地址。
 * @param ticketId [OUT] 类型#U32 *，用于暂存ticketId值的地址。
 *
 * @retval #uintptr_t类型 中断状态寄存器
 * @par 依赖
 * <ul><li>SRE_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TicketSplIrqUnlock
 */
extern uintptr_t SRE_TicketSplIrqLock(struct TicketSpinLock *spinLock, U32 *ticketId);

/*
 * @brief 释放自旋锁（ticket实现）并恢复中断使能位。
 *
 * @par 描述
 * 释放自旋锁（ticket实现）并恢复中断使能位。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_TicketSplIrqLock接口成对使用。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct TicketSpinLock *，自旋锁地址。
 * @param ticketId [IN]  类型#U32，加锁时返回的ticketId值。
 * @param intSave  [IN]  类型#uintptr_t，中断句柄，自旋锁时的返回值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>SRE_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TicketSplIrqLock
 */
extern void SRE_TicketSplIrqUnlock(struct TicketSpinLock *spinLock, U32 ticketId, uintptr_t intSave);
#endif

/*
 * @brief 自旋锁地址申请。
 *
 * @par 描述
 * 自旋锁地址申请。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SpinlockFree接口成对使用。</li></ul>
 *
 * @param addr [IN]  类型#volatile uintptr_t *，存放锁地址的地址。
 *
 * @retval #OS_ERRNO_SPL_ALLOC_ADDR_INVALID 0x02004904，输入地址为空。
 * @retval #OS_ERRNO_SPL_NO_FREE            0x02004905，没有空闲的锁地址分配
 * @retval #SRE_OK                          0x00000000，申请成功。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SpinlockFree
 */
extern U32 SRE_SpinlockAlloc(volatile uintptr_t *addr);

/*
 * @brief 自旋锁地址释放。
 *
 * @par 描述
 * 自旋锁地址释放。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SpinlockAlloc接口成对使用。</li><ul>
 *
 * @param addr [IN]  类型#uintptr_t，SRE_SpinlockAlloc申请出来的锁地址。
 *
 * @retval #OS_ERRNO_SPL_FREE_ADDR_INVALID  0x02004906，锁释放地址为空
 * @retval #SRE_OK                          0x00000000，释放成功。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SpinlockAlloc
 */
extern U32 SRE_SpinlockFree(uintptr_t addr);

/*
 * @brief 自旋锁删除。
 *
 * @par 描述
 * 自旋锁的删除。对于锁地址需要管理的平台，在自旋锁删除时需要释放锁地址
 *
 * @attention
 * <ul><li>该接口与SRE_SplLockInit成对使用。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，自旋锁地址。
 *
 * @retval #OS_ERRNO_SPL_FREE_ADDR_INVALID  0x02004906，锁释放地址为空
 * @retval #SRE_OK                          0x00000000，删除成功。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SplLockInit
 */
extern U32 SRE_SplLockDelete(struct SreSpinLock *spinLock);

/*
 * @brief 自旋锁的锁值初始化。
 *
 * @par 描述
 * 自旋锁的锁值初始化。
 *
 * @attention
 * <ul><li>OS的自旋锁功能的锁值含义:0表示未锁，1表示上锁，建议调用该接口进行初始化，</li></ul>
 * <ul><li>因为锁地址包含还维测字段，若锁空间不够会则进入fatal error异常。</li></ul>
 * <ul><li>对于入参spinLock，OS已做基本校验，无法校验所有非法地址，其合法性由业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，自旋锁地址。
 *
 * @retval #OS_ERRNO_SPL_ALLOC_ADDR_INVALID 0x02004904，输入地址为空。
 * @retval #OS_ERRNO_SPL_NO_FREE            0x02004905，没有空闲的锁地址分配
 * @retval #SRE_OK                          0x00000000，初始化成功。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li><ul>
 * @see SRE_SplLockDelete
 */
extern U32 SRE_SplLockInit(struct SreSpinLock *spinLock);

#if defined(OS_OPTION_INT_LOCK_TC)
/*
 * @brief splIrq超时注册
 *
 * @par 描述
 * 注册关中断自旋锁的超时时间和超时回调函数
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplIrqUnlockTc,SRE_SplIrqUnlockTc接口成对使用。</li></ul>
 *
 * @param threshold  [IN] 类型#U32，关中断自旋锁的超时时间阈值。
 * @param splTcHook  [IN] 类型#SreSplTcFunc，关中断阀值检测用户回调函数。
 *
 * @retval #SRE_OK                                     0x00000000，注册钩子成功。
 * @retval #OS_ERRNO_HWI_INT_LOCK_REG_PARA_INVALID     0x0200141b，关中断超时注册形参非法。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li><ul>
 * @see SRE_SplIrqLockTc
 */
extern U32 SRE_SplIrqLockTcReg(U32 threshold, SreSplTcFunc splTcHook);

/*
 * @brief 获取自旋锁并关闭中断使能位,带超时检查。
 *
 * @par 描述
 * 获取自旋锁并关闭中断使能位。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplIrqUnlockTc接口成对使用。</li></ul>
 * <ul><li>该接口支持spinlock维测，性能相对SRE_SplIrqLock较低。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，自旋锁地址。
 *
 * @retval #uintptr_t类型 中断状态寄存器
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li><ul>
 * @see SRE_SplIrqUnlockTc
 */
extern uintptr_t SRE_SplIrqLockTc(struct SreSpinLock *spinLock);

/*
 * @brief 释放自旋锁并恢复中断使能位,带超时检查。
 *
 * @par 描述
 * 释放自旋锁并恢复中断使能位。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_SplIrqLockTc接口成对使用。</li></ul>
 * <ul><li>该接口支持spinlock维测，性能相对SRE_SplIrqUnlock较低。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct SreSpinLock *，自旋锁地址。
 * @param intSave  [IN]  类型#uintptr_t，中断句柄，自旋锁时的返回值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li><ul>
 * @see SRE_SplIrqLockTc
 */
extern void SRE_SplIrqUnlockTc(struct SreSpinLock *spinLock, uintptr_t intSave);

#if defined(OS_OPTION_SPINLOCK_TICKET)
/*
 * @brief splIrq超时注册，适用Ticket Spinlock接口
 *
 * @par 描述
 * 注册关中断自旋锁的超时时间和超时回调函数,适用Ticket Spinlock接口
 *
 * @attention
 * <ul><li>该接口需要和#SRE_TicketSplIrqUnlockTc,SRE_TicketSplIrqUnlockTc接口成对使用。</li></ul>
 *
 * @param threshold  [IN] 类型#U32，关中断自旋锁的超时时间阈值。
 * @param splTcHook  [IN] 类型#TicketSplTcFunc，关中断阀值检测用户回调函数。
 *
 * @retval #SRE_OK                                     0x00000000，注册钩子成功。
 * @retval #OS_ERRNO_HWI_INT_LOCK_REG_PARA_INVALID     0x0200141b，关中断超时注册形参非法。
 * @par 依赖
 * <ul><li>SRE_atomic.h：该接口声明所在的头文件。</li><ul>
 * @see SRE_TicketSplIrqLockTc
 */
extern U32 SRE_TicketSplIrqLockTcReg(U32 threshold, TicketSplTcFunc splTcHook);

/*
 * @brief 获取自旋锁并关闭中断使能位,带超时检查，ticket方式实现。
 *
 * @par 描述
 * 获取自旋锁并关闭中断使能位，ticket方式实现。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_TicketSplIrqUnlockTc接口成对使用。</li></ul>
 * <ul><li>该接口支持TicketSpinLock维测，性能相对SRE_TicketSplIrqLock较低。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct TicketSpinLock *，自旋锁地址。
 * @param ticketId [OUT] 类型#U32 *，暂存加锁时返回的ticketId值的地址。
 *
 * @retval #uintptr_t类型 中断状态寄存器
 * @par 依赖
 * <ul><li>SRE_atomic.h：该接口声明所在的头文件。</li><ul>
 * @see SRE_TicketSplIrqUnlockTc
 */
extern uintptr_t SRE_TicketSplIrqLockTc(struct TicketSpinLock *spinLock, U32 *ticketId);

/*
 * @brief 释放自旋锁并恢复中断使能位，带超时检查，ticket方式实现。
 *
 * @par 描述
 * 释放自旋锁并恢复中断使能位，ticket方式实现。
 *
 * @attention
 * <ul><li>该接口需要和#SRE_TicketSplIrqLockTc接口成对使用。</li></ul>
 * <ul><li>该接口支持TicketSpinLock维测，性能相对SRE_TicketSplIrqUnLock较低。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param spinLock [IN]  类型#struct TicketSpinLock *，自旋锁地址。
 * @param ticketId [IN]  类型#U32，加锁时返回的ticketId值。
 * @param intSave  [IN]  类型#uintptr_t，中断句柄，自旋锁时的返回值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>SRE_atomic.h：该接口声明所在的头文件。</li><ul>
 * @see SRE_TicketSplIrqLockTc
 */
extern void SRE_TicketSplIrqUnlockTc(struct TicketSpinLock *spinLock, U32 ticketId, uintptr_t intSave);
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* SRE_ATOMIC_H */
