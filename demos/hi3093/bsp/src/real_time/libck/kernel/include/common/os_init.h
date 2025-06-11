/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: 初始化模块的头文件
 * Author: LibCK项目组
 * Create: 2021-01-12
 * Notes: 仅允许sre_config_internal.h中引用，用户不能直接引用。
 * History: 2021-01-12 LibCK项目组 创建文件
 */
#ifndef OS_INIT_H
#define OS_INIT_H

#include "os_sys.h"
#include "os_task.h"
#include "os_sem.h"
#include "os_tick.h"
#include "os_exc.h"
#include "os_cpup.h"
#include "os_mem.h"
#include "os_msg.h"
#include "os_symbol.h"
#include "os_hook.h"
#include "os_hw.h"
#if defined(OS_OPTION_SMP)
#include "os_sched.h"
#endif
#if defined(OS_OPTION_SHELL)
#include "os_shell.h"
#endif
#if defined(OS_OPTION_SWI)
#include "os_swi.h"
#endif
#if defined(OS_OPTION_TASK_MON)
#include "os_tskmon.h"
#endif
#if (OS_INCLUDE_TRACE == YES)
#include "os_trace.h"
#endif
#if defined(OS_OPTION_NSM)
#include "os_nsm.h"
#endif
#if (OS_INCLUDE_LP == YES)
#include "os_lp.h"
#endif
#if (OS_INCLUDE_PATCH == YES)
#include "os_sp_mcpat.h"
#endif
#if ((OS_INCLUDE_MCHW_TIMER == YES) || defined(OS_OPTION_MODULARIZATION_LOAD))
#include "os_timer.h"
#endif

#if defined(OS_OPTION_FIBER)
#include "os_fiber.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if defined(OS_OPTION_MEM_BLOCK)
extern void OsBlockMemRegister(void);
#endif
extern void OsMcPoolMemRegister(void);
extern void OsDynPoolMemRegister(void);
extern U32 OsPoolMemArrayInit(void);

#if defined(OS_OPTION_MEM_DSSC)
extern void OsDsscMemRegister(void);
#endif

#if defined(OS_OPTION_MEM_SSC)
extern void OsSscMemRegister(void);
#endif

/* LibCK模块注册函数的声明 */
extern U32 OsMemRegister(U8 maxPtNum);
extern void OsFscMemRegister(void);
extern void OsMcDynFscMemRegister(void);
extern U32 OsFscMemDefPtCreate(U8 ptNo, uintptr_t ptAddr, uintptr_t ptSize);
extern U32 OsSysRegister(struct SysModInfo *modInfo);
extern U32 OsSysStackRegister(uintptr_t sysStackAddr, U32 sysStackSize);
extern U32 OsTickRegister(struct TickModInfo *modInfo);
extern void OsIdleReg(void);

#if defined(OS_OPTION_SWI)
extern U32 OsSwiRegister(struct SwiModInfo *modInfo);
#endif

extern U32 OsTskRegister(struct TskModInfo *modInfo);
extern U32 OsCpupRegister(struct CpupModInfo *modInfo);

extern U32 OsExcRegister(ExcProcFunc excepHook, uintptr_t doubleExcStackStart, U32 doubleExcStackSize);

extern U32 OsSemRegister(const struct SemModInfo *modInfo);

#if defined(OS_OPTION_NSM)
extern U32 OsNsmRegister(struct NsmModInfo *modInfo);
#endif

extern U32 OsMsgRegister(U32 maxMsgNum);
extern U32 OsMcMsgRegister(U32 maxMsgNum);

#if defined(OS_OPTION_TASK_MON)
extern void OsTskMonRegister(struct TskMonModInfo *modOverrunInfo, struct TskMonModInfo *modStrvationInfo);
#endif

#if (OS_INCLUDE_TRACE == YES)
extern U32 OsTraceRegister(struct TraceModInfo *modeInfo);
#endif

#if defined(OS_OPTION_SHELL)
extern U32 OsShellRegister(struct HshellModInfo *modeInfo);
#endif

#if defined(OS_OPTION_HUNT)
extern U32 OsHuntRegister(U32 ptNum);
#endif
#if defined(OS_OPTION_SMP)
extern U32 OsSchedDomainInit(struct OsScheduleDomainInfo *schedDomainInfo, U32 schedDomainNum);
#endif

extern U32 OsStkMonRegister(void);
extern U32 OsSymbolRegister(struct SymbolModInfo *modeInfo);
extern U32 OsHookRegister(struct OsHookModInfo *modInfo);

#if (OS_INCLUDE_PATCH == YES)
extern U32 OsPatRegister(struct McPatchModInfo *modInfo);
extern U32 OsPatInit(U32 overTime);
#endif

/* LibCK模块初始化函数的声明 */
extern U32 OsUncacheMemDefPtCreate(U8 ptNo, enum MemArith arith, uintptr_t addr, uintptr_t size);
extern U32 OsMcMemArrayInit(U8 ptNum);
#if defined(OS_OPTION_SMP)
extern U32 OsCpuHPConfigInit(void);
#endif

extern U32 OsCreateDefDynPt(void);
extern U32 OsCreateDefMcDynPt(void);
extern U32 OsModuleInit(void);
extern U32 OsHwiConfigInit(void);
extern U32 OsHwTmrConfigInit(void);
#if defined(OS_OPTION_TRING)
extern U32 OsTringConfigInit(void);
#endif
extern U32 OsTickConfigInit(void);
extern U32 OsSwiInit(void);
extern U32 OsMcSwiInit(void);
extern U32 OsStkMonInit(void);
extern U32 OsTskInit(void);
extern U32 OsTskMonInit(void);
extern U32 OsCpupInit(void);
extern void OsCpupWarnInit(void);
extern void OsCoreSleepInit(void);
extern U32 OsExcConfigInit(void);
extern U32 OsExcBoxInit(void);
extern U32 OsSemInit(void);
extern U32 OsMcSemInit(void);
extern U32 OsMcFsemInit(void);
extern U32 OsMsgInit(void);
extern U32 OsMcMsgInit(void);

#if defined(OS_OPTION_HWSEM)
extern U32 OsHwSemConfigInit(void);
#endif
#if defined(OS_OPTION_MSGM)
extern U32 OsMsgmConfigInit(void);
#endif
#if defined(OS_OPTION_MBX)
extern U32 OsMbxConfigInit(void);
#endif

#if defined(OS_OPTION_MODULARIZATION_LOAD)
extern void OsSysMcHwTmrCfgItemSet(struct McHwtmrModInfo *timerInfo);
extern void OsSysTaskCfgItemSet(struct TskModInfo *modInfo);
#if defined(OS_OPTION_TASK_MON)
extern void OsSysTskMonCfgItemSet(U32 modOverrunProcType, U32 modOverrunThresHold, U32 modStrvationThresHold);
#endif
extern void OsSysSemCfgItemSet(U16 maxNum);
extern void OsSysSwTmrCfgItemSet(U32 tmrMaxNum);
#endif

#if defined(OS_OPTION_SHELL)
extern U32 OsShellConfigInit(void);
#endif
#if defined(OS_OPTION_MEM_PERCPU)
extern U32 OsPerCpuZoneSetup(void);
#endif
extern U32 OsHookConfigInit(void);
#if (defined OS_OPTION_LP_PRELINK)
extern U32 OsSreMngConfigReg(void);
#endif
extern U32 OsSreMngConfigInit(void);

// 支持功能宏裁剪
#if defined(OS_OPTION_NSM)
extern U32 OsNsmConfigInit(void);
#endif

#if defined(OS_OPTION_FIBER)
extern U32 OsFiberRegister(const struct OsFiberModInfo *modInfo);
extern U32 OsFiberConfigInit(void);
#endif

extern U32 OsMsgMemInit(void);
extern U32 OsSymbolConfigInit(void);

extern void OsSysInitTraceErrNo(U32 errNO);
extern void OsSysInitTraceBegin(U32 initPhase, U32 modId);
extern void OsSysInitTraceEnd(U32 osPhase, U32 modId);

/* LibCK系统启动相关函数的声明 */
extern void OsHwInit(void);
extern U32 OsActivate(void);
extern U32 OsTickStart(void);
/* 实现用户自身硬件模块的初始化,该接口也复为用户打冷补丁 */
extern U32 SRE_HardDrvInit(void);
extern void SRE_HardBootInit(void);
/* 实现用户业务代码 */
extern U32 SRE_AppInit(void);
extern U32 SRE_DeviceInit(void);
extern U32 OsSyncConfigInit(void);

#if (OS_INCLUDE_ECC == YES)
extern void OsEccEnable(void);
#endif

#if defined(OS_OPTION_HUNT)
extern U32 OsHuntConfigInit(void);
#endif

extern U32 OsTraceInit(void);
extern U32 OsSwiActivate(void);
extern void OsMagicWordInit(void);
extern void OsInitTraceCBAddrReg(void);

extern U32 OsQueueRegister(U16 maxQueue);
extern U32 OsQueueConfigInit(void);

#if defined(OS_OPTION_HWSEM_PREINIT)
extern void OsHwSemPreInit(void);
#endif
#if (OS_INCLUDE_TICK_SWTMER == YES)
extern U32 OsSwTmrInit(U32 maxTimerNum);
#endif

#if (OS_INCLUDE_MCHW_TIMER == YES)
extern U32 OsMcHwTmrConfigInit(void);
extern U32 OsMcHwTmrRegister(struct McHwtmrModInfo *timerInfo);
#endif
#if (OS_INCLUDE_LP == YES)
extern U32 OsLPConfigInit(void);
extern U32 OsLPRegister(struct LpImagePubInfo *pubPatInfo);
#endif
extern U32 OsMcComConfigInit(void);
extern U32 OsMmuConfigInit(void);
extern U32 OsCacheConfigInit(void);

#if defined(OS_OPTION_VOS)
extern U32 OsOsalConfigReg(void);
#endif

#if defined(OS_OPTION_MEM_SLUB_FSC)
extern U32 OsSlubFscMemDefPtCreate(U8 ptNo, uintptr_t ptAddr, uintptr_t size);
extern void OsSlubFscMemRegister(void);
#endif
#if (defined(OS_OPTION_SREMNG) && defined(OS_OPTION_MEM_MI_FSC))
extern U32 OsMiShareMemRegister(U32 maxPtNum);
extern uintptr_t OsMiFscMemPtInit(void);
#endif

#if defined(OS_OPTION_MEM_MCDYNFSC)
extern U32 OsMcDynFscMemDefPtCreate(U8 ptNo, uintptr_t addr, uintptr_t size);
#endif

#if (defined(OS_OPTION_SREMNG) && defined(OS_OPTION_SREMNG_SECONDARY_CORE))
extern void OsGetSecondaryCorePara(U32 type, uintptr_t *addr, uintptr_t *size);
extern U32 OsGetPgId(void);
#endif

extern U32 OsSpinlockInit(uintptr_t spinlockBaseAddr, U32 spinlockSize);

#if (defined(OS_OPTION_EVENT) && (OS_INCLUDE_BOOK_EVENT == YES))
extern U32 OsBookEventRegister(U32 maxBookEventNum, U32 maxThreadsEachEvent);
extern U32 OsEventConfigInit(void);
#endif

#if defined(OS_OPTION_CRE)
extern U32 OsCreConfigInit(void);
#endif

#if (OS_INCLUDE_MMU_MAP_INFO == YES)
extern U32 OsMmuRegister(uintptr_t mmuInitAddr, U32 mmuInitNum, uintptr_t pt0Addr, uintptr_t pt0Size);
#endif

#if defined(OS_OPTION_INIT_PHASE_QUERY)
extern void OsInitPhaseSet(U32 coreID, U32 phase);
#endif

#if defined(OS_OPTION_INIT_TRACE_USR_HOOK)
extern void OsDumpInitTraceBeg(U32 phase, U32 mid);
extern void OsDumpInitTraceEnd(U32 phase, U32 mid);
#endif

#if (OS_INCLUDE_GIC_BASE_ADDR_CONFIG == YES)
extern U32 OsGicConfigRegister(uintptr_t gicdBase, uintptr_t gicrOffset, uintptr_t gicrStride);
#endif

#if (OS_INCLUDE_MINIMUM_IMG == YES)
extern U32 OsIdleActive(void);
extern U32 OsMsgmNorQueConfigInit(void);
extern U32 OsExcMinimumConfigInit(uintptr_t cdaAddr, uintptr_t size);
#else
extern U32 OsDependModeOpen(U32 mode);
extern bool OsConfigIsSet(U32 config);
#endif

extern U32 __stack;
extern U32 _stack_sentry;
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_INIT_H */
