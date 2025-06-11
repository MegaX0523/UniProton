/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 核架构相关的汇编宏头文件
 * Author: LibCK项目组
 * Create: 2009-10-5
 * Notes: 本头文件会在汇编里面调用，非汇编使用的宏请勿定义在此文件
 * History: 2018-10-5 LibCK项目组  华为规范整改
 */
#ifndef SRE_ASM_CPU_EXTERNAL_H
#define SRE_ASM_CPU_EXTERNAL_H

#include "sre_buildef.h"

#if (OS_HARDWARE_PLATFORM == OS_RISCV)
#include "../cpu/riscv/os_asm_cpu_riscv_external.h"
#endif

/* 有名空间不支持时，从核不能从有名空间获取 */
#if defined(OS_OPTION_NSM)
#define GLB_NS_ADDR     g_glbNSAddr
#define GLB_NSM_START   g_glbNsmStart
#else
#define GLB_NS_ADDR     0
#define GLB_NSM_START   0
#endif

#define OS_CORE_WAKEUP_INT 0 // 仅中断方式唤醒core wait
#define OS_CORE_WAKEUP_INT_AND_MSQ 1 // 中断及消息方式唤醒core wait

#define OS_EXC_REGINFO_OFFSET 160
#define OS_EXC_MAGIC_WORD 0xEDCAACDC
#define OS_SYS_STACK_MAGIC_WORD 0xCACACACA /* 系统栈魔术字 */

/* 内核状态定义 */
#define OS_FLG_HWI_ACTIVE 0x0001U
#define OS_FLG_BGD_ACTIVE 0x0002U
#define OS_FLG_SWI_ACTIVE 0x0004U
#define OS_FLG_TICK_ACTIVE 0x0008U
#define OS_FLG_SYS_ACTIVE 0x0010U
#define OS_FLG_EXC_ACTIVE 0x0020U
#define OS_FLG_INT_DISABLE 0x0040U /* 是否支持中断嵌套 */
#define OS_FLG_CRE_ACTIVE 0x0080U
#define OS_FLG_SWI_ENABLE 0x0100U
#define OS_FLG_SWI_REQ 0x0200U
#define OS_FLG_TSK_REQ 0x1000U
#define OS_FLG_TSK_SWHK 0x2000U /* 任务切换时是否调用切换入口函数 */
#define OS_FLG_HWI_HOOK 0x4000U /* 硬中断进出钩子标志 */
#define OS_FLG_SWI_HOOK 0x8000U /* 软中断进出钩子标志 */

#define OS_FLG_FIBER_ENABLE 0x10000U   /* 纤程使能 */
#define OS_FLG_FIBER_REQ    0x20000U   /* 纤程请求 */
#define OS_FLG_FIBER_ACTIVE 0x40000U   /* 纤程激活 */

#define OS_FLG_INT_DISABLE_BIT 6
#define OS_FLG_SWI_ENABLE_BIT 8
#define OS_FLG_SWI_REQ_BIT 9
#define OS_FLG_TSK_REQ_BIT 12
#define OS_FLG_TSK_SWHK_BIT 13
#define OS_FLG_HWI_HOOK_BIT 14

#define OS_SWI_CREATED 0x0001U
#define OS_SWI_READY 0x0004U
#define OS_SWI_ACTIVE 0x0008U

#define OS_TSK_RUNNING 0x0080U

#define OS_EXC_CONTEX_SAVE_BEFORE 1  // 上下文保存前
#define OS_EXC_CONTEX_SAVE_AFTER 2  // 上下文保存后
#define OS_EXC_ENTER_ENTRY 3  // 进入异常c函数入口
#define OS_EXC_BSS 4  // bss判断
#define OS_EXC_HWSEM_ERR_HANDLE 5  // 硬件信号量错误处理
#define OS_EXC_EMMU_ERR_HANDLE 6  // EMMU错误处理
#define OS_EXC_TRACE_TIME 7  // trace记录时间戳
#define OS_EXC_CP_SAVE 8  // 保存矢量寄存器
#define OS_EXC_INFO_SAVE 9  // 异常信息存储后
#define OS_EXC_HOOK 10  // 异常钩子执行后
#define OS_EXC_FREEZE 11  // 异常钩子冻结后
#define OS_EXC_LAST_WORDS 12  // 临终遗言
#define OS_EXC_ALLOW_LIST 13  // 广播通知消息白名单
#define OS_EXC_RECLAIM_HOOK 14  // 制式资源清理
#define OS_EXC_REBOOT_FLUSH_CACHE 15  // 回刷cache
#define OS_EXC_REMOVE_CPU_MASK 16  // 动态加载回收钩子
#define OS_EXC_REBOOT_HOOK 17  // reboot钩子后
#define OS_EXC_INFO_SAVE_PRE_MMU 18  // 实模式异常信息存储后

/* 表示主核处于实模式启动阶段状态（该阶段主要完成MMU使能初始化） */
#define OS_MCORE_PBOOT_PHASE 0x01  // 对应sre_sys.h 中的OS_MCORE_PHY_BOOT_PHASE
/* 表示表示主核处于虚模式启动阶段状态，仅SD6183平台有效。 */
#define OS_MCORE_VBOOT_PHASE 0x02  // 对应sre_sys.h 中的OS_MCORE_VIR_BOOT_PHASE
/* 进入SRE_HardBootInit，即系统完成DDR自检及加载 */
#define OS_CORE_BOOT_PHASE 0x03  // 对应sre_sys.h 中的OS_BOOT_PHASE
/* 表示退出SRE_HardBootInit。 */
#define OS_CORE_AFTER_BOOT_PHASE 0x05  // 对应sre_sys.h 中的OS_AFTER_BOOT_PHASE
/* 表示Dcache初始化完毕。 */
#define OS_CORE_DCACHEINIT_PHASE 0x07  // 对应sre_sys.h 中的OS_DCACHEINIT_PHASE
/* 表示进入C lib库初始化 */
#define OS_CORE_LIBCINIT_PHASE 0x08  // 对应sre_sys.h 中的OS_LIBCINIT_PHASE

#endif /* SRE_ASM_CPU_EXTERNAL_H */
