/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: LibCK的基础头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_BASE_H
#define SRE_BASE_H

#include "sre_buildef.h"
#include "sre_errno.h"
#include "./common/os_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OS_FLAGS_AND(reg, flg) ((flg) == ((flg) & (reg)))
#define OS_FLAGS_OR(reg, flg) (0 != ((flg) & (reg)))

/*
 * U32类型的PID中，TCB Index占用的比特数
 */
#define OS_TSK_TCB_INDEX_BITS ((4 - OS_TSK_CORE_BYTES_IN_PID) * 8)

/*
 * @ingroup  OS_sys
 * 从线程PID获取核内线程handle号
 */
#define GET_HANDLE(pid) ((pid) & ((1U << OS_TSK_TCB_INDEX_BITS) - 1))

/*
 * @ingroup  OS_sys
 * 硬中断核内线程handle号
 */
#define OS_HWI_HANDLE ((1U << OS_TSK_TCB_INDEX_BITS) - 1)  // 0X000000FF

/*
 * @ingroup  OS_sys
 * 从线程PID获取核号
 */
#define GET_COREID(pid) ((U8)((pid) >> OS_TSK_TCB_INDEX_BITS))

/*
 * @ingroup  OS_sys
 * 将coreid与handle组成PID,  coreid:[0, OS_MAX_CORE_NUM); handle:[0, 255]
 */
#define COMPOSE_PID(coreid, handle) \
    ((((U32)(coreid)) << OS_TSK_TCB_INDEX_BITS) + ((U8)(handle))) /* 将(coreid)与(handle)组成PID,UIPC不使用该接口 */

/*
 * @ingroup  OS_sys
 * 核间功能模块的未初始化标志位
 */
#define OS_MC_PHASE_UNINIT 0
/*
 * @ingroup  OS_sys
 * 核间功能模块的正在初始化标志位
 */
#define OS_MC_PHASE_HANDLE 0xae1234ea
/*
 * @ingroup  OS_sys
 * 核间功能模块的初始化完成标志位
 */
#define OS_MC_PHASE_INITED 0xad5678da

/*
 * @ingroup  OS_sys
 * 核间信号量、信号量、消息等待时间设定：表示不等待。
 */
#define OS_NO_WAIT 0

#define OS_GET_64BIT_HIGH_32BIT(num)    ((U32)((num) >> 32))
#define OS_GET_64BIT_LOW_32BIT(num)     ((U32)((num) & 0xFFFFFFFFU))

/*
 * @ingroup  SRE_sys
 * 系统时间定义结构体
 */
struct SysTime {
    U16 year; /* 年，系统时间取值为1970 ~ 2038或1970 ~ 2100 */
    U8 month; /* 月，取值为 1 - 12 */
    U8 day; /* 日，取值为 1 - 31 */
    U8 hour; /* 时，取值为 0 - 23 */
    U8 minute; /* 分，取值为 0 - 59 */
    U8 second; /* 秒，取值为 0 - 59 */
    U8 week; /* 星期，取值为 0 - 6 */
};

/*
 * @ingroup SRE_sys
 * 系统核掩码结构体
 */
#define OS_CORE_MASK_SIZE ((U32)(31 + OS_MAX_CORE_NUM) >> 5)
struct SysSreCoreMask {
    U32 coreMask[OS_CORE_MASK_SIZE];
};

/*
 * @ingroup  SRE_sys
 * CpuTick结构体类型。
 *
 * 用于记录64位的cycle计数值。
 */
struct SreCpuTick {
    U32 cntHi; /* cycle计数高32位 */
    U32 cntLo; /* cycle计数低32位 */
};

/*
 * @ingroup SRE_sys
 * 矢量寄存器保护区域结构体。
 *
 */
struct CpSaveCfgParam {
    void *saveAreaA; /* coprocessor上下文保存区域的大小 */
    void *saveAreaB; /* coprocessor上下文保存区域的大小 */
};

/*
 * @ingroup  SRE_sys
 * @brief 获取当前的64位time stamp计数(即系统运行的cycles)。
 *
 * @par 描述
 * 获取当前的64位time stamp计数(即系统运行的cycles)。
 *
 * @attention
 * <ul>
 * <li>获取的是64bit cycles 数据。</li>
 * <li>Cortex-AX、Cortex-RX平台不支持此功能。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval [0,0xFFFFFFFFFFFFFFFF] 系统当前的cycle数
 * @par 依赖
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CycleCountGet32()
 */
extern U64 SRE_CycleCountGet64(void);

/*
 * @ingroup  SRE_sys
 * @brief 获取当前的64位cycle计数,cycle计数的高32位保存到参数cntHi，低32为保存到参数cntLo。
 *
 * @par 描述
 * 获取当前的64位cycle计数,cycle计数的高32位保存到参数cntHi，低32为保存到参数cntLo。
 *
 * @attention
 * <ul>
 * <li>入参指针不能为NULL，由用户保证。</li>
 * <li>Cortex-AX、Cortex-RX平台不支持此功能。</li>
 * </ul>
 *
 * @param cntHi [OUT] 类型#U32 *，保存cycle计数的高32位。
 * @param cntLo [OUT] 类型#U32 *，保存cycle计数的低32位。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CycleCountGet64()
 */
OS_SEC_ALW_INLINE INLINE void SRE_CycleCountGet(U32 *cntHi, U32 *cntLo)
{
    U64 count;

    count = SRE_CycleCountGet64();
    if (cntHi == NULL || cntLo == NULL) {
        return;
    }
    *cntHi = OS_GET_64BIT_HIGH_32BIT(count);
    *cntLo = OS_GET_64BIT_LOW_32BIT(count);
}

/*
 * @ingroup  SRE_sys
 * @brief 获取当前的32位time stamp计数(即系统运行的cycles)。
 *
 * @par 描述
 * 获取当前的32位time stamp计数(即系统运行的cycles)。
 *
 * @attention
 * <ul>
 * <li>获取的是32bit cycles 数据。</li>
 * <li>若两次获取时间过长，会发生数据翻转现象，此情况下建议使用获取64bit数据接口。</li>
 * <li>Cortex-AX、Cortex-RX平台不支持此功能。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval [0,0xFFFFFFFF]  系统当前的cycle数
 * @par 依赖
 * <ul><li>sre_base.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CycleCountGet64()
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_CycleCountGet32(void)
{
#if ((OS_HARDWARE_PLATFORM == OS_HIDSP) || (OS_HARDWARE_PLATFORM == OS_HIDSPV200) || \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV200) || (OS_HARDWARE_PLATFORM == OS_HIAVPV280))
    register uintptr_t ccount = 0;
    OS_EMBED_ASM("ccount %0" :"=&r"(ccount) : :"cc");
    return (U32)ccount;
#elif ((OS_HARDWARE_PLATFORM == OS_HIAVPV300LITE) || (OS_HARDWARE_PLATFORM == OS_HIDSPV300LITE))
    register U64 ccount = 0;
    OS_EMBED_ASM("ccount@agl %0" : "=&r"(ccount) : :"cc");
    return (U32)ccount;
#else
    return (U32)SRE_CycleCountGet64();
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_BASE_H */
