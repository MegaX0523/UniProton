/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: 启动相关模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2020-11-22
 * History: 2020-11-22 LibCK项目组 创建文件
 */
#ifndef SRE_BOOT_H
#define SRE_BOOT_H

#include "sre_base.h"
#include "./common/os_boot.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if defined(OS_OPTION_BOOT_DIRECT)
/*
 * @ingroup  SRE_Boot
 * @brief 从核启动
 *
 * @par 描述
 * 根据用户传入的coreBootCfg配置从核vecbase、sp、tblbase，初始化并使能从核MMU、Cache，并激活从核。
 *
 * @attention
 * <ul>
 * <li>适用于从核各段地址已分配好，主核只需要设置启动参数并激活的场景，目前支持8171平台。</li>
 * <li>用户必须在BSS段初始化之后，即进入#main后，调用该函数，否则共享BSS使用会存在风险。</li>
 * </ul>
 *
 * @param coreBootPara [IN]  类型#struct SecondaryCoreBootCfg *，启动参数，其中secNum、secInfo两个参数不需要配置。
 *
 * @retval #OS_ERRNO_MMU_PAGE_TBL_ADDR_NOT_ALIGN      0x02001232，配置页表基地址未按0x4000字节(16K)大小对齐。
 * @retval #OS_ERRNO_MMU_PAGE_TBL_LEN_NOT_ALIGN       0x02001234，配置页表长度未按0x400字节(1k)大小对齐。
 * @retval #OS_ERRNO_MMU_PAGE_TBL_CFG_INVALID         0x02001235，传入的页表地址或长度非法。
 * @retval #SRE_OK                                    0x00000000，启动参数配置并激活成功。
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_BootSecondaryCore(struct SecondaryCoreBootCfg *coreBootCfg);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_BOOT_H */
