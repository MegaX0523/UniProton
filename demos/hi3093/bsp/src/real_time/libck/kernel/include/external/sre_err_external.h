/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 错误处理内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组     华为规范整改
 */
#ifndef SRE_ERR_EXTERNAL_H
#define SRE_ERR_EXTERNAL_H

#include "sre_buildef.h"
#include "sre_err.h"

/*
 * 模块间宏定义
 */
// 防止literal与代码都配置在L1时，CDA工具检测时出现两个同值的魔术字,CDA工具端检测的魔术字为0xa53ce169
#define OS_CDA_MAGIC_WORD 0xa53ce169
#define OS_CDA_VERSION 0xc1d0a8dc

#define OS_ERR_RECORD_N 3  // 表示保留二进制低n位，计算OS_ERR_RECORD_NUM用，OS_ERR_RECORD_NUM为2的n次方+1

#define OS_ERR_RECORD_NUM \
    ((1U << OS_ERR_RECORD_N) + 1)  // 2的n次方+1，目前n取3，NUM为9，第一次错误固定记录，后面8个循环记录。

#define OS_ERR_MAGIC_WORD 0xa1b2c3f8

extern void OsErrRecordErrNo(U32 errNo);
#define OS_REPORT_ERROR(errNo)     OsErrRecordErrNo(errNo)

extern void OsErrHandle(const char *fileName, U32 lineNo, U32 errorNo, U32 paraLen, void *para);
#define OS_REPORT_ERROR_DETAIL(errorNo) OsErrHandle(__FUNCTION__, 0, errorNo, 0, NULL)

/* 用于OS_OPTION_NFTL_ERR_POST_PROC */
#define OS_ERROR_TYPE_NUM (SRE_ERRTYPE_FATAL >> 24) /* 中断中需要延后处理的错误类型(所有低于FATAL等级的类型)数 */

/* errno中错误大类的类型掩码 */
#define OS_ERROR_TYPE_MASK (0xffU << 24)

/*
 * 模块间数据结构定义
 */
/* 上报的错误信息结构体 */
struct OsErrInfo {
    U32 tickCnt;
    U32 errorNo;
};

#if defined(OS_OPTION_ERR_DELAY_REPORT)
extern U8 g_errProcFlag;
extern void OsErrDelayProc(void);
#endif

OS_SEC_ALW_INLINE INLINE void OsErrInHwiProc(void)
{
#if defined(OS_OPTION_ERR_DELAY_REPORT)
    if (g_errProcFlag != 0) {
        g_errProcFlag = 0;
        OsErrDelayProc();
    }
#endif
}

extern U32 OsFatalErrClr(void);

extern void OsErrRecordInCda(U32 errorNo);

// 函数返回值void的，都通过此接口记录返回值到cda
OS_SEC_ALW_INLINE INLINE void OsErrRecord(U32 errorNo)
{
    if (errorNo != SRE_OK) {
        OsErrRecordInCda(errorNo);
    }
    return;
}

#endif /* SRE_ERR_EXTERNAL_H */
