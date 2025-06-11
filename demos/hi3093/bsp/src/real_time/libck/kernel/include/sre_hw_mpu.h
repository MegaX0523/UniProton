/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: mpu模块的外部头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HW_MPU_H
#define SRE_HW_MPU_H

#include "sre_base.h"
#include "sre_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * @ingroup SRE_hw_mpu
 * 表示所有的MPU单元。
 */
#define OS_MPU_ID_ALL 0xFFFF

/*
 * @ingroup SRE_hw_mpu
 * 表示未分配地址的MPU段编号。
 */
#define OS_MPU_UNSIGNED_RANGE_IDX 0xFFFF

/*
 * @ingroup SRE_hw_mpu
 * 表示使能，用于给指定的MPU单元配置段保护属性时，指定该段保护是否使能。
 */
#define OS_MPU_RANGE_ENABLE 1

/*
 * @ingroup SRE_hw_mpu
 * 表示去使能，用于给指定的MPU单元配置段保护属性时，指定该段保护是否使能。
 */
#define OS_MPU_RANGE_DISABLE 0

/*
 * @ingroup SRE_hw_mpu
 * 表示可读可写，用于设置指定Group针对指定MPU段的读写保护属性。
 */
#define OS_MPU_NO_PROTECTED 0

/*
 * @ingroup SRE_hw_mpu
 * 表示可读不可写，用于设置指定Group针对指定MPU段的读写保护属性。
 */
#define OS_MPU_WRITE_PROTECTED 1

/*
 * @ingroup SRE_hw_mpu
 * 表示不可读可写，用于设置指定Group针对指定MPU段的读写保护属性。
 */
#define OS_MPU_READ_PROTECTED 2

/*
 * @ingroup SRE_hw_mpu
 * 表示不可读不可写，用于设置指定Group针对指定MPU段的读写保护属性。
 */
#define OS_MPU_ALL_PROTECTED 3

/*
 * @ingroup SRE_hw_mpu
 * 表示第一次违反的保护操作是违反了读id的保护。
 */
#define OS_MPU_OFFEND_READ_ID_PROTECTED 1

/*
 * @ingroup SRE_hw_mpu
 * 表示第一次违反的保护操作是违反了写id的保护。
 */
#define OS_MPU_OFFEND_WRITE_ID_PROTECTED 2

/*
 * @ingroup SRE_hw_mpu
 * MPU段配置信息结构体定义。
 *
 * 配置指定的MPU属性时指定的参数信息。
 */
struct MpuCfgInfo {
    /*
     * 段ID，不同的MPU能配置的段数目不一样，取值范围也不一样(0 ~
     * N-1，N为最大段数目)。OS_MPU_UNSIGNED_RANGE_IDX代表未分配的地址段
     */
    U16 rangeId;
    /* 是否使能：OS_MPU_RANGE_ENABLE：使能MPU保护，OS_MPU_RANGE_DISABLE：禁止MPU保护 */
    U16 enable;
    /* 段的起始地址，不同MPU有不同的对齐要求 */
    uintptr_t startAddr;
    /* 段的结束地址，不同MPU有不同的对齐要求 */
    uintptr_t endAddr;
    /* 写保护，每个bit代表一个group，1--不允许对应的group写这段地址，0--允许对应的group写这段地址 */
    U32 writeProt;
    /* 读保护，每个bit代表一个group，1--不允许对应的group读这段地址，0--允许对应的group读这段地址 */
    U32 readProt;
};

/*
 * @ingroup SRE_hw_mpu
 * MPU异常时，记录的异常信息。
 *
 * MPU异常时，记录的异常信息结构体。
 */
struct MpuExcInfo {
    /* 异常时访问的地址 */
    uintptr_t faultAddr;
    /* 传输ID，当前无效 */
    U16 tranId;
    /* 发起访问的primary对应的Group ID */
    U8 groupId;
    /* 异常的类型: 01-违反读访问保护，02-违反写访问保护 */
    U8 faultType;
};

/*
 * @ingroup SRE_hw_mpu
 * 表示异常信息。
 */
typedef void (*MpuExcHookFunc)(U16 mpuId, struct MpuExcInfo *mpuExcInfo);

/*
 * MPU模块错误码定义。
 */
/*
 * @ingroup SRE_hw_mpu
 * MPU错误码：输入的Group Id非法。
 *
 * 值: 0x02002101
 *
 * 解决方案: 请确保传入正确的Group Id，范围为[0,31]。
 */
#define OS_ERRNO_MPU_GROUP_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_MPU, 0x01)

/*
 * @ingroup SRE_hw_mpu
 * MPU错误码：输入的MPU Id非法。
 *
 * 值: 0x02002102
 *
 * 解决方案: 请确保传入正确的MPU Id，范围为SD6182:[0,39],SD6157:[0,10]。
 */
#define OS_ERRNO_MPU_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_MPU, 0x02)

/*
 * @ingroup SRE_hw_mpu
 * MPU错误码：用户为指定的MPU单元配置保护属性时，传入的段个数非法
 *
 * 值: 0x02002103
 *
 * 解决方案: 请确保传入的段个数合法(不为0且不超过指定MPU单元硬件支持的最大可划分的MPU段数(包括未划分的段)，
 * 其中DDR为33，sRIO为2，其他为5)。
 */
#define OS_ERRNO_MPU_RANGE_NUM_INVALID SRE_ERRNO_OS_ERROR(OS_MID_MPU, 0x03)

/*
 * @ingroup SRE_hw_mpu
 * MPU错误码：用户输入的段Id非法
 *
 * 值: 0x02002104
 *
 * 解决方案: 请确保传入的段个数合法(为#OS_MPU_ID_ALL或不超过指定MPU单元支持的最大编号(其中DDR为31，sRIO为0，其他为3))。
 */
#define OS_ERRNO_MPU_RANGE_ID_INVALID SRE_ERRNO_OS_ERROR(OS_MID_MPU, 0x04)

/*
 * @ingroup SRE_hw_mpu
 * MPU错误码：用户为指定的MPU单元配置保护属性时，传入的段属性信息缓冲区首地址为NULL
 *
 * 值: 0x02002105
 *
 * 解决方案: 请确保传入有效的缓冲区地址
 */
#define OS_ERRNO_MPU_RANGE_BUFF_NULL SRE_ERRNO_OS_ERROR(OS_MID_MPU, 0x05)

/*
 * @ingroup SRE_hw_mpu
 * MPU错误码：用户输入的保存MPU属性的地址为NULL
 *
 * 值: 0x02002106
 *
 * 解决方案: 请确保传入的保存MPU属性的地址非NULL。
 */
#define OS_ERRNO_MPU_QUERY_PTR_NULL SRE_ERRNO_OS_ERROR(OS_MID_MPU, 0x06)

/*
 * @ingroup SRE_hw_mpu
 * MPU错误码：用户给指定的MPU单元配置段保护属性时，指定的段使能状态非法
 *
 * 值: 0x02002107
 *
 * 解决方案: 请确保传入的段使能状态为#OS_MPU_RANGE_ENABLE或#OS_MPU_RANGE_DISABLE。
 */
#define OS_ERRNO_MPU_RANGE_ENA_STATUS_INVALID SRE_ERRNO_OS_ERROR(OS_MID_MPU, 0x07)

/*
 * @ingroup SRE_hw_mpu
 * MPU错误码：注册MPU异常钩子函数时，已经注册。
 *
 * 值: 0x02002108
 *
 * 解决方案: 请确保注册MPU异常钩子函数时，异常钩子函数未被注册，当前仅支持注册一个MPU异常钩子函数。
 */
#define OS_ERRNO_MPU_EXC_INFO_HOOK_IS_FULL SRE_ERRNO_OS_ERROR(OS_MID_MPU, 0x08)

/*
 * @ingroup  SRE_hw_mpu
 * @brief 给指定的MPU单元配置保护属性。
 *
 * @par 描述
 * 给mpuId指定的MPU单元配置保护属性，MPU段信息缓冲区地址为rangeInfo，配置的段个数为rangeNum。
 *
 * @attention
 * <ul>
 * <li>该操作仅支持SD6182和SD6157平台。</li>
 * <li>该操作不支持ESL平台。</li>
 * <li>用户调用该接口时，需要保证对应的MPU资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * <li>该操作在配置各个MPU段属性的同时会打开该MPU单元的全局MPU使能开关，</li>
 * <li>即若配置段使能，则进行该操作后该MPU段保护就会生效。</li>
 * <li>由于#SRE_MpuDisable，#SRE_MpuRestore以及该接口都会操作MPU使能开关，</li>
 * <li>所以不建议在#SRE_MpuDisable和#SRE_MpuRestore之间调用该接口，否则可能会造成使能状态混乱。</li>
 * <li>MPU保护功能不区分Global/Local和Cacheable/Uncacheable访问，即保护的是实际的物理空间，</li>
 * <li>对各种不同的逻辑地址访问均可做到保护。</li>
 * <li>硬件会根据各个MPU单元的保护粒度自动忽略用户配置的段起始地址和段结束地址的相应低位</li>
 * <li>(保护粒度为4K时，忽略低12bit，保护粒度为1M时，忽略低20bit)，然后将用户输入的起始地址相应低位赋0后</li>
 * <li>组成保护的起始地址，将用户输入的结束地址相应低位赋1后组成保护的结束地址。</li>
 * <li>对于#OS_MPU_HACCC_CLUST_LSIP和#OS_MPU_MAIN_CLUST_LSIP这两个MPU单元，硬件设计的目的只用于保护HACCC寄存器，</li>
 * <li>若用户配置成其他地址段，可能会引发硬件逻辑错误。当传入的rangeId为#OS_MPU_UNSIGNED_RANGE_IDX时，</li>
 * <li>表示对该MPU的未划分地址段(即该MPU所有保护区间中除了已划分段外的其他所有地址空间)进行段属性配置，</li>
 * <li>并且该MPU作用范围由用户保证。</li>
 * </ul>
 * @param mpuId         [IN]  类型#U16，MPU ID号，取值范围为SD6182:[0,39],SD6157:[0,10]。
 * @param rangeNum      [IN]  类型#U16，MPU段信息缓冲区单元个数。对于DDR和SRIO MPU，
 * 取值范围分别为(0,33]和(0,2],对于其它MPU，取值范围为(0,5]。
 * @param rangeInfo     [IN]  类型#const struct MpuCfgInfo *，MPU段信息缓冲区首地址。
 *
 * @retval #OS_ERRNO_MPU_ID_INVALID                 0x02002102，输入的MPU Id非法。
 * @retval #OS_ERRNO_MPU_RANGE_NUM_INVALID          0x02002103，用户为指定的MPU单元配置保护属性时，传入的段个数非法。
 * @retval #OS_ERRNO_MPU_RANGE_ID_INVALID           0x02002104，用户输入的段Id非法。
 * @retval #OS_ERRNO_MPU_RANGE_BUFF_NULL            0x02002105，用户为指定的MPU单元配置保护属性时，
 * @retval                                                      传入的段属性信息缓冲区首地址为NULL。
 * @retval #OS_ERRNO_MPU_RANGE_ENA_STATUS_INVALID   0x02002107，用户给指定的MPU单元配置段保护属性时，
 * @retval                                                      指定的段使能状态非法。
 * @retval #SRE_OK                                  0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_mpu.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MpuDisable | SRE_MpuRestore
 */
extern U32 SRE_MpuCfg(U16 mpuId, U16 rangeNum, const struct MpuCfgInfo *rangeInfo);

/*
 * @ingroup  SRE_hw_mpu
 * @brief 设置指定的Group针对指定的MPU段的读写属性。
 *
 * @par 描述
 * 设置指定Group针对由mpuId和rangeId指定的MPU段的读写属性。
 *
 * @attention
 * <ul>
 * <li>该操作仅支持SD6182和SD6157平台。</li>
 * <li>该操作不支持ESL平台。</li>
 * <li>用户调用该接口时，需要保证对应的MPU资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * <li>若输入的rangeId为OS_MPU_UNSIGNED_RANGE_IDX，表示设置的未分配地址的MPU段读写属性。</li>
 * <li>若指定的MPU段未被调用SRE_MpuCfg接口设置过，则设置的读写属性不生效。</li>
 * </ul>
 * @param groupId [IN]  类型#U16，Group ID号，范围[0,31]。
 * @param mpuId   [IN]  类型#U16，MPU ID号，范围SD6182:[0,39],SD6157:[0,10]。
 * @param rangeId [IN]  类型#U16，MPU段的ID号，范围请参照对应MPU单元的硬件规格。
 * @param wrProt  [IN]  类型#U16，欲设置的读写属性，bit0代表写保护，值1--不允许写，值0--允许写；bit1代表读保护，
 * 值1--不允许读，值0--允许读。
 *
 * @retval #OS_ERRNO_MPU_GROUP_ID_INVALID      0x02002101，输入的Group Id非法。
 * @retval #OS_ERRNO_MPU_ID_INVALID            0x02002102，输入的MPU Id非法。
 * @retval #OS_ERRNO_MPU_RANGE_ID_INVALID      0x02002104，用户输入的段Id非法。
 * @retval #SRE_OK                             0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_mpu.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MpuDisable | SRE_MpuRestore
 */
extern U32 SRE_MpuMasterCfg(U16 groupId, U16 mpuId, U16 rangeId, U16 wrProt);

/*
 * @ingroup  SRE_hw_mpu
 * @brief 查询指定的Group针对指定的MPU段的读写属性。
 *
 * @par 描述
 * 查询指定Group针对由mpuId和rangeId指定的MPU段的读写属性，结果保存在wrProt。
 *
 * @attention
 * <ul>
 * <li>该操作仅支持SD6182和SD6157平台。</li>
 * <li>该操作不支持ESL平台。</li>
 * <li>用户调用该接口时，需要保证对应的MPU资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * <li>若输入的rangeId为OS_MPU_RANGE_ID_ALL，表示查询的未分配地址的MPU段读写属性。</li>
 * </ul>
 * @param groupId [IN]  类型#U16，Group ID号，范围[0,31]。
 * @param mpuId   [IN]  类型#U16，MPU ID号，范围SD6182:[0,39],SD6157:[0,10]。
 * @param rangeId [IN]  类型#U16，MPU段的ID号，范围请参照对应MPU单元的硬件规格。
 * @param wrProt  [OUT] 类型#U16 *，查询出来的读写属性，bit0代表写保护，值1--不允许写，值0--允许写；bit1代表读保护，
 * 值1--不允许读，值0--允许读。
 *
 * @retval #OS_ERRNO_MPU_GROUP_ID_INVALID      0x02002101，输入的Group Id非法。
 * @retval #OS_ERRNO_MPU_ID_INVALID            0x02002102，输入的MPU Id非法。
 * @retval #OS_ERRNO_MPU_RANGE_ID_INVALID      0x02002104，用户输入的段Id非法。
 * @retval #OS_ERRNO_MPU_QUERY_PTR_NULL        0x02002106，输入的保存查询结果的地址为NULL。
 * @retval #SRE_OK                             0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_mpu.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MpuDisable | SRE_MpuRestore
 */
extern U32 SRE_MpuMasterQuery(U16 groupId, U16 mpuId, U16 rangeId, U16 *wrProt);

/*
 * @ingroup  SRE_hw_mpu
 * @brief 禁止指定MPU单元的保护功能。
 *
 * @par 描述
 * 禁止mpuId指定的MPU单元的保护功能。
 *
 * @attention
 * <ul>
 * <li>该操作仅支持SD6182和SD6157平台。</li>
 * <li>该操作不支持ESL平台。</li>
 * <li>用户调用该接口时，需要保证对应的MPU资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * <li>若输入的mpuId为#OS_MPU_ID_ALL，表示禁止所有MPU单元的保护功能。</li>
 * <li>去使能后，恢复使能前，不能进行mpu的配置操作，否则下次去使能不成功。</li>
 * </ul>
 * @param mpuId [IN]  类型#U16，MPU ID号，范围SD6182:[0,39],SD6157:[0,10]或者等于OS_MPU_ID_ALL。
 *
 * @retval #OS_ERRNO_MPU_ID_INVALID            0x02002102，输入的MPU Id非法。
 * @retval #SRE_OK                             0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_mpu.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MpuRestore
 */
extern U32 SRE_MpuDisable(U16 mpuId);

/*
 * @ingroup  SRE_hw_mpu
 * @brief 恢复指定MPU单元的保护功能。
 *
 * @par 描述
 * 恢复mpuId指定的MPU单元的保护功能。
 *
 * @attention
 * <ul>
 * <li>该操作仅支持SD6182和SD6157平台。</li>
 * <li>该操作不支持ESL平台。</li>
 * <li>用户调用该接口时，需要保证对应的MPU资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * <li>若输入的mpuId为#OS_MPU_ID_ALL，表示恢复所有MPU单元的保护功能。</li>
 * </ul>
 * @param mpuId [IN]  类型#U16，MPU ID号，范围SD6182:[0,39],SD6157:[0,10]或者等于OS_MPU_ID_ALL。
 *
 * @retval #OS_ERRNO_MPU_ID_INVALID            0x02002102，输入的MPU Id非法。
 * @retval #SRE_OK                             0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_mpu.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MpuDisable
 */
extern U32 SRE_MpuRestore(U16 mpuId);

/*
 * @ingroup  SRE_hw_mpu
 * @brief 查询MPU错误中断。
 *
 * @par 描述
 * 查询mpuId指定的MPU错误中断。若存在MPU中断，查询之后，会清除相对应的中断。
 *
 * @attention
 * <ul>
 * <li>该操作仅支持SD6182和SD6157平台。</li>
 * <li>该操作不支持ESL平台。</li>
 * <li>用户调用该接口时，需要保证对应的MPU资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * <li>若查询的是DDR MPU，并且已经注册MPU异常钩子函数时，若存在多个AXI口的MPU中断时，会多次调用异常钩子函数。</li>
 * <li>若查询的是DDR MPU，当违反MPU保护属性时，返回给用户的异常地址是实际异常地址的低30Bit。</li>
 * <li>若查询的是LL2 MPU，当存在违反MPU保护属性时，返回给用户的异常地址是全局Uncache地址。</li>
 * <li>调用初始化MPU Dfx功能接口后，才能调用此接口，否则会返回相关错误码。</li>
 * </ul>
 * @param mpuId [IN]  类型#U16，MPU ID号，范围SD6182:[0,39],SD6157:[0,10]。
 *
 * @retval #OS_ERRNO_MPU_ID_INVALID            0x02002102，输入的MPU Id非法。
 * @retval #SRE_OK                             0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_mpu.h：该接口声明所在的头文件。</li></ul>
 * @see None
 */
extern U32 SRE_MpuIntQuery(U16 mpuId);

/*
 * @ingroup  SRE_hw_mpu
 * @brief 注册MPU异常钩子函数。
 *
 * @par 描述
 * 注册MPU异常钩子函数。
 *
 * @attention
 * <ul>
 * <li>该操作仅支持SD6182和SD6157平台。</li>
 * <li>该操作不支持ESL平台。</li>
 * <li>输入参数hook为NULL时，表示取消MPU异常钩子函数。</li>
 * <li>当用户使用#SRE_MpuIntQuery查询MPU错误中断信息时，若存在错误中断，则该接口注册的MPU异常钩子函数会被调用。</li>
 * </ul>
 * @param hook [IN]  类型#MpuExcHookFunc，MPU异常钩子函数。
 *
 * @retval #OS_ERRNO_MPU_EXC_INFO_HOOK_IS_FULL 0x02002108，注册MPU异常钩子函数时，已经注册。
 * @retval #SRE_OK                             0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_mpu.h：该接口声明所在的头文件。</li></ul>
 * @see None
 */
extern U32 SRE_MpuExcHookReg(MpuExcHookFunc hook);

/*
 * @ingroup  SRE_hw_mpu
 * @brief 初始化MPU Dfx功能。
 *
 * @par 描述
 * 初始化MPU Dfx功能, 使能MPU错误中断。
 *
 * @attention
 * <ul>
 * <li>该操作仅支持SD6182和SD6157平台。</li>
 * <li>该操作不支持ESL平台。</li>
 * <li>用户调用该接口时，需要保证待初始化DFX功能的MPU资源可被正常使用(未被关断)，否则会导致核挂死。</li>
 * <li>若输入的mpuId为#OS_MPU_ID_ALL，表示初始化所有MPU DFX功能。</li>
 * <li>如果多个核调用此接口，核间互斥由用户保证。</li>
 * </ul>
 * @param mpuId [IN]  类型#U16，MPU ID号，范围SD6182:[0,39],SD6157:[0,10]或者等于OS_MPU_ID_ALL。
 *
 * @retval #OS_ERRNO_MPU_ID_INVALID            0x02002102，输入的MPU Id非法。
 * @retval #SRE_OK                             0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_hw_mpu.h：该接口声明所在的头文件。</li></ul>
 * @see None
 */
extern U32 SRE_MpuDfxInit(U16 mpuId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */
#endif /* SRE_HW_MPU_H */
