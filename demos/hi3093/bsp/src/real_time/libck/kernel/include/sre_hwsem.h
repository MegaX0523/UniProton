/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 硬件信号量模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HWSEM_H
#define SRE_HWSEM_H

#include "sre_base.h"
#include "sre_module.h"

#if (OS_CPU_TYPE == OS_SD6155 || OS_CPU_TYPE == OS_SD6182 || OS_CPU_TYPE == OS_SD6157 || OS_CPU_TYPE == OS_8171)
#include "./hw/drv/os_hwsemv100x.h"
#endif

#if ((OS_CPU_TYPE == OS_SD6183) || (OS_CPU_TYPE == OS_SD6185) || (OS_CPU_TYPE == OS_SD6186) || \
    (OS_CPU_TYPE == OS_SD6186L) || (OS_CPU_TYPE == OS_HI6881) || (OS_CPU_TYPE == OS_HI1171) || \
    (OS_CPU_TYPE == OS_8021) || (OS_CPU_TYPE == OS_8011))
#include "./hw/drv/os_hwsemv250x.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_hwsem
 * 硬件信号量资源类型:全局硬件信号量资源。
 */
#define SRE_HWSEM_SHARE_GLOBAL 0

/*
 * @ingroup SRE_hwsem
 * 硬件信号量资源类型:cluster内部硬件信号量资源。
 */
#define SRE_HWSEM_SHARE_CLUSTER 1

/*
 * @ingroup SRE_hwsem
 * 硬件信号量工作模式:互斥型硬件信号量资源。
 */
#define SRE_HWSEM_MODE_MUTEX 0

/*
 * @ingroup SRE_hwsem
 * 硬件信号量工作模式:计数型硬件信号量资源访问。
 * 当前只有6181支持该工作模式
 */
#define SRE_HWSEM_MODE_CNT 1

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：创建硬件信号量资源时传入的名字非法。
 *
 * 值: 0x02002401
 *
 * 解决方案: 硬件信号量名不可为0xFFFFFFFF，不可与os预留的0xFFFFFFFE冲突。
 */
#define OS_ERRNO_HWSEM_NAME_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x01)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：创建硬件信号量资源时，传入的模式非法。
 *
 * 值: 0x02002402
 *
 * 解决方案: mode入参必须是 SRE_HWSEM_MODE_MUTEX。
 */
#define OS_ERRNO_HWSEM_MODE_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x02)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：创建硬件信号量资源时，传入的硬件信号量ID指针为空。
 *
 * 值: 0x02002403
 *
 * 解决方案: 确保传入的指针非空。
 */
#define OS_ERRNO_HWSEM_ID_NULL SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x03)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：删除硬件信号量资源时，被检测出硬件信号量ID高4位即不是全局也不是局部资源类型。
 *
 * 值: 0x02002404
 *
 * 解决方案: 硬件信号量ID需是通过SRE_HwSemCreate接口所得。
 */
#define OS_ERRNO_HWSEM_NOT_ANY_TYPE SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x04)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码:  删除硬件信号量资源时，被检测出局部硬件信号量ID的低16位超过了最大资源个数。
 *
 * 值: 0x02002405
 *
 * 解决方案: 硬件信号量ID需是通过SRE_HwSemCreate接口所得。
 */
#define OS_ERRNO_HWSEM_LHWSEM_EXCEED_MAX SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x05)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码:  删除硬件信号量资源时，被检测出全局硬件信号量ID的低20位超过了最大资源个数。
 *
 * 值: 0x02002406
 *
 * 解决方案: 硬件信号量ID需是通过SRE_HwSemCreate接口所得。
 */
#define OS_ERRNO_HWSEM_GHWSEM_EXCEED_MAX SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x06)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码:删除硬件信号量资源时，被检测出硬件信号量ID的24至27位的工作模式与管理信息不匹配。
 *
 * 值: 0x02002407
 *
 * 解决方案: 硬件信号量ID需是通过SRE_HwSemCreate或者SRE_HwSemCreateById接口所得。
 */
#define OS_ERRNO_HWSEM_MODE_NOT_MATCH SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x07)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：创建硬件信号量资源时，被检测出无可用的硬件信号量资源。
 *
 * 值: 0x02002408
 *
 * 解决方案: 硬件信号量资源有限，若资源耗尽时，需先释放部分资源，再行使用。
 */
#define OS_ERRNO_HWSEM_EXHAUST SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x08)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：创建硬件信号量资源时，被检测出重复创建。
 *
 * 值: 0x02002409
 *
 * 解决方案: 同一硬件信号量资源不可进行重复创建。
 */
#define OS_ERRNO_HWSEM_CREATED SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x09)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：使用硬件信号量资源时，被检测出该硬件信号量已被删除或者未创建。
 *
 * 值: 0x0200240a
 *
 * 解决方案: 检查该硬件信号量是否已经被删除或者未创建。
 */
#define OS_ERRNO_HWSEM_DELETED SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x0a)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：VMID的bit位表的值不正确
 *
 * 值: 0x0200240b
 *
 * 解决方案: VMID的bit位表的值范围[0,0xFF]
 */
#define OS_ERRNO_HWSEM_VMID_BIT_MAP_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x0b)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：获取硬件信号量管理信息句柄失败。
 *
 * 值: 0x02002410
 *
 * 解决方案: 查看输入的输入的硬件信号量ID是否正确。
 */
#define OS_ERRNO_HWSEM_INT_POINT_NULL SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x10)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码：硬件信号量初始化时创建错误处理中断失败
 *
 * 值: 0x02002411
 *
 * 解决方案：无
 */
#define OS_ERRNO_HWSEM_INIT_FAILED SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x11)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量创建参数为空。
 *
 * 值: 0x02002418
 *
 * 解决方案：检查硬件信号量创建参数是否为空。
 */
#define OS_ERRNO_HWSEM_CREATE_PARA_NULL SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x18)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量类型不可用。
 *
 * 值: 0x0200241b
 *
 * 解决方案：检查输入的硬件信号量类型是否正确。入参必须是SRE_HWSEM_SHARE_GLOBAL or SRE_HWSEM_SHARE_CLUSTER。
 */
#define OS_ERRNO_HWSEM_TYPE_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x1b)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：VMID的值不正确
 *
 * 值: 0x0200241c
 *
 * 解决方案: VMID值的范围[0,7]
 */
#define OS_ERRNO_HWSEM_VMID_VALUE_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x1c)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量初始化时，内存分配不足。
 *
 * 值: 0x0200241f
 *
 * 解决方案：分配更大的私有FSC静态分区。
 */
#define OS_ERRNO_HWSEM_NO_MEMORY SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x1f)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量做PEND操作时，输入的ID错误。
 *
 * 值: 0x02002420
 *
 * 解决方案：输入的ID值必须是创建接口返回的信号量ID。
 */
#define OS_ERRNO_HWSEM_PEND_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x20)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量做PEND操作时，输入的获取中断开关值的指针为空。
 *
 * 值: 0x02002421
 *
 * 解决方案：保证获取中断开关制的指针为非空。
 */
#define OS_ERRNO_HWSEM_INTVALUE_NULL SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x21)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量做POST操作时，输入的ID错误。
 *
 * 值: 0x02002422
 *
 * 解决方案：输入的ID值必须是创建接口返回的信号量ID。
 */
#define OS_ERRNO_HWSEM_POST_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x22)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量注册配置的个数不一致错误。
 *
 * 值: 0x02002423
 *
 * 解决方案：需要所有的核配置的个数都一致。
 */
#define OS_ERRNO_HWSEM_NUM_NOT_SAME SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x23)

/*
 * *@ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量还未释放，不能删除。
 *
 * 值: 0x02002424
 *
 * 解决方案：需要先释放该硬件信号量。
 */
#define OS_ERRNO_HWSEM_NOT_POST SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x24)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 配置实例启动参数时传入的全局硬件信号量起始ID非法。
 *
 * 值: 0x02002425
 *
 * 解决方案: 请确保全局硬件信号量起始ID合法SD6182/8171：[1,63]，SD6157:[1,31],SD6183/Hi1171：[1,127],SD6185x：[0,127]
 */
#define OS_ERRNO_HWSEM_CFG_GLB_START_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x25)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 配置实例启动参数时传入的全局硬件信号量个数非法。
 *
 * 值: 0x02002426
 *
 * 解决方案: 请确保传入的全局硬件信号量个数合法SD6182/8171:[3,64],SD6157:[3,32],SD6183/Hi1171:[3,128],SD6185x:[0,128]
 */
#define OS_ERRNO_HWSEM_CFG_GLB_NUM_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x26)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 配置实例启动参数时配置的全局硬件信号量区间非法。
 *
 * 值: 0x02002427
 *
 * 解决方案: 请确保传入的全局硬件信号量起始编号和个数决定的区间在合法范围内。
 */
#define OS_ERRNO_HWSEM_CFG_GLB_REGION_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x27)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 配置实例启动参数时传入的局部硬件信号量起始ID过大。
 *
 * 值: 0x02002428
 *
 * 解决方案: 请确保传入的局部硬件信号量起始ID不超过最大编号(SD6182/SD6157:15，SD6183x:31)。
 */
#define OS_ERRNO_HWSEM_CFG_LOCAL_START_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x28)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 配置实例启动参数时传入的局部硬件信号量个数非法。
 *
 * 值: 0x02002429
 *
 * 解决方案: 请确保传入的局部硬件信号量个数不大于(SD6182/SD6157:16，SD6183x:32)。
 */
#define OS_ERRNO_HWSEM_CFG_LOCAL_NUM_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x29)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 配置实例启动参数时配置的局部硬件信号量区间非法。
 *
 * 值: 0x0200242a
 *
 * 解决方案: 请确保传入的局部硬件信号量起始编号和个数在正常的区间范围内(SD6182/SD6157:[0,16], SD6183x:[0,32])。
 */
#define OS_ERRNO_HWSEM_CFG_LOCAL_REGION_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x2a)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 复位或者创建指定物理ID的硬件信号量时，输入的全局硬件信号量物理ID非法。
 *
 * 值: 0x0200242b
 *
 * 解决方案: 请确保传入的全局硬件信号量物理ID合法。
 */
#define OS_ERRNO_HWSEM_GLB_PHYSICAL_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x2b)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 复位或者创建指定物理ID的硬件信号量时，输入的局部硬件信号量物理ID非法。
 *
 * 值: 0x0200242c
 *
 * 解决方案: 请确保传入的局部硬件信号量物理ID合法。
 */
#define OS_ERRNO_HWSEM_LOCAL_PHYSICAL_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x2c)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 复位指定物理ID的硬件信号量，并且类型为局部硬件信号量时，输入的Cluster ID非法。
 *
 * 值: 0x0200242d
 *
 * 解决方案: 请确保传入的输入的Cluster ID合法。
 */
#define OS_ERRNO_HWSEM_CLUSTER_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x2d)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：删除硬件信号量资源时，被检测出硬件信号量ID非法。
 *
 * 值: 0x0200242e
 *
 * 解决方案: 硬件信号量ID需是通过SRE_HwSemCreate或者SRE_HwSemCreateById接口所得。
 */
#define OS_ERRNO_HWSEM_DELETE_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x2e)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：SD6157创建局部硬件信号量失败。
 *
 * 值: 0x0200242f
 *
 * 解决方案: SD6157的12核所在的cluster没有局部硬件信号量，不允许创建局部硬件信号量。
 */
#define OS_ERRNO_HWSEM_LOCAL_CTEATE_FAIL SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x2f)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：通道号错误。
 *
 * 值: 0x02002430
 *
 * 解决方案: 全局的通道索引号[0,15]，局部的通道索引号[0,3]。
 */
#define OS_ERRNO_HWSEM_CHNO_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x30)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：通道组号错误。
 *
 * 值: 0x02002431
 *
 * 解决方案: 通道组号[0,3]。
 */
#define OS_ERRNO_HWSEM_CHGNO_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x31)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：核号错误。
 *
 * 值: 0x02002432
 *
 * 解决方案: 请确保核号合法(SD6183:[0,33], Hi1171:[0,13], SD6185x:[0,BOARD_DSP_THREAD_NUM - 1])。
 */
#define OS_ERRNO_HWSEM_COREID_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x32)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：cluster ID错误。
 *
 * 值: 0x02002433
 *
 * 解决方案: 请确保cluster ID合法。
 */
#define OS_ERRNO_HWSEM_CLUSTER_ID_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x33)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：全局硬件信号量单核撤销未找到与指定核VMID相符的通道组。
 *
 * 值: 0x02002434
 *
 * 解决方案: 检查目的核的VMID的配置和全局硬件信号量通道组VMID的配置。
 */
#define OS_ERRNO_HWSEM_CORE_OR_GCHG_VMID_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x34)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: PEND已持有的直接模式硬件信号量。
 *
 * 值：0x03002435
 *
 * 解决方案：不要重复PEND已持有的直接模式硬件信号量。
 */
#define OS_ERRNO_HWSEM_PEND_REPEAT SRE_ERRNO_OS_FATAL(OS_MID_HWSEM, 0x35)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: POST不持有的直接模式硬件信号量。
 *
 * 值：0x03002436
 *
 * 解决方案：不要POST不持有的直接模式硬件信号量，请先PEND再POST。
 */
#define OS_ERRNO_HWSEM_POST_ILLEGAL SRE_ERRNO_OS_FATAL(OS_MID_HWSEM, 0x36)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码: 硬件信号量PEND超时。
 *
 * 值：0x02002437
 *
 * 解决方案：释放硬件信号量资源。
 */
#define OS_ERRNO_HWSEM_PEND_TIMEOUT SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x37)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：局部硬件信号量单核撤销未找到与指定核VMID相符的通道组。
 *
 * 值: 0x02002438
 *
 * 解决方案: 检查目的核的VMID的配置和局部硬件信号量通道组VMID的配置。
 */
#define OS_ERRNO_HWSEM_CORE_OR_LCHG_VMID_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x38)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：硬件信号量单核撤销时全局或者局部都未找到与指定核VMID相符的通道组。
 *
 * 值: 0x02002439
 *
 * 解决方案: 查看CDA记录的错误码，检查目的核的VMID的配置和全局或局部硬件信号量通道组VMID的配置。
 */
#define OS_ERRNO_HWSEM_CORE_OR_CHG_VMID_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x39)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：设置硬件信号量簇状态时传入类型错误。
 *
 * 值: 0x0200243a
 *
 * 解决方案: 当前只支持传入类型为OS_HWSEM_NORMAL或OS_HWSEM_LOW_POWER。
 */
#define OS_ERRNO_HWSEM_CLUSTER_TYPE_ERR SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x3a)

/*
 * @ingroup SRE_hwsem
 * 硬件信号量错误码：硬件信号量簇状态为不可用。
 *
 * 值: 0x0200243b
 *
 * 解决方案: 修改传入的硬件相关参数，如核号，簇号等，避免落入不可用的簇区间。
 */
#define OS_ERRNO_HWSEM_CLUSTER_UNUSABLE SRE_ERRNO_OS_ERROR(OS_MID_HWSEM, 0x3b)

/*
 * @ingroup SRE_hwsem
 * 计数模式硬件信号量处理函数。
 */
typedef void (*HwsemProcFunc)(U32 hwSemId);

/*
 * @ingroup SRE_hwsem
 * 硬件信号量模块创建参数的结构体定义。
 */
struct HwsemCreatePara {
    /* 硬件信号量名称，信号量资源的唯一标识 */
    U32 name;
    /* 硬件信号量的模式，模式当前仅支持互斥SRE_HWSEM_MODE_MUTEX */
    U8 mode;
    /* 硬件信号量的类型，类型为全局SRE_HWSEM_SHARE_GLOBAL或局部SRE_HWSEM_SHARE_CLUSTER */
    U8 type;
    /* 计数模式中断号，目前不使用 */
    U16 hwiNum;
    /* 硬件信号量计数模式回调函数，目前不使用 */
    HwsemProcFunc hwsemHandler;
    /* 硬件信号量计数模式回调函数参数，目前不使用 */
    U32 arg;
};

/*
 * @ingroup  SRE_hwsem
 * @brief 创建硬件信号量。
 *
 * @par 描述
 * 根据用户指定的模式及类型，创建硬件信号量资源。
 *
 * @attention
 * <ul>
 * <li>硬件信号量用于多核之间的互斥。</li>
 * <li>若多个核需要使用同一硬件信号量资源，则每个核均需进行创建，且名字一定要相同。</li>
 * <li>一个硬件信号量资源名对应一个硬件信号量资源，故不同资源需从名字上进行区别开来。</li>
 * <li>当前仅支持互斥访问模式。</li>
 * <li>适用于SD6182及以后平台。</li>
 * <li>由于低功耗模式下，可能会关闭部分局部硬件信号量，所以使用局部硬件信号量资源时，</li>
 * <li>需要用户保证局部硬件信号量资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * </ul>
 *
 * @param createPara [IN]  类型#struct HwsemCreatePara *，硬件信号量创建参数。
 * @param hwSemId    [OUT] 类型#U32 *，存储返回给用户的硬件信号量逻辑ID，
 * 用户使用此ID进行后续的硬件信号量资源申请释放删除等操作。
 *
 * @retval #OS_ERRNO_HWSEM_NAME_INVALID         0x02002401，硬件信号量名非法。
 * @retval #OS_ERRNO_HWSEM_ID_NULL              0x02002403，传入的硬件信号量ID指针为空。
 * @retval #OS_ERRNO_HWSEM_CREATED              0x02002409，同一硬件信号量资源被重复创建。
 * @retval #OS_ERRNO_HWSEM_EXHAUST              0x02002408，无可用的硬件信号量资源。
 * @retval #OS_ERRNO_HWSEM_CREATE_PARA_NULL     0x02002418，硬件信号量参数结构体指针为空。
 * @retval #OS_ERRNO_HWSEM_TYPE_INVALID         0x0200241b，传入的资源类型非法。
 * @retval #OS_ERRNO_HWSEM_MODE_INVALID         0x02002402，创建硬件信号量资源时，传入的模式非法。
 * @retval #OS_ERRNO_HWSEM_LOCAL_CTEATE_FAIL    0x0200242f，12核不允许创建局部硬件信号量(只对SD6157芯片有效)。
 * @retval #SRE_OK                              0x00000000，创建硬件信号量资源成功。
 *
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwSemDelete
 */
extern U32 SRE_HwSemCreate(struct HwsemCreatePara *createPara, U32 *hwSemId);

/*
 * @ingroup  SRE_hwsem
 * @brief 删除硬件信号量。
 *
 * @par 描述
 * 删除指定的硬件信号量资源。
 *
 * @attention
 * <ul>
 * <li>若多个核使用了同一硬件信号量资源，每个核均需调用删除接口释放资源。</li>
 * <li>由于低功耗模式下，可能会关闭部分局部硬件信号量，所以删除局部硬件信号量资源时，</li>
 * <li>需要用户保证局部硬件信号量资源可被正常访问(未被关断)，否则会导致核挂死。</li>
 * <li>适用于SD6182及以后平台。</li>
 * </ul>
 * @param hwSemId [IN]  类型#U32，硬件信号量逻辑ID，通过SRE_HwSemCreate接口所得。
 *
 * @retval #OS_ERRNO_HWSEM_DELETED              0x0200240a，该硬件信号量资源已被删除。
 * @retval #OS_ERRNO_HWSEM_NOT_ANY_TYPE         0x02002404，该硬件信号量类型错误。
 * @retval #OS_ERRNO_HWSEM_GHWSEM_EXCEED_MAX    0x02002406，该硬件信号量资源全局个数非法。
 * @retval #OS_ERRNO_HWSEM_LHWSEM_EXCEED_MAX    0x02002405，该硬件信号量资源局部个数非法。
 * @retval #OS_ERRNO_HWSEM_INT_POINT_NULL       0x02002410，获取硬件信号量管理信息句柄失败。
 * @retval #OS_ERRNO_HWSEM_MODE_NOT_MATCH       0x02002407，该硬件信号量资源模式与记录不匹配。
 * @retval #OS_ERRNO_HWSEM_DELETE_ID_INVALID    0x0200242e，传入的硬件信号量逻辑ID非法。
 * @retval #OS_ERRNO_HWSEM_NOT_POST             0x02002424，该硬件信号量资源还未释放。
 * @retval #SRE_OK                              0x00000000，删除硬件信号量资源成功。
 *
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwSemCreate
 */
extern U32 SRE_HwSemDelete(U32 hwSemId);

/*
 * @ingroup  SRE_hwsem
 * @brief 关中断并获取互斥硬件信号量（旧接口）。
 *
 * @par 描述
 * 间接模式下，关中断获取指定的互斥模式的硬件信号量。
 *
 * @attention
 * <ul>
 * <li>调用本接口后会关中断。如果Pend失败，系统会返回到关中断之前的状态。</li>
 * <li>Pend接口使用的入参一定是硬件信号量创建后返回的ID。</li>
 * <li>Pend接口只适用于互斥模式的硬件信号量。</li>
 * <li>Pend接口调用成功后，在POST之前不可开中断，或者做任务切换，否则有造成系统死锁的风险。</li>
 * <li>适用于SD6155/SD6157/SD6182平台。</li>
 * <li>由于低功耗模式下，可能会关闭部分局部硬件信号量，所以使用局部硬件信号量资源执行Pend操作时，</li>
 * <li>需要用户保证局部硬件信号量资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * </ul>
 *
 * @param hwSemId [IN]  类型#U32，信号量ID。
 * @param intSave [OUT] 类型#uintptr_t *，关中断前的PS状态值。
 *
 * @retval #OS_ERRNO_HWSEM_INTVALUE_NULL       0x02002421，PEND操作时，输入的获取中断开关值的指针为空。
 * @retval #OS_ERRNO_HWSEM_PEND_ID_INVALID     0x02002420，PEND操作时，输入的ID错误。
 * @retval #OS_ERRNO_HWSEM_DELETED             0x0200240a，PEND操作时，硬件信号量未创建或者已经被删除。
 * @retval #SRE_OK                             0x00000000，PEND硬件信号量资源成功。
 *
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwSemPost
 */
extern U32 SRE_HwSemPend(U32 hwSemId, uintptr_t *intSave);

/*
 * @ingroup  SRE_hwsem
 * @brief 恢复中断并释放互斥硬件信号量（旧接口）。
 *
 * @par 描述
 * 间接模式下，恢复中断并释放指定的互斥模式的硬件信号量。
 *
 * @attention
 * <ul>
 * <li>本接口需要跟SRE_HwSemPend配套使用。调用结束之后会开中断。</li>
 * <li>本接口需要使用者保证intSave值的正确性，否则会造成系统异常。</li>
 * <li>Post接口使用的入参一定是硬件信号量创建后返回的ID。</li>
 * <li>Post接口只适用于互斥模式的硬件信号量。</li>
 * <li>调用Post接口成功之前，不可开中断，或者做任务切换，否则有造成系统死锁的风险。</li>
 * <li>适用于SD6155/SD6157/SD6182平台。</li>
 * <li>由于低功耗模式下，可能会关闭部分局部硬件信号量，所以使用局部硬件信号量资源Post操作时，</li>
 * <li>需要用户保证局部硬件信号量资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * </ul>
 *
 * @param hwSemId [IN]  类型#U32，信号量ID。
 * @param intSave [IN]  类型#uintptr_t，关中断Pend操作时返回的值。
 *
 * @retval #OS_ERRNO_HWSEM_POST_ID_INVALID     0x02002422，POST操作时，输入的ID错误。
 * @retval #OS_ERRNO_HWSEM_DELETED             0x0200240a，POST操作时，硬件信号量未创建或者已经被删除。
 * @retval #OS_ERRNO_HWSEM_POST_ILLEGAL        0x03002436，POST不持有的直接模式硬件信号量。
 * @retval #SRE_OK                             0x00000000，POST硬件信号量资源成功。
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwSemPend
 */
extern U32 SRE_HwSemPost(U32 hwSemId, uintptr_t intSave);

/*
 * @ingroup  SRE_hwsem
 * @brief 关中断并获取互斥硬件信号量。
 *
 * @par 描述
 * 间接模式下，关中断获取指定的互斥模式的硬件信号量。
 *
 * @attention
 * <ul>
 * <li>调用本接口后会关中断。如果Pend失败，系统会返回到关中断之前的状态。</li>
 * <li>Pend接口使用的入参一定是硬件信号量创建后返回的ID。</li>
 * <li>Pend接口只适用于互斥模式的硬件信号量。</li>
 * <li>Pend接口调用成功后，在POST之前不可开中断，或者做任务切换，否则有造成系统死锁的风险。</li>
 * <li>适用于SD6185/SD6186/SD6186L及以后平台。</li>
 * <li>由于低功耗模式下，可能会关闭部分局部硬件信号量，所以使用局部硬件信号量资源执行Pend操作时，</li>
 * <li>需要用户保证局部硬件信号量资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * </ul>
 *
 * @param hwSemId [IN]  类型#U32，信号量ID。
 * @param intSave [OUT] 类型#uintptr_t *，关中断前的PS状态值。
 *
 * @retval #OS_ERRNO_HWSEM_INTVALUE_NULL       0x02002421，PEND操作时，输入的获取中断开关值的指针为空。
 * @retval #OS_ERRNO_HWSEM_PEND_ID_INVALID     0x02002420，PEND操作时，输入的ID错误。
 * @retval #OS_ERRNO_HWSEM_DELETED             0x0200240a，PEND操作时，硬件信号量未创建或者已经被删除。
 * @retval #SRE_OK                             0x00000000，PEND硬件信号量资源成功。
 *
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwSemIrqPost
 */
extern U32 SRE_HwSemIrqPend(U32 hwSemId, uintptr_t *intSave);

/*
 * @ingroup  SRE_hwsem
 * @brief 恢复中断并释放互斥硬件信号量。
 *
 * @par 描述
 * 间接模式下，恢复中断并释放指定的互斥模式的硬件信号量。
 *
 * @attention
 * <ul>
 * <li>本接口需要跟SRE_HwSemIrqPend配套使用。调用结束之后会开中断。</li>
 * <li>本接口需要使用者保证intSave值的正确性，否则会造成系统异常。</li>
 * <li>Post接口使用的入参一定是硬件信号量创建后返回的ID。</li>
 * <li>Post接口只适用于互斥模式的硬件信号量。</li>
 * <li>调用Post接口成功之前，不可开中断，或者做任务切换，否则有造成系统死锁的风险。</li>
 * <li>仅适用于SD6185/SD6186/SD6186L及以后平台。</li>
 * <li>由于低功耗模式下，可能会关闭部分局部硬件信号量，所以使用局部硬件信号量资源Post操作时，</li>
 * <li>需要用户保证局部硬件信号量资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * </ul>
 *
 * @param hwSemId [IN]  类型#U32，信号量ID。
 * @param intSave [IN]  类型#uintptr_t，关中断Pend操作时返回的值。
 *
 * @retval #OS_ERRNO_HWSEM_POST_ID_INVALID     0x02002422，POST操作时，输入的ID错误。
 * @retval #OS_ERRNO_HWSEM_DELETED             0x0200240a，POST操作时，硬件信号量未创建或者已经被删除。
 * @retval #OS_ERRNO_HWSEM_POST_ILLEGAL        0x03002436，POST不持有的直接模式硬件信号量。
 * @retval #SRE_OK                             0x00000000，POST硬件信号量资源成功。
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwSemIrqPend
 */
extern U32 SRE_HwSemIrqPost(U32 hwSemId, uintptr_t intSave);

/*
 * @ingroup  SRE_hwsem
 * @brief 复位指定硬件信号量物理ID的硬件信号量。
 *
 * @par 描述
 * 根据指定的硬件信号量物理ID，复位硬件信号量资源。
 *
 * @attention
 * <ul>
 * <li>供SD6183x平台使用。</li>
 * <li>接口即操作了COMM的寄存器又操作了RAT寄存器,操作者需要COMM和RAT权限。</li>
 * <li>调用此接口前，由用户保证待复位的硬件信号量已经被删除。</li>
 * <li>待复位的硬件信号量不能为0号全局硬件信号量和通过启动参数配置的硬件信号量(所有实例)。</li>
 * <li>复位局部硬件信号量资源时，需要用户保证指定的局部硬件信号量资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * </ul>
 *
 * @param type       [IN]  类型#U32，复位指定物理ID的硬件信号量类型，
 * 为全局(#SRE_HWSEM_SHARE_GLOBAL)或者局部(#SRE_HWSEM_SHARE_CLUSTER)硬件信号量。
 * @param clusterId  [IN]  类型#U32，Cluster ID(簇ID)，取值范围为[0,3]。
 * @param hwsemPhyId [IN]  类型#U32，硬件信号量物理ID，取值范围为实例内配置的硬件信号量资源。
 *
 * @retval #OS_ERRNO_HWSEM_TYPE_INVALID              0x0200241b，传入的资源类型非法。
 * @retval #OS_ERRNO_HWSEM_GLB_PHYSICAL_ID_INVALID   0x0200242b，传入的全局硬件信号量ID非法。
 * @retval #OS_ERRNO_HWSEM_CLUSTER_ID_INVALID        0x0200242d，传入的Cluster ID非法。
 * @retval #OS_ERRNO_HWSEM_LOCAL_PHYSICAL_ID_INVALID 0x0200242c，传入的局部硬件信号量ID非法。
 * @retval #SRE_OK                                   0x00000000，复位指定物理ID的硬件信号量资源成功。
 *
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwSemCreateById
 */
extern U32 SRE_HwSemReset(U32 type, U32 clusterId, U32 hwsemPhyId);

/*
 * @ingroup  SRE_hwsem
 * @brief 创建指定硬件信号量物理ID的硬件信号量。
 *
 * @par 描述
 * 根据指定的硬件信号量物理ID，创建硬件信号量资源。
 *
 * @attention
 * <ul>
 * <li>仅供SD6183x平台使用。</li>
 * <li>硬件信号量用于两个核之间的互斥（核级别的互斥）。</li>
 * <li>第一次使用此接口前，必须先调用复位接口(#SRE_HwSemReset)对相应的硬件信号量资源进行复位。</li>
 * <li>待创建的硬件信号量不能为OS占用和实例已经配置的硬件信号量资源。</li>
 * <li>使用局部硬件信号量资源时，需要用户保证指定的局部硬件信号量资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * </ul>
 *
 * @param type       [IN]  类型#U32，创建指定物理ID的硬件信号量类型，
 * 为全局(#SRE_HWSEM_SHARE_GLOBAL)或者局部(#SRE_HWSEM_SHARE_CLUSTER)硬件信号量。
 * @param hwsemPhyId [IN]  类型#U32，硬件信号量物理ID。
 * @param hwSemId    [OUT] 类型#U32 *，存储返回给用户的硬件信号量逻辑ID，
 * 用户使用此ID进行后续的硬件信号量资源申请释放删除等操作。
 *
 * @retval #OS_ERRNO_HWSEM_TYPE_INVALID              0x0200241b，传入硬件信号量类型不可用。
 * @retval #OS_ERRNO_HWSEM_GLB_PHYSICAL_ID_INVALID   0x0200242b，传入的全局硬件信号量ID非法。
 * @retval #OS_ERRNO_HWSEM_LOCAL_PHYSICAL_ID_INVALID 0x0200242c，传入的局部硬件信号量ID非法。
 * @retval #OS_ERRNO_HWSEM_CREATED                   0x02002409，同一硬件信号量资源被重复创建。
 * @retval #OS_ERRNO_HWSEM_LOCAL_CTEATE_FAIL         0x0200242f，12核不允许创建局部硬件信号量(只对SD6157芯片有效)。
 * @retval #SRE_OK                                   0x00000000，创建硬件信号量资源成功。
 *
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwSemCreate | SRE_HwSemReset
 */
extern U32 SRE_HwSemCreateById(U32 type, U32 hwsemPhyId, U32 *hwSemId);

/*
 * @ingroup  SRE_hwsem
 * @brief 设置通道VMID。
 *
 * @par 描述
 * 设置某个cluster的某个通道(8个硬件信号量一个通道)的VMID bit表。
 * @attention
 * <ul>
 * <li>仅供SD6183x平台使用。</li>
 * <li>仅MCPU(单核)调用，否则硬件会上报错误中断。</li>
 * <li>main函数之后，调用硬件信号量的VMID设置，否则可能出现硬件信号量VMID保护不使能的情况。</li>
 * </ul>
 *
 * @param clusterId    [IN]  类型#U32，cluster ID号取值[0,4]，含义参考#OS_HWSEM_CLUSTER_E。
 * @param chNo         [IN]  类型#U32，通道号，全局硬件信号量取值[0,15]，局部硬件信号量取值[0,3]。
 * @param chVmidBitMap [IN]  类型#U32，通道VMID（8bit）取值[0,0xFF]。
 *
 * @retval #SRE_OK                          0x00000000，设置成功。
 * @retval #OS_ERRNO_HWSEM_CLUSTER_ID_ERR   0x02002433，cluster ID错误。
 * @retval #OS_ERRNO_HWSEM_CHNO_ERR         0x02002430，通道号错误。
 * @retval #OS_ERRNO_HWSEM_VMID_BIT_MAP_ERR 0x0200240b，VMID的bit位表的值不正确。
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwsemIntVmidSet
 */
extern U32 SRE_HwsemChVmidSet(U32 clusterId, U32 chNo, U32 chVmidBitMap);

/*
 * @ingroup  SRE_hwsem
 * @brief 中断VMID设置。
 *
 * @par 描述
 * 根据物理核号设置全局硬件信号量和局部(4个cluster)硬件信号量的中断VMID。
 * @attention
 * <ul>
 * <li>仅供SD6183x平台使用。</li>
 * <li>仅MCPU(主核)调用，否则硬件会上报错误中断。</li>
 * <li>main函数之后，调用硬件信号量的VMID设置，否则可能出现硬件信号量VMID保护不使能的情况。</li>
 * </ul>
 *
 * @param coreId        [IN]  类型#U32，核号，取值[0,33]。
 * @param intVmidBitMap [IN]  类型#U32，通道VMID（8bit），取值[0,0xFF]。
 *
 * @retval #SRE_OK                          0x00000000，设置成功。
 * @retval #OS_ERRNO_HWSEM_COREID_ERR       0x02002432，核号错误。
 * @retval #OS_ERRNO_HWSEM_VMID_BIT_MAP_ERR 0x0200240b，VMID的bit位表的值不正确。
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwsemChVmidSet
 */
extern U32 SRE_HwsemIntVmidSet(U32 coreId, U32 intVmidBitMap);

/*
 * @ingroup  SRE_hwsem
 * @brief 通道组VMID设置。
 *
 * @par 描述
 * 按通道组号设置全局硬件信号量和局部(4个cluster)硬件信号量的通道组VMID。
 * @attention
 * <ul>
 * <li>仅供SD6183x平台使用。</li>
 * <li>仅MCPU(主核)调用，否则硬件会上报错误中断。</li>
 * <li>main函数之后，调用硬件信号量的VMID设置，否则可能出现硬件信号量VMID保护不使能的情况。</li>
 * </ul>
 *
 * @param chgNo   [IN]  类型#U32，通道组号，取值[0,3]。
 * @param chgVmid [IN]  类型#U32，通道VMID（3bit），取值[0,7]。
 *
 * @retval #SRE_OK                           0x00000000，设置成功。
 * @retval #OS_ERRNO_HWSEM_VMID_VALUE_ERR    0x0200241c，VMID的值不正确。
 * @retval #OS_ERRNO_HWSEM_CHGNO_ERR         0x02002431，通道组号错误。
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HwsemChVmidSet
 */
extern U32 SRE_HwsemChgVmidSet(U32 chgNo, U32 chgVmid);

/*
 * @ingroup  SRE_hwsem
 * @brief 单核撤销申请。
 *
 * @par 描述
 * 释放指定核持有的硬件信号量并撤销该核的硬件信号量申请请求。
 * @attention
 * <ul>
 * <li>仅供SD6183x平台使用。</li>
 * </ul>
 *
 * @param coreId [IN]  类型#U32，核号，取值[0,33]。
 *
 * @retval #SRE_OK                                  0x00000000，返回成功。
 * @retval #OS_ERRNO_HWSEM_COREID_ERR               0x02002432，核号错误。
 * @retval #OS_ERRNO_HWSEM_CORE_OR_GCHG_VMID_ERR    0x02002434，全局通道组VMID匹配失败。
 * @retval #OS_ERRNO_HWSEM_CORE_OR_LCHG_VMID_ERR    0x02002438，局部通道组VMID匹配失败。
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_HwsemPendCancel(U32 coreId);

enum HwsemState {
    OS_HWSEM_NORMAL,
    OS_HWSEM_LOW_POWER, /* 低功耗之后对应簇HWSEM不可用 */
    OS_HWSEM_STATE_BUTT
};

/*
 * @ingroup  SRE_hwsem
 * @brief 设置硬件信号量簇可用状态。
 *
 * @par 描述
 * 根据用户指定的簇号及簇状态，设置硬件信号量簇状态。
 *
 * @attention
 * <ul>
 * <li>该接口需要在DDR初始化之后调用。</li>
 * <li>该接口需要在硬件信号量初始化之前调用。</li>
 * <li>该接口只对传入参数合法性进行校验，由调用者保证传入参数正确。</li>
 * <li>目前仅适用于SD6186平台。</li>
 * </ul>
 *
 * @param clusterId     [IN] 类型#U32，硬件信号量簇范围，含义参考#enum HwSemCluster。
 * @param state         [IN] 类型#enum HwsemState，硬件信号量簇状态值。
 *
 * @retval #OS_ERRNO_HWSEM_CLUSTER_ID_ERR       0x02002433，cluster ID错误。
 * @retval #OS_ERRNO_HWSEM_CLUSTER_TYPE_ERR     0x0200243a，设置硬件信号量簇状态时传入类型错误。
 * @retval #SRE_OK                              0x00000000，创建硬件信号量资源成功。
 *
 * @par 依赖
 * <ul><li>sre_hwsem.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_HwsemSetState(U32 clusterId, enum HwsemState state);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_HWSEM_H */
