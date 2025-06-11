/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: LibCK的基础头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_BASE_H
#define OS_BASE_H

#include "sre_buildef.h"
#include "os_typedef.h"

#if (OS_HARDWARE_PLATFORM == OS_ARM7)
#include "./hw/cpu/os_cpu_arm7.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_ARM9)
#include "./hw/cpu/os_cpu_arm9.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_CORTEX_RX)
#include "./hw/cpu/os_cpu_armv7.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_HIDSP) || (OS_HARDWARE_PLATFORM == OS_HIDSPV200) ||     \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV200) || (OS_HARDWARE_PLATFORM == OS_HIAVPV280) || \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV300LITE) || (OS_HARDWARE_PLATFORM == OS_HIDSPV300LITE))
#include "./hw/cpu/os_cpu_hidsp.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_DSP220) || (OS_HARDWARE_PLATFORM == OS_RISC220) || \
     (OS_HARDWARE_PLATFORM == OS_HACCC) || (OS_HARDWARE_PLATFORM == OS_DSP100))
#include "./hw/cpu/os_cpu_xtensa.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_CORTEX_M7) || (OS_HARDWARE_PLATFORM == OS_CORTEX_M4) || \
     (OS_HARDWARE_PLATFORM == OS_CORTEX_M3))
#include "./hw/cpu/os_cpu_armv7_m.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_CORTEX_AX)
#include "./hw/cpu/os_cpu_armv7_a.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_ARMV8)
#include "./hw/cpu/os_cpu_armv8.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_RISCV)
#include "./hw/cpu/os_cpu_riscv.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_HITSP_X)
#include "./hw/os_cpu_hitsp.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_sem
 * 核间信号量、信号量、消息等待时间设定：表示永久等待。
 */
#define OS_WAIT_FOREVER 0xFFFFFFFF

/*
 * @ingroup  OS_sys
 * CpuTick结构体类型。
 *
 * 用于记录64位的cycle计数值。
 */
struct OsCpuTick {
    U32 cntHi; /* cycle计数高32位 */
    U32 cntLo; /* cycle计数低32位 */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_BASE_H */
