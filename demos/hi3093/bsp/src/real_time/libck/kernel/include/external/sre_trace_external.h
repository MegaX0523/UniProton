/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: TRACE模块的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组     华为规范整改
 */
#ifndef SRE_TRACE_EXTERNAL_H
#define SRE_TRACE_EXTERNAL_H

#include "sre_trace.h"
#include "sre_sys_external.h"

typedef U32(*TraceIntDeleteFunc)(U32);

OS_SEC_ALW_INLINE INLINE void OsTraceSemRecord(U32 arg0, U32 arg1, U32 arg2)
{
    if (g_traceSemRecord != NULL) {
        g_traceSemRecord(arg0, arg1, arg2);
    }
    return;
}

OS_SEC_ALW_INLINE INLINE void OsTraceErrRecord(U32 arg0, U32 arg1, U32 arg2)
{
    if (g_traceErrRecord != NULL) {
        g_traceErrRecord(arg0, arg1, arg2);
    }
    return;
}

#endif /* SRE_TRACE_EXTERNAL_H */
