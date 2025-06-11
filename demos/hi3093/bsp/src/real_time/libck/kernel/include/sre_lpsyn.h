/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: LP同步单元模块的头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * @defgroup SRE_lpsyn LP同步单元
 * @ingroup SRE_lpsyn
 */

#ifndef SRE_LPSYN_H
#define SRE_LPSYN_H

#include "sre_base.h"
#include "sre_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * @ingroup SRE_lpsyn
 *
 * 组内同步单元ID
 */
#define LP_INNER_GROUP_SYNC_UNIT_ID 0

/*
 * @ingroup SRE_lpsyn
 *
 * LP同步默认主核超时时间15秒
 */
#define DEFAULT_SYN_OVERTIME 15000

/*
 * @ingroup SRE_lpsyn
 *
 * LP同步默认从主核超时时间12秒
 */
#define DEFAULT_SYN_SECONDARY_OVERTIME 12000

/*
 * @ingroup SRE_lpsyn
 *
 * LP同步阶段枚举
 */
enum LPSynPhase {
    LP_SYNC_TYPE_FETCH = 0, /* 加载阶段 */
    LP_SYNC_TYPE_ACTIVE = 1, /* 激活阶段 */
    LP_SYNC_TYPE_DEACTIVE = 2, /* 去激活阶段 */
};

/*
 * @ingroup SRE_lpsyn
 *
 * LP同步阶段枚举
 */
enum LPSynUnitType {
    LP_SYNC_TYPE_SP = 1, /* 静态补丁类型 */
    LP_SYNC_TYPE_DL = 2, /* 动态加载类型 */
    LP_SYNC_TYPE_DP = 4 /* 动态补丁类型 */
};

/*
 * @ingroup SRE_lpsyn
 *
 * LP同步信息结构体
 */
struct LPSynInfo {
    /* 操作的外部单元号 */
    U32 patchNo;
    /* 单元类型 DL DP SP */
    enum LPSynUnitType unitType;
    /* 加载阶段 激活 去激活 加载 */
    enum LPSynPhase phaseType;
};

/*
 * @ingroup  SRE_lpsyn
 * @brief 定义同步消息发送函数类型。
 *
 * @par 描述
 * 用户通过同步消息发送函数类型定义消息发送钩子函数，LP同步模块调用该类型的钩子函数发送消息进行同步。
 *
 * @attention
 * <li>仅SD6182/SD6157 支持</li>
 *
 * @param cpuMask  [IN]  类型#U32，本次同步CPU的bitMap表。
 * @param groupId  [IN]  类型#U32，哪个组发起的同步，该信息需要由主核传递给从核。
 * @param syncType [IN]  类型#enum LPSynPhase，发送消息的阶段，用于接收消息时针对不同阶段做不同处理。
 *
 * @retval #SRE_OK  0，消息发送成功。
 * @retval 其他，消息发送失败。
 * @par 依赖
 * <ul><li>sre_lpsyn.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef U32(*LPSynMsgHook)(U32 cpuMask, U32 groupId, enum LPSynPhase syncType);

/*
 * @ingroup SRE_lpsyn
 *
 * 同步单元阶段处理函数结构定义。
 */
struct LPSynProFunc {
    /* 同步阶段的处理函数 */
    OsVoidFunc normalHook;
    /* 发送同步消息的钩子函数 */
    LPSynMsgHook sendSynMsgHook;
};

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：其它同步单元正在被使用。
 *
 * 值: 0x02001ec0
 *
 * 解决方案:在进行同步操作前，确认所有的LP同步都已经退出。
 */
#define OS_ERRNO_LPSYN_STARTSYN_USINGOTHERUNIT SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc0)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：发送同步消息失败。
 *
 * 值: 0x02001ec1
 *
 * 解决方案:请参考用户注册的消息发送接口失败原因的解决方法。
 */
#define OS_ERRNO_LPSYN_STARTSYN_SENDMSGFAIL SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc1)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：输入同步CPU掩码表为NULL。
 *
 * 值: 0x02001ec2
 *
 * 解决方案:同步时CPU掩码不能为空。
 */
#define OS_ERRNO_LPSYN_STARTSYN_MASKPTRISNULL SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc2)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码定义: 当前LP组中没有有效的可以同步的核
 *
 * 值: 0x02001ec3
 *
 * 解决方案:确保该LP组中配置的有效核是否已经在线，或者查看需要同步的LP组中的生效核范围是否配置为0。
 */
#define OS_ERROR_LPSYN_NO_CORE_IN_THIS_IMAGE SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc3)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：开始同步时超时。
 *
 * 值: 0x02001ec4
 *
 * 解决方案:尝试增加同步操作时间。
 */
#define OS_ERRNO_LPSYN_STARTSYN_TIMEOUT SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc4)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：没有注册同步消息发送函数。
 *
 * 值: 0x02001ec5
 *
 * 解决方案:请注册同步消息发送函数。
 */
#define OS_ERRNO_LPSYN_STARTSYN_SENDFUNCISNULL SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc5)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：管理组的同步单元没有进入同步状态。
 *
 * 值: 0x02001ec6
 *
 * 解决方案:同步失败，重新发起同步或者检查同步消息处理时用户传入的管理组ID是否为需要同步的有效管理组ID。
 */
#define OS_ERRNO_LPSYN_PROCSYN_STATUSINVALID SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc6)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：执行同步单元超时。
 *
 * 值: 0x02001ec7
 *
 * 解决方案:增加同步超时时间。
 */
#define OS_ERRNO_LPSYN_PROCSYN_TIMEOUT SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc7)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：调用该函数的核不是开始同步的核。
 *
 * 值: 0x02001ec8
 *
 * 解决方案:调用该函数和发起该函数必须是同一个核。
 */
#define OS_ERRNO_LPSYN_PROCSYN_VCPUIDINVALID SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc8)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：同步单元没有进入同步状态。
 *
 * 值: 0x02001ec9
 *
 * 解决方案:重新发起同步。
 */
#define OS_ERRNO_LPSYN_ENDSYN_STATUSINVALID SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xc9)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：结束同步过程超时。
 *
 * 值: 0x02001eca
 *
 * 解决方案:增加同步超时时间。
 */
#define OS_ERRNO_LPSYN_ENDSYN_TIMEOUT SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xca)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：结束同步过程的核不是开始同步的核。
 *
 * 值: 0x02001ecb
 *
 * 解决方案:开始和结束同步必须是同一个核。
 */
#define OS_ERRNO_LPSYN_ENDSYN_VCPUIDINVALID SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xcb)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：输入同步消息发送函数为NULL。
 *
 * 值: 0x02001ecc
 *
 * 解决方案:同步消息的函数的地址不能为NULL。
 */
#define OS_ERRNO_LPSYN_REGSENDHOOK_HOOKISNULL SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xcc)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：输入同步单元的ID大于等于同步单元的最大数目。
 *
 * 值: 0x02001ecd
 *
 * 解决方案:确认同步单元的ID必须为0。
 */
#define OS_ERRNO_LPSYN_SETARGS_SYNIDINVALID SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xcd)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：同步单元没有被注册。
 *
 * 值: 0x02001ece
 *
 * 解决方案:请初始化同步单元。
 */
#define OS_ERRNO_LPSYN_SETARGS_STATUSINVALID SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xce)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：输入同步单元的ID大于等于同步单元的最大数目。
 *
 * 值: 0x02001ecf
 *
 * 解决方案:同步单元ID必须为0。
 */
#define OS_ERRNO_LPSYN_GETARGS_SYNIDINVALID SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd8)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：同步单元没有被注册。
 *
 * 值: 0x02001ed0
 *
 * 解决方案:请初始化同步单元。
 */
#define OS_ERRNO_LPSYN_GETARGS_STATUSINVALID SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd0)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：输入的获取参数地址的指针为NULL。
 *
 * 值: 0x02001ed1
 *
 * 解决方案:确保输入的指针不能NULL。
 */
#define OS_ERRNO_LPSYN_GETARGS_INPUTADDRISNULL SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd1)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：同步模块的ID非法。
 *
 * 值: 0x02001ed2
 *
 * 解决方案:排查同步模块管理信息是否被踩。
 */
#define OS_ERRNO_LPSYN_UNIT_ERROR SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd2)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：主核还没有发起同步。
 *
 * 值: 0x02001ed3
 *
 * 解决方案:查看主核是否发起同步。
 */
#define OS_ERRNO_LPSYN_UNIT_RUNNING SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd3)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：当前核不在同步器同步的范围。
 *
 * 值: 0x02001ed4
 *
 * 解决方案:查看LP组初始化是否成功或者检查当前核处理同步消息时传入的同步管理组ID是否合法。
 */
#define OS_ERRNO_LPSYN_NOT_IN_SYNLIST SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd4)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：主核和从核的同步机不匹配，不在同一个同步中。
 *
 * 值: 0x02001ed5
 *
 * 解决方案:查看主核和从核同步是否匹配。
 */
#define OS_ERRNO_LPSYN_ALREADY_RESTART SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd5)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：同步器中有错误。
 *
 * 值: 0x02001ed6
 *
 * 解决方案:查看整个同步过程是否已经超时。
 */
#define OS_ERRNO_LPSYN_HAS_ERROR SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd6)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：同步信息获取失败。
 *
 * 值: 0x02001ed7
 *
 * 解决方案:查看当前是否在同步过程中，只有同步过程才能获取同步信息。
 */
#define OS_ERRNO_LPSYN_INFO_GET_ERROR SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd7)

/*
 * @ingroup SRE_lpsyn
 * 同步单元错误码：同步信息获取接口入参为NULL。
 *
 * 值: 0x02001ed8
 *
 * 解决方案:查看传入存储同步信息的结构体指针是否为NULL。
 */
#define OS_ERRNO_LPSYN_INFO_PARA_IS_NULL SRE_ERRNO_OS_ERROR(OS_MID_LP, 0xd8)

/*
 * @ingroup  SRE_lpsyn
 * @brief 同步消息处理函数。
 *
 * @par 描述
 * 调用该接口进行同步消息处理。用户接收到同步消息后调用该接口进行同步消息处理
 * （同步消息的发送函数为用户提供，通过#SRE_LPRegSynMsgSendHook接口注册到系统中），该接口内部调用同步执行阶段函数。
 *
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157 支持</li>
 * <li>使用该接口需要锁任务调度 </li>
 * </ul>
 *
 * @param groupId [IN]  类型#U32，动态加载及补丁管理信息组ID，需要用户保证其正确性。
 *
 * @retval #OS_ERRNO_LPSYN_UNIT_ERROR                              0x02002c19，同步模块的ID非法。
 * @retval #OS_ERRNO_LPSYN_UNIT_RUNNING                            0x02002c1a，主核还没有发起同步。
 * @retval #OS_ERRNO_LPSYN_NOT_IN_SYNLIST                          0x02002c1b，当前核不在同步器同步的范围。
 * @retval #OS_ERRNO_LPSYN_ALREADY_RESTART                         0x02002c1c，主核和从核的同步机不匹配，
 * @retval                                                                     不在同一个同步中。
 * @retval #OS_ERRNO_LPSYN_HAS_ERROR                               0x02002c1d，同步器中有错误。
 * @retval #OS_ERRNO_LPSYN_ALREADY_ENDSYN                          0x02002c1e，在主核发起同步时，
 * @retval                                                                     从核已经进入结束同步的状态。
 * @retval #OS_ERRNO_LPSYN_PROCSYN_STATUSINVALID                   0x02002c0c, 管理组的同步单元没有进入同步状态。
 * @retval #SRE_OK                                                 0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_lpsyn.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_LPRegSynMsgSendHook
 */
extern U32 SRE_MCSynProcess(U32 groupId);

/*
 * @ingroup  SRE_lpsyn
 * @brief 注册同步消息发送钩子函数的接口。
 *
 * @par 描述
 * 注册同步消息发送函数的接口，同步消息发送函数由用户根据自己的业务进行定义。
 *
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157 支持</li>
 * <li>如果一个进程需要发起同步，必须先使用该接口注册同步消息发送函数。</li>
 * <li>不支持在LP操作过程中注册修改同步消息发送函数。</li>
 * <li>发送同步消息钩子镜像内唯一，镜像内各组使用同一钩子。</li>
 * </ul>
 *
 * @param hook [IN]  类型#LPSynMsgHook，同步消息发送函数指针。
 *
 * @retval #OS_ERRNO_LPSYN_REGSENDHOOK_HOOKISNULL                  0x02002c13，输入同步消息发送函数为NULL。
 * @retval #SRE_OK                                                 0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_lpsyn.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MCSynProcess
 */
extern U32 SRE_LPRegSynMsgSendHook(LPSynMsgHook hook);

/*
 * @ingroup  SRE_lpsyn
 * @brief 获取同步信息接口。
 *
 * @par 描述
 * 在用户接收到同步消息后，调用该接口获取当前同步状态。
 *
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157 支持</li>
 * <li>该接口必须在同步的时候调用，否则会返回错误码。</li>
 * <li>改接口建议在接收到同步消息后调用。</li>
 * </ul>
 *
 * @param groupId [IN]  类型#U32，动态加载及补丁管理信息组ID，需要用户保证其正确性。
 * @param synInfo [OUT] 类型#struct LPSynInfo *，同步块信息结构体。
 *
 * @retval #OS_ERRNO_LPSYN_INFO_GET_ERROR                         0x02002c1a，同步信息获取失败。
 * @retval #OS_ERRNO_LPSYN_INFO_PARA_IS_NULL                      0x02002c1b，同步信息获取接口入参为NULL。
 * @retval #SRE_OK                                                0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_lpsyn.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MCSynProcess
 */
extern U32 SRE_LpSynInfoGet(U32 groupId, struct LPSynInfo *synInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_LPSYN_H */
