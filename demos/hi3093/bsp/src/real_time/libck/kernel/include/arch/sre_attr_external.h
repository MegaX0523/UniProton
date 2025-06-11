/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 属性宏相关内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_ATTR_EXTERNAL_H
#define SRE_ATTR_EXTERNAL_H

#include "sre_buildef.h"
#include "sre_typedef.h"
#if defined(OS_OPTION_INIT)
#include "sre_preinit.h"
#endif

#ifndef OS_ATTR_WEAK
#define OS_ATTR_WEAK __attribute__((__weak__))
#endif

#if defined(OS_OPTION_API_PEG)
#ifndef OS_ATTR_PEG
#define OS_ATTR_PEG OS_ATTR_WEAK
#endif
#else
#ifndef OS_ATTR_PEG
#define OS_ATTR_PEG
#endif
#endif

#if defined(OS_OPTION_INIT)
#define OS_FUNCTION_INIT(func, level) U32 func(void) __attribute__((constructor(level)))
#else
#define OS_FUNCTION_INIT(func, level) U32 func(void)
#endif

#if defined(OS_ARCH_RISCV)
#include "../cpu/riscv/os_attr_riscv_external.h"
#endif

/* 此处统一定义通用段属性的默认值 */
#ifndef OS_SEC_L0_TEXT
#define OS_SEC_L0_TEXT
#endif

#ifndef OS_SEC_TEXT
#define OS_SEC_TEXT
#endif

#ifndef OS_SEC_L2_TEXT
#define OS_SEC_L2_TEXT
#endif

#ifndef OS_SEC_L4_TEXT
#define OS_SEC_L4_TEXT
#endif

#ifndef OS_SEC_LX0_TEXT
#define OS_SEC_LX0_TEXT
#endif

#ifndef OS_SEC_LX_TEXT
#define OS_SEC_LX_TEXT
#endif

#ifndef OS_SEC_DATA
#define OS_SEC_DATA
#endif

#ifndef OS_SEC_L4_DATA
#define OS_SEC_L4_DATA
#endif

#ifndef OS_SEC_BSS
#define OS_SEC_BSS
#endif

#ifndef OS_SEC_L4_BSS
#define OS_SEC_L4_BSS
#endif

#ifndef OS_SEC_L4_INSTSH_DATA
#define OS_SEC_L4_INSTSH_DATA
#endif

#ifndef OS_SEC_L0_OPTION_TEXT
#define OS_SEC_L0_OPTION_TEXT OS_SEC_L0_TEXT
#endif

#ifndef OS_SEC_L1_OPTION_TEXT
#define OS_SEC_L1_OPTION_TEXT OS_SEC_TEXT
#endif

#ifndef OS_SEC_L2_OPTION_TEXT
#define OS_SEC_L2_OPTION_TEXT OS_SEC_L2_TEXT
#endif

#endif /* SRE_ATTR_EXTERNAL_H */
