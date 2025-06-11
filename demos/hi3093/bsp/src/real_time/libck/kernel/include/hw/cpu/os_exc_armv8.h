/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: 异常模块的外部头文件。
 * Author: LibCK项目组
 * Create: 2020-06-16
 * History: 2020-06-16 LibCK项目组 创建文件
 */
#ifndef OS_EXC_ARMV8_H
#define OS_EXC_ARMV8_H

#include "sre_typedef.h"
#include "sre_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_exc
 * OS使用的EL个数，EL1~EL3
 */
#define OS_ELX_NUM       3

/*
 * @ingroup SRE_exc
 * 通用寄存器个数, x0~x30
 */
#define XREGS_NUM     31

/*
 * @ingroup SRE_exc
 * CORTEX_RX异常寄存器信息结构
 */
struct ExcElxRegs {
    uintptr_t esr;
    uintptr_t far;
    uintptr_t spsr;
    uintptr_t elr;
    uintptr_t sctlr;
    uintptr_t sp;
    uintptr_t vbar;
    uintptr_t ttbr0;
    uintptr_t tcr;
    uintptr_t mair;
};

struct ExcRegInfo {
    uintptr_t ttbr0;
    uintptr_t ttbr1;
    uintptr_t tcr;
    uintptr_t mair;
    uintptr_t sctlr;
    uintptr_t vbar;
    uintptr_t currentEl;
    uintptr_t sp;
    // 以下字段的内存布局与TskContext保持一致
#if defined(OS_OPTION_HAVE_FPU)
    __uint128_t q[OS_FPU_CONTEXT_REG_NUM];
    uintptr_t fpcr;
    uintptr_t fpsr;
#endif
    uintptr_t elr;               // 返回地址
    uintptr_t spsr;
    uintptr_t far;
    uintptr_t esr;
    uintptr_t xzr;
    uintptr_t xregs[XREGS_NUM];         // 0~30: x30~x0
};

/*
 * @ingroup SRE_exc
 *
 * 异常信息结构体
 */
struct ExcInfo {
    /* *< OS版本号                                         */
    char osVer[OS_SYS_OS_VER_LEN];
    /* *< 产品版本号                                       */
    char appVer[OS_SYS_APP_VER_LEN];
    /* *< 异常原因                                         */
    U32 excCause;
    /* *< 异常前的线程类型                                 */
    U32 threadType;
    /* *< 异常前的线程ID, 该ID组成濉禩ID = LTID | ProcessID | COREID | GTID> */
    U32 threadId;
    /* *< 字节序                                           */
    U16 byteOrder;
    /* *< CPU类型                                          */
    U16 cpuType;
    /* *< CPU ID                                           */
    U32 coreId;
    /* *< CPU Tick                                         */
    struct SreCpuTick cpuTick;
    /* *< 异常嵌套计数                                     */
    U32 nestCnt;
    /* *< 致命错误码，发生致命错误时有效 */
    U32 fatalErrNo;
    /* *< 异常前栈指针                                     */
    uintptr_t sp;
    /* *< 异常前栈底                                       */
    uintptr_t stackBottom;
    /* *< 异常发生时的核内寄存器上下文信息                         */
    struct ExcRegInfo regInfo;
};

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:异常原因参见ESR寄存器。
 */
#define OS_EXCEPT_ESR        0

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:其他核异常。
 */
#define OS_EXCEPT_OTHER_CORE 1

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:致命错误异常。
 */
#define OS_EXCEPT_FATALERROR 2

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:栈越界异常。
 */
#define OS_EXCEPT_STACKOVERFLOW 3

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:非法指令异常。
 */
#define OS_EXCEPT_UNDEF_INSTR 4

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:数据中止异常。
 */
#define OS_EXCEPT_DATA_ABORT 5

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:快速中断异常。
 */
#define OS_EXCEPT_FIQ 6

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:pc非对齐异常。
 */
#define OS_EXCEPT_PC_NOT_ALIGN 7

/*
 * @ingroup SRE_exc
 * ARMV8异常具体类型:sp非对齐异常。
 */
#define OS_EXCEPT_SP_NOT_ALIGN 8

/*
 * @ingroup SRE_exc
 * 异常处理函数返回类型:系统在发生异常后(并做完相关处理后)继续运行。
 */
#define SRE_EXC_PROC_TYPE_RETURN OS_EXC_PROC_TYPE_RETURN

/*
 * @ingroup SRE_exc
 * 异常处理函数返回类型:系统在发生异常后(并做完相关处理后)进入死循环，等待重启。
 */
#define SRE_EXC_PROC_TYPE_NO_RETURN OS_EXC_PROC_TYPE_NO_RETURN

/*
 * @ingroup SRE_exc
 * 异常处理函数返回类型:系统在发生异常后(并做完相关处理后)跳过异常指令继续运行。
 */
#define SRE_EXC_PROC_TYPE_RETURN_SKIP_INST OS_EXC_PROC_TYPE_RETURN_SKIP_INST

extern U32 SRE_ExcProc(struct ExcInfo *excInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_EXC_ARMV8_H */
