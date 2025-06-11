/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: HW模块的头文件, 用户不能直接引用
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_HW_H
#define OS_HW_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：配置实例启动参数时传入的参数为NULL
 *
 * 值: 0x02002000
 *
 * 解决方案: 请确保传入的参数不为NULL
 */
#define OS_ERRNO_HW_START_PARAM_CFG_NULL OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x00)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：配置实例启动参数时，实例内各个核配置不一致
 *
 * 值: 0x02002001
 *
 * 解决方案: 请确保实例内所有核配置的启动参数一致
 */
#define OS_ERRNO_HW_START_PARAM_INCONSISTENT OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x01)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：核启动时，未在#SRE_HardBootInit中调用#SRE_StartParamCfg接口进行启动参数配置
 *
 * 值: 0x02002002
 *
 * 解决方案: 请确保有在#SRE_HardBootInit中调用#SRE_StartParamCfg接口进行启动参数配置
 */
#define OS_ERRNO_HW_START_PARAM_NOT_CFG OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x02)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：设备ID错误
 *
 * 值: 0x02002003
 *
 * 解决方案: 设备ID的取值[0 ,2]
 */
#define OS_ERRNO_HW_DEVICE_ID_ERR OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x03)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：commVmid参数传入错误
 *
 * 值: 0x02002004
 *
 * 解决方案: COMM VMID的取值范围[0 ,7]
 */
#define OS_ERRNO_HW_VMID_ID_ERR OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x04)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码: Cored ID错误
 *
 * 值: 0x02002005
 *
 * 解决方案: 确保Cored
 * ID不能大于等于芯片支持的最大核数[OS_MAX_CORE_NUM]；若对于主核设置从核启动参数或唤醒从核时入参核ID不能是主核ID
 */
#define OS_ERRNO_HW_CORE_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x05)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：启动结构指针参数为NULL
 *
 * 值: 0x02002006
 *
 * 解决方案: 请确保传入的启动参数结构指针不为NULL
 */
#define OS_ERRNO_HW_SECONDARY_CORE_BOOTPARA_NULL OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x06)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：启动参数中段数目配置为0
 *
 * 值: 0x02002007
 *
 * 解决方案: 请确保当采用用户模式配置MMU MAP时，加载的段数不能为0
 */
#define OS_ERRNO_HW_SECONDARY_CORE_BOOTSECNUM_NULL OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x07)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：唤醒中断优先级非法
 *
 * 值: 0x02002008
 *
 * 解决方案: 请确保主核唤醒从核的唤醒中断优先级范围为[1, 7]
 */
#define OS_ERRNO_HW_CORE_WAKE_UP_PRI_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x08)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：启动参数段配置中虚拟地址配置重叠
 *
 * 值: 0x02002009
 *
 * 解决方案: 请确保启动参数各段配置中虚拟地址是否配置重叠
 */
#define OS_ERRNO_HW_SEC_AREA_OVERLAY_IN_VIR_ADDR OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x09)

/*
 * @ingroup os_hw
 * 错误码：启动参数中设置的启动向量基地址vecbase不合法。
 *
 * 值: 0x0200200a
 *
 * 解决方案：检查硬线程启动向量基地址vecbase(配置的虚地址或对应的实地址)，
 * 保证4K字节对齐且对应的实地址不能为零。
 */
#define OS_ERRNO_HW_VECBASE_ADDR_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x0a)

/*
 * @ingroup os_hw
 * 错误码：启动参数中设置的系统栈地址不合法。
 *
 * 值: 0x0200200b
 *
 * 解决方案：检查硬线程启动系统栈地址，需要保证16字节对齐且不能为零。
 * 同时排查该栈地址经过映射后是否存在溢出为零或不对齐的情况。
 */
#define OS_ERRNO_HW_SYS_STACK_ADDR_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x0b)

/*
 * @ingroup os_hw
 * 错误码：启动参数中设置的堆地址不对齐。
 *
 * 值: 0x0200200c
 *
 * 解决方案: 检查硬线程启动系统堆地址（注意配置为虚地址），保证16字节对齐。
 */
#define OS_ERRNO_HW_HEAP_ADDR_NOT_ALIGNED OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x0c)

/*
 * @ingroup os_hw
 * 错误码：传入的参数非法，OS使用的EMT参数配置非法。
 *
 * 值: 0x0200200d
 *
 * 解决方案: 请保证传入的参数合法，OS使用的EMT资源数大小至少为20字节，共5个EMT资源(WORD)。
 */
#define OS_ERRNO_HW_EMT_CFG_TOTAL_SIZE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x0d)

/*
 * @ingroup os_hw
 * 错误码：配置OS使用的EMT资源地址不合法。
 *
 * 值: 0x0200200e
 *
 * 解决方案: 保证配置EMT地址及大小不能翻转溢出。
 *
 */
#define OS_ERRNO_EMT_CFG_ADDR_SIZE_ILLEGAL OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x0e)

/*
 * @ingroup os_hw
 * 错误码：配置OS使用的EMT资源地址及大小必须为4字节对齐。
 *
 * 值: 0x0200200f
 *
 * 解决方案: 缓存区首地址和长度都要求4字节对齐。
 *
 */
#define OS_ERRNO_HW_EMT_ADDR_SIZE_NOT_ALIGN OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x0f)

/*
 * @ingroup os_hw
 * 错误码：配置OS使用的MP 寄存器基地址不对
 *
 * 值: 0x02002010
 *
 * 解决方案: 检查配置的OS_SYS_MP_REG_BASE是否与芯片上的MP_REG_BASE寄存器上的值一致
 *
 */
#define OS_ERRNO_HW_MP_REG_BASE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x10)

/*
 * @ingroup os_hw
 * 错误码：配置OS使用的页表基地址没有对齐
 *
 * 值: 0x02002011
 *
 * 解决方案: TSP上页表基地址必须0x8000对齐
 *
 */
#define OS_ERRNO_HW_MMU_TBL_BASE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x11)

/*
 * @ingroup os_hw
 * 错误码：配置OS使用的页表大小过少
 *
 * 值: 0x02002012
 *
 * 解决方案: TSP上配置OS使用的页表至少64K
 *
 */
#define OS_ERRNO_HW_MMU_TBL_SIZE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x12)

/*
 * @ingroup os_hw
 * 错误码：页表信息检验时出错
 *
 * 值: 0x02002013
 *
 * 解决方案: 检查页表信息控制块是否正确，尤其是魔术字
 *
 */
#define OS_ERRNO_HW_MMU_TBL_INFO_CHECK_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x13)

/*
 * @ingroup os_hw
 * 错误码：不支持OS进行MMU页表映射MAP的建立
 *
 * 值: 0x02002014
 *
 * 解决方案: 主核直接启动从核方式下，从核不支持OS进行MMU表项的建立
 *
 */
#define OS_ERRNO_HW_MMU_MAP_UNSUPPORT_OS_BUILD OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x14)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码: 接口调用时入参为空
 *
 * 值: 0x02002015
 *
 * 解决方案: 确保接口调用时入参不为空
 *
 */
#define OS_ERRNO_HW_PARA_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x15)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码: 获取核上MSQ地址时，MSQ ID不合法
 *
 * 值: 0x02002016
 *
 * 解决方案: 确保获取核上MSQ地址时，MSQ ID只能是1或2
 *
 */
#define OS_ERRNO_HW_CPU_MSQ_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x16)

/*
 * @ingroup os_hw
 * 错误码：页表的虚实地址不一致
 *
 * 值: 0x02002017
 *
 * 解决方案: 检查页表空间的虚实地址是否一致
 *
 */
#define OS_ERRNO_HW_MMU_TBL_VAPA_NOT_SAME OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x17)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：等待核clock时钟关闭状态超时
 *
 * 值：0x02002018
 *
 * 解决方案；超时原因可能为DCC处于busy状态、核没进waiti、OCD时钟没有关闭、bubble reason等。
 */
#define OS_ERRNO_HW_MP_CLK_OFF_STATUS_WAIT_OVER_TIME OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x18)

/*
 * @ingroup os_hw
 * 硬件相关信息错误码：等待硬件lpc状态超时
 *
 * 值：0x02002019
 *
 * 解决方案；排查硬件配置流程是否正确或配置流程某个环节被非法修改。
 */
#define OS_ERRNO_HW_LPC_CHECK_STATUS_WAIT_OVER_TIME OS_ERRNO_BUILD_ERROR(OS_MID_HW, 0x19)

// config 配置项中内存类型
// 核私有
#define OS_SYS_STACK_MEM 1
#define OS_FSC_PT_MEM 3
#define OS_TRACE_AREA_MEM 4

// 核共享
#define OS_MC_FSC_PT_MEM 0x81U
#define OS_LP_SHARE_PT_MEM 0x82U
#define OS_SYM_TBL_MEM 0x84U

#if (defined OS_OPTION_LP_PRELINK)
#define OS_LP_PLK_MEM 0x88U
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_HW_H */
