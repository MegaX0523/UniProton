/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: LibCK的初始化C文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 *          2019-09-30 LibCK项目组  sre_config.c重构
 */

#include "sre_config_internal.h"
#include "sre_log_external.h"

U8 __attribute__((aligned(OS_CACHE_LINE_SIZE))) g_memRegion00[OS_MEM_FSC_PT_SIZE];

OS_SEC_ALW_INLINE INLINE void OsConfigAddrSizeGet(U32 memType, uintptr_t addr, uintptr_t size,
                                                  uintptr_t *destAddr, uintptr_t *destSize)
{
#if (defined(OS_OPTION_SREMNG) && defined(OS_OPTION_SREMNG_SECONDARY_CORE))
    (void)addr;
    (void)size;
    OsGetSecondaryCorePara(memType, destAddr, destSize);
#else
    (void)memType;
    *destAddr = addr;
    *destSize = size;
#endif
}

#if (OS_INCLUDE_MINIMUM_IMG == NO)
/* 依赖关系检查 */
INIT_SEC_L4_TEXT U32 OsDependModeOpen(U32 mode)
{
    if (mode == NO) {
        return OS_ERRNO_SYS_MODE_CLOSE;
    }
    return OS_OK;
}

INIT_SEC_L4_TEXT bool OsConfigIsSet(U32 config)
{
    return (config == YES);
}

static INIT_SEC_L4_TEXT U32 OsMemConfigReg(void)
{
    U32 ret;

    /* 注册FSC算法信息 */
    /* 表示系统在进行模块注册阶段，匹配MOUDLE_ID之后，标记进入FSC内存模块的注册 */
    OS_INIT_TRACE_BEG(OS_REGISTER_PHASE, OS_MID_FSCMEM);
    ret = OsMemRegister(OS_MEM_MAX_PT_NUM);
    if (ret != OS_OK) {
        return ret;
    }

    OsFscMemRegister();

#if (OS_MEM_ALGORITHM_MCDYNFSC == YES)
    /* 表示系统在进行模块注册阶段，匹配MOUDLE_ID之后，标记进入共享动态FSC内存算法的注册 */
    OS_INIT_TRACE_BEG(OS_REGISTER_PHASE, OS_MID_DYNFSCMEM);
    OsMcDynFscMemRegister();
    /* 由于OS_MID_DYNFSCMEM模块注册完成后已经完成了初始化阶段工作，故此时记录系统OS_MID_DYNFSCMEM模块完成初始化 */
    OS_INIT_TRACE_END(OS_INITIALIZE_PHASE, OS_MID_DYNFSCMEM);
#endif

#if (OS_MEM_ALGORITHM_SLUB_FSC == YES)
    /* 表示系统在进行模块注册阶段，匹配MOUDLE_ID之后，标记进入slub+fsc内存算法的注册 */
    OS_INIT_TRACE_BEG(OS_REGISTER_PHASE, OS_MID_SLUB_FSC);
    OsSlubFscMemRegister();
#endif

#if (OS_MEM_ALGORITHM_BLOCK == YES)
    /* 表示系统在进行模块注册阶段，匹配MOUDLE_ID之后，标记进入BLOCK内存算法的注册 */
    OS_INIT_TRACE_BEG(OS_REGISTER_PHASE, OS_MID_BLOCKMEM);
    OsBlockMemRegister();
#endif

#if (OS_MEM_ALGORITHM_SSC == YES)
    /* 表示系统在进行模块注册阶段，匹配MOUDLE_ID之后，标记进入SSC内存算法的注册 */
    OS_INIT_TRACE_BEG(OS_REGISTER_PHASE, OS_MID_SSC);
    OsSscMemRegister();
#endif
#if (OS_MEM_ALGORITHM_POOL == YES)
    OsDynPoolMemRegister();
#endif

#if (OS_MEM_ALGORITHM_MCPOOL == YES)
    OsMcPoolMemRegister();
#endif

#if (OS_MEM_ALGORITHM_DSSC == YES)
    OsDsscMemRegister();
#endif

#if (defined(OS_OPTION_SREMNG) && !defined(OS_OPTION_SREMNG_SECONDARY_CORE) && defined(OS_OPTION_MEM_MI_FSC))
    OsMiShareMemRegister(OS_MI_SHARE_MEM_MAX_PT_NUM);
#endif
    return ret;
}

static INIT_SEC_L4_TEXT U32 OsMemDefPtInit(void)
{
    U32 ret;
    uintptr_t pt0Addr;
    uintptr_t pt0Size;

    OsConfigAddrSizeGet(OS_FSC_PT_MEM, (uintptr_t)OS_MEM_FSC_PT_ADDR, (uintptr_t)OS_MEM_FSC_PT_SIZE,
                        &pt0Addr, &pt0Size);
    /* 系统默认FSC内存分区初始化 */
#if (OS_MEM_ALGORITHM_SLUB_FSC == YES)
    ret = OsSlubFscMemDefPtCreate(OS_MEM_DEFAULT_FSC_PT, pt0Addr, pt0Size);
#else
    ret = OsFscMemDefPtCreate(OS_MEM_DEFAULT_FSC_PT, pt0Addr, pt0Size);
#endif
    if (ret != OS_OK) {
        return ret;
    }

    return ret;
}

static INIT_SEC_L4_TEXT U32 OsMemMcPtInit(void)
{
#if ((OS_INCLUDE_MCFSEM | OS_INCLUDE_MCMSG | OS_INCLUDE_MCSWI | OS_INCLUDE_SYNC | OS_MEM_ALGORITHM_MCDYNFSC) == YES)
    U32 ret;
    uintptr_t pt1Addr;
    uintptr_t pt1Size;

    OsConfigAddrSizeGet(OS_MC_FSC_PT_MEM, (uintptr_t)OS_MEM_MCDYN_FSC_PT_ADDR, (uintptr_t)OS_MEM_MCDYN_FSC_PT_SIZE,
                        &pt1Addr, &pt1Size);
    /* 创建系统缺省共享动态FSC分区 */
    OS_INIT_TRACE_BEG(OS_INITIALIZE_PHASE, OS_MID_DYNFSCMEM);
    ret = OsMcDynFscMemDefPtCreate(OS_MEM_DEFAULT_MCSTA_DYN_PT, pt1Addr, pt1Size);
    if (ret != OS_OK) {
        return ret;
    }

    /* 分配g_memPtCheck空间 */
    ret = OsMcMemArrayInit(OS_MEM_DEFAULT_MCSTA_DYN_PT);
    if (ret != OS_OK) {
        return ret;
    }
    /* 记录系统动态FSC内存模块完成初始化创建 */
    OS_INIT_TRACE_END(OS_INITIALIZE_PHASE, OS_MID_DYNFSCMEM);
#endif

    return OS_OK;
}

static INIT_SEC_L4_TEXT U32 OsMemConfigInit(void)
{
    U32 ret;

    /* 表示系统在进行模块初始化阶段，匹配MOUDLE_ID之后，标记进入FSC MEM模块的初始化 */
    OS_INIT_TRACE_BEG(OS_INITIALIZE_PHASE, OS_MID_FSCMEM);

    /* 系统默认FSC内存分区初始化 */
    ret = OsMemDefPtInit();
    if (ret != OS_OK) {
        return ret;
    }

#if (OS_MEM_UNCACHE_STATIC == YES)
    /* uncache内存分区创建，使用1号分区 */
#if ((OS_MEM_ALGORITHM_SLUB_FSC == YES) && defined(OS_OPTION_MEM_SLUB_FSC))
    ret = OsUncacheMemDefPtCreate(OS_MEM_DEFAULT_UNCACHE_PT, MEM_ARITH_SLUB_FSC, (uintptr_t)OS_MEM_UNCACHE_PT_ADDR,
                                  OS_MEM_UNCACHE_PT_SIZE);
#else
    ret = OsUncacheMemDefPtCreate(OS_MEM_DEFAULT_UNCACHE_PT, MEM_ARITH_FSC, (uintptr_t)OS_MEM_UNCACHE_PT_ADDR,
                                  OS_MEM_UNCACHE_PT_SIZE);
#endif
    if (ret != OS_OK) {
        return ret;
    }
#endif
    /* 记录系统FSC内存模块完成初始化 */
    OS_INIT_TRACE_END(OS_INITIALIZE_PHASE, OS_MID_FSCMEM);

    /* SMP系统不需要MC消息通信模块，且1号分区划分为uncache内存分区 */
    ret = OsMemMcPtInit();
    if (ret != OS_OK) {
        return ret;
    }

#if defined(OS_OPTION_MEM_PERCPU)
    ret = OsPerCpuZoneSetup();
    if (ret != OS_OK) {
        return ret;
    }
#endif

#if (OS_MEM_ALGORITHM_MCPOOL == YES || OS_MEM_ALGORITHM_POOL == YES)
    /* 分配m_sliceTypeToSize, m_sliceSizeToType空间 */
    ret = OsPoolMemArrayInit();
    if (ret != OS_OK) {
        return ret;
    }
#endif

#if (defined(OS_OPTION_SREMNG) && defined(OS_OPTION_SREMNG_SECONDARY_CORE) && defined(OS_OPTION_MEM_MI_FSC))
    ret = OsMiFscMemPtInit();
    if (ret != OS_OK) {
        return ret;
    }
#endif
    return OS_OK;
}

#if (OS_INCLUDE_TSKMON == YES)
static INIT_SEC_L4_TEXT U32 OsTskMonConfigReg(void)
{
    if (OsConfigIsSet(OS_CONFIG_TSKMON)) {
        /* 依赖关系检查 */
        if (OsDependModeOpen(OS_INCLUDE_TICK) != OS_OK) {
            return OS_ERRNO_SYS_TICK_CLOSE;
        }

        if (OsDependModeOpen(OS_INCLUDE_TASK) != OS_OK) {
            return OS_ERRNO_SYS_TASK_CLOSE;
        }

        if ((OS_TSKMON_STARVATION == YES) || (OS_TSKMON_OVERRUN == YES)) {
            struct TskMonModInfo tskMonOverrun;
            struct TskMonModInfo tskMonStarvation;

            tskMonOverrun.procType = OS_TSKMON_DOMODE;
            tskMonOverrun.thresHold = OS_TSKMON_OVERRUN_THRESHOLD;
            tskMonStarvation.thresHold = OS_TSKMON_STARVATION_THRESHOLD;
            OsTskMonRegister(&tskMonOverrun, &tskMonStarvation);
        }
    }

    return OS_OK;
}
#endif

#if (OS_INCLUDE_SEM == YES)
static INIT_SEC_L4_TEXT U32 OsSemConfigReg(void)
{
    U32 ret = OS_OK;
    struct SemModInfo semModInfo;

    semModInfo.maxNum = OS_SEM_MAX_SUPPORT_NUM;
    if (OsConfigIsSet(OS_CONFIG_SEM)) {
        ret = OsSemRegister(&semModInfo);
    }

    return ret;
}
#endif

#if defined(OS_OPTION_HUNT)
static INIT_SEC_L4_TEXT U32 OsHuntConfigReg(void)
{
    U32 ptNum;

#if ((OS_INCLUDE_MCFSEM | OS_INCLUDE_MCMSG | OS_INCLUDE_MCSWI | OS_INCLUDE_SYNC | OS_MEM_ALGORITHM_MCDYNFSC) == YES)
    ptNum = OS_MEM_DEFAULT_MCSTA_DYN_PT;
#else
    ptNum = OS_MEM_DEFAULT_FSC_PT;
#endif

    return OsHuntRegister(ptNum);
}
#endif

#if (OS_INCLUDE_LP == YES)
static INIT_SEC_L4_TEXT U32 OsLPConfigReg(void)
{
    U32 ret;
    struct LpImagePubInfo pubInfo;

    pubInfo.groupNum = OS_LP_MAX_GROUP_NUM;
    pubInfo.lpPtNo = OS_LP_SHARE_NC_PT_NO;

    OsConfigAddrSizeGet(OS_LP_SHARE_PT_MEM, (uintptr_t)OS_LP_SHARE_NC_PT_ADDR, (uintptr_t)OS_LP_SHARE_NC_PT_SIZE,
                        &pubInfo.lpPtAddr, &pubInfo.lpPtSize);
    pubInfo.chipType = OS_CPU_TYPE;
    pubInfo.maxLPUnitNum = OS_LP_MAX_UNIT_NUM;

    ret = OsLPRegister(&pubInfo);
    if (ret != OS_OK) {
        return ret;
    }

    return OS_OK;
}
#endif

#if (OS_INCLUDE_PATCH == YES)
static INIT_SEC_L4_TEXT U32 OsPatConfigReg(void)
{
    struct McPatchModInfo modInfo;

    modInfo.areaStartAddr = PATCH_AREA_START;
    modInfo.dateStartAddr = PATCH_DATA_AREA_START;
    modInfo.shareDataStartAddr = PATCH_SHARE_DATA_AREA_START;
    modInfo.shareDataSegLength = PATCH_SHARE_DATA_LENGTH;
    modInfo.codeSegLength = PATCH_CODE_LENGTH;
    modInfo.dataSegLength = PATCH_DATA_LENGTH;
    modInfo.backUpAreaStartAddr = (uintptr_t)PATCH_DATA_BACKUPAREA_START;
    modInfo.backUpAreaLength = PATCH_DATA_BACKUPAREA_LENGTH;
    /* 目前只支持一个映像文件补丁 */
    modInfo.progImageNum = 0;
    modInfo.hpMaxFuncNum = PATCH_FUNC_NUM_IN_ONE_UNIT;
    modInfo.maxHpNum = PATCH_MAX_UNIT_NUM;
    modInfo.savePatchState = PATCH_FUNC_WRITEPATCHSTATE;
    modInfo.compInstallInfo = PATCH_FUNC_COMPINSTALLINFO;
    modInfo.dssNum = PATCH_DSS_NUM;

    modInfo.malloc = (McPatchMallocFunc)PATCH_FUNC_MALLOC;
    modInfo.free = (McPatchFreeFunc)PATCH_FUNC_FREE;
    modInfo.dcacheInv = (McPatchCacheFunc)PATCH_FUNC_DCACHEINV;
    modInfo.pcacheInv = (McPatchCacheFunc)PATCH_FUNC_PCACHEINV;
    modInfo.dcacheWbInv = (McPatchCacheFunc)PATCH_FUNC_DCACHEWBINV;
    modInfo.getCoreId = (McPatchUlFunc)PATCH_FUNC_GETCOREID;
    modInfo.getCpuTick = (McPatchCpuTickFunc)PATCH_FUNC_GETCPUTICK;
    modInfo.getTick2Ms = (McPatchTick2MsFunc)PATCH_FUNC_TICK2MS;
    modInfo.spinLockGet = (McPatchSpinlock)PATCH_FUNC_GETSPINLOCK;
    modInfo.spinLockRelease = (McPatchSpinlock)PATCH_FUNC_RELEASE_SPINLOCK;
    modInfo.intDis = (McPatchIntDisFunc)PATCH_FUNC_DISINT;
    modInfo.intRes = (McPatchIntResFunc)PATCH_FUNC_RESINT;
    modInfo.hwwdgSer = (McPatchHwdogFunc)PATCH_FUNC_WDGSERVICE;
    modInfo.getCpuMask = (McPatchUlFunc)PATCH_FUNC_GETCOREIDMASKFROMCPU;

    if (strncpy_s(modInfo.progVer, sizeof(modInfo.progVer), PROGRAM_VERSION, sizeof(modInfo.progVer) - 1) !=
        EOK) {
        return OS_ERROR_PAT_CONFIG_FAIL;
    }
    modInfo.progVer[sizeof(PROGRAM_VERSION) - 1] = '\0';

    if (strncpy_s(modInfo.bspVersion, sizeof(modInfo.bspVersion), PATCH_FLAG, strlen(PATCH_FLAG)) != EOK) {
        return OS_ERROR_PAT_CONFIG_FAIL;
    }
    modInfo.bspVersion[strlen(PATCH_FLAG)] = '\0';

    return OsPatRegister(&modInfo);
}
#endif

#if (OS_INCLUDE_TASK == YES) || defined(OS_OPTION_MODULARIZATION_LOAD)
static INIT_SEC_L4_TEXT void OsTaskInfoSet(struct TskModInfo *taskModInfo)
{
    taskModInfo->maxNum = OS_TSK_MAX_SUPPORT_NUM;
    taskModInfo->defaultSize = OS_TSK_DEFAULT_STACK_SIZE;
    taskModInfo->idleStackSize = OS_TSK_IDLE_STACK_SIZE;
    taskModInfo->magicWord = WORD_PACK((U32)OS_TSK_STACK_MAGIC_WORD);
}
#endif

#if defined(OS_OPTION_NSM)
static INIT_SEC_L4_TEXT U32 OsNsmConfigReg(void)
{
    U32 ret;
    struct NsmModInfo nsmModInfo;

    nsmModInfo.va = (uintptr_t)OS_NSM_SPACE_ADDR;
    nsmModInfo.pa = (uintptr_t)OS_NSM_SPACE_PADDR;
    nsmModInfo.size = OS_NSM_SPACE_SIZE;
    ret = OsNsmRegister(&nsmModInfo);
    if (ret != OS_OK) {
        return ret;
    }
    return ret;
}
#endif

#if (OS_INCLUDE_MCHW_TIMER == YES)
static INIT_SEC_L4_TEXT U32 OsMcHwTmrConfigReg(void)
{
    struct McHwtmrModInfo timerInfo;
    timerInfo.mcHwtmrClock = OS_MCHW_TIMER_CLOCK;

    return OsMcHwTmrRegister(&timerInfo);
}
#endif

#if (OS_INCLUDE_TICK == YES)
static INIT_SEC_L4_TEXT U32 OsTickConfigReg(void)
{
    struct TickModInfo tickModInfo;

    tickModInfo.tickPerSecond = OS_TICK_PER_SECOND;
    tickModInfo.tickPriority = OS_TICK_PRIORITY;

    return OsTickRegister(&tickModInfo);
}
#endif

#if defined(OS_OPTION_MMU)
static INIT_SEC_L4_TEXT U32 OsMmuReg(void)
{
#if (OS_INCLUDE_MMU_MAP_INFO == YES)
    uintptr_t pt0Addr;
    uintptr_t pt0Size;
    OsConfigAddrSizeGet(OS_FSC_PT_MEM, (uintptr_t)OS_MEM_FSC_PT_ADDR, (uintptr_t)OS_MEM_FSC_PT_SIZE,
                        &pt0Addr, &pt0Size);
    return OsMmuRegister((uintptr_t)OS_MMU_MAP_INFO_ADDR, OS_MMU_MAP_INFO_NUM, pt0Addr, pt0Size);
#else
    return OS_OK;
#endif
}
#endif

#if ((OS_INCLUDE_SWI == YES) || (OS_INCLUDE_MCSWI == YES))
static INIT_SEC_L4_TEXT U32 OsSwiConfigReg(void)
{
#if (OS_INCLUDE_SWI == YES)
    struct SwiModInfo swiModInfo;

    swiModInfo.maxNum = (U16)OS_SWI_MAX_SUPPORT_NUM;
    return OsSwiRegister(&swiModInfo);
#else
    return OS_OK;
#endif
}
#endif

#if ((OS_INCLUDE_SWI == YES) || (OS_INCLUDE_MCSWI == YES))
static INIT_SEC_L4_TEXT U32 OsSwiConfigInit(void)
{
#if (OS_INCLUDE_SWI == YES)
    U32 ret;
    ret = OsSwiInit();
    if (ret != OS_OK) {
        return ret;
    }
#endif
#if (OS_INCLUDE_MCSWI == YES)
    return OsMcSwiInit();
#endif
    return OS_OK;
}
#endif

#if (OS_INCLUDE_TASK == YES)
static INIT_SEC_L4_TEXT U32 OsTskConfigReg(void)
{
    struct TskModInfo taskModInfo;
    OsTaskInfoSet(&taskModInfo);
    return OsTskRegister(&taskModInfo);
}
#endif

#if (OS_INCLUDE_FIBER == YES)
INIT_SEC_L4_TEXT U32 OsFiberConfigReg(void)
{
    struct OsFiberModInfo info = {OS_FIBER_MAX_SUPPORT_NUM};
    return OsFiberRegister(&info);
}
#endif

#if (OS_INCLUDE_MSG == YES)
static INIT_SEC_L4_TEXT U32 OsMsgConfigReg(void)
{
    U32 ret;
    ret = OsMsgRegister(OS_MSG_MAX_SUPPORT_NUM);
    if (ret != OS_OK) {
        return ret;
    }
#if ((OS_INCLUDE_MCMSG == YES) && defined(OS_OPTION_MSG_MULTICAST))
    ret = OsMcMsgRegister(OS_MSG_MAX_MULTICORE_NUM);
#endif
    return ret;
}
#endif

#if (OS_INCLUDE_MSG == YES)
static INIT_SEC_L4_TEXT U32 OsMsgConfigInit(void)
{
    U32 ret;
    ret = OsMsgInit();
    if (ret != OS_OK) {
        return ret;
    }
#if defined(OS_OPTION_CORE_RESET)
    OS_INIT_TRACE_BEG(OS_INITIALIZE_PHASE, OS_MID_MSGMEM);
    ret = OsMsgMemInit();
    if (ret != OS_OK) {
        return ret;
    }
    OS_INIT_TRACE_END(OS_INITIALIZE_PHASE, OS_MID_MSGMEM);
#endif
#if ((OS_INCLUDE_MCMSG == YES) && defined(OS_OPTION_MSG_MULTICAST))
    OS_INIT_TRACE_BEG(OS_INITIALIZE_PHASE, OS_MID_MCMSG);
    ret = OsMcMsgInit();
    if (ret != OS_OK) {
        return ret;
    }
    OS_INIT_TRACE_END(OS_INITIALIZE_PHASE, OS_MID_MCMSG);
#endif
    return ret;
}
#endif

#if (OS_INCLUDE_BOOK_EVENT == YES)
static INIT_SEC_L4_TEXT U32 OsEventConfigReg(void)
{
    return OsBookEventRegister(OS_EVENT_MAX_BOOK_EVENT_NUM, OS_EVENT_MAX_THREADS_EACH_BOOK_EVENT);
}
#endif

#if (OS_INCLUDE_QUEUE == YES)
static INIT_SEC_L4_TEXT U32 OsQueueConfigReg(void)
{
    return OsQueueRegister(OS_QUEUE_MAX_SUPPORT_NUM);
}
#endif

#if (OS_INCLUDE_CPUP == YES)
static INIT_SEC_L4_TEXT U32 OsCpupConfigReg(void)
{
    struct CpupModInfo cpupModInfo;

    cpupModInfo.cpupWarnFlag = (bool)OS_CONFIG_CPUP_WARN;
    cpupModInfo.sampleNum = OS_CPUP_SAMPLE_RECORD_NUM;
    cpupModInfo.sampleTime = OS_CPUP_SAMPLE_INTERVAL;
    cpupModInfo.warn = OS_CPUP_SHORT_WARN;
    cpupModInfo.resume = OS_CPUP_SHORT_RESUME;

    return OsCpupRegister(&cpupModInfo);
}
#endif

#if (OS_INCLUDE_TRACE == YES)
static INIT_SEC_L4_TEXT U32 OsTraceConfigReg(void)
{
    U32 ret = OS_OK;
    if (OsConfigIsSet(OS_CONFIG_TRACE)) {
        struct TraceModInfo traceModInfo;

        OsConfigAddrSizeGet(OS_TRACE_AREA_MEM, (uintptr_t)OS_TRACE_AREA_ADDR, (uintptr_t)OS_TRACE_AREA_SIZE,
                            &traceModInfo.traceBufAddr, &traceModInfo.traceBufSize);
        ret = OsTraceRegister(&traceModInfo);
    }
    return ret;
}
#endif

#if (OS_INCLUDE_TSKSTACKMON == YES)
static INIT_SEC_L4_TEXT U32 OsStkMonConfigReg(void)
{
    U32 ret = OS_OK;
    if (OsConfigIsSet(OS_CONFIG_TSKSTACKMON)) {
        if (OsDependModeOpen(OS_INCLUDE_TASK) != OS_OK) {
            return OS_ERRNO_SYS_TASK_CLOSE;
        }

        ret = OsStkMonRegister();
    }
    return ret;
}
#endif

#if (OS_INCLUDE_SYMBOL == YES)
static INIT_SEC_L4_TEXT U32 OsSymbolConfigReg(void)
{
    struct SymbolModInfo symbolInfo;

    symbolInfo.symRegNum = OS_SYMBOL_TABLE_PT_NO;

    OsConfigAddrSizeGet(OS_SYM_TBL_MEM, (uintptr_t)OS_SYMBOL_TABLE_NC_PT_ADDR, (uintptr_t)OS_SYMBOL_TABLE_NC_PT_SIZE,
                        (uintptr_t *)&symbolInfo.symRegAddr, &symbolInfo.symRegSize);
#if defined(OS_OPTION_SYMBOL_COREBITMAP)
    symbolInfo.symUnitNum = OS_LP_MAX_UNIT_NUM;
#endif
    return OsSymbolRegister(&symbolInfo);
}
#endif

#if (OS_INCLUDE_SHELL == YES)
static INIT_SEC_L4_TEXT U32 OsShellConfigReg(void)
{
    struct HshellModInfo shellModInfo;

    shellModInfo.shTskPriority = OS_SHTSK_PRIORITY;
    shellModInfo.shTskStackSize = OS_SHTSK_STACKSIZE;

    return OsShellRegister(&shellModInfo);
}
#endif

#if defined(OS_OPTION_MODULARIZATION_LOAD)
static INIT_SEC_L4_TEXT U32 OsDLConfigReg(void)
{
    struct TskModInfo taskModInfo;

    OsSysTskMonCfgItemSet(OS_TSKMON_DOMODE, OS_TSKMON_OVERRUN_THRESHOLD, OS_TSKMON_STARVATION_THRESHOLD);
    OsTaskInfoSet(&taskModInfo);
    OsSysTaskCfgItemSet(&taskModInfo);
    OsSysSwTmrCfgItemSet((U32)OS_TICK_SWITIMER_MAX_NUM);
    OsSysSemCfgItemSet(OS_SEM_MAX_SUPPORT_NUM);
    return OS_OK;
}
#endif

#if (defined(OS_OPTION_STKMON) && (OS_INCLUDE_TSKSTACKMON == YES))
static INIT_SEC_L4_TEXT U32 OsStkMonConfigInit(void)
{
    U32 ret = OS_OK;
    if (OsConfigIsSet(OS_CONFIG_TSKSTACKMON)) {
        ret = OsStkMonInit();
    }
    return ret;
}
#endif

#if (OS_INCLUDE_TSKMON == YES)
static INIT_SEC_L4_TEXT U32 OsTskMonConfigInit(void)
{
    U32 ret = OS_OK;
    if (OsConfigIsSet(OS_CONFIG_TSKMON)) {
        ret = OsTskMonInit();
    }
    return ret;
}
#endif

#if (OS_INCLUDE_TICK_SWTMER == YES)
static INIT_SEC_L4_TEXT U32 OsSwTmrConfigInit(void)
{
    return OsSwTmrInit(OS_TICK_SWITIMER_MAX_NUM);
}
#endif

#if (OS_INCLUDE_CPUP == YES)
#if (OS_INCLUDE_TICK == NO)
#error "OS_INCLUDE_CPUP depend on OS_INCLUDE_TICK!"
#endif
static INIT_SEC_L4_TEXT U32 OsCpupConfigInit(void)
{
    /* CPUP初始化 */
#if !defined(OS_OPTION_CPUP_LAZY_INIT)
    U32 ret;
    ret = OsCpupInit();
    if (ret != OS_OK) {
        return ret;
    }
#endif

    if (OS_CONFIG_CPUP_WARN == YES) {
#if defined(OS_OPTION_CPUP_WARN)
        OsCpupWarnInit();
#else
        return OS_ERRNO_SYS_NO_CPUP_WARN;
#endif
    }
    return OS_OK;
}
#endif

#if (OS_INCLUDE_CORESLEEP == YES)
static INIT_SEC_L4_TEXT U32 OsCoreSleepConfigInit(void)
{
    OsCoreSleepInit();
    return OS_OK;
}
#endif

#if (OS_INCLUDE_SEM == YES)
static INIT_SEC_L4_TEXT U32 OsSemConfigInit(void)
{
    U32 ret = OS_OK;
    if (OsConfigIsSet(OS_CONFIG_SEM)) {
        ret = OsSemInit();
    }
    return ret;
}
#endif

#if defined(OS_OPTION_SMP)
static INIT_SEC_L4_TEXT U32 OsSchedDomainConfigInit(void)
{
    return OsSchedDomainInit(OS_SCHEDULE_DOMAIN_INFO, OS_SCHEDULE_DOMAIN_INFO_NUM);
}
#endif

#if (OS_INCLUDE_TASK == YES)
static INIT_SEC_L4_TEXT U32 OsTskConfigInit(void)
{
    U32 ret;
    ret = OsTskInit();
#if (OS_INCLUDE_MCFSEM == YES)
    if (OsConfigIsSet(OS_CONFIG_MCFSEM)) {
        ret |= OsMcFsemInit();
    }
#endif
    return ret;
}
#endif

#if (OS_INCLUDE_TRACE == YES)
static INIT_SEC_L4_TEXT U32 OsTraceConfigInit(void)
{
    U32 ret = OS_OK;
    if (OsConfigIsSet(OS_CONFIG_TRACE)) {
        ret = OsTraceInit();
    }
    return ret;
}
#endif

#if (OS_INCLUDE_PATCH == YES)
static INIT_SEC_L4_TEXT U32 OsPatConfigInit(void)
{
    return OsPatInit(PATCH_SYN_OVERTIME);
}
#endif

static INIT_SEC_L4_TEXT U32 OsHookConfigReg(void)
{
    struct OsHookModInfo hookModInfo;

    hookModInfo.maxNum[OS_HOOK_HWI_ENTRY] = OS_HOOK_HWI_ENTRY_NUM;
    hookModInfo.maxNum[OS_HOOK_HWI_EXIT] = OS_HOOK_HWI_EXIT_NUM;
    hookModInfo.maxNum[OS_HOOK_SWI_ENTRY] = OS_HOOK_SWI_ENTRY_NUM;
    hookModInfo.maxNum[OS_HOOK_SWI_EXIT] = OS_HOOK_SWI_EXIT_NUM;
    hookModInfo.maxNum[OS_HOOK_TSK_CREATE] = OS_HOOK_TSK_CREATE_NUM;
    hookModInfo.maxNum[OS_HOOK_TSK_DELETE] = OS_HOOK_TSK_DELETE_NUM;
    hookModInfo.maxNum[OS_HOOK_TSK_SWITCH] = OS_HOOK_TSK_SWITCH_NUM;

#if defined(OS_OPTION_SMP)
    hookModInfo.maxNum[OS_HOOK_READY_QUE_IN] = 1;
    hookModInfo.maxNum[OS_HOOK_READY_QUE_OUT] = 1;
#endif
    hookModInfo.maxNum[OS_HOOK_IDLE_PERIOD] = OS_HOOK_IDLE_NUM;
    hookModInfo.maxNum[OS_HOOK_LAST_WORDS] = OS_HOOK_LAST_WORDS_NUM;
#if defined(OS_OPTION_POWERGATE)
    hookModInfo.maxNum[OS_HOOK_DYNPWRGT_ENTRY] = OS_HOOK_DYNPWRGT_ENTRY_NUM;
    hookModInfo.maxNum[OS_HOOK_DYNPWRGT_EXIT] = OS_HOOK_DYNPWRGT_EXIT_NUM;
#endif

#if defined(OS_OPTION_FIBER)
    hookModInfo.maxNum[OS_HOOK_FIBER_ENTRY] = OS_HOOK_FIBER_ENTRY_NUM;
    hookModInfo.maxNum[OS_HOOK_FIBER_EXIT] = OS_HOOK_FIBER_EXIT_NUM;
#endif

    return OsHookRegister(&hookModInfo);
}
#endif

static INIT_SEC_L4_TEXT U32 OsSystemReg(void)
{
    U32 ret;
    struct SysModInfo sysModInfo;

    sysModInfo.systemClock = OS_SYS_CLOCK;
    sysModInfo.coreRunNum = OS_SYS_CORE_RUN_NUM;
    sysModInfo.coreMaxNum = OS_SYS_CORE_MAX_NUM;
    sysModInfo.corePrimary = OS_SYS_CORE_PRIMARY;
    sysModInfo.rebootHook = OS_SYS_REBOOT_HOOK;
    sysModInfo.sysTimeHook = (SysTimeFunc)OS_SYS_TIME_HOOK;
    sysModInfo.cpuType = OS_CPU_TYPE;
#if defined(OS_OPTION_HWI_MAX_NUM_CONFIG)
    sysModInfo.hwiMaxNum = OS_HWI_MAX_NUM_CONFIG;
#endif

    /* 如果补丁和符号表都没有使用，这两项配置项赋默认值 */
#if ((OS_INCLUDE_LP == NO) && (OS_INCLUDE_SYMBOL == NO))
    sysModInfo.imageNum = 1;
    sysModInfo.imageId = 0;
#else
    sysModInfo.imageNum = OS_SYS_MAX_IMAGE_NUM;

#if (defined(OS_OPTION_SREMNG) && defined(OS_OPTION_SREMNG_SECONDARY_CORE))
    sysModInfo.imageId = OsGetPgId();
#else
    sysModInfo.imageId = OS_SYS_IMAGE_ID;
#endif
#endif

#if (OS_SRE_MNG_LOAD_SIMULATION == YES)
    sysModInfo.loadSimFlag = 1;
#else
    sysModInfo.loadSimFlag = 0;
#endif

    /* 配置的镜像个数不能大于最大核数 */
    if (sysModInfo.imageNum > OS_MAX_CORE_NUM) {
        return OS_ERRNO_SYS_IMAGE_NUM_INVALID;
    }

    ret = OsSysRegister(&sysModInfo);
    if (ret != OS_OK) {
        return ret;
    }

    return ret;
}

static INIT_SEC_L4_TEXT U32 OsSysConfigReg(void)
{
    U32 ret;
    OsIdleReg();

    ret = OsSystemReg();
    if (ret != OS_OK) {
        return ret;
    }

    ret = OsSysStackRegister((uintptr_t)OS_SYS_STACK_AREA_START_ADDR, OS_SYS_STACK_AREA_SIZE);
    if (ret != OS_OK) {
        return ret;
    }

    /* 尽早注册用户异常接管钩子，并且xtensa平台在此配置double栈、NMI栈、Dbg栈信息 */
    return OsExcRegister(SRE_ExcProc, (uintptr_t)OS_DOUBLE_EXC_STACK_START, OS_DOUBLE_EXC_STACK_SIZE);
}

static INIT_SEC_L4_TEXT U32 OsHwiConfigReg(void)
{
#if(OS_INCLUDE_GIC_BASE_ADDR_CONFIG == YES)
    U32 ret;
    ret = OsGicConfigRegister((uintptr_t)OS_GIC_BASE_ADDR, (uintptr_t)OS_GICR_OFFSET, (uintptr_t)OS_GICR_STRIDE);
    if (ret != OS_OK) {
        return ret;
    }
#endif
    return OS_OK;
}

/* 系统初始化注册表 */
#if (OS_INCLUDE_MINIMUM_IMG == NO)
INIT_SEC_L4_SH_DATA struct OsModuleConfigInfo g_moduleConfigTab[] = {
    /* {模块号， 模块注册函数， 模块初始化函数} */
    {OS_MID_SYS, {OsSysConfigReg, NULL}},
    {OS_MID_MEM, {OsMemConfigReg, OsMemConfigInit}},
    {OS_MID_HWI, {OsHwiConfigReg, OsHwiConfigInit}},
#if defined(OS_OPTION_NSM)
    {OS_MID_NSM, {OsNsmConfigReg, OsNsmConfigInit}},
#endif
#if (OS_INCLUDE_SYNC == YES)
    {OS_MID_SYNC, {NULL, OsSyncConfigInit}},
#endif
#if !defined(OS_OPTION_LP_COLD_PAT)
    {OS_MID_HARDDRV, {NULL, SRE_HardDrvInit}},
#endif
    {OS_MID_HOOK, {OsHookConfigReg, OsHookConfigInit}},
    {OS_MID_EXC, {NULL, OsExcConfigInit}},
    {OS_MID_CACHE, {NULL, OsCacheConfigInit}},
#if !defined(OS_OPTION_LP_COLD_PAT)
#if defined(OS_OPTION_MSGM)
    {OS_MID_MSGM, {NULL, OsMsgmConfigInit}},
#endif
#if defined(OS_OPTION_HWSEM)
    {OS_MID_HWSEM, {NULL, OsHwSemConfigInit}},
#endif
#if (OS_INCLUDE_MCHW_TIMER == YES)
    {OS_MID_MCHWTMR, {OsMcHwTmrConfigReg, OsMcHwTmrConfigInit}},
#endif
#endif
#if (OS_INCLUDE_HW_TIMER == YES)
    {OS_MID_HWTMR, {NULL, OsHwTmrConfigInit}},
#endif
#if !defined(OS_OPTION_LP_COLD_PAT)
#if defined(OS_OPTION_TRING)
#if (OS_INCLUDE_TRING_TIMER == YES)
    {OS_MID_TRING, {NULL, OsTringConfigInit}},
#endif
#endif
#endif
#if defined(OS_OPTION_MBX)
    {OS_MID_MBX, {NULL, OsMbxConfigInit}},
#endif
#if defined(OS_OPTION_HOTPLUG_CPU)
    {OS_MID_CPUHP, {NULL, OsCpuHPConfigInit}},
#endif
#if ((OS_INCLUDE_SWI == YES) || (OS_INCLUDE_MCSWI == YES))
    {OS_MID_SWI, {OsSwiConfigReg, OsSwiConfigInit}},
#endif
#if defined(OS_OPTION_MODULARIZATION_LOAD)
    {OS_MID_DL, {OsDLConfigReg, NULL}},
#endif
#if defined(OS_OPTION_SMP)
    {OS_MID_SCHED, {NULL, OsSchedDomainConfigInit}},
#endif
#if (OS_INCLUDE_TASK == YES)
    {OS_MID_TSK, {OsTskConfigReg, OsTskConfigInit}},
#endif
#if defined(OS_OPTION_CRE)
    {OS_MID_CRE, {NULL, OsCreConfigInit}},
#endif
#if (OS_INCLUDE_FIBER == YES)
    {OS_MID_FIBER, {OsFiberConfigReg, OsFiberConfigInit}},
#endif
#if (OS_INCLUDE_TSKSTACKMON == YES)
    {OS_MID_STKMON, {OsStkMonConfigReg, OsStkMonConfigInit}},
#endif
#if (OS_INCLUDE_TSKMON == YES)
    {OS_MID_TSKMON, {OsTskMonConfigReg, OsTskMonConfigInit}},
#endif
#if !defined(OS_OPTION_LP_COLD_PAT)
#if defined(OS_OPTION_AMP_MULTICORE)
    {OS_MID_MCCOM, {NULL, OsMcComConfigInit}},
#endif
#endif
#if defined(OS_OPTION_HUNT)
    {OS_MID_HUNT, {OsHuntConfigReg, OsHuntConfigInit}},
#endif
#if (OS_INCLUDE_TICK == YES)
    {OS_MID_TICK, {OsTickConfigReg, OsTickConfigInit}},
#endif
#if defined(OS_OPTION_MMU)
    {OS_MID_MMU, {OsMmuReg, OsMmuConfigInit}},
#endif
#if (OS_INCLUDE_TICK_SWTMER == YES)
    {OS_MID_SWTMR, {NULL, OsSwTmrConfigInit}},
#endif
#if (OS_INCLUDE_MSG == YES)
    {OS_MID_MSG, {OsMsgConfigReg, OsMsgConfigInit}},
#endif
#if (OS_INCLUDE_BOOK_EVENT == YES)
    {OS_MID_EVENT, {OsEventConfigReg, OsEventConfigInit}},
#endif
#if (OS_INCLUDE_CPUP == YES)
    {OS_MID_CPUP, {OsCpupConfigReg, OsCpupConfigInit}},
#endif
#if (OS_INCLUDE_CORESLEEP == YES)
    {OS_MID_CORESLEEP, {NULL, OsCoreSleepConfigInit}},
#endif
#if (OS_INCLUDE_SEM == YES)
    {OS_MID_SEM, {OsSemConfigReg, OsSemConfigInit}},
#endif
#if (OS_INCLUDE_QUEUE == YES)
    {OS_MID_QUEUE, {OsQueueConfigReg, OsQueueConfigInit}},
#endif
#if (OS_INCLUDE_TRACE == YES)
    {OS_MID_TRACE, {OsTraceConfigReg, OsTraceConfigInit}},
#endif
#if (OS_INCLUDE_DEVICE == YES)
    {OS_MID_DEVICE, {NULL, SRE_DeviceInit}},
#endif
#if (OS_INCLUDE_SYMBOL == YES)
    {OS_MID_SYMBOL, {OsSymbolConfigReg, OsSymbolConfigInit}},
#endif
#if (OS_INCLUDE_SHELL == YES)
    {OS_MID_SHELL, {OsShellConfigReg, OsShellConfigInit}},
#endif
#if (defined(OS_OPTION_SREMNG) && !defined(OS_OPTION_SREMNG_SECONDARY_CORE))
#if (defined OS_OPTION_LP_PRELINK)
    {OS_MID_SREMNG, {OsSreMngConfigReg, OsSreMngConfigInit}},
#else
    {OS_MID_SREMNG, {NULL, OsSreMngConfigInit}},
#endif
#endif
#if (OS_INCLUDE_LP == YES)
    {OS_MID_LP, {OsLPConfigReg, OsLPConfigInit}},
#endif
#if defined(OS_OPTION_LP_COLD_PAT)
    /* SRE_HardDrvInit接口实现用户自身硬件模块的初始化，并支持对硬件模块打冷补丁 */
    {OS_MID_HARDDRV, {NULL, SRE_HardDrvInit}},
    /* 硬件IP类模块初始化下移到补丁初始化后，便于对硬件IP打冷补丁，该补丁不卸载 */
#if defined(OS_OPTION_MSGM)
    {OS_MID_MSGM, {NULL, OsMsgmConfigInit}},
#endif
#if defined(OS_OPTION_HWSEM)
    {OS_MID_HWSEM, {NULL, OsHwSemConfigInit}},
#endif
#if (OS_INCLUDE_MCHW_TIMER == YES)
    {OS_MID_MCHWTMR, {OsMcHwTmrConfigReg, OsMcHwTmrConfigInit}},
#endif
#if defined(OS_OPTION_TRING)
#if (OS_INCLUDE_TRING_TIMER == YES)
    {OS_MID_TRING, {NULL, OsTringConfigInit}},
#endif
#endif
    /* 该模块也要同msgm下移 */
#if defined(OS_OPTION_AMP_MULTICORE)
    {OS_MID_MCCOM, {NULL, OsMcComConfigInit}},
#endif
#endif
    {OS_MID_APP, {NULL, SRE_AppInit}},
#if (OS_INCLUDE_PATCH == YES)
    {OS_MID_PAT, {OsPatConfigReg, OsPatConfigInit}},
#endif
#if defined(OS_OPTION_VOS)
    {OS_MID_OSAL, {OsOsalConfigReg, NULL}},
#endif
    {OS_MID_BUTT, {NULL, NULL}},
};
#else
static INIT_SEC_L4_TEXT U32 OsExcMiniConfigInit(void)
{
    uintptr_t pt0Addr;
    uintptr_t pt0Size;

    OsConfigAddrSizeGet(OS_FSC_PT_MEM, (uintptr_t)OS_MEM_FSC_PT_ADDR, (uintptr_t)OS_MEM_FSC_PT_SIZE,
                        &pt0Addr, &pt0Size);
    return OsExcMinimumConfigInit(pt0Addr, pt0Size);
}

INIT_SEC_L4_SH_DATA struct OsModuleConfigInfo g_moduleConfigTab[] = {
    /* {模块号， 模块注册函数， 模块初始化函数} */
    {OS_MID_SYS, {OsSysConfigReg, NULL}},
    {OS_MID_HARDDRV, {NULL, SRE_HardDrvInit}},
    {OS_MID_EXC, {NULL, OsExcMiniConfigInit}},
    {OS_MID_HWI, {OsHwiConfigReg, OsHwiConfigInit}},
    {OS_MID_HOOK, {NULL, OsHookConfigInit}},
#if defined(OS_OPTION_MSGM)
    {OS_MID_MSGM, {NULL, OsMsgmNorQueConfigInit}},
#endif
    {OS_MID_APP, {NULL, SRE_AppInit}},
    {OS_MID_BUTT, {NULL, NULL}},
};
#endif

static INIT_SEC_L4_TEXT U32 OsMoudleDepend(void)
{
#if (!defined(OS_OPTION_SWI) && (OS_INCLUDE_TASK == NO))
    return OS_ERRNO_SYS_NO_SWI;
#else
    return OS_OK;
#endif
}

/* OS模块注册、初始化运行函数 */
static INIT_SEC_L4_TEXT U32 OsModuleConfigRun(enum OsinitPhaseId initPhaseId, U32 initPhase)
{
    U32 idx = 0;
    U32 ret = OS_OK;
    while (g_moduleConfigTab[idx].moudleId != OS_MID_BUTT) {
        OS_INIT_TRACE_BEG(initPhase, g_moduleConfigTab[idx].moudleId);
        if (g_moduleConfigTab[idx].moudleConfigFunc[initPhaseId] == NULL) {
            idx++;
            continue;
        }
        ret = g_moduleConfigTab[idx].moudleConfigFunc[initPhaseId]();
        if (ret != OS_OK) {
            break;
        }
        OS_INIT_TRACE_END(initPhase, g_moduleConfigTab[idx].moudleId);
        idx++;
    }
    return ret;
}
static INIT_SEC_L4_TEXT U32 OsRegister(void)
{
    U32 ret;
    ret = OsMoudleDepend();
    if (ret != OS_OK) {
        return ret;
    }
    return OsModuleConfigRun(OS_REGISTER_ID, OS_REGISTER_PHASE);
}

/* OsInitialize阶段对相应的运行模块进行OS_INIT_TRACE_END TRACE记录 */
static INIT_SEC_L4_TEXT U32 OsInitialize(void)
{
    return OsModuleConfigRun(OS_INIT_ID, OS_INITIALIZE_PHASE);
}

/* OsStart阶段对相应的运行模块不进行OS_INIT_TRACE_END TRACE记录 */
static INIT_SEC_L4_TEXT U32 OsStart(void)
{
    U32 ret;
#if (OS_INCLUDE_MINIMUM_IMG == NO)

#if (OS_INCLUDE_TICK == YES)
    /* 表示系统在进行启动阶段，匹配MOUDLE_ID之后，标记进入TICK模块的启动 */
    OS_INIT_TRACE_BEG(OS_START_PHASE, OS_MID_TICK);
    ret = OsTickStart();
    if (ret != OS_OK) {
        return ret;
    }
#endif

#if (OS_INCLUDE_ECC == YES)
    OsEccEnable();
#endif

#if (OS_INCLUDE_TASK == YES)
    /* 表示系统在进行启动阶段，匹配MOUDLE_ID之后，标记进入任务模块的启动 */
    OS_INIT_TRACE_BEG(OS_START_PHASE, OS_MID_TSK);
    ret = OsActivate();

#elif defined(OS_OPTION_SWI)
    /* 表示系统在进行启动阶段，匹配MOUDLE_ID之后，标记进入软中断模块的启动 */
    OS_INIT_TRACE_BEG(OS_START_PHASE, OS_MID_SWI);
    ret = OsSwiActivate();
#else
    ret = OS_OK;
#endif
#else
    ret = OsIdleActive();
#endif
    return ret;
}

#if defined(OS_OPTION_SMP)
static INIT_SEC_L4_TEXT U32 OsSmpPreInit(void)
{
    U32 ret;
    if (OsGetCoreID() != OS_SYS_CORE_PRIMARY) {
        ret = OsModuleInit();
        if (ret != OS_OK) {
            OS_INIT_TRACE_ERR_ID(ret);
            return ret;
        }

        ret = OsStart();
        if (ret != OS_OK) {
            OS_INIT_TRACE_ERR_ID(ret);
            return ret;
        }
        /* Execution should not reach this point */
        return OS_FAIL;
    }

    OsMagicWordInit();
    OsInitTraceCBAddrReg();
    OS_INIT_TRACE_BEG(OS_BOOT_PHASE, 0);

#if (OS_INCLUDE_EXCLUSIVE_REGION == YES)
    ret = OsSpinlockInit((uintptr_t)OS_EXCLUSIVE_REGION_START, (U32)OS_EXCLUSIVE_REGION_SIZE);
    if (ret != OS_OK) {
        OS_INIT_TRACE_ERR_ID(ret);
        return ret;
    }
#endif

    return OS_OK;
}
#endif

INIT_SEC_L4_TEXT S32 OsAppMain(void);
INIT_SEC_L4_TEXT S32 OsAppMain(void)
{
    U32 ret;
    OS_INIT_PHASE_SELF_SET((U32)PHASE_IN_MAIN_ENTRY);

#if defined(OS_OPTION_SMP)
    ret = OsSmpPreInit();
    if (ret != OS_OK) {
        OS_INIT_TRACE_ERR_ID(ret);
        return (S32)ret;
    }
#endif

#if (OS_INCLUDE_MINIMUM_IMG == NO)
    OsHwInit();
#endif
    /* OS模块注册 */
    ret = OsRegister();
    if (ret != OS_OK) {
        OS_INIT_TRACE_ERR_ID(ret);
        return (S32)ret;
    }

    /* OS模块初始化 */
    ret = OsInitialize();
    if (ret != OS_OK) {
        OS_INIT_TRACE_ERR_ID(ret);
        return (S32)ret;
    }
    /* OS启动调度 */
    ret = OsStart();
    if (ret != OS_OK) {
        OS_INIT_TRACE_ERR_ID(ret);
        return (S32)ret;
    }

    /* Execution should not reach this point */
    return (S32)OS_FAIL;
}
