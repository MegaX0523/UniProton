/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: LibCK配置私有文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_CONFIG_INTERNAL_H
#define SRE_CONFIG_INTERNAL_H

#include <stdint.h>
#include "sre_base.h"
#include "sre_config.h"
#if (OS_INCLUDE_PATCH == YES)
#include "securec.h"
#include "sre_sp_mcpat.h"
#include "sre_sp_mcpat_config.h"
#endif

#include "./common/os_init.h"
#include "./common/os_boot.h"
#include "./common/os_shell.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */
#define WORD_PACK(val) (((val) << 24) | ((val) << 16) | ((val) << 8) | (val))

#define CHIPID_BASE (void *)0x14720000
#define CHIP_ID_ADDR (CHIPID_BASE + 0x00000020)
#define CHIP_ID_306X 0x65200234

/*
 * 用于标记当前系统开始运行到哪个阶段(实时值),initPhase值 = OS_INIT_PHASE | MODULE_ID
 * OS_INIT_PHASE表示系统初始化阶段状态bit[31-16]，MODULE_ID表示当前系统初始化的模块号bit[0-15]
 */
#define OS_INIT_TRACE_BEG(osPhase, modId) OsSysInitTraceBegin((osPhase), (U32)(modId))

/*
 * 记录系统中模块是否初始化结束，0-63位用来记录已进行初始化的所有模块的模块号，若某个模块正确进行完了初始化操作，
 * 将其模块号对应的位置为1
 */
#define OS_INIT_TRACE_END(osPhase, modId) OsSysInitTraceEnd((osPhase), (U32)(modId))

/* 记录错误码 */
#define OS_INIT_TRACE_ERR_ID(errNO) OsSysInitTraceErrNo(errNO)

#if defined(OS_OPTION_INIT_PHASE_QUERY)
#define OS_INIT_PHASE_SELF_SET(phase) OsInitPhaseSet(SRE_GetCoreID(), phase)
#else
#define OS_INIT_PHASE_SELF_SET(phase)
#endif

#if defined(OS_OPTION_BOOT_PARAM_CFG)
/*
 * 主核启动参数结构体,适用于主核启动参数的配置，该结构体中的成员secInfo[1]数组维数可以更改
 */
struct MainCoreBootPara {
    /* 取值范围0~33 */
    U32 coreId;
    /* 系统模式context ID, 由OS组装context ID： VMID（bit5~7）|THDID（bit4） */
    U32 vmid;
    /* MMU页表入口基地址，保证0x4000(16K)对齐 */
    U32 pageTblAddr;
    /*
     * MMU页表占用的内存长度（一级及二级页表总长度），保证大于等于一级页表长度0x4000（16K）,且保证0x400(1K)对齐。
     * 页表大小配置必须保证能完成所有段的映射，建议预留页表长度空间，若页表长度配置为0x4000，
     * 则需要保证映射的所有段（包括工具方式映射或OS映射）不需要二级页表空间。
     * 1、二级页表的起始地址为(pageTblAddr + 0x4000)，二级页表空间供给OS启动过程中的段映射及用户调用SRE_MmuAssign
     * 接口映射使用;
     * 2、启动阶段过程中使用的二级页表空间计算,从启动参数配置的段(secInfo)或工具加载映射的段(目前从mmu_config中获取)
     * 中找出映射段起始地址中bit[31,30]不相同的段，且映射段的长度小于1M，这样条件下的映射段数为N,
     * 二级页表占用大小为(N * 1K)Bytes; 3、调用MMU接口SRE_MmuAssign使用的二级页表空间
     * 根据入参vStart地址及映射长度len，划出1K空间给二级页表，原理同启动加载映射关系一致。
     */
    U32 pageTblLen;

    /* 实地址映射虚地址表空间基地址，需配置在DDR空间,一一映射，保证0x1000(4K)对齐 */
    U32 phy2VirTblAddr;
    /* 实地址映射虚地址表空间长度，保证0x1000(4K)对齐，若不需要该空间，必须配置为0，否则空间大小至少133K */
    U32 phy2VirTblLen;
    /* TRUE:表示OS进行MMU页表映射MAP的建立; FALSE:通过工具进行MMU页表映射MAP的建立 */
    bool mmuMapFlag;
    /* 硬线程启动向量基地址（注意配置为虚地址），指定vecbase，保证4K字节对齐 */
    U32 vecbase;
    /* 硬线程启动系统堆地址（注意配置为虚地址），保证16字节对齐，需与LSP中配置的堆地址保持一致 */
    U32 sysHeap;
    /* 硬线程启动系统栈地址（系统栈栈底，注意配置为虚地址），保证16字节对齐，且不能为零，需与LSP配置的栈地址保持一致 */
    U32 sysStack;
    /* 配置的映射段个数 */
    U32 secNum;
    /*
     * 该处开始顺序存放映射的段结构参数，向量基地址、系统栈、系统堆及".os.PL2.cwna.data"段所在的映射段必须配置,
     * 数组维数用户可以根据实际MMU映射段数secNum进行更改
     */
    struct SecAtt secInfo[3];
};

#ifndef LIBOS_RUNTIME_TF_CONFIG
#if !defined(OS_OPTION_SREMNG_SECONDARY_CORE)
/*
 * 注意：用户在主核镜像代码中必须定义主核启动参数，同时"RESET_SEC_DATA
 * __attribute__((aligned(16)))"部分用户配置必须保持不变，变量名可以更改，MAIN_CORE_BOOTPARA_S结构中的成员变量
 * "struct SecAtt secInfo[]" 的数组维数用户可以根据实际映射段数secNum进行更改
 * 定义的启动参数值用户需要根据实际启动场景务必自行重新设置, "RESET_SEC_DATA"段的变量只允许配置一个
 * 支持PGood模式启动，必须将coreId值设置成0xFFFFFFFF，不设置主核，等芯片启动之后确定；
 * 否则默认按照原有模式启动，核号匹配设置的coreId的核做主核
 */
RESET_SEC_DATA struct MainCoreBootPara g_mainCoreBootPara __attribute__((aligned(16))) = {
    0x0, 0x0, 0, 0, 0x0, 0x0, 0x0, 0x10000000, 0x10030000, 0x10040000, 0x3, {
        { 0x10000000, 0x3f000000, 0x2000, 0x1f },
        { 0x10030000, 0x3f030000, 0x10000, 0x1f },
        { 0x10010000, 0x3f010000, 0x10000, 0x1f }
    }
};
#else
/* 注意:该段放置在PL2的代码段，同进程组两个硬线程共享，如果有两个硬线程区分开的字段，需要特殊处理 */
RESET_SEC_DATA struct SlvCoreBootPara g_bmSreCoreBootPara __attribute__((aligned(16))) = {
#if (defined OS_OPTION_LP_PRELINK)
    0x1000, 0x1000,
#endif
    0, 0, 0, 0, 0, (uintptr_t)&__stack, (uintptr_t)&_stack_sentry, 0, 0
};
#endif
#endif

#endif

#if defined(OS_OPTION_INIT_TRACE_USR_HOOK)
typedef void (*InitTraceHook)(U32 initPhase, U32 modId);
INIT_SEC_L4_DATA InitTraceHook g_initTraceBeginUsrHook = NULL;
INIT_SEC_L4_DATA InitTraceHook g_initTraceEndUsrHook = NULL;
#endif

#if (defined(OS_OPTION_SMP) || defined(OS_ARCH_ARMV7_A) || defined(OS_ARCH_ARMV7) || defined(OS_ARCH_ARMV8))
RESET_SEC_DATA U32 g_cfgPrimaryCore = OS_SYS_CORE_PRIMARY;
#endif

#if defined(OS_OPTION_FUSION_INTEGRATE_API)
RESET_SEC_DATA struct TagBootPara g_bootPara = {
    OS_SYS_MP_REG_BASE,
    OS_SYS_RUN_COREMASK,
#if defined(OS_OPTION_MMU)
    OS_MMU_MAP_INFO_ADDR
#endif
};
#endif

enum OsinitPhaseId {
    OS_REGISTER_ID = 0,
    OS_INIT_ID,
    OS_MOUDLE_CONFIG
};
typedef U32 (*ConfigInitFunc)(void);
struct OsModuleConfigInfo {
    enum OsMoudleId moudleId;
    ConfigInitFunc moudleConfigFunc[OS_MOUDLE_CONFIG];
};
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_CONFIG_INTERNAL_H */
