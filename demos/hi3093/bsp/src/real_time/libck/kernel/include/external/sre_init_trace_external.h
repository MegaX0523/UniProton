/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: INIT TRACE内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组     华为规范整改
 */
#ifndef SRE_INIT_TRACE_EXTERNAL_H
#define SRE_INIT_TRACE_EXTERNAL_H

#include "sre_lib_external.h"
#include "sre_base.h"
#include "sre_sys.h"
#include "sre_cpu_external.h"

/*
 * 模块间宏定义
 */
#define OS_MOUDLE_NAME_STR_LEN 20

#define OS_SYS_INIT_PHASE_SHIFT 0x10 /* 初始化阶段状态为高16位 */

/*
 * 用于标记当前系统开始运行到哪个阶段(实时值),initPhase --- OS_INIT_PHASE | MODULE_ID。
 * OS_INIT_PHASE表示系统初始化阶段状态bit[31-16]，MODULE_ID表示当前系统初始化的模块号bit[0-15]
 */
#define OS_INIT_TRACE_BEG(osPhase, modId) OsSysInitTraceBegin((osPhase), (U32)(modId))

/*
 * 记录系统中模块是否初始化结束，0-63位用来记录已进行初始化的所有模块的模块号，
 * 若某个模块正确进行完了初始化操作，将其模块号对应的位置为1
 */
#define OS_INIT_TRACE_END(osPhase, modId) OsSysInitTraceEnd((osPhase), (U32)(modId))
#if defined(OS_OPTION_INIT)
/* 记录错误码 */
#define OS_INIT_TRACE_ERR_ID(errNO) OsSysInitTraceErrNo(errNO)

#if defined(OS_OPTION_INIT_PHASE_QUERY)
#define OS_INIT_PHASE_SELF_SET(phase) OsInitPhaseSet(SRE_GetCoreID(), phase)
#else
#define OS_INIT_PHASE_SELF_SET(phase)
#endif
#endif
// 防止literal与代码都配置在L1时，若CDA工具检测时出现两个同值的魔术字,CDA工具端检测的魔术字为0x69965AB6
#define OS_INIT_TRACE_PARSE_MAGIC_WORD 0x69965AB6

/*
 * 模块间全局变量声明
 */
extern struct SysTraceInfo g_sysInitTraceInfo;

#if defined(OS_OPTION_INIT_TRACE_USR_HOOK)
typedef void (*InitTraceHook)(U32 initPhase, U32 modId);
extern InitTraceHook g_initTraceBeginUsrHook;
extern InitTraceHook g_initTraceEndUsrHook;
#endif

/*
 * 模块间函数声明
 */
extern void OsSysInitTraceBegin(U32 initPhase, U32 modId);
extern void OsSysInitTraceEnd(U32 osPhase, U32 modId);
extern void OsInitTraceCBAddrReg(void);
extern void OsSysInitTraceErrNo(U32 errNo);

OS_SEC_ALW_INLINE INLINE void OsInitTraceModuleSet(U32 modId)
{
    if (modId < (U32)OS_MID_BUTT) {
        g_sysInitTraceInfo.initTrace[OS_GET_32BIT_ARRAY_INDEX(modId)] |= OS_32BIT_MASK(modId);
    }
}

OS_SEC_ALW_INLINE INLINE bool OsInitTraceIsModuleReady(U32 mid)
{
    if (mid < (U32)OS_MID_BUTT) {
        if ((g_sysInitTraceInfo.initTrace[OS_GET_32BIT_ARRAY_INDEX(mid)] & (OS_32BIT_MASK(mid))) != 0) {
            return TRUE;
        }
    }

    return FALSE;
}

/* smp下主核才记录,       否则从核就重新并发修改了 */
OS_SEC_ALW_INLINE INLINE bool OsInitTraceNeedRecord(void)
{
#if defined(OS_OPTION_SMP)
    return (OsGetHwThreadId() == SRE_GetPrimaryCore());
#else
    return TRUE;
#endif
}

// 实例间初始化trace
extern uintptr_t OsInitPhaseAddrGet(void);
extern void OsInitPhaseAddrSet(uintptr_t addr);
extern void OsInitPhaseGet(U32 coreID, U32 *phase);

#if defined(OS_OPTION_INIT_PHASE_QUERY)
extern void OsInitPhaseSet(U32 coreID, U32 phase);
#define OS_INIT_PHASE_SET(coreID, phase) OsInitPhaseSet((coreID), (U32)(phase))
#else
#define OS_INIT_PHASE_SET(coreID, phase)
#endif

#endif /* SRE_INIT_TRACE_EXTERNAL_H */
