/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: LibCK配置头文件，裁剪开关和配置项。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_CONFIG_H
#define SRE_CONFIG_H

#include "sre_buildef.h"
#include "sre_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* ***************************** 配置系统基本信息 ******************************* */
/* 芯片主频 */
#define OS_SYS_CLOCK                                    25000000
/* 实际运行的核数,单位：个 */
#define OS_SYS_CORE_RUN_NUM                             1
/* 最大可支持的核数,单位：个 */
#define OS_SYS_CORE_MAX_NUM                             4
/* 需要运行的核掩码 */
#define OS_SYS_RUN_COREMASK                             0x1
/* 运行LibCK多核系统的核号(SD6183x为硬线程号)起点 */
#define OS_SYS_CORE_START                               0
/* 主核ID */
#define OS_SYS_CORE_PRIMARY                              0
/* MP寄存器基地址 */
#define OS_SYS_MP_REG_BASE                              0x0
/* 用户挂接的单板复位函数 */
#define OS_SYS_REBOOT_HOOK                              NULL
/* 从核自检小版本 */
#define OS_INCLUDE_MINIMUM_IMG                          NO

/*
 * 用户注册的获取系统时间函数，类型为SYS_TIME_FUNC,
 * 仅对Cortex-RX有效，时钟源频率需和OS_SYS_CLOCK一致
 */
#define OS_SYS_TIME_HOOK                                SRE_GetCpuCycle
/* 最大镜像个数，实例内所有镜像配置相同， 仅SD6157/SD6182支持 */
#define OS_SYS_MAX_IMAGE_NUM                            0
/* 注意：使用SRE_MNG 管理，如下3个参数从核配置无效，由SRE自动传参 */
/*
 * 当前镜像ID，从0开始，用户保证镜像内配置一致,
 * 实例内镜像间的配置不同,且不能大于等于最大镜像个数。
 */
#define OS_SYS_IMAGE_ID                                 0
/* 系统栈起始地址，用户保证随机化前的起始地址cacheline对齐 */
#define OS_SYS_STACK_AREA_START_ADDR                    NULL
/* 系统栈大小，用户保证随机化前的大小cacheline对齐 */
#define OS_SYS_STACK_AREA_SIZE                          0

/* ***************************** 配置定时器模块 ***************************** */
/* 硬件定时器模块裁剪开关 */
#define OS_INCLUDE_HW_TIMER                             NO
/* 全局共享硬件定时器裁剪开关 */
#define OS_INCLUDE_MCHW_TIMER                           NO
/* 全局共享硬件定时器时钟频率 */
#define OS_MCHW_TIMER_CLOCK                             0
/* 基于TICK的软件定时器裁剪开关 */
#define OS_INCLUDE_TICK_SWTMER                          YES
/* 基于TICK的软件定时器最大个数 */
#define OS_TICK_SWITIMER_MAX_NUM                        8
/* TRING定时器模块裁剪开关, 仅SD6185x支持 */
#define OS_INCLUDE_TRING_TIMER                          NO

/* ***************************** 配置Tick中断模块 *************************** */
/* Tick中断模块裁剪开关 */
#define OS_INCLUDE_TICK                                 YES
/* Tick中断的优先级等级配置,仅SD6183x支持 */
#define OS_TICK_PRIORITY                                2
/* Tick中断时间间隔，tick处理时间不能超过1/OS_TICK_PER_SECOND(s) */
#define OS_TICK_PER_SECOND                              1000
/* ***************************** 配置软中断模块 ***************************** */
/* 软中断模块裁剪开关 */
#define OS_INCLUDE_SWI                                  NO
/* 核间软中断裁剪开关 */
#define OS_INCLUDE_MCSWI                                NO
/* 最大支持的软中断个数,最大可配置值为125 */
#define OS_SWI_MAX_SUPPORT_NUM                          16

/* ***************************** 配置任务模块 ******************************* */
/* 任务模块裁剪开关 */
#define OS_INCLUDE_TASK                                 YES
/* 最大支持的任务数,软中断和任务最大共支持254个 */
#define OS_TSK_MAX_SUPPORT_NUM                          16
/* 缺省的任务栈大小 */
#define OS_TSK_DEFAULT_STACK_SIZE                       0x1000
/* IDLE任务栈的大小 */
#define OS_TSK_IDLE_STACK_SIZE                          0x1000
/* 任务栈初始化魔术字，默认是0xCA，只支持配置一个字节 */
#define OS_TSK_STACK_MAGIC_WORD                         0xCA

/* ***************************** 配置纤程模块 ******************************* */
/* 纤程模块裁剪开关 */
#define OS_INCLUDE_FIBER                                 NO
/* 最大支持的纤程数 */
#define OS_FIBER_MAX_SUPPORT_NUM                         20

/* ***************************** 配置调度域模块 ***************************** */
/* 调度域信息的结构体指针，SMP场景使用，非空时有效 */
#define OS_SCHEDULE_DOMAIN_INFO                         NULL
/* 调度域信息的结构体个数 */
#define OS_SCHEDULE_DOMAIN_INFO_NUM                     0

/* ***************************** 配置TSKMON模块  ************************* */
/* TSKMON模块裁剪开关 */
#define OS_INCLUDE_TSKMON                               NO
/* TSKMON动态配置项 */
#define OS_CONFIG_TSKMON                                OS_INCLUDE_TSKMON
/* 任务撑死检测裁剪开关 */
#define OS_TSKMON_OVERRUN                               NO
/* 任务撑死阀值时间，单位tick */
#define OS_TSKMON_OVERRUN_THRESHOLD                     0
/* 任务死循环处理方式 */
#define OS_TSKMON_DOMODE                                OS_TSKMON_MODE_REAVE
/* 任务饿死检测裁剪开关 */
#define OS_TSKMON_STARVATION                            NO
/* 任务饿死阀值时间，单位tick */
#define OS_TSKMON_STARVATION_THRESHOLD                  0

/* ***************************** 配置任务栈越界检测模块 ******************** */
/* 任务栈越界检测裁剪开关 */
#define OS_INCLUDE_TSKSTACKMON                          NO
/* 任务栈越界检测动态配置项 */
#define OS_CONFIG_TSKSTACKMON                           OS_INCLUDE_TSKSTACKMON

/* ***************************** 配置异常管理模块 ************************** */
/* 异常信息记录后，复位前调用 */
#define OS_EXC_PROC_HOOK                                SRE_ExcProc
/* DoubleException堆栈起始地址(double异常、NMI、Dbg异常共用) , 仅Xtensa平台支持 */
#define OS_DOUBLE_EXC_STACK_START                       NULL
/* DoubleException堆栈大小，(double异常、NMI、Dbg异常共用)，每个核不小于0xc00， 仅Xtensa平台支持 */
#define OS_DOUBLE_EXC_STACK_SIZE                        0

/* ***************************** 配置CPU占用率及CPU告警模块 **************** */
/* CPU占用率模块裁剪开关 */
#define OS_INCLUDE_CPUP                                 YES
/* CPU采样样本的最大个数，仅对系统级CPU占用率有效，正常取值范围(0,30]，若大于30，则取值30 */
#define OS_CPUP_SAMPLE_RECORD_NUM                       0
/*
 * 采样时间间隔(单位tick)，配置为系统级时，为采样周期；
 * 配置为线程级时，若其值大于0，则作为采样周期，否则两次调用SRE_CpupNow或SRE_CpupThread间隔作为周期
 */
#define OS_CPUP_SAMPLE_INTERVAL                         10
/* CPU占用率告警动态配置项 */
#define OS_CONFIG_CPUP_WARN                             NO
/* CPU占用率告警阈值(精度为万分比) */
#define OS_CPUP_SHORT_WARN                              8500
/* CPU占用率告警恢复阈值(精度为万分比) */
#define OS_CPUP_SHORT_RESUME                            7500

/* ***************************** 配置内存管理模块 ************************** */
/*
 * 用户可以创建的最大分区数，取值范围[0,253]，
 * SD6183/SD6182/SD6157的实例内所有镜像需配置一致
 */
#define OS_MEM_MAX_PT_NUM                               10
/* 实例间共享分区最大分区号，只主核有效 */
#define OS_MI_SHARE_MEM_MAX_PT_NUM                      100
/* 支持共享动态FSC内存算法 */
#define OS_MEM_ALGORITHM_MCDYNFSC                       NO
/* 支持私有Pool内存算法 */
#define OS_MEM_ALGORITHM_POOL                           NO
/* 支持共享Pool内存算法 */
#define OS_MEM_ALGORITHM_MCPOOL                         NO
/* 支持Block内存算法 */
#define OS_MEM_ALGORITHM_BLOCK                          NO
/* 支持SLUB+FSC内存算法 */
#define OS_MEM_ALGORITHM_SLUB_FSC                       NO
/* 支持SSC内存算法 */
#define OS_MEM_ALGORITHM_SSC                            NO
/* 支持共享MAP-POOL内存算法 */
#define OS_MEM_ALGORITHM_MCMAPPOOL                      NO
/* 支持基于SSC的头体分离算法，DSSC内存算法 */
#define OS_MEM_ALGORITHM_DSSC                           NO

/* 私有FSC内存分区大小 */
#define OS_MEM_FSC_PT_SIZE                              0x80000

extern U8 g_memRegion00[OS_MEM_FSC_PT_SIZE];
/* 注意：使用SRE_MNG 管理，如下4个参数从核配置无效，由SRE自动传参 */
/* 私有FSC内存分区起始地址，用户保证随机化前的起始地址cacheline对齐 */
#define OS_MEM_FSC_PT_ADDR                              ((uintptr_t)&g_memRegion00[0])

/* 共享动态FSC内存分区起始地址 */
#define OS_MEM_MCDYN_FSC_PT_ADDR                        NULL
/* 共享动态FSC内存分区大小 */
#define OS_MEM_MCDYN_FSC_PT_SIZE                        0

/* 仅对Cortex-RX生效 */
#define OS_MEM_UNCACHE_STATIC                           NO
/* 不可cache内存分区的起始地址 */
#define OS_MEM_UNCACHE_PT_ADDR                          0
/* 不可cache内存分区的大小 */
#define OS_MEM_UNCACHE_PT_SIZE                          0

/* ***************************** 配置信号量管理模块 ************************* */
/* 信号量模块裁剪开关 */
#define OS_INCLUDE_SEM                                  YES
/* 信号量模块动态配置项 */
#define OS_CONFIG_SEM                                   OS_INCLUDE_SEM
/* 最大支持的信号量数 */
#define OS_SEM_MAX_SUPPORT_NUM                          100

/* ***************************** 配置核间快速信号量 ************************** */
/* 核间快速信号模块裁剪开关 */
#define OS_INCLUDE_MCFSEM                               NO
/* 核间快速信号模块动态配置项 */
#define OS_CONFIG_MCFSEM                                OS_INCLUDE_MCFSEM

/* ***************************** 配置消息管理模块 **************************** */
/* 消息模块裁剪开关 */
#define OS_INCLUDE_MSG                                  NO
/* 核间消息模块裁剪开关 */
#define OS_INCLUDE_MCMSG                                NO
/* 最大支持的消息结点个数 */
#define OS_MSG_MAX_SUPPORT_NUM                          0
/* 最大核间消息结点数 */
#define OS_MSG_MAX_MULTICORE_NUM                        0

/* **************************** 可订阅事件管理模块 *************************** */
/* 可订阅事件管理模块开关 */
#define OS_INCLUDE_BOOK_EVENT                           NO
/* 可订阅事件的个数，最多支持32个 */
#define OS_EVENT_MAX_BOOK_EVENT_NUM                     0
/* 一个可订阅事件可订阅的线程个数 */
#define OS_EVENT_MAX_THREADS_EACH_BOOK_EVENT            0

/* ***************************** 配置多核通信模块 **************************** */
/* 核间同步功能剪裁开关 */
#define OS_INCLUDE_SYNC                                 NO

/* ***************************** 配置ECC模块 ********************************* */
/* ECC模块裁剪开关，不支持Cortex-RX/SD6183，其中6183不依赖该开关，默认支持 */
#define OS_INCLUDE_ECC                                  NO

/* ***************************** 配置SHELL模块 ******************************* */
/* hshell模块裁剪开关 */
#define OS_INCLUDE_SHELL                                YES
/* hshell线程优先级 */
#define OS_SHTSK_PRIORITY                               0
/* hshell线程栈大小，该值为0时，系统默认设置0x1000，用户可通过根据需要适当修改OS_SHAGT_STACKSIZE值 */
#define OS_SHTSK_STACKSIZE                              0

/* ***************************** 配置系统TRACE模块 **************************** */
/* 系统TRACE模块剪裁开关，若配置为YES且使能了硬中断/软中断/任务的系统TRACE，将会降低系统调度的性能 */
#define OS_INCLUDE_TRACE                                NO
#define OS_CONFIG_TRACE                                 OS_INCLUDE_TRACE
/* 注意：使用SRE_MNG 管理，如下2个参数从核配置无效，由SRE自动传参 */
/* 系统TRACE空间起始地址, 按16字节对齐 */
#define OS_TRACE_AREA_ADDR                              NULL
/* 系统TRACE空间大小,含控制头(16字节),扣除控制头大小应为2的N次幂且不小于16 */
#define OS_TRACE_AREA_SIZE                              0x410

/* ***************************** 配置驱动模块 ******************************** */
/* 主要用于TRAX核TRACE功能，DMA，SRIO,ETH等设备初始化 */
#define OS_INCLUDE_DEVICE                               NO

/* ***************************** Loader & Patch模块 ******************************* */
/* 动态加载、动态补丁模块剪裁开关 */
#define OS_INCLUDE_LP                                   NO
/* LP模块分区号,范围:[2,MAX) */
#define OS_LP_SHARE_NC_PT_NO                            0x0
/* 注意：使用SRE_MNG 管理，如下2个参数从核配置无效，由SRE自动传参 */
/* LP模块分区起始地址,SD6183/SD6182/SD6181/SD6108/SD6157需配置实例内全部核都能访问的不可CACHE共享内存 */
#define OS_LP_SHARE_NC_PT_ADDR                          NULL
/* LP模块分区长度 */
#define OS_LP_SHARE_NC_PT_SIZE                          0x0

/* 实例内最大管理信息组个数，实例内所有核配置相同，SD6182/SD6181/SD6108/SD6157:[1,254], SD6183无效 */
#define OS_LP_MAX_GROUP_NUM                             0x0
/* 实例内可使用的最大单元个数(静态补丁个数 + 动态补丁个数 + 动态加载库个数) */
#define OS_LP_MAX_UNIT_NUM                              0x0

/* ***************************** 静态补丁模块 ******************************* */
/* 静态补丁模块剪裁开关 ，仅SD6155支持 */
#define OS_INCLUDE_PATCH                                NO

/* ***************************** 低功耗裁剪开关 ***************************** */
/* 低功耗裁剪开关。若配置打开，会替换CPU占用率统计算法，支持SD6182x */
#define OS_INCLUDE_CORESLEEP                            NO

/* ***************************** 配置队列模块 ******************************* */
/* 队列模块裁剪开关 */
#define OS_INCLUDE_QUEUE                                YES
/* 最大支持的队列数,范围(0,0xFFFF] */
#define OS_QUEUE_MAX_SUPPORT_NUM                        10

/* ************************* 钩子模块配置 *********************************** */
/* 硬中断进入钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_HWI_ENTRY_NUM                           5
/* 硬中断退出钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_HWI_EXIT_NUM                            5
/* 软中断进入钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_SWI_ENTRY_NUM                           1
/* 软中断退出钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_SWI_EXIT_NUM                            1
/* 任务创建钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_TSK_CREATE_NUM                          1
/* 任务删除钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_TSK_DELETE_NUM                          1
/* 任务切换钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_TSK_SWITCH_NUM                          8
/* IDLE钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_IDLE_NUM                                2
/* 临终遗言钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_LAST_WORDS_NUM                          8
/* 动态PG进入钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_DYNPWRGT_ENTRY_NUM                      2
/* 动态PG退出钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_DYNPWRGT_EXIT_NUM                       2
/* 纤程进入钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_FIBER_ENTRY_NUM                         2
/* 纤程退出钩子最大支持个数, 范围[0, 255] */
#define OS_HOOK_FIBER_EXIT_NUM                          2

/* ************************* 配置有名空间模块 ******************** */
/* 0号有名空间COMM部分首地址，需为共享UNCACHE空间，虚拟空间首地址且需要4K字节对齐，仅SD6183x支持 */
#define OS_NSM_SPACE_ADDR                               NULL
/* 0号有名空间COMM部分物理空间首地址，需为共享UNCACHE空间，4K字节对齐，仅SD6183x支持 */
#define OS_NSM_SPACE_PADDR                              NULL
/* 0号有名空间COMM部分大小，要4K字节对齐，仅SD6183x支持 */
#define OS_NSM_SPACE_SIZE                               0

/* ***************************** 符号表管理模块 ***************************** */
/* 符号表裁剪开关(支持SD6182x/Cortex-RX) */
#define OS_INCLUDE_SYMBOL                               YES
/* 符号表内存分区号 */
#define OS_SYMBOL_TABLE_PT_NO                           0
/* 注意：使用SRE_MNG 管理，如下2个参数从核配置无效，由SRE自动传参 */
/* 符号表分区起始地址 */
#define OS_SYMBOL_TABLE_NC_PT_ADDR                      NULL
/* 符号表分区大小 */
#define OS_SYMBOL_TABLE_NC_PT_SIZE                      0

/* ************************* SRE_MNG是否支持模拟加载 ************************ */
/* SRE_MNG模拟加载开关，打开后基础bin动态库需要通过模拟加载工具加载 */
#define OS_SRE_MNG_LOAD_SIMULATION                      NO

/* ************************* MMU配置 ************************ */
/* MMU 映射信息开关，当前仅对Cortex-AX生效 */
#define OS_INCLUDE_MMU_MAP_INFO                         NO
/* MMU映射信息首地址，只对hitsp & Cortex-AX有效 */
#define OS_MMU_MAP_INFO_ADDR                            0x0
/* MMU映射结构数组元素个数，目前仅对Cortex-AX生效 */
#define OS_MMU_MAP_INFO_NUM                             0

/* ************************* EXCLUSIVE 配置项 ************************ */
/* 是否需要配置EXCLUSIV REGION */
#define OS_INCLUDE_EXCLUSIVE_REGION                     NO
/* EXCLUSIV REGION起始地址 */
#define OS_EXCLUSIVE_REGION_START                       0x0
/* EXCLUSIV REGION大小 */
#define OS_EXCLUSIVE_REGION_SIZE                        0

/* ***************************** 用户中断个数配置 ************************** */
/* 用户中断最大个数配置 */
#define OS_HWI_MAX_NUM_CONFIG                           150

/* ************************* GIC模块配置 *********************************** */
/* GIC地址可配置开关 */
#define OS_INCLUDE_GIC_BASE_ADDR_CONFIG                 YES
/* GIC基地址配置 */
#define OS_GIC_BASE_ADDR                                0x24000000U
/* GICR相对于GIC基地址偏移量配置 */
#define OS_GICR_OFFSET                                  0x40000U
/* GICR核间偏移量配置 */
#define OS_GICR_STRIDE                                  0x20000U

extern unsigned long long SRE_GetCpuCycle(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_CONFIG_H */
