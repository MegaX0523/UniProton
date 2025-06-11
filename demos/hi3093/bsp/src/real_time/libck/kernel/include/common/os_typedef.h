/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 定义基本数据类型和数据结构。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_TYPEDEF_H
#define OS_TYPEDEF_H

#include <stdint.h>
#include <stdbool.h>

#ifndef LIBCK_TOOL

#ifndef INLINE
#define INLINE static __inline __attribute__((always_inline))
#endif

#ifndef OS_EMBED_ASM
#define OS_EMBED_ASM __asm__ __volatile__
#endif

#else
#ifdef _WIN32
#include <windows.h>
#endif

#ifndef INLINE
#define INLINE static __inline
#endif

#ifndef OS_EMBED_ASM
#define OS_EMBED_ASM(...)
#endif

#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;
typedef signed char S8;
typedef signed short S16;
typedef signed int S32;
typedef signed long long S64;

typedef void (*OsVoidFunc)(void);

typedef void *VirtAddr;
typedef void *PhyAddr;

#define ALIGN(addr, boundary) (((uintptr_t)(addr) + (boundary) - 1) & ~((uintptr_t)(boundary) - 1))
#define TRUNCATE(addr, size)  ((addr) & ~((uintptr_t)(size) - 1))

#ifndef OS_SEC_ALW_INLINE
#define OS_SEC_ALW_INLINE
#endif

#ifndef FALSE
#define FALSE ((bool)0)
#endif

#ifndef TRUE
#define TRUE ((bool)1)
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#define OS_FAIL    1
#define OS_ERROR   (U32)(-1)
#define OS_INVALID (-1)

#ifndef OS_OK
#define OS_OK 0
#endif

#ifndef U8_INVALID
#define U8_INVALID 0xffU
#endif

#ifndef U12_INVALID
#define U12_INVALID 0xfffU
#endif

#ifndef U16_INVALID
#define U16_INVALID 0xffffU
#endif

#ifndef U32_INVALID
#define U32_INVALID 0xffffffffU
#endif

#ifndef U64_INVALID
#define U64_INVALID 0xffffffffffffffffUL
#endif

#ifndef U32_MAX
#define U32_MAX 0xFFFFFFFFU
#endif

#ifndef S32_MAX
#define S32_MAX 0x7FFFFFFF
#endif

#ifndef S32_MIN
#define S32_MIN (-S32_MAX-1)
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_TYPEDEF_H */
