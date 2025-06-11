/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: exc模块内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_KEXC_EXTERNAL_H
#define SRE_KEXC_EXTERNAL_H

#include "sre_exc.h"
#include "sre_cpu_external.h"

/*
 * 模块内宏定义
 */
#define EXC_RECORD_SIZE (sizeof(struct ExcInfo))
#define OS_EXC_INFO_ADDR (&EXC_INFO_INTERNAL.excInfo)

#ifndef INVALIDPID
#define INVALIDPID         0xFFFFFFFF
#endif

#ifndef INVALIDTYPE
#define INVALIDTYPE        0xFFFFFFFF
#endif

#ifndef INVALIDSTACKBOTTOM
#define INVALIDSTACKBOTTOM 0xFFFFFFFF
#endif
/*
 * 模块内数据结构定义
 */
struct TagExcInfoInternal {
#if defined(OS_OPTION_CDA)
    /* 异常信息魔术字，用于异常信息解析 */
    U32 excInfoMagicWord;
    /* 芯片类型 */
    U32 platform;
#endif
    /* 异常信息 */
    struct ExcInfo excInfo;
};

/*
 * 模块内全局变量声明
 */
extern struct ExcModInfo g_excModInfo;

#if !defined(OS_OPTION_SMP)
extern U32 g_curNestCount;
#define CUR_NEST_COUNT g_curNestCount

extern struct TagExcInfoInternal g_excInfoInternal;
#define EXC_INFO_INTERNAL g_excInfoInternal
#else
extern U32 g_curNestCount[OS_MAX_CORE_NUM];
#define CUR_NEST_COUNT (g_curNestCount[THIS_CORE()])

extern struct TagExcInfoInternal g_excInfoInternal[OS_MAX_CORE_NUM];
#define EXC_INFO_INTERNAL (g_excInfoInternal[THIS_CORE()])
#endif

extern void OsExcTrace(U32 excPhase);
#define OS_EXC_TRACE(excPhase) OsExcTrace(excPhase)

#endif /* SRE_KEXC_EXTERNAL_H */
