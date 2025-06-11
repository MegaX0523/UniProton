/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 定义基本数据类型和数据结构。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_TYPEDEF_H
#define SRE_TYPEDEF_H

#include "./common/os_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_NULL_BYTE ((U8)0xffU)
#define OS_NULL_SHORT ((U16)0xffffU)
#define OS_NULL_INT ((U32)0xffffffffU)
#define NULL_DWORD ((U32)0xFFFFFFFFU)

#ifndef LIKELY
#define LIKELY(x) __builtin_expect(!!(x), 1)
#endif
#ifndef UNLIKELY
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif

#ifndef SRE_OK
#define SRE_OK 0
#endif

#ifndef SRE_FAIL
#define SRE_FAIL 1
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_TYPEDEF_H */
