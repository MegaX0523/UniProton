/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: cda模块内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组     华为规范整改
 */
#ifndef SRE_CDA_EXTERNAL_H
#define SRE_CDA_EXTERNAL_H

#include "sre_lib_external.h"
#include "sre_mem.h"
#include "sre_err_external.h"
#include "sre_mem_external.h"

#if defined(OS_OPTION_CDA)
/*
 * CDA记录信息中任务栈或各个内存分区的起始地址和大小。
 */
struct CdaPtInfo {
    uintptr_t addr; /* 内存分区起始地址 */
    uintptr_t size; /* 内存分区的大小 */
};

/*
 * CDA内存信息结构体,注意同步更新测试代码中的结构体TST_CDA_INFO_S
 */
struct TagCdaInfo {
    /* CDA结构信息魔术字 */
    U32 magicWord;
    /* OS版本号首地址 */
    uintptr_t osVerAddr;
    /* APP版本号首地址 */
    uintptr_t appVerAddr;
    /* 芯片类型 */
    U8 platform;
    /* CPU型号 */
    U8 cpuType;
    /* os缺省创建了几个分区 */
    U16 osPtNum;
    /* 当前实际创建分区总个数 */
    U16 maxPtNum;
    /* 对齐用 */
    U16 reserved1;
    /* 当前运行核号 */
    U8 coreID;
    /* 系统发生异常时记录的异常标志位 */
    U8 excFlag;
    /* 对齐用 */
    U16 reserved2;
    /* UniDSP系统状态地址 */
    uintptr_t uniFlagAddr;
    /* 异常信息首地址 */
    uintptr_t excBufAddr;
    /* 保存异常上下文地址 */
    uintptr_t excRegsAddr;
    /* 异常嵌套计数的地址 */
    uintptr_t excNestCntAddr;
    /* Hwi中断嵌套计数的地址 */
    uintptr_t intCountAddr;
    /* 存储硬中断嵌套关系的地址,Cortex-M4系统向量起始地址 */
    uintptr_t hwiNestAddr;
    /* 任务锁的地址 */
    uintptr_t taskLockAddr;
    /* 硬中断处理函数数组的首地址 */
    uintptr_t hwiFormAddr;
    /* 硬中断工作模式数组的首地址 */
    uintptr_t hwiModeFormAddr;
    /* 硬中断默认处理函数的地址 */
    uintptr_t hwiDefault;
    /* 内存中任务控制块起始地址 */
    uintptr_t tcbStartAddr;
    /* 内存中软中断控制块起始地址 */
    uintptr_t scbStartAddr;
    /* 核内消息节点链表起始地址 */
    uintptr_t msgStartAddr;
    /* 核内信号量节点链表起始地址 */
    uintptr_t semStartAddr;
    /* 内存中软件定时器控制块起始地址 */
    uintptr_t swtmrStartAddr;
    /* 核内消息个数 */
    U32 msgNum;
    /* 系统支持的最大任务个数 */
    U16 taskMaxNum;
    /* 系统支持的最大软中断个数 */
    U16 swiMaxNum;
    /* 软件定时器最大个数 */
    U16 swtmrMaxNum;
    /* 硬件定时器最大个数 */
    U16 hwtmrMaxNum;
    /* 系统记录错误信息索引号 */
    U32 recordIndex;
    /* 系统记录错误信息 */
    struct OsErrInfo errInfo[OS_ERR_RECORD_NUM];
    /* 除任务，软中断外线程最后记录的错误码 */
    U32 lastSysErr;
    /* 任务模块注册信息首地址 */
    uintptr_t tskModInfoAddr;
    /* TRACE模块起始地址 */
    uintptr_t traceModAddr;
    /* HUNT模块起始地址 */
    uintptr_t huntBufAddr;
    /* 系统栈栈底地址 */
    uintptr_t sysStackBottom;
    /* 系统栈栈顶地址 */
    uintptr_t sysStackTop;
    /* 每秒钟TICK数 */
    U32 tickPerSecond;
    /* 记录分区中各种块类型对应的块的大小 */
    uintptr_t sliceTypeToSizeAddr;
    /* 实例ID,针对83有效 */
    U8 instId;

#if defined(OS_OPTION_QUEUE)
    /* 队列最大个数 */
    U16 queueMaxNum;
    /* 队列的起始地址 */
    uintptr_t queueAddr;
#else
    U16 rev0;
#endif

    /* 核内信号量个数 */
    U16 semNum;
    U16 rev1;
    /* 硬件定时器控制块地址 */
    uintptr_t hwtmrCBAddr;

#if defined(OS_OPTION_HWSEM)
    /* HWSEM 局部硬件信号量个数 */
    U16 lhwSemNum;
    /* HWSEM 全局硬件信号量个数 */
    U16 gHwSemNum;
    /* HWSEM 控制块起始地址 */
    uintptr_t hwSemStartAddr;
#endif

    /* CDA版本号 */
    U32 cdaVersion;

#if defined(OS_OPTION_LP_DIS)
    /* 反汇编需要导出文件的物理起始地址 */
    uintptr_t disassemblyAddr;
    /* 反汇编需要导出文件的内存大小 */
    U32 disassemblySize;
#endif

    /* 消息使用+消息峰值统计地址 */
    uintptr_t msgUsedAddr;

    /* 各内存分区信息 */
    struct CdaPtInfo *cdaPtInfo;

#if defined(OS_OPTION_SHM)
    /* SHM 队列深度 */
    U32 shmQueDepth;
    /* SHM 控制块地址 */
    uintptr_t shmQueCtrlAddr;
#endif
};

/* CDA信息接口体 */
extern struct TagCdaInfo *g_cdaInfoAddr;

extern void OsCdaInfoReg(uintptr_t cdaAddr, U16 ptNum, U16 defaultPtNum);
extern U32 OsCdaInit(uintptr_t *addr, uintptr_t *size);
extern void OsCdaErrorInfoRecord(U32 errorNo, U32 uniTicks, bool lastErrFlag);

OS_SEC_ALW_INLINE INLINE void OsCdaExcNestCntAddrRecored(uintptr_t excNestCnt)
{
    g_cdaInfoAddr->excNestCntAddr = excNestCnt;
}

OS_SEC_ALW_INLINE INLINE U32 OsCdaLastSysErrGet(void)
{
    U32 errNo;

    if (g_cdaInfoAddr != NULL) {
        errNo = g_cdaInfoAddr->lastSysErr;
    } else {
        errNo = 0;
    }

    return errNo;
}

OS_SEC_ALW_INLINE INLINE void OsCdaHwiInit(uintptr_t defaultFuncAddr, uintptr_t intCountAddr, uintptr_t hwiNestAddr,
                                           uintptr_t hwiFormAddr, uintptr_t modeFormAddr)
{
    g_cdaInfoAddr->hwiDefault = defaultFuncAddr;
    g_cdaInfoAddr->intCountAddr = intCountAddr;
    g_cdaInfoAddr->hwiNestAddr = hwiNestAddr;
    g_cdaInfoAddr->hwiFormAddr = hwiFormAddr;
    g_cdaInfoAddr->hwiModeFormAddr = modeFormAddr;
}

/*
 * 描述: 初始化g_cdaInfoAddr的flag值
 */
OS_SEC_ALW_INLINE INLINE void OsCdaExcInit(void)
{
    g_cdaInfoAddr->excFlag = OS_MAX_U8;
}

OS_SEC_ALW_INLINE INLINE void OsCdaExcFlagRecord(U8 excFlag)
{
    if (g_cdaInfoAddr != NULL) {
        g_cdaInfoAddr->excFlag = excFlag;
    }
}

OS_SEC_ALW_INLINE INLINE void OsCdaExcRegsAddrRecord(uintptr_t excRegsAddr)
{
    if (g_cdaInfoAddr != NULL) {
        g_cdaInfoAddr->excRegsAddr = excRegsAddr;
    }
}

OS_SEC_ALW_INLINE INLINE void OsCdaMemPtInfoRecord(U8 ptNo, uintptr_t ptAddr, uintptr_t ptSize)
{
    g_cdaInfoAddr->cdaPtInfo[ptNo].addr = ptAddr;
    g_cdaInfoAddr->cdaPtInfo[ptNo].size = ptSize;
}

#if defined(OS_OPTION_HUNT)
OS_SEC_ALW_INLINE INLINE void OsCdaHuntInit(uintptr_t huntBufAddr)
{
    g_cdaInfoAddr->huntBufAddr = huntBufAddr;
}
#endif

#if defined(OS_OPTION_HWSEM)
OS_SEC_ALW_INLINE INLINE void OsCdaHwSemInit(U16 mcGHwsemNum, U16 hwSemNum, uintptr_t hwSemStartAddr)
{
    g_cdaInfoAddr->gHwSemNum = mcGHwsemNum;
    g_cdaInfoAddr->lhwSemNum = hwSemNum;
    g_cdaInfoAddr->hwSemStartAddr = hwSemStartAddr;
}
#endif

OS_SEC_ALW_INLINE INLINE void OsCdaHwtmrInit(uintptr_t hwtmrCBAddr, U16 hwtmrMaxNum)
{
    g_cdaInfoAddr->hwtmrCBAddr = hwtmrCBAddr;
    g_cdaInfoAddr->hwtmrMaxNum = hwtmrMaxNum;
}

OS_SEC_ALW_INLINE INLINE void OsCdaMsgInit(uintptr_t msgStartAddr, uintptr_t msgUsedAddr, U32 msgNum)
{
    g_cdaInfoAddr->msgStartAddr = msgStartAddr;
    g_cdaInfoAddr->msgNum = msgNum;
    g_cdaInfoAddr->msgUsedAddr = msgUsedAddr;
}

#if defined(OS_OPTION_QUEUE)
OS_SEC_ALW_INLINE INLINE void OsCdaQueuInit(U16 queueMaxNum, uintptr_t queueAddr)
{
    g_cdaInfoAddr->queueMaxNum = queueMaxNum;
    g_cdaInfoAddr->queueAddr = queueAddr;
}
#endif

OS_SEC_ALW_INLINE INLINE void OsCdaSemInit(U16 semNum, uintptr_t semStartAddr)
{
    g_cdaInfoAddr->semNum = semNum;
    g_cdaInfoAddr->semStartAddr = semStartAddr;
}

OS_SEC_ALW_INLINE INLINE void OsCdaSwtmrInit(U16 swtmrNum, uintptr_t swtmrStartAddr)
{
    g_cdaInfoAddr->swtmrMaxNum = swtmrNum;
    g_cdaInfoAddr->swtmrStartAddr = swtmrStartAddr;
}

OS_SEC_ALW_INLINE INLINE void OsCdaTaskInit(uintptr_t tcbStartAddr, uintptr_t tskModInfoAddr, U16 taskNum,
                                            uintptr_t uniFlagAddr, uintptr_t taskLockAddr)
{
    g_cdaInfoAddr->tcbStartAddr = tcbStartAddr;
    g_cdaInfoAddr->tskModInfoAddr = tskModInfoAddr;
    g_cdaInfoAddr->taskMaxNum = taskNum;
    g_cdaInfoAddr->uniFlagAddr = uniFlagAddr;
    g_cdaInfoAddr->taskLockAddr = taskLockAddr;
}

OS_SEC_ALW_INLINE INLINE void OsCdaTickInit(U32 tickPerSecond)
{
    g_cdaInfoAddr->tickPerSecond = tickPerSecond;
}

#if defined(OS_OPTION_TRACE)
OS_SEC_ALW_INLINE INLINE void OsCdaTraceInit(uintptr_t traceModAddr)
{
    g_cdaInfoAddr->traceModAddr = traceModAddr;
}
#endif

#if defined(OS_OPTION_SWI)
OS_SEC_ALW_INLINE INLINE void OsCdaSwiInit(U16 swiNum, uintptr_t scbStartAddr)
{
    g_cdaInfoAddr->swiMaxNum = swiNum;
    g_cdaInfoAddr->scbStartAddr = scbStartAddr;
}
#endif

OS_SEC_ALW_INLINE INLINE void OsCdaInstIdRecord(U8 instId)
{
    g_cdaInfoAddr->instId = instId;
}

#if ((!defined OS_OPTION_LP_PRELINK) && (defined OS_OPTION_LP_DIS))
OS_SEC_ALW_INLINE INLINE void OsCdaLpDisAreaRecord(uintptr_t addr, U32 size)
{
    g_cdaInfoAddr->disassemblyAddr = addr;
    g_cdaInfoAddr->disassemblySize = size;
}
#endif

OS_SEC_ALW_INLINE INLINE U32 OsCdaMemSizeGet(void)
{
    /* CDA结构体空间，结构体中存放内存首地址和大小数组(实际大小为m_MaxPtNum*2)中只预留一个元素 */
    return ALIGN(sizeof(struct TagCdaInfo) + sizeof(struct CdaPtInfo) * g_maxPtNum, OS_MEM_ADDR_ALIGN);
}

#if defined(OS_OPTION_SHM)
OS_SEC_ALW_INLINE INLINE void OsCdaShmInit(U32 shmQueDepth, uintptr_t shmQueCtrlAddr)
{
    g_cdaInfoAddr->shmQueDepth = shmQueDepth;
    g_cdaInfoAddr->shmQueCtrlAddr = shmQueCtrlAddr;
}
#endif
#endif

#endif /* SRE_CDA_EXTERNAL_H */
