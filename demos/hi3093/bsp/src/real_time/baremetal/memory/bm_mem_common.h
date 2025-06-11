/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */
#ifndef BM_MEM_COMMON_H
#define BM_MEM_COMMON_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

/* 判断初始化内存地址和大小是否为4字节对齐 */
#define MEM_GETBIT(addr) ((addr) & (unsigned int)(sizeof(unsigned int) - 1))

#define ALIGN(addr, boundary) (((uintptr_t)(addr) + (boundary) - 1) & ~((uintptr_t)(boundary) - 1))

/* Success */
#ifndef EOK
#define EOK 0
#endif

/* Fail */
#ifndef HW_FAIL
#define HW_FAIL 1
#endif

#ifndef SEC_ALW_INLINE
#define SEC_ALW_INLINE
#endif

#ifndef INLINE
#define INLINE static __inline __attribute__((always_inline))
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef FALSE
#define FALSE ((bool)0)
#endif

#ifndef TRUE
#define TRUE ((bool)1)
#endif

/*
 * 定义错误的等级:严重级别
 *
 */
#define ERRTYPE_ERROR  (0x02U << 24)

/*
 * 错误码标记位，0x00表示Hw
 *
 */
#define ERRNO_HW_ID    (0x00U << 16)

#define MID_MEM    0x1
#define ERRNO_BUILD_ERROR(mid, errno) (ERRTYPE_ERROR | ERRNO_HW_ID | ((unsigned int)(mid) << 8) | (errno))

#define DAIF_IRQ_BIT      (1U << 1)
#define INT_MASK          (1U << 7)

/*
 * 内存对齐方式。
 */
enum MemAlign {
    MEM_ADDR_ALIGN_004 = 2, /* 4字节对齐 */
    MEM_ADDR_ALIGN_008 = 3, /* 8字节对齐 */
    MEM_ADDR_ALIGN_016 = 4, /* 16字节对齐 */
    MEM_ADDR_ALIGN_032 = 5, /* 32字节对齐 */
    MEM_ADDR_ALIGN_064 = 6, /* 64字节对齐 */
    MEM_ADDR_ALIGN_128 = 7, /* 128字节对齐 */
    MEM_ADDR_ALIGN_256 = 8, /* 256字节对齐 */
    MEM_ADDR_ALIGN_512 = 9, /* 512字节对齐 */
    MEM_ADDR_ALIGN_1K = 10, /* 1K字节对齐 */
    MEM_ADDR_ALIGN_2K = 11, /* 2K字节对齐 */
    MEM_ADDR_ALIGN_4K = 12, /* 4K字节对齐 */
    MEM_ADDR_ALIGN_8K = 13, /* 8K字节对齐 */
    MEM_ADDR_ALIGN_16K = 14, /* 16K字节对齐 */
    MEM_ADDR_BUTT /* 字节对齐非法 */
};

#ifndef OS_EMBED_ASM
#define OS_EMBED_ASM __asm__ __volatile__
#endif

/* ***************************** 配置内存管理模块 ************************** */
/* 用户可以创建的最大分区数，取值范围[0,253] */
#define MEM_MAX_PT_NUM                               200
/* 私有FSC内存分区起始地址 */
#define MEM_FSC_PT_ADDR                              (uintptr_t)(&g_memory_region[0])
/* 私有FSC内存分区大小 */
#define MEM_FSC_PT_SIZE                              0x3000

extern unsigned char g_memory_region[];

unsigned int bm_init_hwi(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif  /* BM_MEM_COMMON_H */