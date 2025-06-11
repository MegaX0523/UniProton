/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: armv8相关的内部公共头文件。
 * Author: LibCK项目组
 * Create: 2020-06-16
 * History: 2020-06-16 LibCK项目组 创建文件
 */
#ifndef OS_CPU_ARMV8_EXTERNAL_H
#define OS_CPU_ARMV8_EXTERNAL_H

#include "sre_buildef.h"
#include "sre_hwi.h"
#include "sre_gic_external.h"
#include "sre_atomic.h"

/*
 * 模块间宏定义
 */
#define OS_IRQ2HWI(irqNum)           (irqNum)
#define OS_HWI2IRQ(hwiNum)           (hwiNum)
#define OS_HWI_GET_HWINUM(archNum)   (archNum)
#define OS_HWI_GET_HWI_PRIO(hwiPrio) (hwiPrio)

#if defined(OS_OPTION_HWI_DYN_DESC)
#define OS_HWI_MAX_NUM           0x10000U
#define OS_HWI_LPI_START         0x2000U
#define OS_HWI_LPI_END           OS_HWI_MAX_NUM
#define OS_HWI_NUM_CHECK(hwiNum)     \
    (((hwiNum) >= OS_HWI_LPI_END) || \
    (((hwiNum) > MAX_SPI_ID) &&      \
    ((hwiNum) < OS_HWI_LPI_START)))
#else
/* OS_HWI_MAX_NUM 大小会影响bss段大小。需要根据实际使用hwi个数配置 */
#define OS_HWI_MAX_NUM           0x182U
#define OS_HWI_NUM_CHECK(hwiNum) ((hwiNum) >= OS_HWI_MAX_NUM)
#endif

#define OS_HWI_MAX           (OS_HWI_MAX_NUM - 1)
#define OS_HWI_FORMARRAY_NUM OS_HWI_MAX_NUM
#define OS_HWI_MIN           0
#define OS_HWI_PRI_NUM       14

/* 中断优先级0~15，但非安全世界的中断优先级只能是偶数 */
#define OS_HWI_PRIO_CHECK(hwiPrio)     ((hwiPrio) >= OS_HWI_PRI_NUM || ((hwiPrio) & 1U))
#define OS_HWI_NEST_ADDR               NULL
#define OS_HWI_SET_HOOK_ATTR(hwiNum, hwiPrio, hook)
#if defined(OS_OPTION_HWI_AFFINITY)
#define OS_HWI_AFFINITY_MIN_NUM (MIN_SPI_ID)
#define OS_HWI_AFFINITY_MAX_NUM (OS_HWI_MAX_NUM)

/* 仅1-1/1-N SPI中断支持中断路由 */
#define OS_HWI_AFFINITY_CHECK(hwiNum) OsGicIsSpi(hwiNum)

#endif
#define OS_HWI_CLEAR_CHECK(hwiNum)    ((hwiNum) == GIC_INT_ID_MASK)

/*
 * SMP系统占用的硬件SGI号
 */
#define OS_SMP_SCHED_TRIGGER_OTHER_CORE_SGI OS_HWI_IPI_NO_01   // 触发它核响应一次调度的IPI中断号
#define OS_SMP_EXC_STOP_OTHER_CORE_SGI      OS_HWI_IPI_NO_02   // 一个核异常后将其它核停住的IPI中断号
#define OS_SMP_TICK_TRIGGER_OTHER_CORE_SGI  OS_HWI_IPI_NO_03   // 响应tick中断的核触发它核的模拟tickIPI中断号
#define OS_SMP_MC_CORE_IPC_SGI OS_HWI_IPI_NO_04                // SMP核间通信使用的IPI中断号

/*
 * SMP系统占用的硬件SGI的优先级
 */
#define OS_SMP_SCHED_TRIGGER_OTHER_CORE_SGI_PRI 0
#define OS_SMP_EXC_STOP_OTHER_CORE_SGI_PRI      0  // 一个核异常后将其它核停住的IPI中断号
#define OS_SMP_TICK_TRIGGER_OTHER_CORE_SGI_PRI  0
#define OS_SMP_MC_CORE_IPC_SGI_PRI              0

#define OS_HWI_INTERNAL_NUM 5

#define OS_DI_STATE_CHECK(intSave) ((intSave) & 0x80U)

#define OS_ENABLE_INER_ALL()

#define OS_TICK_COUNT_UPDATE()

OS_SEC_ALW_INLINE INLINE void OsSpinLockInitInner(volatile uintptr_t *lockVar)
{
    *lockVar = OS_SPINLOCK_UNLOCK;
}

#define OS_SPINLOCK_INIT_FOREACH(maxNum, structName, field)
#define OS_SPIN_FREE_FOREACH(maxNum, structName, field)
#define OS_SPIN_FREE(lockVar)

#if defined(OS_OPTION_SMP)
#define OS_HW_TICK_INIT() OsHwtickInitSmp()
#else
#define OS_HW_TICK_INIT() SRE_OK
#endif
#define OS_IS_TICK_PERIOD_INVALID(cyclePerTick) (FALSE)

#define OS_TSK_STACK_SIZE_ALIGN  16U
#define OS_TSK_STACK_SIZE_ALLOC_ALIGN MEM_ADDR_ALIGN_016
#define OS_TSK_STACK_ADDR_ALIGN  16U
#define OS_ALLCORES_MASK g_validAllCoresMask

#define OS_MAX_CACHE_LINE_SIZE   4 /* 单核芯片定义为4 */

/* 标准64位cpu on的funcid */
#define PSCI_CPU_ON_FUNCID        0xc4000003
#define PSCI_CPU_STATE_FUNCID     0xc4000004

/* 任务栈最小值 */
#if defined(OS_OPTION_HAVE_FPU)
#define OS_TSK_MIN_STACK_SIZE (ALIGN((0x3E0 + 0x10 + 0x4), 16))
#else
#define OS_TSK_MIN_STACK_SIZE (ALIGN((0x1D0 + 0x10 + 0x4), 16))
#endif

/* Idle任务的消息队列数 */
#define OS_IDLE_TASK_QUE_NUM 1

/* CDA相关 */
#define NUMBER_FOR_MAGIC_WORD_ADD 0

/* Unwind相关 寄存器在解析帧中的位置，前面30个通用寄存器忽略 */
#define OS_UWD_LR_OFFSET 0x1EU
#define OS_UWD_SP_OFFSET 0x24U
#define OS_UWD_PC_OFFSET 0x23U

#define OS_SPINLOCK_RWLOCK_NEGATIVE 0x80000000U

#define ELF_MACHINE OS_ELF_EM_AARCH64

#define DIV64(a, b) ((a) / (b))
#define DIV64_REMAIN(a, b) ((a) % (b))

#define OsIntUnLock() SRE_IntUnLock()
#define OsIntLock()   SRE_IntLock()
#define OsIntRestore(intSave) SRE_IntRestore(intSave)

#if defined(OS_OPTION_SMP)
#define OsTaskTrap(task) OsTskTrapSmp(task)
#endif

/* 硬件平台保存的任务上下文 */
struct TagHwContext {
#if defined(OS_OPTION_HAVE_FPU)
    __uint128_t q[OS_FPU_CONTEXT_REG_NUM];
    uintptr_t fpcr;
    uintptr_t fpsr;
#endif
    uintptr_t pc;
    uintptr_t spsr;
    uintptr_t far;
    uintptr_t esr;
    uintptr_t xzr;
    uintptr_t lr;
    uintptr_t x[30];
};

/* CallStack保存的任务上下文 */
struct TagCallStackContext {
    uintptr_t sp;
    uintptr_t pc;
    uintptr_t resv[4];
    uintptr_t lr;
    uintptr_t x[30];
};

/* Fetch Set结构体 */
struct TagLpFetchSet {
    // 第1个指令
    U8 ins1[4];
    // 第2个指令
    U8 ins2[4];
    // 第3个指令
    U8 ins3[4];
    // 第4个指令
    U8 ins4[4];
    // 每个指令的高1bit + EOV + 指令类型
    U8 hins;
};

/*
 * 模块间变量声明
 */
extern uintptr_t __os_sys_sp_end;
extern uintptr_t __os_sys_sp_start;
extern uintptr_t __bss_end__;
extern uintptr_t __bss_start__;
extern uintptr_t __mmu_page_start__;
extern uintptr_t __mmu_page_end__;

/*
 * 模块间函数声明
 */
extern uintptr_t OsGetSysStackSP(void);
extern void OsSetSysStackSP(uintptr_t stackPointer, U32 hwiNum);
extern uintptr_t OsGetSysStackStart(void);
extern uintptr_t OsGetSysStackEnd(void);
#if defined(OS_OPTION_SMP)
extern void OsTskTrapSmp(uintptr_t task);
#else
extern void OsTaskTrap(void);
#endif
extern void OsTskContextLoad(uintptr_t stackPointer);
extern void OsHwiMcTrigger(U32 coreMask, U32 hwiNum);

/*
 * 模块内内联函数定义
 */
/*
 * 描述: spinlock初始化
 */
OS_SEC_ALW_INLINE INLINE U32 OsSplLockInit(struct SreSpinLock *spinLock)
{
    OsSpinLockInitInner(&spinLock->rawLock);
    return SRE_OK;
}

/*
 * 描述: 获取硬线程ID
 */
OS_SEC_ALW_INLINE INLINE U32 OsGetHwThreadId(void)
{
    return SRE_GetCoreID();
}

/*
 * 描述: 计算一个32bit非0数字的最右位, e.g. 0x01000020 ----> 结果返回 5
 */
OS_SEC_ALW_INLINE INLINE U32 OsGetRMB(U32 bit)
{
    U32 rev = bit - 1;
    U32 result;

    OS_EMBED_ASM("EOR %0, %1, %2" : "=r"(result) : "r"(rev), "r"(bit));
    OS_EMBED_ASM("CLZ %0, %1" : "=r"(rev) : "r"(result));

    return (OS_DWORD_BIT_NUM - rev - 1);
}

/*
 * 描述: 计算一个32bit非0 bit的最左位数
 */
OS_SEC_ALW_INLINE INLINE U32 OsGetLMB1(U32 value)
{
    U32 mb;

    OS_EMBED_ASM("CLZ %0, %1" : "=r"(mb) : "r"(value));

    return (mb - OS_WORD_BIT_NUM);
}

/*
 * 描述: 使能IRQ中断
 */
OS_SEC_ALW_INLINE INLINE void OsIntEnable(void)
{
    OS_EMBED_ASM(
        "msr    daifclr, %0"
        :
        : "i"(DAIF_IRQ_BIT) // IRQ mask
        : "memory");
}

/*
 * 描述: 使能FIQ中断
 */
OS_SEC_ALW_INLINE INLINE void OsFiqEnable(void)
{
    OS_EMBED_ASM(
        "msr    daifclr, %0"
        :
        : "i"(DAIF_FIQ_BIT) // FIQ mask
        : "memory");
}

/*
 * 描述: 禁止IRQ中断
 */
OS_SEC_ALW_INLINE INLINE void OsIntDisable(void)
{
    OS_EMBED_ASM(
        "msr    daifset, %0"
        :
        : "i"(DAIF_IRQ_BIT) // IRQ mask
        : "memory", "cc");
}

/*
 * 描述: 设置中断亲和性（中断触发时路由到哪个核）
 */
OS_SEC_ALW_INLINE INLINE U32 OsHwiAffinitySet(HwiHandle hwiNum, U32 coreMask)
{
    U32 targetCore;

    if ((coreMask & (coreMask - 1)) != 0) { // SPI中断目前不支持1-N，只支持1-1
        return OS_ERRNO_MULTI_TARGET_CORE;
    }

    targetCore = OsGetRMB(coreMask);
    OsGicSetTargetId(hwiNum, targetCore);

    return SRE_OK;
}

/*
 * 描述: 获取当前PENDING的中断号, 中断状态PENDING->ACTIVE
 */
OS_SEC_ALW_INLINE INLINE U32 OsHwiNumGet(void)
{
    U32 iar;

    OS_EMBED_ASM("MRS    %0," REG_ALIAS(ICC_IAR1_EL1)" \n"
                 : "=&r"(iar) : : "memory");
    SRE_DSB();

    return iar;
}

OS_SEC_ALW_INLINE INLINE U32 OsGetMpidr(void)
{
    U32 mpid;

    OS_EMBED_ASM("MRS  %0, MPIDR_EL1" : "=r"(mpid)::"memory", "cc");

    return mpid;
}

OS_SEC_ALW_INLINE INLINE U32 OsPsciSmcFunc(uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3)
{
    U32 ret;

    OS_EMBED_ASM(
        "mov x0, %1\n"
        "mov x1, %2\n"
        "mov x2, %3\n"
        "mov x3, %4\n"
        "smc #0    \n"
        "mov %w0, w0"
        : "=r"(ret)
        : "r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3)
        : "memory", "%x0", "%x1", "%x2", "%x3");

    return ret;
}

OS_SEC_ALW_INLINE INLINE void OsPsciStartCpu(U32 cpuId, uintptr_t entry)
{
    OsPsciSmcFunc(PSCI_CPU_ON_FUNCID, cpuId, entry, 0);
}

OS_SEC_ALW_INLINE INLINE U32 OsPsciCheckCpuState(U32 cpuId)
{
    return OsPsciSmcFunc(PSCI_CPU_STATE_FUNCID, cpuId, 0, 0);
}

/*
 * 描述: 清除中断ACTIVE状态
 */
OS_SEC_ALW_INLINE INLINE void OsHwiClear(U32 intId)
{
    OS_EMBED_ASM("MSR " REG_ALIAS(ICC_EOIR1_EL1)", %0 \n"
                 : : "r"(intId) : "memory");
    SRE_ISB();
    return;
}

/*
 * 描述: 获取SP
 */
OS_SEC_ALW_INLINE INLINE uintptr_t OsGetSP(void)
{
    uintptr_t sp;

    OS_EMBED_ASM("MOV  %0, SP" : "=r"(sp));

    return sp;
}

/*
 * 描述: 获取LR
 */
OS_SEC_ALW_INLINE INLINE uintptr_t OsGetLR(void)
{
    uintptr_t lr;

    OS_EMBED_ASM("MOV  %0, x30" : "=r"(lr));

    return lr;
}

/*
 * 描述: 获取PC
 */
OS_SEC_ALW_INLINE INLINE uintptr_t OsGetPC(void)
{
    uintptr_t pc;

    OS_EMBED_ASM("1:adr %0, 1b" : "=r"(pc));

    return pc;
}

/*
 * 描述: 唤醒所有硬线程
 */
OS_SEC_ALW_INLINE INLINE void OsWakeUpProcessors(void)
{
    OS_EMBED_ASM("sev" : : : "memory");
}

/*
 * 描述: 将指定地址转换成uncache的地址
 */
OS_SEC_ALW_INLINE INLINE uintptr_t OsMemAddrToUncache(uintptr_t addr)
{
    return addr;
}

/*
 * 描述: 将指定地址转换成original地址
 */
OS_SEC_ALW_INLINE INLINE uintptr_t OsMemAddrToOriginal(uintptr_t addr, uintptr_t referAddr)
{
    (void)referAddr;
    return addr;
}

/*
 * 描述: 虚地址转换成实地址
 */
OS_SEC_ALW_INLINE INLINE U32 OsVa2Pa(VirtAddr vAddr, PhyAddr *addr)
{
    *addr = vAddr;
    return SRE_OK;
}

OS_SEC_ALW_INLINE INLINE U32 OsSymbolVa2PaCheck(const VirtAddr vAddr, PhyAddr *addr)
{
    *addr = (PhyAddr)vAddr; // 内部使用 注意使用地方是否需要真的虚实转换
    return SRE_OK;
}

#if defined(OS_MAX_CORE_NUM) && (OS_MAX_CORE_NUM == 1)
/*
 * 描述: 自旋锁上锁
 */
#if defined(OS_OPTION_SPINLOCK_OWNER_TRACE)
OS_SEC_ALW_INLINE INLINE void OsSplLock(volatile uintptr_t *spinLock)
{
    U32 tmp = 0;
    OS_EMBED_ASM(
        "1: ldaxr %w0, [%1]      \n"
        "   and %w0, %w0, #0x1   \n"
        "   cbnz  %w0, 1b        \n"
        "   stxr  %w0, %w2, [%1] \n"
        "   cbnz  %w0, 1b        \n"
        : "=&r" (tmp)
        : "r" (spinLock), "r" (1)
        :"memory", "cc");
    return;
}

/*
 * 描述: 自旋锁解锁
 */
OS_SEC_ALW_INLINE INLINE void OsSplUnlock(volatile uintptr_t *spinLock)
{
    U32 tmp = 0;
    OS_EMBED_ASM (
        "ldr %w0, [%1]      \n"
        "bic %w0, %w0, #0x1 \n"
        "stlr  %w0, [%1]    \n"
        : "=&r" (tmp)
        : "r"(spinLock)
        : "memory");
    return;
}
#else
/*
 * 描述: 自旋锁上锁
 */
OS_SEC_ALW_INLINE INLINE void OsSplLock(volatile uintptr_t *spinLock)
{
    U32 tmp = 0;
    OS_EMBED_ASM(
        "1: ldaxr %w0, [%1]      \n"
        "   cbnz  %w0, 1b        \n"
        "   stxr  %w0, %w2, [%1] \n"
        "   cbnz  %w0, 1b        \n"
        : "=&r" (tmp)
        : "r" (spinLock), "r" (1)
        :"memory", "cc");
    return;
}

/*
 * 描述: 自旋锁解锁
 */
OS_SEC_ALW_INLINE INLINE void OsSplUnlock(volatile uintptr_t *spinLock)
{
    OS_EMBED_ASM (
        "stlr  %w1, [%0] "
        :
        : "r"(spinLock), "r"(0)
        : "memory");
    return;
}
#endif

/*
 * 描述: 自旋锁尝试上锁
 */
OS_SEC_ALW_INLINE INLINE bool OsSplTryLock(volatile uintptr_t *spinLock)
{
    bool tmp;

    OS_EMBED_ASM(
        "   ldaxr   %w0, [%1]      \n"
        "   cbnz %w0, 1f           \n"
        "   stxr %w0, %w2, [%1]    \n"
        "1:                        \n"
        : "=&r"(tmp), "+r" (spinLock)
        : "r"(1)
        : "memory");

    return !tmp;
}

OS_SEC_ALW_INLINE INLINE void OsSplReadLock(volatile uintptr_t *spinLock)
{
    U32 tmp0 = 0;
    U32 tmp1 = 0;

    OS_EMBED_ASM(
        "1: ldaxr %w0, [%2]          \n"
        "   add   %w0, %w0,   #0x1   \n"
        "   tbnz  %w0, #31, 1b     \n"
        "   stxr  %w1, %w0,   [%2]   \n"
        "   cbnz  %w1, 1b            \n"
        : "=&r" (tmp0), "=&r" (tmp1)
        : "r" (spinLock)
        : "memory", "cc");
    return;
}

OS_SEC_ALW_INLINE INLINE void OsSplReadUnlock(volatile uintptr_t *spinLock)
{
    U32 tmp0 = 0;
    U32 tmp1 = 0;

    OS_EMBED_ASM(
        "1: ldaxr %w0, [%2]        \n"
        "   sub   %w0, %w0, #0x1   \n"
        "   stlxr %w1, %w0, [%2]   \n"
        "   cbnz  %w1, 1b          \n"
        : "=&r" (tmp0), "=&r" (tmp1)
        : "r" (spinLock)
        : "memory", "cc");
    return;
}

OS_SEC_ALW_INLINE INLINE void OsSplWriteLock(volatile uintptr_t *spinLock)
{
    U32 tmp0 = 0;

    OS_EMBED_ASM(
        "1: ldaxr %w0, [%1]      \n"
        "   cbnz  %w0, 1b        \n"
        "   stxr  %w0, %w2, [%1] \n"
        "   cbnz  %w0, 1b        \n"
        : "=&r" (tmp0)
        : "r" (spinLock), "r" (OS_SPINLOCK_RWLOCK_NEGATIVE)
        : "memory", "cc");
    return;
}

OS_SEC_ALW_INLINE INLINE void OsSplWriteUnlock(volatile uintptr_t *spinLock)
{
    OS_EMBED_ASM(
        "stlr wzr, [%0]      \n"
        :
        : "r" (spinLock)
        : "memory", "cc");
    return;
}

#else
OS_SEC_ALW_INLINE INLINE void OsSplLock(volatile uintptr_t *spinLock)
{
    (void)spinLock;
}

OS_SEC_ALW_INLINE INLINE void OsSplUnlock(volatile uintptr_t *spinLock)
{
    (void)spinLock;
}

OS_SEC_ALW_INLINE INLINE void OsSplReadLock(volatile uintptr_t *spinLock)
{
    (void)spinLock;
}

OS_SEC_ALW_INLINE INLINE void OsSplReadUnlock(volatile uintptr_t *spinLock)
{
    (void)spinLock;
}

OS_SEC_ALW_INLINE INLINE void OsSplWriteLock(volatile uintptr_t *spinLock)
{
    (void)spinLock;
}

OS_SEC_ALW_INLINE INLINE void OsSplWriteUnlock(volatile uintptr_t *spinLock)
{
    (void)spinLock;
}

#endif /* OS_MAX_CORE_NUM */

/*
 * 描述: 传入任务切换时的栈地址
 */
OS_SEC_ALW_INLINE INLINE uintptr_t OsTskGetInstrAddr(uintptr_t addr)
{
    return ((struct TagHwContext *)addr)->pc;
}

OS_SEC_ALW_INLINE INLINE void OsWaitForReboot(void)
{
}

OS_SEC_ALW_INLINE INLINE void OsHwiTriggerByMask(U32 coreMask, U32 hwiNum)
{
    OsHwiMcTrigger(coreMask, hwiNum);
}

OS_SEC_ALW_INLINE INLINE void OsHwiSetSplLockHook(OsVoidFunc hook)
{
    (void)hook;
}

OS_SEC_ALW_INLINE INLINE void OsHwiSetSplUnLockHook(OsVoidFunc hook)
{
    (void)hook;
}

#endif /* OS_CPU_ARMV8_EXTERNAL_H */
