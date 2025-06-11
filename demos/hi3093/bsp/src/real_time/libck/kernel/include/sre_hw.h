/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 硬件相关模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HW_H
#define SRE_HW_H

#include "sre_base.h"
#include "./hw/board/os_board.h"
#include "./common/os_hw.h"
#include "./common/os_boot.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if defined(OS_OPTION_START_PARAM_CFG)
/*
 * @ingroup  SRE_hw
 * @brief 配置实例启动参数。
 *
 * @par 描述
 * 根据startParam的内容配置实例启动参数，OS根据该配置将Hwsem和MSGM队列资源按实例进行分段管理。
 *
 * @attention
 * <ul>
 * <li>用户必须在#SRE_HardBootInit中且在DDR代码加载后调用该函数，否则初始化失败。</li>
 * <li>实例间配置的资源分段不能重叠，该合法性由用户保证。</li>
 * <li>在非SD6185平台，OS固定使用0号硬件信号量作为实例间互斥，用户无需通过该API将0号硬件信号量配置给OS。</li>
 * </ul>
 *
 * @param startParam [IN]  类型#struct SreStartParam *，启动参数。
 *
 * @retval #OS_ERRNO_HW_START_PARAM_CFG_NULL          0x02002000，配置实例启动参数时传入的参数为NULL。
 * @retval #OS_ERRNO_HW_START_PARAM_INCONSISTENT      0x02002001，配置实例启动参数时，实例内各个核配置不一致。
 * @retval #OS_ERRNO_HW_START_PARAM_NOT_CFG           0x02002002，核启动时，未在#SRE_HardBootInit中调用
 * @retval                                                        #SRE_StartParamCfg接口进行启动参数配置。
 * @retval #OS_ERRNO_HWSEM_CFG_GLB_START_ID_INVALID   0x02002425，配置实例启动参数时传入的全局硬件信号量起始ID非法。
 * @retval #OS_ERRNO_HWSEM_CFG_GLB_NUM_INVALID        0x02002426，配置实例启动参数时传入的全局硬件信号量个数非法
 * @retval #OS_ERRNO_HWSEM_CFG_GLB_REGION_INVALID     0x02002427，配置实例启动参数时配置的全局硬件信号量区间非法。
 * @retval #OS_ERRNO_HWSEM_CFG_LOCAL_START_ID_INVALID 0x02002428，配置实例启动参数时传入的局部硬件信号量起始ID过大。
 * @retval #OS_ERRNO_HWSEM_CFG_LOCAL_NUM_INVALID      0x02002429，配置实例启动参数时传入的局部硬件信号量个数非法。
 * @retval #OS_ERRNO_HWSEM_CFG_LOCAL_REGION_INVALID   0x0200242a，配置实例启动参数时配置的局部硬件信号量区间非法。
 * @retval #OS_ERRNO_HW_EMT_CFG_TOTAL_SIZE_INVALID    0x0200200d，传入的参数非法，OS使用的EMT总大小配置非法。
 * @retval #OS_ERRNO_EMT_CFG_ADDR_SIZE_ILLEGAL        0x0200200e，配置OS使用的EMT资源地址不合法。
 * @retval #OS_ERRNO_HW_EMT_ADDR_SIZE_NOT_ALIGN       0x0200200f，配置OS使用的EMT资源地址及大小必须为4字节对齐。
 * @retval #SRE_OK                                    0x00000000，启动参数配置成功。
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_StartParamCfg(struct SreStartParam *startParam);
#endif

/*
 * @ingroup SRE_hw
 * 设备编号
 */
enum SreDevice {
#if defined(OS_OPTION_HWSEM)
    OS_DEVICE_HWSEM_ID = 0, /* 硬件信号量ID */
#endif
    OS_DEVICE_MSGM_ID = 1, /* MSGM ID */
    OS_DEVICE_TIMER_ID = 2, /* 全局定时器ID */

#if defined(OS_OPTION_HAC_SYNCQ)
    OS_DEVICE_HACSYNQ_ID = 3, /* HAC同步队列ID */
#endif
#if defined(OS_OPTION_TRING)
    OS_DEVICE_TRING_ID = 4,
#endif
};

/*
 * @ingroup SRE_hw
 * @brief Comm VMID配置接口。
 *
 * @par 描述
 * 设置COMM VMID并使能VMID检查。
 *
 * @attention
 * <ul>
 * <li>主核调用。</li>
 * <li>main函数之后，调用硬件信号量的VMID设置，否则可能出现硬件信号量VMID保护不使能的情况。</li>
 * </ul>
 *
 * @param deviceId [IN]  类型#U32，设备ID，取值范围[0,4]，含义参考#enum SreDevice。
 * @param commVmid [IN]  类型#U32，Comm Vmid值，取值范围[0,7]。
 *
 * @retval #OS_ERRNO_HW_VMID_ID_ERR      0x02002004，commVmid参数传入错误。
 * @retval #OS_ERRNO_HW_DEVICE_ID_ERR    0x02002003，设备ID错误。
 * @retval #SRE_OK                       0x00000000，返回成功。
 *
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_CommVmidSet(U32 deviceId, U32 commVmid);

enum OsSysMode {
    OS_SUPER_MODE,
    OS_USER_MODE,
    OS_ERR_MODE
};

/*
 * @ingroup SRE_hw
 * @brief VMID切换接口。
 *
 * @par 描述
 * 重设本核的VMID。
 *
 * @attention
 * <ul>
 * <li>仅支持MCPU核。</li>
 * </ul>
 *
 * @param mode   [IN]  类型#enum OsSysMode，切换VMID的模式，超级模式或者用户模式。
 * @param cntxID [IN]  类型#U8，切换的contextID值。
 *
 * @retval #OS_ERRNO_SYS_MODE_INVALID      0x0200000f，传入的模式错误。
 * @retval #SRE_OK                         0x00000000，返回成功。
 *
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CntxIdFastSwitch
 */
extern U32 SRE_CntxIdSwitch(enum OsSysMode mode, U8 cntxID);

/*
 * @ingroup SRE_hw
 * @brief VMID切换接口，不关中断版本。
 *
 * @par 描述
 * 重设本核的VMID。
 * @attention
 * <ul>
 * <li>仅支持MCPU核。</li>
 * <li>不开关中断版本，由调用者保证不发生抢占。</li>
 * </ul>
 *
 * @param mode   [IN]  类型#enum OsSysMode，切换VMID的模式，超级模式或者用户模式。
 * @param cntxID [IN]  类型#U8，切换的contextID值。
 *
 * @retval #OS_ERRNO_SYS_MODE_INVALID      0x0200000f，传入的模式错误。
 * @retval #SRE_OK                         0x00000000，返回成功。
 *
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CntxIdSwitch
 */
extern U32 SRE_CntxIdFastSwitch(enum OsSysMode mode, U8 cntxID);

/*
 * @ingroup SRE_sys
 * @brief 获取当前系统实例ID。
 *
 * @par 描述
 * 返回当前系统实例ID。
 *
 * @attention 无
 *
 * @param instId [OUT] 类型#U32 *，用于存储实例ID的地址。
 *
 * @retval #OS_ERRNO_SYS_INST_ID_ADDR_NULL   0x02002f04，创建实例时传入的用户存放实例ID的地址为空
 * @par 依赖
 * <ul><li>sre_hw.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_InstIdSelf(U32 *instId);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_HW_H */
