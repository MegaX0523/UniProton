/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: ECC模块的外部头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * *@defgroup SRE_ecc ECC
 * @ingroup SRE_inspect
 */

#ifndef SRE_HW_ECC_H
#define SRE_HW_ECC_H

#include "sre_base.h"

#if ((OS_HARDWARE_PLATFORM == OS_HIAVPV280) || (OS_HARDWARE_PLATFORM == OS_HIDSPV200) || \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV200) || (OS_HARDWARE_PLATFORM == OS_HIDSP) ||     \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV300LITE) || (OS_HARDWARE_PLATFORM == OS_HIDSPV300LITE))
#include "./hw/cpu/os_ecc_hidsp.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_RISC220) || (OS_HARDWARE_PLATFORM == OS_DSP220) || \
     (OS_HARDWARE_PLATFORM == OS_HACCC) || (OS_HARDWARE_PLATFORM == OS_DSP100))
#include "./hw/cpu/os_ecc_xtensa.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_ecc
 * @brief 用户ECC可纠正错误的处理函数原型定义。
 *
 * @par 描述 无。
 * @attention
 * <ul>
 * <li>适用于HIDSP/XTENSA平台。</li>
 * </ul>
 *
 * @param regs [IN]  类型#struct SreEccRegInfo *，指向ECC异常接管后保存的ECC寄存器信息结构体。
 *
 * @retval 无。
 * @par 依赖
 * <li>sre_hw_ecc.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef void (*EccCorrectedHook)(struct SreEccRegInfo *regs);

/*
 * @ingroup SRE_ecc
 * @brief 注册用户ECC可纠正错误的处理钩子。
 *
 * @par 描述
 * 注册用户ECC可纠正错误的处理钩子。
 * @attention
 * <ul>
 * <li>支持钩子重复注册。</li>
 * <li>输入参数为NULL可删除已注册钩子。</li>
 * <li>适用于HIDSP/XTENSA平台。</li>
 * </ul>
 *
 * @param correctedHook [IN]  类型#EccCorrectedHook，用户ECC可纠正错误的处理钩子函数原型。
 *
 * @retval #SRE_OK 0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_ecc.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_EccCorrectedHookAdd(EccCorrectedHook correctedHook);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_HW_ECC_H */
