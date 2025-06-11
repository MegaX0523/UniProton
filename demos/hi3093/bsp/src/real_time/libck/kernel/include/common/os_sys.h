/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 系统模块的基础头文件，用户不能直接包含
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_SYS_H
#define OS_SYS_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：指针参数为空。
 *
 * 值: 0x02000001
 *
 * 解决方案: 请检查入参是否为空
 */
#define OS_ERRNO_SYS_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x01)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：系统主频配置非法。
 *
 * 值: 0x02000002
 *
 * 解决方案: 在sre_config.h中配置合理的系统主频。
 */
#define OS_ERRNO_SYS_CLOCK_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x02)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：用户的配置选项OS_SYS_CORE_MAX_NUM有误，
 * 应该和芯片匹配，且OS_SYS_CORE_MAX_NUM值不能超过该芯片支持的最大核数。
 *
 * 值: 0x02000003
 *
 * 解决方案:修改配置选项OS_SYS_CORE_MAX_NUM，和芯片匹配。
 *
 */
#define OS_ERRNO_SYS_MAXNUMOFCORES_IS_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x03)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：配置的运行核数非法。
 *
 * 值: 0x02000004
 *
 * 解决方案:配置的运行核数必须小于配置的芯片最大核数，且不能为0.
 *
 */
#define OS_ERRNO_SYS_CORE_RUNNUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x04)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：获取系统时间函数Cortex-AX平台配置为NULL。
 *
 * 值: 0x02000006
 *
 * 解决方案:配置获取系统时间函数时，Cortex-AX平台必须配置为非NULL。
 *
 */
#define OS_ERRNO_SYS_TIME_HOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x06)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码:镜像的个数配置错误。
 *
 * 值: 0x02000007
 *
 * 解决方案: 查看每个镜像配置的镜像个数是否不相等或者镜像个数为0或者大于最大镜像个数。
 *
 */
#define OS_ERRNO_SYS_IMAGE_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x07)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码:镜像的镜像ID配置不可用。
 *
 * 值: 0x02000008
 *
 * 解决方案: 查看每个镜像配置的镜像ID是否有重复或者配置值错误。镜像ID值不能大于等于镜像个数。
 *
 */
#define OS_ERRNO_SYS_IMAGE_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x08)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：用户的配置选项OS_SYS_CORE_MAX_NUM有误，
 * 应该和芯片匹配，且OS_SYS_CORE_MAX_NUM值不能超过该芯片支持的最大核数。
 *
 * 值: 0x02000009
 *
 * 解决方案:修改配置选项OS_SYS_CORE_MAX_NUM，和芯片匹配。
 *
 */
#define OS_ERRNO_SYS_CORERUNNUM_IS_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x09)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：依赖的模块功能宏没有打开。
 *
 * 值: 0x0200000a
 *
 * 解决方案:请打开相应的模块的功能宏。
 *
 */
#define OS_ERRNO_SYS_MODE_CLOSE OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x0a)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：配置的系统栈过小。
 *
 * 值: 0x0200000b
 *
 * 解决方案:请增大配置的系统栈大小。
 *
 */
#define OS_ERRNO_SYS_STACK_SIZE_NOT_ENOUGH OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x0b)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：tskmon模块功能宏依赖的任务模块功能未打开。
 *
 * 值: 0x0200000c
 *
 * 解决方案:检查依赖的任务模块裁剪开关是否打开。
 *
 */
#define OS_ERRNO_SYS_TASK_CLOSE OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x0c)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：任务饿死撑死功能控依赖的功能tskmon模块未打开。
 *
 * 值: 0x0200000d
 *
 * 解决方案:检查依赖的tskmon模块裁剪开关是否打开。
 *
 */
#define OS_ERRNO_SYS_TASKMON_CLOSE OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x0d)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：任务监功能控依赖的功能TICK未打开。
 *
 * 值: 0x0200000e
 *
 * 解决方案:检查依赖的TICK裁剪开关是否打开。
 *
 */
#define OS_ERRNO_SYS_TICK_CLOSE OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x0e)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：配置的系统模式不正确。
 *
 * 值: 0x0200000f
 *
 * 解决方案:系统模式必须为super模式或者user模式。
 *
 */
#define OS_ERRNO_SYS_MODE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x0f)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：CPUP告警被裁减
 *
 * 值: 0x02000011
 *
 * 解决方案:排除config项是否正确，该平台不支持CPUP告警
 *
 */
#define OS_ERRNO_SYS_NO_CPUP_WARN OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x11)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：获取或清除对应核初始化阶段状态时，核号错误。
 *
 * 值: 0x02000013
 *
 * 解决方案:确保入参核号不能大于等于OS_MAX_CORE_NUM。
 *
 */
#define OS_ERRNO_SYS_COREID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x13)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：软中断模块已经被裁减。
 *
 * 值: 0x0200001c
 *
 * 解决方案:排除config项是否正确，该平台不支持软中断，必须将配置项OS_TASK_INCLUDE配置为YES。
 *
 */
#define OS_ERRNO_SYS_NO_SWI OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x1c)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：init trace模块名字空间对象申请失败。
 *
 * 值: 0x0200001d
 *
 * 解决方案:扩大名字空间大小。
 *
 */
#define OS_ERRNO_SYS_INIT_TRACE_NAME_ALLOC_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x1d)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：获取系统栈信息时指定的出参结构体为空。
 *
 * 值: 0x02000023
 *
 * 解决方案: 检查获取系统栈时传入的结构体是否为空。
 */
#define OS_ERRNO_SYS_STACKINFO_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x23)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：获取当前系统实例ID时的保存参数指针为空。
 *
 * 值: 0x02000024
 *
 * 解决方案: 请确保传入的用户存放实例ID的地址不为空。
 */
#define OS_ERRNO_SYS_INST_ID_ADDR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x24)

/*
 * @ingroup OS_sys
 * init trace的地址为NULL
 *
 * 值: 0x02000025
 *
 * 解决方案:可能是使用过早，建议在触发后使用。
 *
 */
#define OS_ERRNO_SYS_TRACE_ADDR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x25)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：设置RND值的时候入参非法。
 *
 * 值: 0x02000026
 *
 * 解决方案: 请确保设置RND值时入参合法。
 */
#define OS_ERRNO_SYS_RND_PARA_INVALID  OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x26)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：注册获取系统时间函数重复注册了
 *
 * 值: 0x02000027
 *
 * 解决方案:获取系统时间函数不允许重复注册
 *
 */
#define OS_ERRNO_SYS_TIME_HOOK_REGISTER_REPEATED OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x27)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：内存分区过小。
 *
 * 值: 0x02000028
 *
 * 解决方案: 请确保分区待截取的size小于分区大小。
 */
#define OS_ERRNO_SYS_RND_OVERFLOW_PTSIZE  OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x28)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：自检小版本Idle激活失败。
 *
 * 值: 0x02000029
 *
 * 解决方案: 请确保Idle钩子注册。
 */
#define OS_ERRNO_SYS_IDLE_ACTIVE_FAILED  OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x29)

/*
 * @ingroup OS_sys
 * 系统基本功能错误码：配置的中断个数不正确
 *
 * 值: 0x0200002a
 *
 * 解决方案: 检查配置的中断个数是否为0或者超过了硬件范围
 */
#define OS_ERRNO_SYS_HWI_MAX_NUM_CONFIG_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYS, 0x2a)

/*
 * 系统初始化阶段状态
 */
/*
 * @ingroup OS_sys
 * 表示初始态。
 *
 */
#define OS_DEFAULT_PHASE 0x00

/*
 * @ingroup OS_sys
 * 表示主核处于实模式启动阶段状态（该阶段主要完成MMU使能初始化），仅SD6183平台有效。
 *
 */
#define OS_MCORE_PHY_BOOT_PHASE 0x01

/*
 * @ingroup OS_sys
 * 表示主核处于虚模式启动阶段状态，仅SD6183平台有效。
 *
 */
#define OS_MCORE_VIR_BOOT_PHASE 0x02

/*
 * @ingroup OS_sys
 * 表示进入SRE_HardBootInit。
 *
 */
#define OS_BOOT_PHASE 0x03

/*
 * @ingroup OS_sys
 * 表示开始多实例启动参数配置。
 *
 */
#define OS_STARTPARAMCFG_PHASE 0x04

/*
 * @ingroup OS_sys
 * 表示退出SRE_HardBootInit。
 *
 */
#define OS_AFTER_BOOT_PHASE 0x05

/*
 * @ingroup OS_sys
 * 表示进入BSS段初始化。
 *
 */
#define OS_BSSINIT_PHASE 0x06

/*
 * @ingroup OS_sys
 * 表示Dcache初始化完毕，SD6183平台由于在reset中已经完成，故不支持该阶段状态。
 *
 */
#define OS_DCACHEINIT_PHASE 0x07

/*
 * @ingroup OS_sys
 * 表示进入C lib库初始化。
 *
 */
#define OS_LIBCINIT_PHASE 0x08

/*
 * @ingroup OS_sys
 * 表示系统在进行OS模块注册阶段，匹配MOUDLE_ID之后，标记进入MODULE_ID的注册。
 *
 */
#define OS_REGISTER_PHASE 0x09

/*
 * @ingroup OS_sys
 * 表示系统在进行OS模块初始化阶段，匹配MOUDLE_ID之后，标记进入MODULE_ID的初始化。
 *
 */
#define OS_INITIALIZE_PHASE 0x0a

/*
 * @ingroup OS_sys
 * 表示系统在进行产品驱动初始化阶段，匹配MOUDLE_ID之后，标记进入MODULE_ID的初始化。
 *
 */
#define OS_DEVDRVINIT_PHASE 0x0b

/*
 * @ingroup OS_sys
 * 表示系统在进行OS启动阶段，匹配MOUDLE_ID之后，标记进入MODULE_ID的启动。
 *
 */
#define OS_START_PHASE 0x0c

/*
 * @ingroup OS_sys
 * 系统模块线程类型获取枚举结构定义
 *
 * 系统模块线程类型获取
 */
enum OsThreadType {
    SYS_HWI,      /* 当前线程类型属于硬中断 */
    SYS_SWI,      /* 当前线程类型属于软中断 */
    SYS_TASK,     /* 当前线程类型属于任务 */
    SYS_KERNEL,   /* 当前线程类型属于内核 */
#if defined(OS_OPTION_CRE)
    SYS_CRE,      /* 当前线程类型属于协程 */
#endif
#if defined(OS_OPTION_FIBER)
    SYS_FIBER,    /* 当前线程类型属于纤程 */
#endif
    SYS_BUTT
};

/*
 * @ingroup OS_sys
 * 系统栈信息结构体定义。
 */
struct SysStackInfo {
    /* 系统栈起始地址 */
    uintptr_t sysStackStart;
    /* 系统栈结束地址 */
    uintptr_t sysStackEnd;
#if defined(OS_OPTION_SYS_STACK_PEAK_QUERY)
    /* 系统栈使用的历史峰值 */
    U32 peakUsed;
#endif
};

/*
 * @ingroup OS_sys
 * 初始化的阶段
 */
enum OsInitPhase {
    PHASE_IN_PRE_STARTUP = 0, /* 当前OS在预启动阶段,状态未赋值前 */
    PHASE_IN_MAIN_ENTRY,      /* 当前OS在进入MAIN的阶段 */
    PHASE_IN_EXC_INIT_END,    /* 当前OS异常可全量接管阶段 */
    PHASE_IN_OS_START,        /* 当前OS完成所有初始化，准备切入任务 */
    PHASE_IN_SYS_REBOOT,      /* 当前OS系统异常，资源回收完成，等待复位 */
    PHASE_IN_BUTT             /* 非法阶段 */
};

/*
 * @ingroup os_sys
 * @brief 系统重启钩子函数的类型定义。
 *
 * @par 描述
 * 用户通过系统重启钩子函数的类型定义系统重启钩子，在系统重启之前调用该钩子。
 *
 * @attention 无。
 *
 * @param 无。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>os_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef void (*SysRebootFunc)(void);

/*
 * @ingroup os_sys
 * @brief IDLE循环钩子的类型定义。
 *
 * @par 描述
 * 用户通过IDLE循环钩子的函数类型定义函数，系统在进入IDLE循环之前调用该钩子。
 *
 * @attention 无。
 *
 * @param 无。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>os_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef void (*SreIdleHook)(void);

/*
 * @ingroup os_sys
 * @brief 计算系统绝对时间钩子函数的类型定义。
 *
 * @par 描述
 * 由于在balong平台OS没有接管硬件定时器，需要用户提供计算系统时间的钩子函数。
 *
 * @attention 获取的是64bit cycles 数据。
 *
 * @param 无。
 *
 * @retval 系统绝对时间
 * @par 依赖
 * <ul><li>os_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef U64 (*SysTimeFunc)(void);

/*
 * @ingroup  os_sys
 * @brief 任务调度时对端核处于低功耗时唤醒对端核的回调函数。
 *
 * @par 描述
 * 用户根据唤醒回调函数原型定义回调函数，当指定核在低功耗时，调用该钩子进行唤醒。
 * @attention 无。
 *
 * @param coreId [IN]  类型#U32，需要唤醒的对端核号。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>os_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef U32 (*CoreWakeUpHook)(U32 coreId);

/*
 * @ingroup SRE_sys
 * 系统模块配置信息的结构体定义。
 *
 * 保存系统模块的配置项信息。
 */
struct SysModInfo {
    /* CPU主频，时钟周期 */
    U32 systemClock;
    /* 当前核ID */
    U8 coreID;
    /* 实际运行的核数 */
    U8 coreRunNum;
    /* 最大支持的核数 */
    U8 coreMaxNum;
    /* 主核ID */
    U8 corePrimary;
    /* 用户注册的单板复位函数 */
    SysRebootFunc rebootHook;
    /* 用户注册的IDLE钩子函数 */
    SreIdleHook idleHook;
    /* 用户注册的获取系统时间函数 */
    SysTimeFunc sysTimeHook;
    /* CPU type */
    U8 cpuType;
    /* 模拟加载工具开关标致1:工具加载 */
    U8 loadSimFlag;
    /* 用户注册最大IDLE任务钩子数 */
    U16 idleHookMaxNum;
    /* 最大镜像个数，所有镜像要一致 */
    U16 imageNum;
    /* 当前镜像ID */
    U16 imageId;
#if defined(OS_OPTION_HWI_MAX_NUM_CONFIG)
    U32 hwiMaxNum;
#endif
};

enum OsRndNumType {
    OS_RND_MEM_DEFAULT_PT,    /* 终端：默认内存分区的RND值；无线：系统堆栈随机化的RND值，sd6183x支持 */
    OS_RND_MEM_PROTECT,       /* 终端：内存保护的RND值；无线：不可用 */
    OS_RND_STACK_PROTECT,     /* 栈保护的RND值，sd6182x, M4,  ARM7,  ARM9,  Hi3911V200,  Cortex-AX,  Cortex-RX支持 */
    OS_RND_BUTT               /* 非法类型 */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_SYS_H */
