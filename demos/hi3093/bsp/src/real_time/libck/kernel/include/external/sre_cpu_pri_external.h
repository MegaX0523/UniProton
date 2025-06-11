/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Author: LibCK项目组
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_CPU_PRI_EXTERNAL_H
#define SRE_CPU_PRI_EXTERNAL_H

#include "sre_hw_external.h"
#include "sre_sys_external.h"
#include "sre_atomic.h"
#include "sre_cpu_external.h"

#if (OS_MAX_CORE_NUM > OS_CORES_32)
#define OS_CORE_MASK U64
#else
#define OS_CORE_MASK U32
#endif

#define OS_CORE_INV_ID 0xFFFFFFFF
#if defined(OS_OPTION_SMP)

/*
 * 模块间宏定义
 */
struct TagCpuMask {
    OS_CORE_MASK bits;
};

#define OS_CORE_PRI_S(rq) (&(rq)->domain->corePri)

// 32个核以内使用该宏，否则需要适配
#define OS_COREMASK_AND(m1, m2) ((m1) & (m2))
#define OS_IS_CORE_IN_MASK(core, mask) OS_COREMASK_AND(((OS_CORE_MASK)(1) << (core)), (mask))
#define OS_COREMASK_FIRST(mask) OsGetRMB(mask)

/*
 * 模块间结构体定义
 */
struct TagCoreVec {
    /* 核 bitmap，目前最大支持32个核 */
    OS_CORE_MASK coreMask;
};

struct TagCorePri {
    /* 64个优先级 */
    volatile uintptr_t lock;
    struct TagCoreVec priToCore[OS_TSK_NUM_OF_PRIORITIES];
};

/*
 * 模块间全局变量声明
 */
extern struct TagCorePri g_corePri;

/*
 * 模块间函数声明
 */
struct TagTskCB;

extern bool OsCorePriFind(struct TagCorePri *corepri, struct TagTskCB *tsk, OS_CORE_MASK *coreMask);
extern void OsCorePriSet(struct TagCorePri *corepri, U32 coreId, U32 newPri, U32 priPrev);
extern void OsCorePriInit(struct TagCorePri *corepri);
extern U32 OsFindHighestPri(const U32 *priArray, U32 size);

/* 清除数组bit，内部函数，传参保证bit位不越界 */
OS_SEC_ALW_INLINE INLINE void OsClearArrayBit(U32 bit, OS_CORE_MASK *array)
{
    // bit落在第几个word里
    U32 *coreBit = (U32 *)array + (uintptr_t)OS_GET_32BIT_ARRAY_INDEX(bit);
    // 外部上锁
    *coreBit &= ~OS_32BIT_MASK(bit);
}

/* 设置数组bit，内部函数，传参保证bit位不越界 */
OS_SEC_ALW_INLINE INLINE void OsSetArrayBit(U32 bit, OS_CORE_MASK *array)
{
    // bit落在第几个word里
    U32 *coreBit = (U32 *)array + (uintptr_t)OS_GET_32BIT_ARRAY_INDEX(bit);
    // 外部上锁
    *coreBit |= OS_32BIT_MASK(bit);
}

/* 清除核的coremask bit */
OS_SEC_ALW_INLINE INLINE void OsCoreMaskClearCore(U32 coreId, OS_CORE_MASK *pcoreMask)
{
    OsClearArrayBit(coreId, pcoreMask);
}

/* 设置核的coremask bit */
OS_SEC_ALW_INLINE INLINE void OsCoreMaskSetCore(U32 coreId, OS_CORE_MASK *pcoreMask)
{
    OsSetArrayBit(coreId, pcoreMask);
}

OS_SEC_ALW_INLINE INLINE U32 OsCoreMaskGetNum(OS_CORE_MASK coreMask)
{
    U32 loop;
    U32 count = 0;
    for (loop = 0; loop < g_maxNumOfCores; loop++) {
        if ((((OS_CORE_MASK)1U << loop) & coreMask) != 0) {
            count++;
        }
    }
    return count;
}

#endif
#endif /* SRE_CPU_PRI_EXTERNAL_H */
