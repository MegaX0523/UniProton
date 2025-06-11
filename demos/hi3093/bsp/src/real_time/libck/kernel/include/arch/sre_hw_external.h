/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Hardware involved
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HW_EXTERNAL_H
#define SRE_HW_EXTERNAL_H

#include "sre_lib_external.h"
#include "sre_hw.h"
#include "sre_task.h"
#include "sre_exc.h"
#include "sre_cache.h"
#include "sre_err_external.h"
#include "sre_init_trace_external.h"
#include "sre_cpu_external.h"
#include "sre_board_external.h"

/*
 * 模块间宏定义
 */
/* 地址判断宏 */
#define OS_SEC_ADDR_START_END_IN_RANGE(startAddr, size, compStartAddr, compSize) \
    (!(((startAddr) >= ((compStartAddr) + (compSize))) || (((startAddr) + (size)) <= (compStartAddr))))

#define OS_START_PRM_CFG_MAGIC_WORD 0xcfa699a1

#define OS_START_PARAM_UNINIT 0xAD37FEB1  // 实例启动参数配置状态:未配置
#define OS_START_PARAM_INIT 0xAB1234DC  // 实例启动参数配置状态:已配置

#define OS_PRIMARY_CORE TRUE
#define OS_SECONDARY_CORE FALSE
#define OS_PRIMARY_CORE_JUDGE(coreType) ((coreType) == g_mcpuFlag)
#define OS_BSS_SEG_OFFSET 2
/*
 * 供CDA解析 启动参数配置状态
 *
 */
struct TagSysStartPrmCfg {
    /* 初始化阶段供CDA解析信息的魔术字 */
    U32 startPrmCfgMagicWord;
    /* 启动参数配置状态 */
    U32 startPrmCfgState;
    /* 启动参数配置错误码,CDA会解出该错误码信息 */
    U32 startPrmCfgErrNo;
};

/*
 * 模块间typedef声明
 */
/*
 * 模块间全局变量声明
 */
extern U8 g_mcpuFlag;
extern U32 g_mcpuID;
#if defined(OS_OPTION_VEC_LAZY_SAVE)
extern void *g_cpOwner;
#endif
extern struct SreCpuTick g_approxCpuTick;
// 实例ID
extern U32 g_instID;
extern U32 g_instCurNum;
// 进程组ID
extern U32 g_pgID;

extern volatile U32 g_startParamInitPhase;

extern U32 _bss_table_start;
extern U32 _bss_table_end;

extern U32 __os_bss_table_start;
extern U32 __os_bss_table_end;
extern uintptr_t g_glbNsmStart;

/*
 * 模块间函数声明
 */
extern void OsSWExcHandlerSP(uintptr_t arg, U32 excCause);
extern void OsSWExcHandler(U32 excCause);
extern U8 OsGetMmuAttr(U32 vAddr);
extern U64 OsGetHwtmr(void);
extern void OsForceWakeUp(U32 coreId);
extern uintptr_t OsPa2GPaByCore(U8 hwCoreId, uintptr_t pa, uintptr_t dramLBase);
extern void OsCoreReset(U32 coreId);

#if defined(OS_OPTION_DEEPSLEEP)
extern U32 OsDssDeepSleepRequest(U32 coreId);
extern bool OsDssDeepSleepStatus(U32 coreId);
extern void OsDssDeepSleepWakeUp(U32 coreId);
extern void OsDssDeepSleepIntSet(U32 coreId, U32 dsIntNum);
#endif

extern void OsCntxIdSwitch(U32 cntxId);
extern uintptr_t OsScuVecBaseRegGet(U32 coreId);
extern uintptr_t OsScuInitAddrRegGet(U32 coreId);
extern void OsGetSecondaryCorePara(U32 type, uintptr_t *addr, uintptr_t *size);

/*
 * 模块间内联函数定义
 */
/*
 * @ingroup  SRE_hw
 * @brief 设置指定从核的启动参数
 *
 * @par 描述
 * 设置指定从核的启动参数
 *
 * @attention
 *
 * @param  coreBootPara    [IN] 类型#struct SecondaryCoreBootCfg *，从核启动参数结构指针
 *
 * @retval #OS_ERRNO_HW_SECONDARY_CORE_BOOTPARA_NULL            0x02002006，从核启动结构指针参数为NULL。
 * @retval #OS_ERRNO_HW_CORE_ID_INVALID                     0x02002005，Cored ID入参错误。
 * @retval #OS_ERRNO_HW_VMID_ID_ERR                         0x02002004，commVmid参数传入错误。
 * @retval #OS_ERRNO_HW_SECONDARY_CORE_BOOTSECNUM_NULL          0x02002007，启动参数中段数目配置为0。
 * @retval #OS_ERRNO_HW_SEC_AREA_OVERLAY_IN_VIR_ADDR        0x02002009，启动参数段配置中虚拟地址配置重叠。
 * @retval #OS_ERRNO_HW_VECBASE_ADDR_INVALID                0x0200200a，启动参数中设置的启动向量基地址vecbase不合法。
 * @retval #OS_ERRNO_HW_SYS_STACK_ADDR_INVALID              0x0200200b，启动参数中设置的系统栈地址不合法。
 * @retval #OS_ERRNO_HW_HEAP_ADDR_NOT_ALIGNED               0x0200200c，启动参数中设置的堆地址不对齐。
 * @retval #OS_ERRNO_MMU_PAGE_TBL_CFG_INVALID               0x02001235，传入的页表地址或长度非法。
 * @retval #OS_ERRNO_MMU_PAGE_TBL_ADDR_NOT_ALIGN            0x02001233，配置页表基地址未按0x4000字节(16K)大小对齐。
 * @retval #OS_ERRNO_MMU_PAGE_TBL_LEN_NOT_ALIGN             0x02001234，配置页表长度或地址未按0x400字节(1k)大小对齐。
 * @retval #OS_ERRNO_MMU_ADDR_OVERFLOW                      0x02001209，映射内存地址设置溢出。
 * @retval #OS_ERRNO_MMU_SECTION_CFG_INVALID                0x02001202，配置MMU段的物理地址或长度为0。
 * @retval #OS_ERRNO_MMU_NOT_PAGE_ALIGNED                   0x02001201，虚拟地址、物理地址或长度非page对齐。
 * @retval #OS_ERRNO_MMU_PAGEMAP_STATE_INVALID              0x02001224，设置的内存属性非法。
 * @retval #OS_ERRNO_MMU_SIZE_ERROR                         0x02001229，二级页表内存不够。
 * @retval #SRE_OK                                          0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_mmu.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 OsSecondaryPreBoot(struct SecondaryCoreBootCfg *coreBootPara);

/*
 * @ingroup  SRE_hw
 * @brief 唤醒核启动
 *
 * @par 描述
 * 触发核号为coreId的核的wakeup中断
 *
 *
 * @attention
 *
 * @param  coreId          [IN] 类型#UINT32，核号
 * @param  wakeupIntPri    [IN] 类型#UINT32，中断优先级,取值范围[1,7]。
 *
 * @retval #OS_ERRNO_HW_CORE_ID_INVALID                     0x02002005，Cored ID入参错误。
 * @retval #OS_ERRNO_HW_CORE_WAKE_UP_PRI_ERROR              0x02002008，唤醒中断优先级非法。
 * @retval #SRE_OK                                          0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_mmu.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 OsCoreWakeUp(U32 coreId, U32 wakeupIntPri);

#if defined(OS_OPTION_POWERGATE)
extern void OsDynPwrGtCorePowerUpProc(U32 coreId);
extern void OsDynPwrGtCoreWakeUp(U32 coreId, U32 wakeupIntPri);
extern void OsDynPwrGtCacheEnable(U32 coreId);
extern void OsDynPwrGtCacheInitPrimary(U32 coreId);
extern void OsDynPwrGtCoreWait(void);
extern void OsDynPwrGtSaveResetPara(struct SecondaryCoreBootCfg *bootParas);
#endif

#endif /* SRE_HW_EXTERNAL_H */
