/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */
#ifndef BM_MEM_H
#define BM_MEM_H

#include "bm_mem_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


/* 申请一个内存块 */
typedef void *(*MemAllocFunc)(unsigned int size);

/* 申请size字节并返回指向已分配内存的指针，内存地址将按照alignPow动态对齐 */
typedef void *(*MemAllocAlignFunc)(unsigned int size, enum MemAlign align_pow);

/* 释放一个内存块  */
typedef unsigned int (*MemFreeFunc)(void *addr);

struct tag_memory_funcs {
    void *addr;        /* 分区起始地址 */
    MemAllocFunc alloc; /* 申请一个内存块 */
    MemAllocAlignFunc alloc_align; /* 申请size字节并返回指向已分配内存的指针，内存地址将按照alignPow动态对齐 */
    MemFreeFunc free;   /* 释放一个内存块 */
};

/**
 * FSC算法块控制头结构
 * 注意各成员顺序是和其他算法保持一致偏移的，不能随便改动，保持ptNo和其他算法偏移一致
 */
struct tag_memory_ctrl_fsc {
    struct tag_memory_ctrl_fsc *next;
    uintptr_t size;     // 块大小
    uintptr_t prev_size;  // 若前面相邻的物理块空闲，则此字段记录前面空闲块大小，否则为FSC_MEM_PREV_USED
    struct tag_memory_ctrl_fsc *prev;  // 空闲时为上一个控制块地址
};

/*
 * 申请的内存地址对齐标准:4/8字节对齐。
 */
#define MEM_ADDR_ALIGN sizeof(uintptr_t)

/*
 * 内存错误码：动态内存释放时，发现内存越界。
 *
 * 值: 0x02000101
 *
 * 解决方案：使用时注意内存申请时大小，使用大小不要超过内存本身大小。
 */
#define ERRNO_MEM_OVERWRITE ERRNO_BUILD_ERROR(MID_MEM, 0x01)

/*
 * 内存错误码：释放的地址为空。
 *
 * 值: 0x02000102
 *
 * 解决方案：请检查释放的内存块地址是否正确。
 */
#define ERRNO_MEM_FREE_ADDR_INVALID ERRNO_BUILD_ERROR(MID_MEM, 0x02)

/*
 * 内存错误码：内存初始化时，地址为空，初始化失败。
 *
 * 值: 0x02000103
 *
 * 解决方案: 内存初始化时，地址不能为空。
 */
#define ERRNO_MEM_INITADDR_ISINVALID ERRNO_BUILD_ERROR(MID_MEM, 0x03)

/*
 * 内存错误码：内存初始化时，内存分区大小非4字节对齐。
 *
 * 值: 0x02000105
 *
 * 解决方案: 内存分区大小需4字节对齐。
 */
#define ERRNO_MEM_INITSIZE_INVALID ERRNO_BUILD_ERROR(MID_MEM, 0x05)

/*
 * 内存错误码：内存初始化时，地址要4字节对齐，初始化失败。
 *
 * 值: 0x02000106
 *
 * 解决方案: 内存初始化时，地址要4字节对齐。
 */
#define ERRNO_MEM_INITADDR_INVALID ERRNO_BUILD_ERROR(MID_MEM, 0x06)

/*
 * 内存错误码：分区初始化时的分区大小太小。
 *
 * 值: 0x02000107
 *
 * 解决方案: 初始化分区大小改大。
 */
#define ERRNO_MEM_PTCREATE_SIZE_ISTOHWMALL ERRNO_BUILD_ERROR(MID_MEM, 0x07)

/*
 * 内存错误码：分区初始化时的分区大小太大。
 *
 * 值: 0x02000108
 *
 * 解决方案: 初始化分区大小改小。
 */
#define ERRNO_MEM_PTCREATE_SIZE_ISTOOBIG ERRNO_BUILD_ERROR(MID_MEM, 0x08)

/*
 * 动态内存错误码：动态内存释放时要释放的内存块的头被破坏，或已释放。
 *
 * 值: 0x0200010d
 *
 * 解决方案: 保证对内存写操作时，不要出现写越界。
 *
 */
#define ERRNO_MEM_FREE_SH_DAMAGED ERRNO_BUILD_ERROR(MID_MEM, 0x0b)


#define FSC_MEM_LAST_IDX                                 31
#define FSC_MEM_IDX2BIT(idx)                             (0x80000000UL >> (idx))
#define FSC_MEM_MAGIC_USED                               (struct tag_memory_ctrl_fsc *)0x5a5aa5a5
#define FSC_MEM_TAIL_MAGIC                               0xABCDDCBA
#define FSC_MEM_SZ2IDX(size)                             (31 - bm_get_bits((unsigned int)(size)))
#define FSC_MEM_SIZE_ALIGN                               MEM_ADDR_ALIGN
#define FSC_MEM_USED_HEAD_SIZE                           (sizeof(struct tag_memory_ctrl_fsc))
#define FSC_MEM_TAIL_SIZE                                (sizeof(unsigned int))
#define FSC_MEM_MAXVAL                                   ((1U << FSC_MEM_LAST_IDX) - FSC_MEM_SIZE_ALIGN)

/*
 * 模块间宏定义
 */
#define FSC_MEM_SLICE_HEAD_SIZE sizeof(struct tag_memory_ctrl_fsc)
#define FSC_MEM_MIN_SIZE (FSC_MEM_SLICE_HEAD_SIZE + FSC_MEM_SIZE_ALIGN)

/*
 * 模块间宏定义
 */
extern unsigned int bm_get_bits(unsigned int value);
extern void *bm_memory_alloc(unsigned int size);
extern unsigned int bm_memory_free(void *addr);
extern void *bm_memory_alloc_align(unsigned int size, enum MemAlign align_pow);
extern unsigned int bm_memory_init_fsc(uintptr_t addr, unsigned int size);
extern struct tag_memory_funcs g_memory_arith_api; /* 算法对应API */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* BM_MEM_H */
