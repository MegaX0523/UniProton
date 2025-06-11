/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 异常模块的内部头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_EXC_EXTERNAL_H
#define SRE_EXC_EXTERNAL_H

#include "sre_kexc_external.h"
#include "sre_task.h"
#include "sre_sys.h"

/* 异常类型定义 */
/* 内核进程下异常 */
#define EXC_IN_HWI      0
#define EXC_IN_TICK     1
#define EXC_IN_SWI      2
#define EXC_IN_TASK     3
#define EXC_IN_SYS_BOOT 4
#define EXC_IN_SYS      5

#define EXC_SP_INV 0xABCD4321
#define EXC_STACK_SIZE_GET(entry) ((0x00fff000U & (entry)) >> 9)
#define EXC_SP_OFFSET 48

#define OS_EXCCAUSE_NOCHANGE 0xFFFFFFFF

/*
 * 模块间宏定义
 */
#if defined(OS_OPTION_CORE_RESET)
#define OS_HW_THREAD_RESET_BIT_MAP (&g_hwThrdResetBitMap)
#endif

/*
 * 模块间数据结构定义
 */
/*
 * 模块间typedef声明
 */
typedef U32 (*StackMonHook)(U32 beginAddr, U32 endAddr);
typedef void (*ExcOsProcFunc)(U32 arg, struct ExcInfo *excInfo);
typedef void (*ExcTaskInfoFunc)(TskHandle *threadID, struct TskInfo *taskInfo);
typedef void (*ExcMchwtmrSaveFunc)(struct ExcInfo *excInfo);
typedef U32 (*ExcReclaimProcFunc)(void);

#if defined(OS_OPTION_INST_RESET)
typedef void (*SpinLockReclaimProcFunc)(struct ExcRegInfo *excRegs);
#endif

typedef void (*McmemReclaimProcFunc)(void);
typedef void (*MsgmReclaimProcFunc)(void);

/*
 * 模块间全局变量声明
 */
extern U32 g_msgqBusErrorCoreId;

// 异常时获取当前任务的信息
extern ExcTaskInfoFunc g_excTaskInfoGet;
// 全局硬件信号保存钩子
extern ExcMchwtmrSaveFunc g_excMcHwTmrInfoSave;

extern U32 g_hwSemErrLGFlag;
extern ExcOsProcFunc g_hwSemErrHandle;

extern McmemReclaimProcFunc g_mcMemReclaimHook;
extern MsgmReclaimProcFunc g_msgmReclaimHook;

#if (defined OS_OPTION_CORE_RESET)
extern struct SysSreCoreMask g_hwThrdResetBitMap;
extern U32 OsHwThreadResetBitMapGet(U32 instID);
#endif
extern uintptr_t g_doubleExcStackStart;
extern U32 g_doubleExcStackSize;

/*
 * 模块间函数声明
 */
#if defined(OS_OPTION_INST_RESET)
extern void OsExcReclaimHookAdd(ExcReclaimProcFunc reclaimHook);
extern void OsExcReclaimHookRegister(SpinLockReclaimProcFunc reclaimHook);
#endif

#if ((defined OS_OPTION_CORE_RESET) || (defined OS_OPTION_LP_PRELINK))
extern void OsExcSingleCoreReclaimHookAdd(ExcReclaimProcFunc reclaimHook);
#endif

extern void OsExcForTraceEnd(void);
extern void OsExcCBAddrReg(void);
extern void OsExcDispatch(U32 arg);
#if (defined OS_OPTION_LP_PRELINK)
extern void OsExcSafeFrozen(void);
#endif
extern U32 OsExcDoubleStackReg(uintptr_t doubleExcStackStart, U32 doubleExcStackSize);

#endif /* SRE_EXC_EXTERNAL_H */
