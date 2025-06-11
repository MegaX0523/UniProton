/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 系统模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * @defgroup SRE_sys 系统基本功能接口
 * @ingroup SRE_kernel
 */

#ifndef SRE_SYS_H
#define SRE_SYS_H

#include "sre_base.h"
#include "sre_hook.h"
#include "./common/os_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_sys
 * 系统核数
 */
#define OS_CORES_32 32
#define OS_CORES_64 64
#define OS_CORES_128 128

/*
 * @ingroup OS_sys
 * 每秒毫秒数
 */
#define OS_SYS_MS_PER_SECOND 1000

/*
 * @ingroup OS_sys
 * 每秒微秒数
 */
#define OS_SYS_US_PER_SECOND 1000000

/*
 * @ingroup OS_sys
 * 每秒纳秒数
 */
#define OS_SYS_NS_PER_SECOND 1000000000

#if defined(OS_OPTION_INIT_PHASE_QUERY)
/*
 * @ingroup  SRE_sys
 * @brief 实例间初始化阶段获取
 *
 * @par 描述
 * 根据输入的核号获取对应核的初始化阶段
 *
 * @attention
 * <ul>
 * <li>支持SD6182x平台。</li>
 * <li>用户必须在进入main之后调用该函数。</li>
 * </ul>
 *
 * @param coreID [IN]  类型#U32，核号。
 * @param phase  [OUT] 类型#enum OsInitPhase *，保存指定核的初始化阶段状态值。
 *
 * @retval #OS_ERRNO_SYS_COREID_INVALID          0x02000013，核号错误。
 * @retval #OS_ERRNO_SYS_PTR_NULL                0x02000001，指针为空。
 * @retval #SRE_OK                               0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_InitPhaseClr
 */
extern U32 SRE_InitPhaseGet(U32 coreID, enum OsInitPhase *phase);

/*
 * @ingroup  SRE_sys
 * @brief 实例间初始化阶段清除
 *
 * @par 描述
 * 根据输入的核号，清除对应核的阶段。
 *
 * @attention
 * <ul>
 * <li>支持SD6182x平台。</li>
 * <li>用户必须在进入main之后调用该函数。</li>
 * <li>用户须在复位从核前调用此接口，清除对应核的初始化阶段。</li>
 * <li>调用该接口后指定核的初始化阶段状态为PHASE_IN_PRE_STARTUP。</li>
 * </ul>
 *
 * @param coreID [IN]  类型#U32，核号。
 *
 * @retval #OS_ERRNO_SYS_COREID_INVALID          0x02000013，核号错误。
 * @retval #SRE_OK                               0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_InitPhaseGet
 */
extern U32 SRE_InitPhaseClr(U32 coreID);
#endif

/*
 * @ingroup SRE_sys
 * 每秒毫秒数
 */
#define SRE_SYS_MS_PER_SECOND OS_SYS_MS_PER_SECOND  // 1000

/*
 * @ingroup SRE_sys
 * 每秒微秒数
 */
#define SRE_SYS_US_PER_SECOND OS_SYS_US_PER_SECOND  // 1000000

/*
 * @ingroup SRE_sys
 * 每秒微秒数
 */
#define SRE_SYS_NS_PER_SECOND OS_SYS_NS_PER_SECOND  // 1000000000

/*
 * @ingroup SRE_sys
 * OS版本号
 */
#define OS_SYS_OS_VER_LEN 48

/*
 * @ingroup SRE_sys
 * 产品版本号
 */
#define OS_SYS_APP_VER_LEN 64

/*
 * @ingroup  SRE_sys
 * @brief 转换cycle为毫秒。
 *
 * @par 描述
 * 转换cycle为毫秒。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param cycle [IN]  类型#U64，cycle数。
 *
 * @retval [0,0xFFFFFFFFFFFFFFFF] 转换后的毫秒数。
 * @par 依赖
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_Cycle2US
 */
extern U64 SRE_Cycle2MS(U64 cycle);

/*
 * @ingroup  SRE_sys
 * @brief 转换cycle为微秒。
 *
 * @par 描述
 * 转换cycle为微秒。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param cycle [IN]  类型#U64，cycle数。
 *
 * @retval [0,0xFFFFFFFFFFFFFFFF] 转换后的微秒数。
 * @par 依赖
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_Cycle2MS
 */
extern U64 SRE_Cycle2US(U64 cycle);

/*
 * @ingroup  SRE_sys
 * @brief 获取本核的tick计数私有地址。
 *
 * @par 描述
 * 获取本核的tick计数私有地址。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param 无。
 * @retval 本核的tick计数私有地址
 * @par 依赖
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern uintptr_t SRE_GetTickAddr(void);

/*
 * @ingroup  SRE_sys
 * @brief 获取当前的tick计数。
 *
 * @par 描述
 * 获取当前的tick计数。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param 无。
 *
 * @retval  [0,0xFFFFFFFFFFFFFFFF] 当前的tick计数。
 * @par 依赖
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CycleCountGet
 */
extern U64 SRE_TickCountGet(void);

/*
 * @ingroup SRE_sys
 * @brief 获取每个tick对应的硬件cycle数。
 *
 * @par 描述
 * 获取每个tick对应的硬件cycle数。
 *
 * @attention
 * <ul>
 * <li>tick模块被裁剪时该接口返回值不准确。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval [0,0xFFFFFFFF] 每个tick对应的硬件cycle数。
 * @par 依赖
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_CyclePerTickGet(void);

/*
 * @ingroup SRE_sys
 * @brief 延迟时间(单位cycle)。
 *
 * @par 描述
 * 延迟时间(单位cycle)。
 *
 * @attention
 * <ul>
 * <li>对于Cortex-RX平台，该功能依赖于sre_config.h中的配置项OS_SYS_TIME_HOOK配置的获取系统时间函数功能。</li>
 * <li>对于Cortex-RX平台，单位cycle为获取系统时间使用的定时器一个step。</li>
 * </ul>
 *
 * @param cycles [IN]  类型#U32，延迟cycle数
 *
 * @retval 无
 * @par 依赖
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern void SRE_DelayCycle(U32 cycles);

/*
 * @ingroup SRE_sys
 * @brief 延迟时间(单位微秒)。
 *
 * @par 描述
 * 延迟时间(单位微秒)。
 *
 * @attention
 * <ul>
 * <li>对于Cortex-RX平台，该功能依赖于sre_config.h中的配置项OS_SYS_TIME_HOOK配置的获取系统时间函数功能。</li>
 * <li>对于Cortex-RX平台，需要保证钩子函数使用的定时器主频与配置项OS_SYS_CLOCK一致。</li>
 * </ul>
 *
 * @param delay [IN]  类型#U32，延迟微秒数。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_DelayMs
 */
extern void SRE_DelayUs(U32 delay);

/*
 * @ingroup SRE_sys
 * @brief 延迟时间(单位毫秒)。
 *
 * @par 描述
 * 延迟时间(单位毫秒)。
 *
 * @attention
 * <ul>
 * <li>对于Cortex-RX平台，该功能依赖于sre_config.h中的配置项OS_SYS_TIME_HOOK配置的获取系统时间函数功能。</li>
 * <li>对于Cortex-RX平台，需要保证钩子函数使用的定时器主频与配置项OS_SYS_CLOCK一致。</li>
 * </ul>
 *
 * @param delay [IN]  类型#U32，延迟毫秒数。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>SRE_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_DelayUs
 */
extern void SRE_DelayMs(U32 delay);

/* IIU_BEGIN+41: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
struct MemOutputCfg {
    // 以下各配置项按核配置，每个核可以配置不同
    // 系统栈地址
    uintptr_t sysStackAreaVirAddr;
    // 系统栈大小
    uintptr_t sysStackAreaSize;
    // 独立任务栈区间起始地址(无效)
    uintptr_t tskStackAreaVirAddr;
    // 独立任务栈区间大小(无效)
    uintptr_t tskStackAreaSize;
    // 私有FSC分区地址
    uintptr_t fscPtVirAddr;
    // 私有FSC分区大小
    uintptr_t fscPtSize;

    // Trace内存类型
    uintptr_t traceAreaVirAddr;
    // Trace内存大小
    uintptr_t traceAreaSize;

    // MMU物理地址到虚拟地址转换映射表大小(无效)
    uintptr_t mmuPhy2VirAreaSize;
    // MMU物理地址到虚拟地址转换映射表地址(无效)
    uintptr_t mmuPhy2VirAreaVirAddr;

    // 以下各配置项按实例配置，实例内每个核配置相同
    // 共享静态FSC分区地址
    uintptr_t mcFscPtVirAddr;
    // 共享静态FSC分区大小
    uintptr_t mcFscPtSize;
    // 补丁分区地址
    uintptr_t lpShareNcPtVirAddr;
    // 补丁分区大小
    uintptr_t lpShareNcPtSize;
    // 符号表内存分区地址
    uintptr_t symTblNcPtVirAddr;
    // 符号表内存分区大小，进程组内各个核配置相同
    uintptr_t symTblNcPtSize;
};

/*
 * @ingroup SRE_sys
 * @brief 复位单板。
 *
 * @par 描述
 * 复位单板，程序重新加载执行。
 *
 * @attention
 * <ul>
 * <li>并非直接复位单板，而是关中断，等待看门狗复位。</li>
 * <li>用户可以通过配置项OS_SYS_REBOOT_HOOK挂接复位函数。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern void SRE_Reboot(void);

/*
 * @ingroup SRE_sys
 * @brief 核休眠接口。
 *
 * @par 描述
 * 核休眠接口，等待芯片复位，实例复位或者是单硬线程复位。
 *
 * @attention
 * <ul>
 * <li>只支持SD6183/SD6185及以后的芯片</li>
 * <li>若用户需要进行单硬线程复位，那么需要在g_sysRebootHook复位钩子内先调用该接口等待单硬线程复位</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern void SRE_CoreWait(void);

/*
 * @ingroup SRE_sys
 * 系统初始化阶段信息解析结构体定义。
 *
 * 用于标记当前系统运行到哪个阶段,initPhase值 = OS_INIT_PHASE | MODULE_ID
 * OS_INIT_PHASE表示系统初始化阶段状态bit[31-16].
 * MODULE_ID表示当前系统初始化的模块号bit[0-15]，具体模块号在sre_module.h中有定义。
 * 记录系统模块是否完成初始化，0-63位用来记录已进行初始化的所有模块的模块号，
 * 若某个模块进行了初始化操作，将其模块号对应的位置为1。
 * initTrace[0] : 标示模块号为0~31，initTrace[1] : 标示模块号为32~63,
 * 以此类推，记录的是当前系统中各模块的初始化完成情况以及系统初始化 化阶段状态的实时值。
 */
struct SysTraceInfo {
    /* 初始化阶段供CDA解析信息的魔术字 */
    U32 initMagicWord;
    /*
     * 当前核初始化阶段状态标识用于标记当前系统运行到哪个阶段,initPhase = OS_INIT_PHASE | MODULE_ID。
     * OS_INIT_PHASE表示系统初始化阶段状态bit[31-16]，MODULE_ID表示当前系统初始化的模块号bit[0-15]。
     */
    U32 initPhase;

    /*
     * 当前核初始化的所有模块进行TRACE标识记录系统模块是否完成初始化，[0]bit0-31。[1]bit32~63] ......用来记录
     * 已进行初始化的所有模块的模块号，若某个模块进行了初始化操作，将其模块号对应位设置1。
     */
    U32 initTrace[(U32)(OS_MID_BUTT + 31) >> 5];

    /* 初始化阶段错误码返回值记录 */
    U32 errNo;
};

/*
 * @ingroup  SRE_sys
 * @brief 获取配置的主核ID。
 *
 * @par 描述
 * 获取配置的主核ID。
 *
 * @attention
 * <ul>
 * <li>获取的核ID为配置的主核ID号，与真实主核ID号可能不同。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 配置的物理核ID。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_GetNumberOfCores | SRE_GetCoreID
 */
extern U8 SRE_GetPrimaryCore(void);

/*
 * @ingroup  SRE_sys
 * @brief 获取实际运行的核数。
 *
 * @par 描述
 * 获取实际运行的核数。
 *
 * @attention
 * <ul>
 * <li>获取的为实际运行的核数，而不是OS最大支持的核数。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 核数。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_GetCoreID | SRE_GetPrimaryCore
 */
extern U8 SRE_GetNumberOfCores(void);

/*
 * @ingroup  SRE_sys
 * @brief 获取最大可支持运行的核数。
 *
 * @par 描述
 * 获取最大可支持运行的核数。
 *
 * @attention
 * <ul>
 * <li>获取的为OS最大支持的核数，而不是OS实际运行的核数。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 核数。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_GetCoreID | SRE_GetMaxNumberOfCores
 */
extern U8 SRE_GetMaxNumberOfCores(void);

/*
 * @ingroup  SRE_sys
 * @brief 获取系统的主频。
 *
 * @par 描述
 * 获取系统的主频。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param 无。
 *
 * @retval 系统主频
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_GetCoreID
 */
extern U32 SRE_GetSysClk(void);

/*
 * @ingroup  SRE_sys
 * @brief 获取OS的版本号。
 *
 * @par 描述
 * 获取OS的版本号。版本编号格式为libck 207.x.x VERSIONID(XXX)。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param 无。
 *
 * @retval 指向OS版本号的字符串指针。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SetAppVersion | SRE_GetAppVersion
 */
extern char *SRE_GetOsVersion(void);

/*
 * @ingroup  SRE_sys
 * @brief 注册应用程序的版本号。
 *
 * @par 描述
 * 注册应用程序的版本号。
 *
 * @attention
 * <ul>
 * <li>系统APP版本号保存在全局数组中，可以设置的APP版本号最大长度为64字节(包括结束字符)。</li>
 * </ul>
 *
 * @param appVersion [IN]  类型#const char *，指向应用程序版本号的字符串指针。
 *
 * @retval #OS_ERRNO_SYS_PTR_NULL           0x02000001，指针参数为空。
 * @retval #SRE_OK                          0x02000000，注册APP版本号成功。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_GetAppVersion | SRE_GetOsVersion
 */
extern U32 SRE_SetAppVersion(const char *appVersion);

/*
 * @ingroup  SRE_sys
 * @brief 获取应用程序的版本号。
 *
 * @par 描述
 * 获取应用程序的版本号。
 *
 * @attention
 * <ul><li>若之前未注册，则返回指向"Unknown"的指针。</li></ul>
 *
 * @param 无。
 *
 * @retval 指向应用程序版本号的字符串指针。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SetAppVersion | SRE_GetOsVersion
 */
extern char *SRE_GetAppVersion(void);

/*
 * @ingroup  SRE_sys
 * @brief 将cycle值转换成毫秒。
 *
 * @par 描述
 * 将cycle值转换成毫秒。
 *
 * @attention
 * <ul>
 * <li>当系统主频不为1000整数倍时，该接口获取到的ms数有误差。</li>
 * </ul>
 *
 * @param cpuTick [IN]  类型#struct SreCpuTick *，待转换的cycle计数地址。
 * @param msHi    [OUT] 类型#U32 *，保存毫秒的高32位的地址。
 * @param msLo    [OUT] 类型#U32 *，保存毫秒的低32位的地址。
 *
 * @retval #OS_ERRNO_SYS_PTR_NULL           0x02000001，指针参数为空。
 * @retval #SRE_OK                          0，操作成功。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpuTick2US
 */
extern U32 SRE_CpuTick2MS(struct SreCpuTick *cpuTick, U32 *msHi, U32 *msLo);

/*
 * @ingroup  SRE_sys
 * @brief 将cycle值转换成微秒。
 *
 * @par 描述
 * 将cycle值转换成微秒。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param cpuTick [IN]  类型#struct SreCpuTick *，待转换的cycle计数地址。
 * @param hi      [OUT] 类型#U32 *，保存微秒的高32位的地址。
 * @param lo      [OUT] 类型#U32 *，保存微秒的低32位的地址。
 *
 * @retval #OS_ERRNO_SYS_PTR_NULL           0x02000001，指针参数为空。
 * @retval #SRE_OK                          0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpuTick2MS
 */
extern U32 SRE_CpuTick2US(struct SreCpuTick *cpuTick, U32 *hi, U32 *lo);

/*
 * @ingroup  SRE_sys
 * @brief 获取系统当前线程类型。
 *
 * @par 描述
 * 获取系统当前线程类型。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param 无。
 *
 * @retval #OS_THREAD_TYPE_E                    线程类型。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern enum OsThreadType SRE_CurThreadType(void);

/*
 * @ingroup SRE_sys
 * @brief 业务给OS传递RND值，用作后续相关功能模块的保护。
 *
 * @par 描述
 * 业务传递RND值给OS，OS用于运行时必要的保护。
 *
 * @attention
 * <ul>
 * <li>终端：OS_RND_MEM_DEFAULT_PT约定的offset范围在4K以内。</li>
 * <li>栈保护随机值设置必须在SRE_HardBootInit中调用。</li>
 * </ul>
 *
 * @param type   [IN]  类型#enum OsRndNumType，设置的目标RND值的类型。
 * @param rndNum [IN]  类型#U32，传递的RND值。
 *
 * @retval #SRE_OK                          0x00000000，操作成功。
 * @retval #OS_ERRNO_SYS_RND_PARA_INVALID   0x02000026，设置RND值入参非法。
 *
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_SysRndNumSet(enum OsRndNumType type, U32 rndNum);

/*
 * @ingroup SRE_sys
 * @brief 业务调用该接口确认相应的RND功能是否已打开。
 *
 * @par 描述
 * 业务确认相应的RND功能是否已打开。
 *
 * @attention
 * <ul>
 * <li>该函数只提供给终端使用。</li>
 * </ul>
 *
 * @param type [IN]  类型#enum OsRndNumType，待确认的目标RND值的类型。
 *
 * @retval #TRUE     0x00000001，RND功能已打开。
 * @retval #FALSE    0x00000000，check的RND类型非法或者RND功能未打开。
 *
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_SysRndFeatureCheck(enum OsRndNumType type);

#if defined(OS_OPTION_SMP)
/*
 * @ingroup  SRE_sys
 * @brief 获取系统当前线程类型，无锁版。
 *
 * @par 描述
 * 获取系统当前线程类型，无锁版。
 *
 * @attention
 * <ul>
 * <li>调用该接口时请确保外部已关中断，该接口为提升获取线程类型的性能，内部实现无关中断。
 * </ul>
 *
 * @param 无。
 *
 * @retval #OS_THREAD_TYPE_E                    线程类型。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CurThreadType
 */
extern enum OsThreadType SRE_CurThreadTypeNoIntLock(void);
#endif

/*
 * @ingroup  SRE_sys
 * @brief 注册IDLE循环进入前调用的钩子。
 *
 * @par 描述
 * 为本核注册IDLE循环进入前调用的钩子，该钩子只会被调用一次。
 *
 * @attention
 * <ul>
 * <li>注册的钩子只在进入IDLE循环前执行一次。</li>
 * <li>若任务未裁剪，则作用的是任务IDLE循环；若任务被裁剪，则作用的是软中断IDLE循环。</li>
 * <li>IDLE任务钩子中使用矢量寄存器需要在前置钩子中调用#SRE_TaskCpSaveCfg接口设置矢量操作上下文保护区。</li>
 * <li>IDLE软中断钩子中使用矢量寄存器需要在前置钩子中调用#SRE_SwiCpSaveCfg接口设置矢量操作上下文保护区。</li>
 * </ul>
 *
 * @param hook [IN]  类型#SreIdleHook，钩子函数。
 *
 * @retval #OS_ERRNO_SYS_PTR_NULL           0x02000001，指针参数为空。
 * @retval #SRE_OK                          0x00000000，操作成功。
 *
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_IdleHookAdd
 */
extern U32 SRE_IdlePrefixHookAdd(SreIdleHook hook);

/*
 * @ingroup  SRE_sys
 * @brief 注册IDLE循环中调用的钩子
 *
 * @par 描述
 * 注册在IDLE任务或IDLE软中断循环中调用的钩子函数。
 *
 * @attention
 * <ul>
 * <li>钩子中不能调用引起任务阻塞或切换的函数。</li>
 * </ul>
 *
 * @param hook [OUT] 类型#SreIdleHook，IDLE钩子函数，该参数不能为空。
 *
 * @retval #OS_ERRNO_HOOK_TYPE_INVALID      0x02001600，HOOK类型错误。
 * @retval #OS_ERRNO_HOOK_PTR_NULL          0x02001602，HOOK指针空。
 * @retval #SRE_OK                          0x00000000，操作成功。
 *
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_IdleHookDel
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_IdleHookAdd(SreIdleHook hook)
{
    return SRE_HookAdd(OS_HOOK_IDLE_PERIOD, (OsVoidFunc)hook);
}

/*
 * @ingroup  SRE_sys
 * @brief 删除IDLE循环中调用的钩子
 *
 * @par 描述
 * 删除在IDLE任务或IDLE软中断循环中调用的钩子函数。
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param hook [OUT] 类型#SreIdleHook，IDLE钩子函数，该参数不能为空。
 *
 * @retval #OS_ERRNO_HOOK_TYPE_INVALID      0x02001600，HOOK类型错误。
 * @retval #OS_ERRNO_HOOK_PTR_NULL          0x02001602，HOOK指针空。
 * @retval #SRE_OK                          0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_IdleHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_IdleHookDel(SreIdleHook hook)
{
    return SRE_HookDel(OS_HOOK_IDLE_PERIOD, (OsVoidFunc)hook);
}

/*
 * @ingroup  SRE_sys
 * @brief 获取指定核的系统栈信息
 *
 * @par 描述
 * 获取指定核的系统栈信息。
 *
 * @attention
 * <ul>
 * <li>支持CORTEX-RX/HITXP/CORTEX-AX/SD6185X平台。</li>
 * <li>系统栈峰值查询算法出于性能考虑，查询结果可能会有误差。</li>
 * </ul>
 *
 * @retval #OS_ERRNO_SYS_COREID_INVALID        0x02000013，指定的核号非法。
 * @retval #OS_ERRNO_SYS_STACKINFO_PTR_NULL    0x02000023，出参为空。
 * @retval #SRE_OK                             0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sys.h：该接口声明所在的头文件。</li></ul>
 * @see NA
 */
extern U32 SRE_GetSysStackInfo(U32 coreId, struct SysStackInfo *stackInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_SYS_H */
