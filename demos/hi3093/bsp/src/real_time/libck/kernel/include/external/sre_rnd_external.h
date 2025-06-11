/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 随机数获取的内部头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_RND_EXTERNAL_H
#define SRE_RND_EXTERNAL_H

#include "sre_base.h"
#include "sre_sys.h"

/* 随机rand参数 */
#define OS_RND_MEM_PROTCECT_DEFAULT_VALUE      0x0UL // 内存保护初始化值
#define OS_RND_DEFAULT_VALUE                   0xFFFFFFFFUL // 分区默认初始化值
#define OS_RND_DEFAULT_MEMPT_MAX               0x1000  // 约定的默认内存分区的RND值最大不超4K
#define OS_RND_DEF_PT_VALUE                    0xAAAAAAAAUL // 分区初始值为0xFFFFFFFF，读清后变成0xAAAAAAAA

extern U32 OsRndNumGet(enum OsRndNumType type);
extern void OsRndNumClr(enum OsRndNumType type);

extern U32 g_memDefPtRdmOffset;
#if defined(OS_OPTION_RND)
extern U32 g_memProtectRdmValue;
OS_SEC_ALW_INLINE INLINE U32 OsGetRndU32(void)
{
    return g_memProtectRdmValue;
}
#else
OS_SEC_ALW_INLINE INLINE U32 OsGetRndU32(void)
{
    return OS_RND_MEM_PROTCECT_DEFAULT_VALUE;
}
#endif

OS_SEC_ALW_INLINE INLINE U16 OsGetRndU16(void)
{
    return (U16)OsGetRndU32();
}

#endif /* SRE_RND_EXTERNAL_H */
