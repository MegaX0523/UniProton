/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: LIB模块内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_LIB_EXTERNAL_H
#define SRE_LIB_EXTERNAL_H

#include "securec.h"
#include "sre_sys.h"
#include "sre_attr_external.h"

/* 对齐操作API */
#define OS_ADDR_OVERTURN_CHK(addr, size) (((size) != 0) && (((addr) + (size) - 1) < (addr)))
#define OS_ALIGN_CHK(addr, align)       (((uintptr_t)(addr) & ((align) - 1)) == 0)
#define OS_NOT_ALIGN_CHK(addr, align)   (!OS_ALIGN_CHK(addr, align))
#define OS_ALIGN(addr, align)           ((uintptr_t)(addr) & ((align) - 1))

#define OS_32BIT_MOD(num)               ((num) % 32)

#define OS_GET_32BIT_HIGH_8BIT(num)     (((num) >> 24) & 0xffU)
#define OS_GET_32BIT_LOW_8BIT(num)      ((num) & 0xffU)

#define OS_GET_32BIT_HIGH_16BIT(num)    ((num) >> 16)
#define OS_GET_32BIT_LOW_16BIT(num)     ((num) & 0xFFFFU)

#define OS_GET_16BIT_HIGH_8BIT(num)     ((num) >> 8)
#define OS_GET_16BIT_LOW_8BIT(num)      ((num) & 0xFFU)

#define OS_GET_8BIT_HIGH_4BIT(num)      ((num) >> 4)
#define OS_GET_8BIT_LOW_4BIT(num)       ((num) & 0xFU)

#define OS_GET_32BIT_ARRAY_INDEX(num)   ((num) >> 5)
#define OS_GET_32BIT_ARRAY_BASE(num)    ((num) << 5)

#define OS_32BIT_MASK(bit)              (1U << OS_32BIT_MOD(bit))
#define OS_32BIT_VERSE_MASK(bit)        (~(1U << OS_32BIT_MOD(bit)))

#define OS_GET_BIT_IN_WORD(num)         ((num) & 0x1FUL)

#define OS_64BIT_SET(high, low)         (((U64)(high) << 32) + (U64)(low))
#define OS_LMB32                        31

#define CPUMASK_HAS_BIT(mask, bit) ((mask) & (1U << (bit)))
#define CPUMASK_BCLR(mask, bit) ((mask) &= ~(1U << (bit)))
#define CPUMASK_BSET(mask, bit) ((mask) |= (1U << (bit)))
#define CPUMASK_AND(m1, m2) ((m1) & (m2))

#define OS_MAX_U32 0xFFFFFFFFU
#define OS_MAX_U16 0xFFFFU
#define OS_MAX_U12 0xFFFU
#define OS_MAX_U8 0xFFU
#define OS_MAX_U4 0xFU

#define OS_MAX_S8 0xFF

#define OS_BYTES_PER_DWORD              (sizeof(U64))
#define OS_BYTES_PER_WORD               (sizeof(U32))
#define OS_BITS_PER_BYTE                8

#define OS_BITNUM_2_MASK(bitNum) ((1U << (bitNum)) - 1)
#define OS_WORD_ALLBIT_MASK 0xFFFFFFFFU

#define OS_IS_VISIBLE_CHAR(word) ((((word) >= 'a') && ((word) <= 'z')) || (((word) >= 'A') && ((word) <= 'Z')) || \
                                  ((word) == '_') || (((word) <= '9') && ((word) >= '0')))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define OS_DECIMAL    10
#define OS_OCTAL      8
#define OS_HEX        16

#define OS_BIT0_MASK  (0x1U << 0)
#define OS_BIT1_MASK  (0x1U << 1)
#define OS_BIT2_MASK  (0x1U << 2)
#define OS_BIT3_MASK  (0x1U << 3)
#define OS_BIT4_MASK  (0x1U << 4)
#define OS_BIT5_MASK  (0x1U << 5)
#define OS_BIT6_MASK  (0x1U << 6)
#define OS_BIT7_MASK  (0x1U << 7)
#define OS_BIT8_MASK  (0x1U << 8)
#define OS_BIT9_MASK  (0x1U << 9)
#define OS_BIT10_MASK (0x1U << 10)
#define OS_BIT11_MASK (0x1U << 11)
#define OS_BIT12_MASK (0x1U << 12)
#define OS_BIT13_MASK (0x1U << 13)
#define OS_BIT14_MASK (0x1U << 14)
#define OS_BIT15_MASK (0x1U << 15)
#define OS_BIT16_MASK (0x1U << 16)
#define OS_BIT17_MASK (0x1U << 17)
#define OS_BIT18_MASK (0x1U << 18)
#define OS_BIT19_MASK (0x1U << 19)
#define OS_BIT20_MASK (0x1U << 20)
#define OS_BIT21_MASK (0x1U << 21)
#define OS_BIT22_MASK (0x1U << 22)
#define OS_BIT23_MASK (0x1U << 23)
#define OS_BIT24_MASK (0x1U << 24)
#define OS_BIT25_MASK (0x1U << 25)
#define OS_BIT26_MASK (0x1U << 26)
#define OS_BIT27_MASK (0x1U << 27)
#define OS_BIT28_MASK (0x1U << 28)
#define OS_BIT29_MASK (0x1U << 29)
#define OS_BIT30_MASK (0x1U << 30)
#define OS_BIT31_MASK (0x1U << 31)

#define OS_BIT_SET_VALUE(bit) (0x1UL << (bit))

#define BNUM_TO_WNUM(bnum)   (((bnum) + 31) >> 5)
#define OS_WORD_BIT_NUM      32
#define OS_WORD_BIT_POW      5
#define OS_DWORD_BIT_NUM     64
#define OS_DWORD_BIT_POW     6
#define OS_HALF_WORD_BIT_NUM 16

#define BITS_PER_CHAR 8

#if defined(OS_OPTION_CPU64)
#define OS_64BIT_CHK(num)    (((num) & (~0xffffffffUL)) != 0)
#else
#define OS_64BIT_CHK(num)    FALSE
#endif

#define OS_DIV_UP(a, b) (((a) + (b) - 1) / (b))

#define QUEUE0_IDX 0
#define QUEUE1_IDX 1
#define QUEUE2_IDX 2

/* 宏展开优先展开参数，但是#会阻止参数展开，故需要定义另外一个宏 */
#define OS_STR(string) #string
#define OS_STR_FICATION(string) OS_STR(string)

#ifndef OS_STATIC_NO_INLINE
#define OS_STATIC_NO_INLINE static __noinline __attribute__((noinline))
#endif

#ifndef OS_NO_INLINE
#define OS_NO_INLINE  __attribute__((noinline))
#endif

#ifndef ATTR_ALIGN_128
#define ATTR_ALIGN_128 __attribute__((aligned(128))) /* 表示128 字节对齐 */
#endif

#ifndef ATTR_ALIGN_64
#define ATTR_ALIGN_64 __attribute__((aligned(64)))   /* 表示64 字节对齐 */
#endif

#ifndef ATTR_ALIGN_32
#define ATTR_ALIGN_32 __attribute__((aligned(32)))   /* 表示32 字节对齐 */
#endif

#ifndef ATTR_ALIGN_16
#define ATTR_ALIGN_16 __attribute__((aligned(16)))   /* 表示16 字节对齐 */
#endif

#ifndef ATTR_ALIGN_8
#define ATTR_ALIGN_8 __attribute__((aligned(8)))     /* 表示8字节对齐 */
#endif

#ifndef ATTR_ALIGN_4
#define ATTR_ALIGN_4 __attribute__((aligned(4)))     /* 表示4字节对齐 */
#endif

#define OS_SYS_UINT_IDX(coreId) ((U32)(coreId) >> 5)  // 一个UINT32表示32个核
#define OS_SYS_UINT_LCOREID_OFFSET(coreId) (((coreId) & 0x1fUL))  // 核号在一个UINT32里的偏移

// 判断coreId是否在corebitMap内 非0:在BitMap内，0:不在bitMap内 ((1 << (coreID%32)) & coreBitMap[coreID/32])
#define IS_CORE_IN_BITMAP(coreId, coreBitMap) \
    (((1UL << OS_SYS_UINT_LCOREID_OFFSET(coreId)) & (coreBitMap)[OS_SYS_UINT_IDX(coreId)]) != 0)
#define OS_SYS_CORE_MASK_BIT_VALID(coreBitMap, coreId) \
    (((coreBitMap)->coreMask[OS_SYS_UINT_IDX(coreId)] & (1UL << (OS_SYS_UINT_LCOREID_OFFSET(coreId)))) != 0)

#define OS_SYS_CORE_MASK_BIT_CLEAR(coreBitMap, coreId)                                                     \
    do {                                                                                                   \
        (coreBitMap)->coreMask[OS_SYS_UINT_IDX(coreId)] &= (~(1UL << OS_SYS_UINT_LCOREID_OFFSET(coreId))); \
    } while (0)

#define OS_SYS_CORE_MASK_BIT_SET(coreBitMap, coreId)                                                    \
    do {                                                                                                \
        (coreBitMap)->coreMask[OS_SYS_UINT_IDX(coreId)] |= (1UL << OS_SYS_UINT_LCOREID_OFFSET(coreId)); \
    } while (0)

#define OS_SYS_CORE_MASK_BIT_AND(coreBitMap, bitAndValue, coreId)                                            \
    do {                                                                                                     \
        (coreBitMap)->coreMask[OS_SYS_UINT_IDX(coreId)] &= (bitAndValue)->coreMask[OS_SYS_UINT_IDX(coreId)]; \
    } while (0)

/* single world struct to integer */
#define TO_INTEGER(s)     \
    ({                    \
        union {           \
            typeof(s) us; \
            uintptr_t uv; \
        } u_;             \
        u_.us = (s);      \
        u_.uv;            \
    })

/* single world struct from integer */
#define FROM_INTEGER(v, type) \
    ({                        \
        union {               \
            type us;          \
            uintptr_t uv;     \
        } u_;                 \
        u_.uv = (v);          \
        u_.us;                \
    })

/*
 * 模块间宏定义
 */
extern void OsAdd64(U32 *low, U32 *high, U32 oldLow, U32 oldHigh);
extern void OsSub64(U32 *low, U32 *high, U32 oldLow, U32 oldHigh);

#if defined(OS_OPTION_MEM_OP_HPF)
extern void OsMemHpfSet(void *dest, U32 c, U32 count);
extern void OsMemHpfCopy(void *dest, const void *src, U32 count);
#endif

#if defined(OS_OPTION_HW_DDR_CONFLICT)
extern void OsVecMemSetDelay(void *dst, U32 value, U32 size);
extern void OsVecMemCpyDelay(void *dst, const void *src, U32 size);
#endif

extern void OsSysCoreMaskOrOp(struct SysSreCoreMask *coreMaskSet, const struct SysSreCoreMask *orOpValue);
extern bool OsSysCoreMaskCrossCheck(const struct SysSreCoreMask *coreMaskBase,
                                    const struct SysSreCoreMask *coreMaskCompare);
extern void OsSysCoreMaskAndOp(struct SysSreCoreMask *coreMaskSet, const struct SysSreCoreMask *andOpValue);
extern void OsSysCoreMaskClear(const struct SysSreCoreMask *coreMaskClear, struct SysSreCoreMask *coreMaskBase);
extern bool OsSysCoreMaskCompare(struct SysSreCoreMask *coreMaskBase, struct SysSreCoreMask *coreMaskCompare);
extern bool OsSysCoreMaskValid(const struct SysSreCoreMask *coreMask);
extern void OsSysCoreMaskSet(struct SysSreCoreMask *coreMask, struct SysSreCoreMask *setValue);
extern bool OsSysCoreMaskSubset(const struct SysSreCoreMask *coreMask, const struct SysSreCoreMask *coreMaskBase);
extern bool OsSysCoreMaskSubsetGet(const struct SysSreCoreMask *coreMaskSet,
                                   const struct SysSreCoreMask *coreMaskBase,
                                   struct SysSreCoreMask *coreMaskOut);

extern U32 OsSysCoreMaskWeight(const struct SysSreCoreMask *coreMask, U32 *coreNum);
extern U32 OsSysCoreMaskGetMinCore(const struct SysSreCoreMask *coreMaskSet, U32 *coreId);

#endif /* SRE_LIB_EXTERNAL_H */
