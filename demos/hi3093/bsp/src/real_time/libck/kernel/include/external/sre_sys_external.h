/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 系统信息的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_SYS_EXTERNAL_H
#define SRE_SYS_EXTERNAL_H

#include "sre_init_trace_external.h"
#include "sre_hw.h"
#include "sre_sys.h"
#if defined(OS_OPTION_INIT)
#include "sre_exc.h"
#endif
#include "sre_task.h"
#include "sre_tskmon.h"
#include "sre_sem.h"
#include "sre_tick.h"
#include "sre_mem.h"
#include "sre_msg.h"
#include "sre_list_external.h"
#include "sre_cpu_external.h"

#if !defined(OS_OPTION_SMP)
#define OS_VAR_ARRAY_NUM 1
#else
#define OS_VAR_ARRAY_NUM OS_MAX_CORE_NUM
#endif

#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

#if defined(OS_OPTION_SMP)
#define THIS_CORE() (OsGetHwThreadId())
#else
#define THIS_CORE() 0x0U
#endif

#define OS_SYS_PID_BASE (OsGetHwThreadId() << OS_TSK_TCB_INDEX_BITS)

#define OS_INT_ACTIVE_MASK                                        \
    (OS_FLG_HWI_ACTIVE | OS_FLG_SWI_ACTIVE | OS_FLG_TICK_ACTIVE | \
     OS_FLG_SYS_ACTIVE | OS_FLG_EXC_ACTIVE | OS_FLG_FIBER_ACTIVE)

#define OS_INT_ACTIVE ((UNI_FLAG & OS_INT_ACTIVE_MASK) != 0)
#define OS_INT_INACTIVE (!(OS_INT_ACTIVE))
#define OS_HWI_ACTIVE_MASK  (OS_FLG_HWI_ACTIVE | OS_FLG_TICK_ACTIVE | OS_FLG_SYS_ACTIVE | OS_FLG_EXC_ACTIVE)
#define OS_HWI_ACTIVE ((UNI_FLAG & OS_HWI_ACTIVE_MASK) != 0)
#define OS_HWI_INACTIVE (!(OS_HWI_ACTIVE))

#if defined(OS_OPTION_CRE)
#define OS_THREAD_FLAG_MASK  (OS_FLG_BGD_ACTIVE | OS_INT_ACTIVE_MASK | OS_FLG_CRE_ACTIVE)
#else
#define OS_THREAD_FLAG_MASK  (OS_FLG_BGD_ACTIVE | OS_INT_ACTIVE_MASK)
#endif

#define OS_SYS_TASK_STATUS(flag) (((flag) & OS_THREAD_FLAG_MASK) == OS_FLG_BGD_ACTIVE)
#define OS_SYS_CRE_STATUS(flag) (((flag) & OS_THREAD_FLAG_MASK) == (OS_FLG_BGD_ACTIVE | OS_FLG_CRE_ACTIVE))
#define OS_SYS_HWI_STATUS(flag) (((flag) & OS_FLG_HWI_ACTIVE) != 0)
#define OS_SYS_SWI_STATUS(flag) (((flag) & OS_FLG_SWI_ACTIVE) != 0)
#define OS_SYS_FIBER_STATUS(flag) (((flag) & OS_FLG_FIBER_ACTIVE) != 0)

#define OS_MS2CYCLE(ms, clock) DIV64(((ms) * (U64)(clock)), SRE_SYS_MS_PER_SECOND) /* 毫秒转换成cycle */
#define OS_US2CYCLE(us, clock) DIV64(((us) * (U64)(clock)), SRE_SYS_US_PER_SECOND) /* 微秒转换成cycle */

#define OS_HARDWARE_LOCK_REG 0x0  // 外设的LOCK寄存器加锁值

#define OS_BASE_PROCESS_INDEX 0  // 内核基进程索引

/* 进程PID <PID = INDEX [0~7]| COREID [0~MAX_COERID]> */
#define OS_GETCOREID_BY_PID(processId) ((processId) & 0xffUL)
#define OS_GETIDX_BY_PID(processId) (((U32)((processId) & 0x00000f00UL)) / 256)

/* 线程TID(threadId), TID由LTID|PID|COREID|GTID组成 */
#define OS_COREID_GET(tid) GET_COREID(tid)
#define OS_PID_GET(tid) (((tid) & 0x000f0000UL) >> 16)
#define OS_GTID_GET(tid) ((tid) & ((1UL << OS_TSK_TCB_INDEX_BITS) - 1))
#define OS_LTID_PID_GET(tid) (((tid) & 0xffff0000UL) >> 16)

#define OS_PIDCID_SET(coreId, index) (((index) << 8) | (coreId))

#define OS_UTID_CREATE(threadID, pid, coreId, gTid) \
    (((U32)(threadID) << 24) | ((U32)(pid) << 16) | ((U32)(coreId) << 8) | ((U32)(gTid)))
#define OS_LTID_ADD2BASE(threadID, base) (((U32)(threadID) << 24) | (base))

// BARE OS
#define OS_LTID_GET(threadId) OS_GTID_GET(threadId)

// P_ID | CORE_ID
#define OS_PIDCID_GET(tid) ((((U32)(tid)) >> OS_TSK_TCB_INDEX_BITS) & 0xffffUL)

#define OS_SYS_GET_UINT_WORD_NUM(num) (((num) + 31) >> 5)

#define OS_CORE_STR_MAX_LEN     4
#define OS_CORE_STR_END_INDEX   (OS_CORE_STR_MAX_LEN - 1)   // 结束字符位置索引
#define OS_CORE_STR_NUM_INDEX   (OS_CORE_STR_END_INDEX - 1) // 最后一个有效字符位置索引

#if defined(OS_OPTION_MODULARIZATION_LOAD)
struct TagSysCfgItem {
    /* 共享硬件定时器时钟频率 */
    U32 mcHwtmrClock;
    /* 任务模块的配置项 */
    struct TskModInfo tskModCfgItem;
    /* tskmon模块任务撑死配置项 */
    struct TskMonModInfo tskOverRunInfo;
    /* tskmon模块任务饿死配置项 */
    struct TskMonModInfo tskStrvationInfo;
    struct SemModInfo semModCfgItem;
    /* 软件定时器最大数目 */
    U32 tmrMaxNum;
};
#endif

#if defined(OS_OPTION_MMU)
/* OS各数据段PA转化为VA的偏移 */
struct OsSecPA2VAOffset {
    /* .os.PL2.cwna.data段虚实映射关系 */
    uintptr_t osDataSec;
    /* .os.PL2.cwna.bss段虚实映射关系 */
    uintptr_t osBssSec;
    /* .os.init.data段虚实映射关系 */
    uintptr_t osInitData;
    /* .os.init.bss段虚实映射关系 */
    uintptr_t osInitBss;
    /* 系统栈虚实映射关系 */
    uintptr_t sysStack;
    /* 0号分区虚实映射关系 */
    U32 memPT0;
};
#endif

struct MagicWordInfo {
    /* 魔术字管理控制块供CDA解析信息的魔术字 */
    U32 magicWord;
    /* 芯片类型 */
    U32 platForm;
    /* CPU型号 */
    U32 cpuType;
    /* 初始化trace控制块地址 */
    char *initTraceCBAddr;
    /* CDA控制块地址 */
    uintptr_t cdaCBAddr;
    /* 异常控制块地址，对Cortex-Ax无效 */
    uintptr_t excCBAddr;
    /* 启动参数控制块地址，针对SD6182 */
    uintptr_t startPrmCfgCBAddr;
#if defined(OS_OPTION_MMU)
    struct OsSecPA2VAOffset secOffset;
#endif
};

struct CoreNumStr {
    char coreNo[OS_CORE_STR_MAX_LEN];
};

/* 入参void与返回值Void的函数钩子类型 */
typedef void (*TickDispFunc)(void);
/* 有TICK情况下CPU占用率触发函数类型定义。 */
typedef void (*TickEntryFunc)(void);
typedef void (*TaskScanFunc)(void);
typedef void (*TimeSliceScanFunc)(void);
typedef void (*TraceEventRecord)(U32 evtId, U32 arg1, U32 arg2);
typedef void (*OsTringMsgTrptHandlerFunc)(uintptr_t qData);

/*
 * 模块间全局变量声明
 */
extern U32 g_threadNum;
extern U32 g_cpupCompensationFlag;
#if defined(OS_OPTION_SREMNG)
extern uintptr_t *g_nsmMagicWordAddr;
#endif
#if defined(OS_OPTION_INIT_PHASE_QUERY)
#if defined(OS_OPTION_SREMNG)
/* 全实例共享的变量 */
extern U32 *g_instShTrace;
#else
extern U32 g_instShTrace[OS_MAX_CORE_NUM];
#endif
#endif

#if defined(OS_OPTION_SMP)
#define TICK_NO_RESPOND_CNT (GET_RUNQ(THIS_CORE())->tickNoRespondCnt)
#else
extern U32 g_tickNoRespondCnt;
#define TICK_NO_RESPOND_CNT g_tickNoRespondCnt
#endif

typedef void *(*MemAllocHook)(U32 mid, U32 ptId, uintptr_t size, enum MemAlign align);
extern MemAllocHook g_osMemAllocAndClr;
OS_SEC_ALW_INLINE INLINE void OsHookSetMemAllocAndClrHook(MemAllocHook hook)
{
    g_osMemAllocAndClr = hook;
}

extern U8 g_maxNumOfCores;
extern TraceEventRecord g_traceExc;

extern struct MagicWordInfo g_magicWordInfo;

extern SysRebootFunc g_sysRebootHook;
extern SysRebootFunc g_sysRemoveCpuMask;

extern U32 g_systemClock;
extern U8 g_numOfCores;
extern U8 g_primaryCoreId;

extern struct TickModInfo g_tickModInfo;
extern U64 g_uniTicks;
/* Tick计数补偿值 */
extern S64 g_ticksCompen;
extern TickEntryFunc g_tickTaskEntry;
extern TaskScanFunc g_taskScanHook;
extern TimeSliceScanFunc g_timeSliceScanHook;
extern TickDispFunc g_tickDispatcher;
extern TraceEventRecord g_traceErrRecord;
extern TraceEventRecord g_traceSemRecord;
extern U64 g_sleepEnd;
extern U64 g_sleepTime;
extern U64 g_sleepBegin;

#if defined(OS_OPTION_MODULARIZATION_LOAD)
extern struct TagSysCfgItem g_sysCfgItem;
extern U32 OsSysTaskNumGet(void);
#endif

extern bool g_loadSimFlag;
extern OsVoidFunc g_idleEntry;

#if !defined(OS_OPTION_SMP)
extern U32 g_uniFlag;
#define UNI_FLAG g_uniFlag
#else
#define UNI_FLAG (GET_RUNQ(THIS_CORE())->uniFlag)
extern CoreWakeUpHook g_coreWakeupHook;
OS_SEC_ALW_INLINE INLINE void OsCoreWakeupIpc(U32 coreId)
{
    CoreWakeUpHook wakeupHook = g_coreWakeupHook;

    if (wakeupHook != NULL) {
        wakeupHook(coreId);
    }
    return;
}
#endif

OS_SEC_ALW_INLINE INLINE U64 OsSysUs2Cycle(U32 us)
{
    return OS_US2CYCLE(us, g_systemClock);
}

/*
 * 模块间函数声明
 */
extern U8 OsGetCpuType(void);
extern U8 OsGetBssInitFlag(void);
extern void OsMagicWordInit(void);
extern U64 OsCurCycleGet64(void);
extern U32 OsSetSysTimeHook(SysTimeFunc hook);
extern void OsGetCoreStr(struct CoreNumStr *str);
extern void SRE_TickCountSet(U64 ticks);
extern void OsStartCheckUserParam(void);
#if defined(OS_OPTION_INIT)
extern U32 OsSysStackRegister(uintptr_t sysStackAddr, U32 sysStackSize);
extern void OsIdleReg(void);
extern U32 OsExcRegister(ExcProcFunc excepHook, uintptr_t doubleExcStackStart, U32 doubleExcStackSize);
#endif
/* 地址判断宏 */
#define OS_ADDR_START_END_IN_RANGE(startAddr, size, compStartAddr, compSize) \
    (!(((startAddr) >= ((compStartAddr) + (compSize))) || (((startAddr) + (size)) <= (compStartAddr))))

/*
 * 模块间内联函数定义
 */
#define OS_TASK_SCAN()                \
    do {                              \
        if (g_taskScanHook != NULL) { \
            g_taskScanHook();         \
        }                             \
    } while (0)

#endif /* SRE_SYS_EXTERNAL_H */
