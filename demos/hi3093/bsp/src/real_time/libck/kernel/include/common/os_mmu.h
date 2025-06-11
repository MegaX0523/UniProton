/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: mmu模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2022-2-10
 */
#ifndef OS_MMU_H
#define OS_MMU_H

#include "sre_buildef.h"
#include "os_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define MMU_VA_T VirtAddr
#define MMU_PA_T PhyAddr

/*
 * @ingroup SRE_mmu
 * TLB无效操作时指定的无效方式:页无效、context无效、无效ALL.
 *
 * TLB无效方式。
 */
enum OsTlbInvPolicy {
    NON_INV     = 0,              /* none opreation */
    PAGE_INV    = 1,              /* page invalidate */
    CONTEXT_INV = 2,              /* context invalidate */
    ALL_INV     = 3,              /* all invalidate */
    RESERVED    = 4
};

/*
 * @ingroup SRE_mmu
 * TLB的三种类型:ITLB、DTLB.
 *
 * TLB类型。
 */
enum OsTlbType {
    ITLB,            /* ITLB */
    DTLB             /* DTLB */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

// 公共部分
#if ((OS_HARDWARE_PLATFORM == OS_HIDSP) || (OS_HARDWARE_PLATFORM == OS_HIDSPV200) ||     \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV200) || (OS_HARDWARE_PLATFORM == OS_HIAVPV280) || \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV300LITE) || (OS_HARDWARE_PLATFORM == OS_HIDSPV300LITE))
#include "./hw/cpu/os_mmu_hidsp.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_CORTEX_AX)
#include "./hw/cpu/os_mmu_armv7_a.h"
#endif

#if (OS_HARDWARE_PLATFORM == OS_HITSP_X)
#include "./hw/os_mmu_hitsp.h"
#endif

// 扩展部分
#if ((OS_HARDWARE_PLATFORM == OS_HIAVPV200) || (OS_HARDWARE_PLATFORM == OS_HIAVPV280) || \
    (OS_HARDWARE_PLATFORM == OS_HIAVPV300LITE))
#include "./hw/cpu/os_mmu_ex_hiavp.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_HIDSP) || (OS_HARDWARE_PLATFORM == OS_HIDSPV200) || \
     (OS_HARDWARE_PLATFORM == OS_HIDSPV300LITE))
#include "./hw/cpu/os_mmu_ex_hidsp.h"
#endif

#endif /* OS_MMU_H */
