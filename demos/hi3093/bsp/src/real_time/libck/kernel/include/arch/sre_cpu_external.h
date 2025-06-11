/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 核架构相关的头文件
 * Author: LibCK项目组
 * Create: 2009-10-5
 * Notes: 核架构相关的对外功能定义在对应的头文件下
 * History: 2018-10-5 LibCK项目组  华为规范整改
 */
#ifndef SRE_CPU_EXTERNAL_H
#define SRE_CPU_EXTERNAL_H

#include "sre_base.h"
#include "sre_asm_cpu_external.h"
#include "sre_attr_external.h"

#define NOP1()                                    \
    do {                                          \
        OS_EMBED_ASM("nop" : : : "memory", "cc"); \
    } while (0)

#define NOP4()  \
    do {        \
        NOP1(); \
        NOP1(); \
        NOP1(); \
        NOP1(); \
    } while (0)

#define NOP8()  \
    do {        \
        NOP4(); \
        NOP4(); \
    } while (0)

#define ASM_NOP() \
    do {          \
        NOP8();   \
        NOP8();   \
    } while (0)

#define OS_GOTO_SYS_ERROR() \
    do {                    \
        OsAsmIll();         \
    } while (0)

/* 仅提供给安全函数返回失败使用 */
#define OS_GOTO_SYS_ERROR1() OS_GOTO_SYS_ERROR()

#define OS_GOTO_SYS_ERROR_REC_RET(ret) \
    do {                               \
        OS_REPORT_ERROR_DETAIL(ret);   \
        OS_GOTO_SYS_ERROR();           \
    } while (0)

#define OS_SW_EXC_HWSEM_ERR 0
#define OS_SW_EXC_MSGM_ERR 1
#define OS_SW_EXC_MCHWTMR_ERR 2
#define OS_SW_EXC_PCC_ERR 3
#define OS_SW_EXC_DCC_ERR 4
#define OS_SW_EXC_MMU_ERR 5

#define OS_HWTMR_MAX_NUM 2

#define OS_HWI_EXC OS_HWI_MAX_NUM

#define OS_SYS_STACK_ADDR_ALIGN 16
#define OS_SYS_STACK_PADDING_VALUE 0xCA

extern void OsAsmIll(void);
extern void OsWait(uintptr_t intLevel, U32 wakeup);
extern void OsFirstTimeSwitch(void);
extern void OsHwiFirstTimeSwitch(void);
extern void *OsTskContextInit(U32 taskID, U32 stackSize, uintptr_t *topStack, uintptr_t funcTskEntry);
extern void OsTskContextGet(uintptr_t saveAddr, struct TskContext *context);
extern void OsTickStartRegSet(U16 tickHwTimerIndex, U32 cyclePerTick);
extern void OsTickSchedC(void);

#if defined(OS_ARCH_RISCV)
#include "../cpu/riscv/os_cpu_riscv_external.h"
#endif

#if defined(OS_ARCH_ARMV8)
#include "os_cpu_armv8_external.h"
#endif

#endif /* SRE_CPU_EXTERNAL_H */
