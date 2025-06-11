/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: 启动模块的头文件, 用户不能直接引用
 * Author: LibCK项目组
 * Create: 2020-11-22
 * History: 2020-11-22 LibCK项目组 创建文件
 */
#ifndef OS_BOOT_H
#define OS_BOOT_H

#include "os_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_hw
 * 从核启动的映射段定义。
 */
struct SecAtt {
    uintptr_t secVA;
    uintptr_t secPA;
    U32 secSize;
    U32 secAttr;
};

struct TagMmuMapInfo {
    U32 secNum;
    struct SecAtt *secAttr;
};

/* hitsp/hitvp 平台启动参数 */
struct TagBootPara {
    uintptr_t mpRegBase;  /* MP 寄存器基地址 */
    U32 runCoreMask; /* 运行时的核掩码，替代OS_SYS_CORE_RUN_NUM和OS_SYS_CORE_MAX_NUM */
#if defined(OS_OPTION_MMU)
    struct TagMmuMapInfo  *mmuMapInfo; /* 启动时的MMU映射配置信息 */
#endif
};

#ifndef LIBCK_TOOL
// 主核需要在从核启动前把以下内容放在版本空间中
struct SlvCoreBootPara {
#if (defined OS_OPTION_LP_PRELINK)
    U32 slvUpgradeId;
    U32 primUpgradeId;
#endif
    U32 mcpuId; /* *主核ID */
    uintptr_t glbNsmAddr; /* *配置的实例间共享内存首地址 */
    uintptr_t resv;
    U32 instId; /* *实例ID */
    U32 pgId; /* *进程组ID */
    uintptr_t oldStack; /* *配置前链接生成的系统栈底 */
    uintptr_t oldStackEntry; /* *配置前链接生成的系统栈顶 */
    uintptr_t initTraceAddr; /* *从核初始化trace变量地址 */
    uintptr_t nsmMagicWordAddr; /* *主核sremng提供的nsm共享地址，用于存放从核供CDA解析的魔术字地址值(LocalPa) */
};
#endif

/*
 * @ingroup SRE_hw
 * 从核启动参数配置结构体，适用于设置从核启动参数，通过SRE_BootSecondaryCore接口入参传入
 */
struct SecondaryCoreBootCfg {
    /* 取值范围0~33 */
    U32 coreId;
    /* 系统模式context ID, 由OS组装context ID： VMID（bit5~7）|THDID（bit4） */
    U32 vmid;
    /* MMU页表入口基地址，保证0x4000(16K)对齐 */
    uintptr_t pageTblAddr;
    /*
     * MMU页表占用的内存长度（一级及二级页表总长度），保证大于等于一级页表长度0x4000（16K）,且保证0x400(1K)对齐。
     * 页表大小配置必须保证能完成所有段的映射，建议预留页表长度空间，若页表长度配置为0x4000，
     * 则需要保证映射的所有段（包括工具方式映射或OS映射）不需要二级页表空间。
     * 1、二级页表的起始地址为(pageTblAddr +
     * 0x4000)，二级页表空间供给OS启动过程中的段映射及用户调用SRE_MmuAssign接口映射使用;
     * 2、启动阶段过程中使用的二级页表空间计算，
     * 从启动参数配置的段(secInfo)或工具加载映射的段(目前从mmu_config中获取)中找出映射段起始地址中bit[31,30]不相
     * 同的段，且映射段的长度小于1M，这样条件下的映射段数为N,
     * 二级页表占用大小为(N * 1K)Bytes. 3、调用MMU接口SRE_MmuAssign使用的二级页表空间:根据入参vStart地址及
     * 映射长度len，划出1K空间给二级页表，原理同启动加载映射关系一致。
     */
    U32 pageTblLen;

    /* 实地址映射虚地址表空间基地址，需配置在DDR空间，保证0x1000(4K)对齐 */
    uintptr_t phy2VirTblAddr;
    /* 实地址映射虚地址表空间长度，若不需要该空间，必须配置为0 */
    U32 phy2VirTblLen;
    /* TRUE:表示OS进行MMU页表映射MAP的建立; FALSE:通过工具进行MMU页表映射MAP的建立 */
    bool mmuMapFlag;
    /* 硬线程启动向量基地址（注意配置为虚地址），指定vecbase，保证4K字节对齐 */
    uintptr_t vecbase;
    /* 硬线程启动系统堆地址（注意配置为虚地址），保证16字节对齐，需与LSP中配置的堆地址保持一致 */
    uintptr_t sysHeap;
    /*
     * 硬线程启动系统栈地址（系统栈的栈底，注意配置为虚地址），保证16字节对齐，且不能为零，需与LSP配置的栈地址保持一致
     */
    uintptr_t sysStack;
    /* 从核启动PC地址（注意配置为虚地址），保证PC准确 */
    uintptr_t initAddr;
    /* 配置的映射段个数 */
    U32 secNum;
    /* 该处开始顺序存放映射的段结构参数 */
    struct SecAtt *secInfo;
};
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_BOOT_H */
