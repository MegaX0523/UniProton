/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: armv8平台原子运算操作模块的外部头文件。
 * Author: LibCK项目组
 * Create: 2020-06-16
 * History: 2020-06-16 LibCK项目组 创建文件
 */
#ifndef OS_ATOMIC_ARMV8_H
#define OS_ATOMIC_ARMV8_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

struct Atomic32 {
    volatile U32 counter;
};

struct Atomtic64 {
    volatile U64 counter;
};

OS_SEC_ALW_INLINE INLINE void OsArmPrefecthW(volatile void *ptr)
{
    OS_EMBED_ASM("prfm    pstl1strm, %0"::"Q"(ptr):"memory", "cc");
}

OS_SEC_ALW_INLINE INLINE S64 OsAtomic64Add(S64 i, struct Atomtic64 *v)
{
    uintptr_t temp;
    S64 ret;

    OsArmPrefecthW(&v->counter);

    OS_EMBED_ASM("1: ldxr            %0, %2          \n"
                 "   add             %0, %0, %3      \n"
                 "   stlxr           %w1, %0, %2     \n"
                 "   cbnz            %w1, 1b         \n"
                 "   dmb ish"
                 : "=&r" (ret), "=&r" (temp), "+Q" (v->counter)
                 : "r" (i)
                 : "memory");

    return  ret;
}

OS_SEC_ALW_INLINE INLINE S32 OsAtomic32Add(S32 i, struct Atomic32 *v)
{
    uintptr_t temp;
    S32 ret;

    OsArmPrefecthW(&v->counter);

    OS_EMBED_ASM("1: ldxr            %w0, %2         \n"
                 "   add             %w0, %w0, %w3   \n"
                 "   stlxr           %w1, %w0, %2    \n"
                 "   cbnz            %w1, 1b         \n"
                 "   dmb ish"
                 : "=&r" (ret), "=&r" (temp), "+Q" (v->counter)
                 : "r" (i)
                 : "memory");
    return  ret;
}

/*
 * @brief 有符号32位变量的原子加，并返回累加后的值
 *
 * @par 描述
 * 有符号32位变量的原子加，并返回累加后的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#S32*，要累加变量的地址。
 * @param[in]       incr 类型#S32， 要累加的数值。
 *
 * @retval 变量累加后的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAdd32 | SRE_AtomicFetchAndAdd32
 */
OS_SEC_ALW_INLINE INLINE S32 SRE_AtomicAdd32Rtn(S32 *ptr, S32 incr)
{
    return OsAtomic32Add(incr, (struct Atomic32 *)ptr);
}

/*
 * @brief 无符号32位变量的原子加，并返回累加后的值
 *
 * @par 描述
 * 无符号32位变量的原子加，并返回累加后的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#U32*，要累加变量的地址。
 * @param[in]       incr 类型#U32， 要累加的数值。
 *
 * @retval 变量累加后的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAddU32 | SRE_AtomicFetchAndAddU32
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicAddU32Rtn(U32 *ptr, U32 incr)
{
    return (U32)OsAtomic32Add((S32)incr, (struct Atomic32 *)ptr);
}

/*
 * @brief 有符号64位变量的原子加，并返回累加后的值
 *
 * @par 描述
 * 有符号64位变量的原子加，并返回累加后的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   atomic 类型#S64*，要累加变量的地址。
 * @param[in]       incr   类型#S64， 要累加的数值。
 *
 * @retval 变量累加后的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAdd64
 */
OS_SEC_ALW_INLINE INLINE S64 SRE_AtomicAdd64Rtn(S64 *atomic, S64 incr)
{
    return OsAtomic64Add(incr, (struct Atomtic64 *)atomic);
}

/*
 * @brief 无符号64位变量的原子加，并返回累加后的值
 *
 * @par 描述
 * 无符号64位变量的原子加，并返回累加后的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   atomic 类型#U64*，要累加变量的地址。
 * @param[in]       incr   类型#U64， 要累加的数值。
 *
 * @retval 变量累加后的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAddU64 | SRE_AtomicFetchAndAddU64
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicAddU64Rtn(U64 *atomic, U64 incr)
{
    return (U64)OsAtomic64Add((S64)incr, (struct Atomtic64 *)atomic);
}

/*
 * @brief 有符号32位变量的原子加
 *
 * @par 描述
 * 有符号32位变量的原子加。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#S32*，要累加变量的地址。
 * @param[in]       incr 类型#S32， 要累加的数值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAdd32Rtn | SRE_AtomicFetchAndAdd32
 */
OS_SEC_ALW_INLINE INLINE void SRE_AtomicAdd32(S32 *ptr, S32 incr)
{
    (void)OsAtomic32Add(incr, (struct Atomic32 *)ptr);
}

/*
 * @brief 带TST的有符号32位变量的原子加
 *
 * @par 描述
 * 带TST的有符号32位变量的原子加。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#S32*，要累加变量的地址。
 * @param[in]       incr 类型#S32， 要累加的数值。
 *
 * @retval 并判断执行该操作后累加变量的值是否为0
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAddTst64
 */
OS_SEC_ALW_INLINE INLINE bool SRE_AtomicAddTst32(S32 *ptr, S32 incr)
{
    return OsAtomic32Add(incr, (struct Atomic32 *)ptr) == 0;
}

/*
 * @brief 无符号32位变量的原子加
 *
 * @par 描述
 * 无符号32位变量的原子加。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#U32*，要累加变量的地址。
 * @param[in]       incr 类型#U32， 要累加的数值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAddU32Rtn | SRE_AtomicFetchAndAddU32
 */
OS_SEC_ALW_INLINE INLINE void SRE_AtomicAddU32(U32 *ptr, U32 incr)
{
    (void)OsAtomic32Add((S32)incr, (struct Atomic32 *)ptr);
}

/*
 * @brief 有符号64位变量的原子加
 *
 * @par 描述
 *  有符号64位变量的原子加。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   atomic 类型#S64*，要累加变量的地址。
 * @param[in]       incr   类型#S64， 要累加的数值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAdd64Rtn
 */
OS_SEC_ALW_INLINE INLINE void SRE_AtomicAdd64(S64 *atomic, S64 incr)
{
    (void)OsAtomic64Add(incr, (struct Atomtic64 *)atomic);
}

/*
 * @brief 带TST的有符号64位变量的原子加
 *
 * @par 描述
 * 带TST的有符号64位变量的原子加。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#S64*，要累加变量的地址。
 * @param[in]       incr 类型#S64， 要累加的数值。
 *
 * @retval 并判断执行该操作后累加变量的值是否为0
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAddTst32
 */
OS_SEC_ALW_INLINE INLINE bool SRE_AtomicAddTst64(S64 *ptr, S64 incr)
{
    return OsAtomic64Add(incr, (struct Atomtic64 *)ptr) == 0;
}

/*
 * @brief 无符号64位变量的原子加
 *
 * @par 描述
 * 无符号64位变量的原子加。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   atomic 类型#U64*，要累加变量的地址。
 * @param[in]       incr   类型#U64， 要累加的数值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAddU64Rtn | SRE_AtomicFetchAndAddU64
 */
OS_SEC_ALW_INLINE INLINE void SRE_AtomicAddU64(U64 *atomic, U64 incr)
{
    (void)OsAtomic64Add((S64)incr, (struct Atomtic64 *)atomic);
}

/*
 * @brief 32位原子交换，并返回交换前的值
 *
 * @par 描述
 * 32位原子交换，并返回交换前的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr      类型#U32*，要交换变量的地址。
 * @param[in]       newValue 类型#U32， 要交换的值。
 *
 * @retval 变量交换前的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicSwap64
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicSwap32(U32 *ptr, U32 newValue)
{
    U32 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr   %w0, [%2]       \n"
        "   stxr    %w1, %w3, [%2]  \n"
        "   cbnz    %w1, 1b         \n"
        : "=&r" (tmp), "=&r" (ret)
        : "r"(ptr), "r" (newValue)
        : "memory", "cc");
    return tmp;
}

/*
 * @brief 64位原子交换，并返回交换前的值
 *
 * @par 描述
 * 64位原子交换，并返回交换前的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr      类型#U64*，要交换变量的地址。
 * @param[in]       newValue 类型#U64， 要交换的值。
 *
 * @retval 变量交换前的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicSwap32
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicSwap64(U64 *ptr, U64 newValue)
{
    U64 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr   %0, [%2]        \n"
        "   stxr    %w1, %3, [%2]   \n"
        "   cbnz    %w1, 1b         \n"
        : "=&r" (tmp), "=&r" (ret)
        : "r"(ptr), "r" (newValue)
        : "memory", "cc");
    return tmp;
}

/*
 * @brief 完成32位无符号的变量与指定内存的值比较并返回比较结果，相等时赋值并返回1，不相等时返回零
 *
 * @par 描述
 * 完成32位无符号的变量与指定内存的值比较，并在相等的情况下赋值，相等时赋值并返回1，不相等时返回零。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr    类型#U32*，  要比较/赋值变量的地址。
 * @param[in]       oldVal 类型#U32， 要比较的值。
 * @param[in]       newVal 类型#U32， 相等时要写入的新值。
 *
 * @retval 1，相等并赋值。
 * @retval 0，不相等。
 * @par 依赖
 * @li SRE_atomic.h：该接口声明所在的头文件。
 * @see SRE_AtomicCompareAndStore64
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicCompareAndStore32(U32 *ptr, U32 oldVal, U32 newVal)
{
    U32 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr   %w0, [%2]      \n"
        "   cmp     %w0, %w3       \n"
        "   bne     2f             \n"
        "   stxr    %w1, %w4, [%2] \n"
        "   cbnz    %w1, 1b        \n"
        "2:                        \n"
        : "=&r"(tmp), "+&r"(ret), "+&r"(ptr)
        : "r"(oldVal), "r"(newVal)
        : "memory", "cc");

    return (~ret) & 0x1U;
}

/*
 * @brief 完成32位无符号的变量与指定内存的值比较，并在相等的情况下赋值，返回赋值前的原始内容
 *
 * @par 描述
 * 完成32位无符号的变量与指定内存的值比较，并在相等的情况下赋值，返回赋值前的原始内容。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr    类型#U32*，  要比较/赋值变量的地址。
 * @param[in]       oldVal 类型#U32， 要比较的值。
 * @param[in]       newVal 类型#U32， 相等时要写入的新值。
 *
 * retval 返回值是ptr所在的内存地址单元的原始内容。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicXchg64
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicXchg32(U32 *ptr, U32 oldVal, U32 newVal)
{
    U32 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr   %w0, [%2]      \n"
        "   cmp     %w0, %w3       \n"
        "   bne     2f             \n"
        "   stxr    %w1, %w4, [%2] \n"
        "   cbnz    %w1, 1b        \n"
        "2:                        \n"
        : "+&r"(tmp), "=&r"(ret)
        : "r"(ptr), "r"(oldVal), "r"(newVal)
        : "memory", "cc");

    return tmp;
}

/*
 * @brief 有符号32位变量的原子加，并返回累加前的值
 *
 * @par 描述
 * 有符号32位变量的原子加，并返回累加前的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#S32*，要累加变量的地址。
 * @param[in]       incr 类型#S32， 要累加的数值。
 *
 * @retval 变量累加前的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAdd32 | SRE_AtomicAdd32Rtn
 */
OS_SEC_ALW_INLINE INLINE S32 SRE_AtomicFetchAndAdd32(S32 *ptr, S32 incr)
{
    S32 tmp = 0;
    S32 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %w2,   [%3]       \n"
        "   add   %w0, %w2, %w4     \n"
        "   stlxr %w1, %w0, [%3]    \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(ptr), "r"(incr)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 无符号的32位原子或操作
 *
 * @par 描述
 * 无符号的32位原子或操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U32*，要执行逻辑或变量的地址。
 * @param[in]       val  类型#U32， 要逻辑或的值。
 *
 * @retval 执行或操作以后的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicOr(U32 *addr, U32 val)
{
    U32 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr %w0,   [%2]    \n"
        "   orr   %w0, %w0, %w3  \n"
        "   stlxr %w1, %w0, [%2] \n"
        "   cbnz  %w1, 1b        \n"
        : "=&r"(tmp), "=&r"(ret)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return tmp;
}

/*
 * @brief 带预取的无符号的32位原子或操作
 *
 * @par 描述
 * 带预取的无符号的32位原子或操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U32*，要执行逻辑或变量的地址。
 * @param[in]       val  类型#U32， 要逻辑或的值。
 *
 * @retval 执行或操作以前的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicFetchOr(U32 *addr, U32 val)
{
    U32 tmp = 0;
    U32 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %w2,   [%3]       \n"
        "   orr   %w0, %w2, %w4     \n"
        "   stlxr %w1, %w0, [%3]    \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 无符号的32位原子与操作
 *
 * @par 描述
 * 无符号的32位原子与操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U32*，要执行逻辑与变量的地址。
 * @param[in]       val  类型#U32， 要逻辑与的值。
 *
 * @retval 执行与操作以后的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicAnd(U32 *addr, U32 val)
{
    U32 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr %w0,   [%2]    \n"
        "   and   %w0, %w0, %w3  \n"
        "   stlxr %w1, %w0, [%2] \n"
        "   cbnz  %w1, 1b        \n"
        : "=&r"(tmp), "=&r"(ret)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return tmp;
}

/*
 * @brief 带预取的无符号的32位原子与操作
 *
 * @par 描述
 * 带预取的无符号的32位原子与操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U32*，要执行逻辑与变量的地址。
 * @param[in]       val  类型#U32， 要逻辑与的值。
 *
 * @retval 执行与操作以前的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicFetchAnd(U32 *addr, U32 val)
{
    U32 tmp = 0;
    U32 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %w2,   [%3]       \n"
        "   and   %w0, %w2, %w4     \n"
        "   stlxr %w1, %w0, [%3]    \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 无符号的32位原子异或操作
 *
 * @par 描述
 * 无符号的32位原子异或操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U32*，要执行逻辑异或变量的地址。
 * @param[in]       val  类型#U32， 要逻辑异或的值。
 *
 * @retval 执行与操作以后的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicXor(U32 *addr, U32 val)
{
    U32 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr %w0,   [%2]    \n"
        "   eor   %w0, %w0, %w3  \n"
        "   stlxr %w1, %w0, [%2] \n"
        "   cbnz  %w1, 1b        \n"
        : "=&r"(tmp), "=&r"(ret)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return tmp;
}

/*
 * @brief 带预取的无符号的32位原子异或操作
 *
 * @par 描述
 * 带预取的无符号的32位原子异或操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U32*，要执行逻辑异或变量的地址。
 * @param[in]       val  类型#U32， 要逻辑异或的值。
 *
 * @retval 执行异或操作以前的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicFetchXor(U32 *addr, U32 val)
{
    U32 tmp = 0;
    U32 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %w2,   [%3]       \n"
        "   eor   %w0, %w2, %w4     \n"
        "   stlxr %w1, %w0, [%3]    \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 无符号的64位原子或操作
 *
 * @par 描述
 * 无符号的64位原子或操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U64*，要执行逻辑或变量的地址。
 * @param[in]       val  类型#U64， 要逻辑或的值。
 *
 * @retval 执行或操作以后的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicOr64(U64 *addr, U64 val)
{
    U64 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr %0,   [%2]    \n"
        "   orr   %0, %0, %3    \n"
        "   stlxr %w1, %0, [%2] \n"
        "   cbnz  %w1, 1b       \n"
        : "=&r"(tmp), "=&r"(ret)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return tmp;
}

/*
 * @brief 带预取的无符号的64位原子或操作
 *
 * @par 描述
 * 带预取的无符号的64位原子或操作
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U64*，要执行逻辑或变量的地址。
 * @param[in]       val  类型#U64， 要逻辑或的值。
 *
 * @retval 执行或操作以前的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicFetchOr64(U64 *addr, U64 val)
{
    U64 tmp = 0;
    U64 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %2,   [%3]        \n"
        "   orr   %0, %2, %4        \n"
        "   stlxr %w1, %0, [%3]     \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 无符号的64位原子与操作
 *
 * @par 描述
 * 无符号的64位原子与操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U64*，要执行逻辑与变量的地址。
 * @param[in]       val  类型#U64， 要逻辑与的值。
 *
 * @retval 执行与操作以后的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicAnd64(U64 *addr, U64 val)
{
    U64 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr %0,   [%2]    \n"
        "   and   %0, %0, %3    \n"
        "   stlxr %w1, %0, [%2] \n"
        "   cbnz  %w1, 1b       \n"
        : "=&r"(tmp), "=&r"(ret)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return tmp;
}

/*
 * @brief 带预取的无符号的64位原子与操作
 *
 * @par 描述
 * 带预取的无符号的64位原子与操作
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U64*，要执行逻辑与变量的地址。
 * @param[in]       val  类型#U64， 要逻辑与的值。
 *
 * @retval 执行与操作以前的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicFetchAnd64(U64 *addr, U64 val)
{
    U64 tmp = 0;
    U64 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %2,   [%3]        \n"
        "   and   %0, %2, %4        \n"
        "   stlxr %w1, %0, [%3]     \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 无符号的64位原子异或操作
 *
 * @par 描述
 * 无符号的64位原子异或操作。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U64*，要执行逻辑异或变量的地址。
 * @param[in]       val  类型#U64， 要逻辑异或的值。
 *
 * @retval 执行异或操作以后的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicXor64(U64 *addr, U64 val)
{
    U64 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr %0,   [%2]    \n"
        "   eor   %0, %0, %3    \n"
        "   stlxr %w1, %0, [%2] \n"
        "   cbnz  %w1, 1b       \n"
        : "=&r"(tmp), "=&r"(ret)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return tmp;
}

/*
 * @brief 带预取的无符号的64位原子异或操作
 *
 * @par 描述
 * 带预取的无符号的64位原子异或操作
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#U64*，要执行逻辑异或变量的地址。
 * @param[in]       val  类型#U64， 要逻辑异或的值。
 *
 * @retval 执行异或操作以前的值。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicFetchXor64(U64 *addr, U64 val)
{
    U64 tmp = 0;
    U64 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %2,   [%3]        \n"
        "   eor   %0, %2, %4        \n"
        "   stlxr %w1, %0, [%3]     \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 完成64位无符号的变量与指定内存的值比较并返回比较结果，相等时赋值并返回1，不相等时返回零
 *
 * @par 描述
 * 完成64位无符号的变量与指定内存的值比较，并在相等的情况下赋值，相等时赋值并返回1，不相等时返回零。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr    类型#U64*，  要比较/赋值变量的地址。
 * @param[in]       oldVal 类型#U64， 要比较的值。
 * @param[in]       newVal 类型#U64， 相等时要写入的新值。
 *
 * @retval 1，相等并赋值。
 * @retval 0，不相等。
 * @par 依赖
 * @li SRE_atomic.h：该接口声明所在的头文件。
 * @see SRE_AtomicCompareAndStore32
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicCompareAndStore64(U64 *ptr, U64 oldVal, U64 newVal)
{
    U64 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr   %0, [%2]      \n"
        "   cmp     %0, %3        \n"
        "   bne     2f            \n"
        "   stxr    %w1, %4, [%2] \n"
        "   cbnz    %w1, 1b       \n"
        "2:                       \n"
        : "=&r"(tmp), "+&r"(ret), "+&r"(ptr)
        : "r"(oldVal), "r"(newVal)
        : "memory", "cc");

    return (U64)((~ret) & 0x1U);
}

/*
 * @brief 完成64位无符号的变量与指定内存的值比较，并在相等的情况下赋值，返回赋值前的原始内容
 *
 * @par 描述
 * 完成64位无符号的变量与指定内存的值比较，并在相等的情况下赋值，返回赋值前的原始内容。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr    类型#U64*，  要比较/赋值变量的地址。
 * @param[in]       oldVal 类型#U64， 要比较的值。
 * @param[in]       newVal 类型#U64， 相等时要写入的新值。
 *
 * @retval 返回值是ptr所在的内存地址单元的原始内容。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicXchg32
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicXchg64(U64 *ptr, U64 oldVal, U64 newVal)
{
    U64 tmp = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "1: ldaxr   %0, [%2]      \n"
        "   cmp     %0, %3        \n"
        "   bne     2f            \n"
        "   stxr    %w1, %4, [%2] \n"
        "   cbnz    %w1, 1b       \n"
        "2:                       \n"
        : "+&r"(tmp), "=&r"(ret)
        : "r"(ptr), "r"(oldVal), "r"(newVal)
        : "memory", "cc");

    return tmp;
}

/*
 * @brief 无符号32位变量的原子加，并返回累加前的值
 *
 * @par 描述
 * 无符号32位变量的原子加，并返回累加前的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#U32*，要累加变量的地址。
 * @param[in]       incr 类型#U32， 要累加的数值。
 *
 * @retval 变量累加前的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAddU32 | SRE_AtomicAddU32Rtn
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_AtomicFetchAndAddU32(U32 *ptr, U32 incr)
{
    U32 tmp = 0;
    U32 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %w2,   [%3]       \n"
        "   add   %w0, %w2, %w4     \n"
        "   stlxr %w1, %w0, [%3]    \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(ptr), "r"(incr)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 无符号64位变量的原子加，并返回累加前的值
 *
 * @par 描述
 * 无符号64位变量的原子加，并返回累加前的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr  类型#U64*，要累加变量的地址。
 * @param[in]       incr 类型#U64， 要累加的数值。
 *
 * @retval 变量累加前的值
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicAddU64 | SRE_AtomicAddU64Rtn
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicFetchAndAddU64(U64 *ptr, U64 incr)
{
    U64 tmp = 0;
    U64 oldVal = 0;
    U32 ret = 1;

    OS_EMBED_ASM(
        "   prfm    pstl1strm, [%3] \n"
        "1: ldaxr %2,   [%3]        \n"
        "   add   %0, %2, %4        \n"
        "   stlxr %w1, %0, [%3]     \n"
        "   cbnz  %w1, 1b           \n"
        : "=&r"(tmp), "=&r"(ret), "=&r"(oldVal)
        : "r"(ptr), "r"(incr)
        : "memory", "cc");
    return oldVal;
}

/*
 * @brief 完成32位有符号的变量与指定内存的值比较，并在不相等时增加addVal
 *
 * @par 描述
 * 完成32位有符号的变量unlessVal与指定内存ptr的值比较，并在不相等时增加addVal。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr       类型#S32*，  要比较/赋值变量的地址。
 * @param[in]       addVal    类型#S32， 并在不相等时增加的值。
 * @param[in]       unlessVal 类型#S32， 进行逻辑比较的值
 *
 * @retval 如果指定内存ptr的值即与unlessVal相等，则返回FALSE；
 *         如果指定内存ptr的值即与unlessVal不相等，返回TRUE。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicCompareUnlessAdd64
 */
OS_SEC_ALW_INLINE INLINE bool SRE_AtomicCompareUnlessAdd32(S32 *ptr, S32 addVal, S32 unlessVal)
{
    U32 tmp = 1;
    S32 ret = 0;
    S32 oldVal = 0;

    OS_EMBED_ASM(
        "1: ldaxr   %w2, [%3]      \n"
        "   cmp     %w2, %w4       \n"
        "   beq     2f             \n"
        "   add     %w1, %w2, %w5  \n"
        "   stxr    %w0, %w1, [%3] \n"
        "   cbnz    %w0, 1b        \n"
        "2:                        \n"
        : "=&r"(tmp), "+&r"(ret), "=&r"(oldVal)
        : "r"(ptr), "r"(unlessVal), "r"(addVal)
        : "memory", "cc");

    return oldVal != unlessVal;
}

/*
 * @brief 完成64位有符号的变量与指定内存的值比较，并在不相等时增加addVal
 *
 * @par 描述
 * 完成64位有符号的变量unlessVal与指定内存ptr的值比较，并在不相等时增加addVal。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr       类型#S64*，  要比较/赋值变量的地址。
 * @param[in]       addVal    类型#S64， 并在不相等时增加的值。
 * @param[in]       unlessVal 类型#S64， 进行逻辑比较的值
 *
 * @retval 如果指定内存ptr的值即与unlessVal相等，则返回FALSE；
 *         如果指定内存ptr的值即与unlessVal不相等，返回TRUE。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicCompareUnlessAdd32
 */
OS_SEC_ALW_INLINE INLINE bool SRE_AtomicCompareUnlessAdd64(S64 *ptr, S64 addVal, S64 unlessVal)
{
    U32 tmp = 1;
    S64 ret = 0;
    S64 oldVal = 0;

    OS_EMBED_ASM(
        "1: ldaxr   %2, [%3]      \n"
        "   cmp     %2, %4        \n"
        "   beq     2f            \n"
        "   add     %1, %2, %5    \n"
        "   stxr    %w0, %1, [%3] \n"
        "   cbnz    %w0, 1b       \n"
        "2:                       \n"
        : "=&r"(tmp), "+&r"(ret), "=&r"(oldVal)
        : "r"(ptr), "r"(unlessVal), "r"(addVal)
        : "memory", "cc");

    return oldVal != unlessVal;
}

/*
 * @brief 无符号64位原子读
 *
 * @par 描述
 * 无符号64位原子读。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   ptr 类型#U64*，要读取的地址。
 *
 * @retval 读取的数据
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_AtomicReadU32
 */
OS_SEC_ALW_INLINE INLINE U64 SRE_AtomicReadU64(U64 *ptr)
{
    U64 ret = 0;
    OS_EMBED_ASM(
        "ldaxr %0,   [%1] \n"
        : "=&r"(ret)
        : "r"(ptr)
        : "memory", "cc");
    return ret;
}

/*
 * @brief 用于设置一个有符号32位变量的值
 *
 * @par 描述
 * 设置一个有符号32位变量的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#S32*，参数addr指向一个S32的指针。
 * @param[in]       val  类型#S32， 存储要设置的值。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE void SRE_AtomicSet32(S32 *addr, S32 val)
{
    U32 tmp = 0;
    U32 ret = 1;
    OS_EMBED_ASM(
        "1: ldaxr %w0, [%2]      \n"
        "   stlxr %w1, %w3, [%2] \n"
        "   cbnz  %w1, 1b        \n"
        : "=&r"(tmp), "=&r"(ret)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return;
}

/*
 * @brief 设置一个有符号64位变量的值
 *
 * @par 描述
 * 设置一个有符号64位变量的值。
 *
 * @attention
 * <ul><li>在部分的CPU或DSP上用特殊指令实现，性能较高。</li></ul>
 * <ul><li>基于性能考虑，OS未对入参地址进行校验，其地址合法性由上层业务保证。</li></ul>
 *
 * @param[in,out]   addr 类型#S64*，参数addr指向一个S64的指针。
 * @param[in]       val  类型#S64， 存储要设置的值。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_atomic.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
OS_SEC_ALW_INLINE INLINE void SRE_AtomicSet64(S64 *addr, S64 val)
{
    U64 tmp = 0;
    U32 ret = 1;
    OS_EMBED_ASM(
        "1: ldaxr %0, [%2]      \n"
        "   stlxr %w1, %3, [%2] \n"
        "   cbnz  %w1, 1b       \n"
        : "=&r"(tmp), "=&r"(ret)
        : "r"(addr), "r"(val)
        : "memory", "cc");
    return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OS_ATOMIC_ARMV8_H */
