/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 栈越界检测模块的内部公共头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组     华为规范整改
 */
#ifndef SRE_STKMON_EXTERNAL_H
#define SRE_STKMON_EXTERNAL_H

#include "sre_stkmon.h"

#define OS_STKMON_BREAK_INDEX 0

extern U32 g_stkMonFlg;

#if defined(OS_OPTION_STKMON)
extern U32 g_stkInitState;
extern U32 g_stkMonTaskID;
OS_SEC_ALW_INLINE INLINE bool OsStkMonInitedChk(void)
{
    return g_stkInitState != 0;
}
#define OS_STKMON_TASKID        g_stkMonTaskID
#else
OS_SEC_ALW_INLINE INLINE bool OsStkMonInitedChk(void)
{
    return FALSE;
}
#define OS_STKMON_TASKID        0
#endif

#endif /* SRE_STKMON_EXTERNAL_H */
