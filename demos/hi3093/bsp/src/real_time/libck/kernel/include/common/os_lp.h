/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: 多核LP模块基础头文件，用户不能直接引用
 * Author: LibCK项目组
 * Create: 2021-01-16
 * History: 2021-01-16 LibCK项目组 创建文件
 */
#ifndef OS_LP_H
#define OS_LP_H

#include "os_typedef.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单元号非法，为0值或者超过了最大的配置。
 *
 * 值: 0x02001e01
 *
 * 解决方案:请检查LP单元号合法性
 */
#define OS_ERRNO_LP_NO_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x01)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态补丁中包含无效符号。
 *
 * 值: 0x02001e02
 *
 * 解决方案:动态补丁中不允许定义全局变量，以及非补丁全局函数
 */
#define OS_ERRNO_LP_INVALID_DP_SYM OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x02)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态补丁中没有补丁函数。
 *
 * 值: 0x02001e03
 *
 * 解决方案:确保动态补丁中至少包含一个补丁函数
 */
#define OS_ERRNO_LP_NO_PAT_FUNC OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x03)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP文件结构变更升级版本号错误。
 *
 * 值: 0x02001e05
 *
 * 解决方案:请检查制作LP文件工具端的LP文件结构协议版本号与SRE_LPGetVersion获取的版本号是否一致,
 * 检查工具端协议版本是否正确
 */
#define OS_ERRNO_LP_PROTOCOL_VER_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x05)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP文件重复加载。
 *
 * 值: 0x02001e06
 *
 * 解决方案:检查代码中是否有LP文件重复加载，并修改
 */
#define OS_ERRNO_LP_LOADED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x06)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 静态补丁文件中代码段与配置中的代码段不一致。
 *
 * 值: 0x02001e07
 *
 * 解决方案:请检查makepatch.ini文件中的静态补丁代码段配置与SRE_LPGroupInfoReg接口中的配置是否相同
 */
#define OS_ERRNO_LP_CODE_ADDR_NOT_SUIT OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x07)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 静态补丁文件中数据段与配置中的共享数据段或私有数据段不一致。
 *
 * 值: 0x02001e08
 *
 * 解决方案:请检查makepatch.ini文件中的静态补丁共享数据段和私有数据段配置与SRE_LPGroupInfoReg接口中的配置是否相同
 */
#define OS_ERRNO_LP_DATA_ADDR_NOT_SUIT OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x08)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP操作中，LP单元未加载.
 *
 * 值: 0x02001e09
 *
 * 解决方案:请确认LP单元已加载.
 */
#define OS_ERRNO_LP_NOT_LOADED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x09)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 该LP单元已经处于激活状态。
 *
 * 值: 0x02001e0a
 *
 * 解决方案:无
 */
#define OS_ERRNO_LP_IS_ACTIVE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x0a)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 该LP单元已经处于去激活状态。
 *
 * 值: 0x02001e0c
 *
 * 解决方案:无
 */
#define OS_ERRNO_LP_IS_DEACTIVE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x0c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 参数指针为空。
 *
 * 值: 0x02001e0d
 *
 * 解决方案:查看输入的指针变量是否为空
 */
#define OS_ERRNO_LP_INPUT_IS_NULL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x0d)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 缓冲区长度不够，或者缓冲区长度为0，请检查入参是否正确。
 *
 * 值: 0x02001e0e
 *
 * 解决方案:查看输入的缓冲区长度
 */
#define OS_ERRNO_LP_BUF_NOT_ENOUGH OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x0e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 内存申请失败，LP操作过程中需要动态申请内存。
 *
 * 值: 0x02001e0f
 *
 * 解决方案:请将内存空间适当配大
 */
#define OS_ERRNO_LP_MEM_NOT_ENOUGH OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x0f)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态库/补丁中未找到对应的段。
 *
 * 值: 0x02001e10
 *
 * 解决方案:查看输入的段名是否正确
 */
#define OS_ERRNO_LP_SEC_NOT_FOUND OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x10)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 静态补丁单元数据段开始地址小于补丁区数据段首地址，或者静态补丁单元数据段超出补丁区数据段范围。
 *
 * 值: 0x02001e13
 *
 * 解决方案:请检查静态补丁单元数据段开始地址或适当增加补丁区数据段大小
 */
#define OS_ERRNO_LP_DATA_OVER_FLOW OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x13)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 静态补丁或者动态补丁删除过程中，原函数指令被破坏，不能回退补丁状态。
 *
 * 值: 0x02001e14
 *
 * 解决方案:静态补丁或者动态补丁删除过程中，原函数指令被破坏，不能回退补丁状态
 */
#define OS_ERRNO_LP_INSTRUCTION_DESTROYED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x14)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 静态补丁或者动态补丁加载的补丁函数个数超过了最大的限制。
 *
 * 值: 0x02001e15
 *
 * 解决方案:请检查配置接口中最大函数个数是否正确
 */
#define OS_ERRNO_LP_FUNC_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x15)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 入参中的LP文件长度不是正确的LP文件长度
 *
 * 值: 0x02001e16
 *
 * 解决方案:检查LP文件正确性，或者重新制作LP文件
 */
#define OS_ERRNO_LP_FILE_IS_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x16)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 重复注册的LP组信息不一致。
 *
 * 值: 0x02001e17
 *
 * 解决方案:确保LP组信息重复注册时注册的组信息参数必须跟前一次相同(包括镜像ID，补丁区或段区配置参数等)
 */
#define OS_ERRNO_LP_INFO_REPEAT_DIFFER OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x17)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 指定镜像的组信息未被初始化。
 *
 * 值: 0x02001e18
 *
 * 解决方案:检查对应镜像中是否有核没有调用SRE_LPGroupInfoReg初始化或者初始化失败或者镜像DSSNUM配置错误。
 */
#define OS_ERRNO_LP_MANAGE_UNIT_NOT_USED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x18)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载，动态补丁段名配置相同
 *
 * 值: 0x02001e19
 *
 * 解决方案:修改相同的段名为不同段名。
 */
#define OS_ERRNO_LP_SEC_NAME_SAME OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x19)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP启动同步过程失败，超时。
 *
 * 值: 0x02001e1a
 *
 * 解决方案:无
 */
#define OS_ERRNO_LP_START_SYN_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x1a)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载或动态补丁调用非补丁替换函数CALL重定位错误。
 *
 * 值: 0x02001e1b
 *
 * 解决方案:确认符号是否4字节对齐或超过call调用范围。
 */
#define OS_ERRNO_LP_CALL_OFFECT_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x1b)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP去激活时LP操作同步过程失败。
 *
 * 值: 0x02001e1c
 *
 * 解决方案:无
 */
#define OS_ERRNO_LP_DEACTIVE_SYN_PHASE_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x1c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 同步过程中需要申请共享内存，共享内存申请失败。
 *
 * 值: 0x02001e1d
 *
 * 解决方案:请配置合理的共享内存空间。
 */
#define OS_ERRNO_LP_ALLOC_COMM_MEM_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x1d)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 设置同步参数失败。
 *
 * 值: 0x02001e1e
 *
 * 解决方案:无
 */
#define OS_ERRNO_LP_SET_PARA_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x1e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP包的FLAG不正确，不等于"DL"、"DP"。
 *
 * 值: 0x02001e20
 *
 * 解决方案:检查版本是否制作正确，第一个LP标志是否是"DL"、"DP"之一。
 */
#define OS_ERRNO_LP_IDENT_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x20)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP加载操作或获取LP文件中的最大LP单元号时传入的缓冲区地址(buf)非4字节对齐。
 *
 * 值: 0x02001e22
 *
 * 解决方案:保证入参缓冲区地址(buf)4字节对齐。
 */
#define OS_ERRNO_LP_BUF_ADDR_NOT_ALIGN OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x22)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 配置的补丁区或段区(起始地址及长度)未按照cache
 * line对齐(SD6181/SD6108为64字节，SD6182为128字节)，或者私有补丁区未按照4字节对齐。
 *
 * 值: 0x02001e23
 *
 * 解决方案:重新配置补丁区或段区的起始地址及长度。
 */
#define OS_ERRNO_LP_REG_NOT_ALIGN OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x23)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单元最大函数个数配置为0。
 *
 * 值: 0x02001e24
 *
 * 解决方案:检查配置的LP单元最大函数个数
 */
#define OS_ERRNO_LP_REG_FUNC_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x24)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 查询动态加载或动态补丁的段信息时指定LP组的LP类型错误。
 *
 * 值: 0x02001e25
 *
 * 解决方案:使用SRE_LPSectionInfoGet接口查询指定LP组段信息时对应LP类型不能为静态补丁类型(LP_TYPE_SP)。
 */
#define OS_ERRNO_LP_SEC_INFO_QUERY_LP_TYPE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x25)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载动态补丁ELF段对齐错误。
 *
 * 值: 0x02001e26
 *
 * 解决方案:查看动态加载或动态补丁ELF文件是否有问题。
 */
#define OS_ERRNO_LP_SEC_ALIGN_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x26)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载动态补丁符号表段头中每个符号节点的大小错误
 *
 * 值: 0x02001e28
 *
 * 解决方案:查看动态加载或动态补丁ELF文件是否有问题。
 */
#define OS_ERRNO_LP_ITEM_SIZE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x28)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 注册保存LP单元状态的钩子函数为空。
 *
 * 值: 0x02001e29
 *
 * 解决方案:排查注册保存LP单元状态的钩子函数是否为空。
 */
#define OS_ERRNO_LP_REG_STATESAVEHOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x29)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 镜像ID错误
 *
 * 值: 0x02001e2a
 *
 * 解决方案:检查配置的镜像ID是否小于镜像数或者配置的镜像ID是否与其他镜像ID冲突。
 */
#define OS_ERRNO_LP_IMAGE_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x2a)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP配置不一致
 *
 * 值: 0x02001e2b
 *
 * 解决方案:请确保补丁的最大LP管理信息组个数、分区号、分区地址、分区长度、芯片类型配置一致
 */
#define OS_ERRNO_LP_CONFIG_NOT_SAME OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x2b)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 最大补丁和动态加载库的总单元个数配置无效
 *
 * 值: 0x02001e2c
 *
 * 解决方案:请确保LP总单元数在config文件中配置范围为[1,9999]
 */
#define OS_ERRNO_LP_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x2c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 供LP使用的分区起始地址非法
 *
 * 值: 0x02001e2d
 *
 * 解决方案:确保分区起始地址(#OS_LP_SHARE_NC_PT_ADDR)非0
 */
#define OS_ERRNO_LP_MEMPT_ADDR_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x2d)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 供LP使用的分区大小为0
 *
 * 值: 0x02001e2e
 *
 * 解决方案:确保分区大小(#OS_PATCH_SHARE_NC_PT_SIZE)非0
 */
#define OS_ERRNO_LP_MEMPT_SIZE_IS_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x2e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP的代码区超过了0x40000000
 *
 * 值: 0x02001e2f
 *
 * 解决方案:SD6182的代码均放在了0x4000000以下的地址空间，由于XTENSA核只支持1G空间内的函数调用，
 * 若将LP的代码区放置在大于等于0x40000000的空间，LP激活后，被打补丁函数将无法返回其父函数。因此，
 * LP的代码区应该配置在0x40000000以下的空间。
 */
#define OS_ERRNO_LP_CODE_PUT_IN_ANOTHER_1G OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x2f)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 补丁区或段区个数错误
 *
 * 值: 0x02001e30
 *
 * 解决方案:补丁区或段区个数不能小于1
 */
#define OS_ERRNO_LP_AREA_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x30)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 输入的LP单元状态不正确,其中空闲状态也为非法。
 *
 * 值: 0x02001e39
 *
 * 解决方案:请填写正确的LP单元状态
 */
#define OS_ERRNO_LP_STATE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x39)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载、动态补丁段属性错误。
 *
 * 值: 0x02001e3b
 *
 * 解决方案:检查ELF文件是否有问题。
 */
#define OS_ERRNO_LP_SECTION_FLAG_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x3b)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载或动态补丁时申请内存失败。
 *
 * 值: 0x02001e3c
 *
 * 解决方案:增大LP的内存分区大小。
 */
#define OS_ERRNO_LP_MEM_ALLOC_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x3c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载重定位段，段头大小错误。
 *
 * 值: 0x02001e3d
 *
 * 解决方案:检查ELF文件是否有问题。
 */
#define OS_ERRNO_LP_REL_ENTSIZE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x3d)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载或动态补丁时ELF文件头错误。
 *
 * 值: 0x02001e3e
 *
 * 解决方案:检查ELF文件是否有问题。
 */
#define OS_ERRNO_LP_ELF_HEAD_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x3e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载或动态补丁时ELF段表头错误。
 *
 * 值: 0x02001e3f
 *
 * 解决方案:检查ELF文件是否有问题。
 */
#define OS_ERRNO_LP_ELF_SEC_HEAD_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x3f)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载或动态补丁时ELF段信息校验错误。
 *
 * 值: 0x02001e40
 *
 * 解决方案:检查ELF文件是否有问题。
 */
#define OS_ERRNO_LP_ELF_SEC_NUM_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x40)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载或动态补丁加载时获取段信息错误。
 *
 * 值: 0x02001e41
 *
 * 解决方案:检查动态加载动态补丁的所属的段是否已经注册。
 */
#define OS_ERRNO_LP_SEC_INFO_GET_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x41)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP组信息注册，入参指针为空。
 *
 * 值: 0x02001e42
 *
 * 解决方案:检查LP组信息注册时入参指针是否为空。
 */
#define OS_ERRNO_LP_PARA_NULL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x42)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP加载时，LP包的类型和核支持的类型不符。
 *
 * 值: 0x02001e43
 *
 * 解决方案:检查核注册的类型和LP包的类型是否一致。
 */
#define OS_ERRNO_LP_FETCH_TYPE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x43)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态补丁替换的函数个数大于用户配置的补丁单元最大函数替换个数。
 *
 * 值: 0x02001e44
 *
 * 解决方案:增大LP单元最大补丁替换函数的配置，或者减少动态补丁函数替换的个数。
 */
#define OS_ERRNO_LP_FETCH_REPLACE_FUNC_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x44)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载符号和运行版本的符号重复定义或者动态补丁全局变量出现重复定义。
 *
 * 值: 0x02001e45
 *
 * 解决方案:检查动态加载文件里面符号和运行版本的符号是否重复或者检查动态补丁文件里面定义的变量是否重复，重复将其删除。
 */
#define OS_ERRNO_LP_FETCH_DL_FUNC_REDEFINE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x45)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 重定位符号对应的指令类型不支持
 *
 * 值: 0x02001e46
 *
 * 解决方案:查看该目标文件编译的核是否跟OS支持的核匹配，包括指令架构、编译选项等。
 */
#define OS_ERRNO_LP_INST_TYPE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x46)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 注册的LP类型错误。
 *
 * 值: 0x02001e47
 *
 * 解决方案:排查注册的LP类型是否是#LP_TYPE枚举的类型之一,对于CORTEX-AX平台，类型必须为LP_TYPE_DP。
 */
#define OS_ERRNO_LP_REG_LP_TYPE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x47)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP模块未初始化，注册LP组信息的镜像没有经过初始化。
 *
 * 值: 0x02001e48
 *
 * 解决方案:检查是否OS中LP模块裁剪开关没打开，或者初始化没有完成。
 */
#define OS_ERRNO_LP_NOT_INITED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x48)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态补丁动态加载配置段区为空。
 *
 * 值: 0x02001e4a
 *
 * 解决方案:检查是否未配置动态补丁动态加载段区或者LP类型错误。
 */
#define OS_ERRNO_LP_DLDPAREAINFO_IS_NULL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x4a)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载动态补丁的段区的起始地址或者长度配置错误
 *
 * 值: 0x02001e4b
 *
 * 解决方案:动态加载动态补丁的段区的起始地址、长度均不能为0
 */
#define OS_ERRNO_LP_CFG_SEC_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x4b)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载动态补丁的段区名字为空
 *
 * 值: 0x02001e4c
 *
 * 解决方案:动态加载动态补丁的段区名字不能为空
 */
#define OS_ERRNO_LP_CFG_SEC_NAME_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x4c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 节区表内存申请错误。
 *
 * 值: 0x02001e4d
 *
 * 解决方案:配大LP内存管理分区的大小。
 */
#define OS_ERRNO_LP_SECTAB_MEM_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x4d)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 读节区表错误。
 *
 * 值: 0x02001e4e
 *
 * 解决方案:ELF文件的格式与要求的不符合。
 */
#define OS_ERRNO_LP_SECTAB_READ_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x4e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载和动态补丁加载时，RO_DATA段溢出
 *
 * 值: 0x02001e4f
 *
 * 解决方案:增大RO_DATA段的空间。
 */
#define OS_ERRNO_LP_RO_DATA_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x4f)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 符号重定位时L32R指令重定位错误，可能是偏移量大于0x3FFFF，或者偏移量非4字节对齐
 *
 * 值: 0x02001e50
 *
 * 解决方案:查看段配置是否有问题。
 */
#define OS_ERRNO_LP_L32R_OFFECT_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x50)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 节区个数错误。
 *
 * 值: 0x02001e51
 *
 * 解决方案:ELF的段个数不能为0，请确认补丁或动态加载文件的正确性。
 */
#define OS_ERRNO_LP_SEC_NUM_ADDR_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x51)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 临时符号表生成错误。
 *
 * 值: 0x02001e52
 *
 * 解决方案:SD6182平台,尝试配大LP管理分区大小，请确认补丁或动态加载文件的正确性。
 */
#define OS_ERRNO_LP_SYM_BUILD_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x52)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LOCAL L2地址不允许配置为全局地址(Cluster Local L2 Global Address)。
 *
 * 值: 0x02001e53
 *
 * 解决方案:SD6181/SD6108/SD6182，使用SP将私有补丁区配置在核Local L2地址时，请使用私有地址（(0,
 * 0x0807FFFF)），而不是全局地址（[0x90000000, 0x9A47FFFF]）。
 */
#define OS_ERRNO_LP_GLB_L2_ADDR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x53)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP激活时对应LP单元的LP类型错误。
 *
 * 值: 0x02001e54
 *
 * 解决方案:LP加载时LP类型是否正确。
 */
#define OS_ERRNO_LP_SYN_SET_TYPE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x54)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载，动态补丁需要依赖符号表模块。
 *
 * 值: 0x02001e55
 *
 * 解决方案:使用动态加载动态补丁需要打开符号表模块。
 */
#define OS_ERRNO_LP_DEPEND_SYMBOL_INITED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x55)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载包和动态补丁包加载之后，代码段溢出
 *
 * 值: 0x02001e56
 *
 * 解决方案:增大代码段的空间。
 */
#define OS_ERRNO_LP_CODE_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x56)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载包和动态补丁包加载之后，DATA段溢出
 *
 * 值: 0x02001e57
 *
 * 解决方案:增大DATA段的空间。
 */
#define OS_ERRNO_LP_DATA_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x57)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态加载包和动态补丁包加载之后，BSS段溢出
 *
 * 值: 0x02001e58
 *
 * 解决方案:增大BSS段的空间。
 */
#define OS_ERRNO_LP_BSS_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x58)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP管理组ID在使用中，不能去注册该LP信息管理组。
 *
 * 值: 0x02001e59
 *
 * 解决方案:排查LP管理组信息去注册时动态加载及LP管理信息组ID管理的所有LP单元是否没有处于空闲状态。
 */
#define OS_ERRNO_LP_GROUP_ID_IN_USE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x59)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 申请LP单元控制块失败
 *
 * 值: 0x02001e5b
 *
 * 解决方案:检查加载的LP单元个数是否已经大于配置的最大LP单元数。
 */
#define OS_ERRNO_LP_PATCH_INDEX_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x5b)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 被打补丁函数大小小于16字节
 *
 * 值: 0x02001e5c
 *
 * 解决方案:检查被打补丁的函数是否小于16字节。
 */
#define OS_ERRNO_LP_PATCH_FUNC_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x5c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 段内存管理初始化失败
 *
 * 值: 0x02001e5d
 *
 * 解决方案:检查配置的段是否太小，或者地址配置错误。
 */
#define OS_ERRNO_LP_MEM_INIT_FAILD OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x5d)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: ARM MOVW/MOVT指令重定位错误。
 *
 * 值: 0x02001e5e
 *
 * 解决方案:检查ELF文件是否正确。
 */
#define OS_ERRNO_LP_ARM_MOVE_RELOC_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x5e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: THUMB MOVW/MOVT指令重定位错误。
 *
 * 值: 0x02001e5f
 *
 * 解决方案:检查ELF文件是否正确。
 */
#define OS_ERRNO_LP_THUMB_MOVE_RELOC_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x5f)

/*
 * @ingroup os_lp
 * 多核LP错误码定义:非Global符号内存申请失败。
 *
 * 值: 0x02001e60
 *
 * 解决方案:适当配大LP分区的大小。
 */
#define OS_ERRNO_LP_MEM_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x60)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单元个数配置的太小
 *
 * 值: 0x02001e61
 *
 * 解决方案:增加LP单元的配置个数
 */
#define OS_ERRNO_LP_NUM_TOO_SMALL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x61)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 6108、6181、6182平台LP使用的内存分区号不能是0,1
 *
 * 值: 0x02001e62
 *
 * 解决方案:配置LP使用的内存分区号大于1，小于最大LP分区号
 */
#define OS_ERRNO_LP_MEM_PT_NO_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x62)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态补丁递归调用补丁替换函数call重定位失败。
 *
 * 值: 0x02001e63
 *
 * 解决方案:确认LP文件中该符号是否4字节对齐或超过call调用范围。
 */
#define OS_ERRNO_LP_RECUR_CALL_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x63)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP文件类型错误
 *
 * 值: 0x02001e64
 *
 * 解决方案:检查TENSILICA平台是否使用了ARM源文件做的补丁或者ARM平台使用了TENSILICA源文件做的补丁
 */
#define OS_ERRNO_LP_ELF_FILETYPE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x64)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态补丁非递归补丁替换函数call重定位失败。
 *
 * 值: 0x02001e65
 *
 * 解决方案:确认基础版本里的该符号是否4字节对齐或超过call调用范围或汇编中的递归调用。
 */
#define OS_ERRNO_LP_NONRECUR_CALL_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x65)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 符号重定位时并行指令slot0上的L32R指令重定位错误，可能是偏移量大于0x3FFFF，或者偏移量非4字节对齐
 *
 * 值: 0x02001e6a
 *
 * 解决方案:查看段配置是否有问题。
 */
#define OS_ERRNO_LP_L32R_64_S0_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x6a)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 符号重定位时并行指令slot2上的L32R指令重定位错误，可能是偏移量大于0x3FFFF，或者偏移量非4字节对齐
 *
 * 值: 0x02001e6b
 *
 * 解决方案:查看段配置是否有问题。
 */
#define OS_ERRNO_LP_L32R_64_S2_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x6b)

/*
 * @ingroup os_lp
 * 系统基本功能错误码:LP管理信息组的个数配置错误。
 *
 * 值: 0x02001e70
 *
 * 解决方案: 查看实例内每个核配置的最大LP管理信息组个数是否大于0，
 * 而且配置的最大管理信息组个数不能超过254个(Xtensa平台)。
 *
 */
#define OS_ERRNO_LP_MAG_INFO_GROUP_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x70)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP注册时找不到空闲的LP管理组。
 *
 * 值: 0x02001e71
 *
 * 解决方案:排查注册时已经注册的LP管理信息组个数是否已超过配置的LP最大管理信息组个数。
 */
#define OS_ERRNO_LP_REG_NOT_FIND_IDLE_GROUP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x71)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP管理组ID不可用。
 *
 * 值: 0x02001e72
 *
 * 解决方案:查看调用LP接口时入参动态加载及LP管理信息组ID值不能大于等于配置的最大LP管理组个数。
 */
#define OS_ERRNO_LP_GROUP_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x72)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 不同Group间动态补丁或动态加载段与已配置的静态补丁的代码段，共享数据段或私有数据段有配置重叠。
 *
 * 值: 0x02001e73
 *
 * 解决方案:通过SRE_LP_ERRREPORT_HOOK_FUNCPTR钩子查看LP_ERROR_REPORT_INFO,
 * 检查不同Group间动态补丁或动态加载段之间与已配置的静态补丁的代码段，共享数据段或私有数据段是否有配置重叠。
 */
#define OS_ERRNO_LP_DLDP_AREA_OVERLAY_IN_ANOTHER_GROUP_WITH_SP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x73)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 注册段重叠错误上报钩子函数为空。
 *
 * 值: 0x02001e74
 *
 * 解决方案:排查注册段重叠错误上报的钩子函数是否为空。
 */
#define OS_ERRNO_LP_REG_ERRREPORTHOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x74)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP管理组生效核范围错误。
 *
 * 值: 0x02001e75
 *
 * 解决方案:生效核范围不能为0或者与当前运行核范围没有交集。
 */
#define OS_ERRNO_LP_CORE_BIT_MAP_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x75)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 静态LP单元号没有按1自增。
 *
 * 值: 0x02001e76
 *
 * 解决方案:静态的一个LP文件中，必须按1递增，查看LP文件是否正确。
 */
#define OS_ERRNO_LP_PATNO_INCREASE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x76)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在一个组内不允许一个共享数据段在多个单元存在。
 *
 * 值: 0x02001e79
 *
 * 解决方案:请检查本组的共享数据段是否在多个单元存在。
 */
#define OS_ERRNO_LP_SHARED_SEC_IN_MULTI_UNIT OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x79)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 打补丁的LP组不为符号所在的组。
 *
 * 值: 0x02001e7a
 *
 * 解决方案:检查动态补丁源码中的函数是否为其他组的补丁函数。
 */
#define OS_ERRNO_LP_FETCH_DP_GOURP_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x7a)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 找不到指定镜像ID相应组名的组ID。
 *
 * 值: 0x02001e7b
 *
 * 解决方案:检查指定镜像和指定组名的管理组是否完成注册。
 */
#define OS_ERRNO_LP_NOT_FIND_GROUP_ID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x7b)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 找不到指定组中相应段的段信息。
 *
 * 值: 0x02001e7c
 *
 * 解决方案:检查指定的组是否已经注册或者查询的段名是否存在于当前组中。
 */
#define OS_ERRNO_LP_NOT_FIND_GROUP_SECTION_INFO OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x7c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 调用SRE_LPUnitDependedLinkGet时发现输入的依赖链unitNumIn的大小为0。
 *
 * 值: 0x02001e7d
 *
 * 解决方案:检查输入的unitNumIn是不是为0。
 */
#define OS_ERRNO_LP_DEPEND_LIST_NUM_IS_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x7d)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 调用SRE_LPUnitDependedLinkGet时发现输入的依赖链unitNumIn的大小小于依赖链中的实际大小。
 *
 * 值: 0x02001e7e
 *
 * 解决方案:按照用户实际的大小输入依赖链个数。
 */
#define OS_ERRNO_LP_DEPEND_LIST_NUM_IS_LITTLE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x7e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义:不同Group间动态补丁或动态加载段之间有配置重叠。
 *
 * 值: 0x02001e7f
 *
 * 解决方案:通过SRE_LP_ERRREPORT_HOOK_FUNCPTR钩子查看LP_ERROR_REPORT_INFO
 * 检查不同Group间动态补丁或动态加载段之间是否有配置重叠。
 */
#define OS_ERRNO_LP_DLDP_AREA_OVERLAY_IN_ANOTHER_GROUP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x7f)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 同一个Group内动态补丁或动态加载段之间有配置重叠。
 *
 * 值: 0x02001e84
 *
 * 解决方案:通过SRE_LP_ERRREPORT_HOOK_FUNCPTR钩子查看LP_ERROR_REPORT_INFO
 * 检查同一个Group内动态补丁或动态加载段之间是否有配置重叠。
 */
#define OS_ERRNO_LP_DLDP_AREA_OVERLAY_IN_SAME_GROUP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x84)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: DP不能引用其他组DP的符号。
 *
 * 值: 0x02001e85
 *
 * 解决方案:检查当前加载的DP是否使用到了其他组DP中的变量或者函数。
 */
#define OS_ERRNO_LP_DP_DEPEND_DP_NOT_SAME_GROUP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x85)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 共享段地址不可配置为LOCAL L2内存。
 *
 * 值: 0x02001e86
 *
 * 解决方案:检查GROUP地址配置是否把共享段地址配置在LOCAL L2内存。
 */
#define OS_ERRNO_LP_SHARE_SECTION_ADDR_IN_LL2 OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x86)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: DL不能引用DP的符号，不能依赖DP。
 *
 * 值: 0x02001e87
 *
 * 解决方案:检查当前加载的DL是否用到了DP中的变量或者函数。
 */
#define OS_ERRNO_LP_DL_DEPEND_DP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x87)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 两个组的共享段配置有重叠或者共享段属性不一致。
 *
 * 值: 0x02001e88
 *
 * 解决方案:通过注册SRE_LPErrReportHookAdd查看是哪两个镜像哪两个组哪个共享段配置重叠或者属性不一致。
 */
#define OS_ERRNO_LP_DLDP_SHARE_AREA_OVERLAY_IN_ANOTHER_GROUP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x88)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 用户配置的生效核范围配置错误。
 *
 * 值: 0x02001e89
 *
 * 解决方案:检查生效核bit表中是否有当前平台不存在的核配置为生效核。
 */
#define OS_ERRNO_LP_VALID_CORE_BIT_TAB_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x89)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单元激活失败。
 *
 * 值: 0x02001e8a
 *
 * 解决方案:检查LP单元激活时状态是否非#UNIT_STATE_DEACTIVATED或者正在激活#UNIT_STATE_ACTIVATING状态。
 */
#define OS_ERRNO_LP_ACTIVE_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x8a)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单元去激活失败。
 *
 * 值: 0x02001e8b
 *
 * 解决方案:检查LP单元去激活时状态是否非#UNIT_STATE_ACTIVATED或者正在去激活#UNIT_STATE_DEACTIVATING状态。
 */
#define OS_ERRNO_LP_DEACTIVE_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x8b)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位时加载了之前未加载过的动态库或者动态补丁。
 *
 * 值: 0x02001e8c
 *
 * 解决方案:检查是否加载之前还未加载的动态库或动态补丁。
 */
#define OS_ERRNO_LP_RESET_LOAD_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x8c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位还未完成时，非故障核做动态补丁，动态库加载操作。
 *
 * 值: 0x02001e8e
 *
 * 解决方案:LP单核复位还未完成时，非故障核不可以做动态补丁，动态库加载操作。
 */
#define OS_ERRNO_LP_RESET_NOT_FETCH OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x8e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位还未完成时，非故障核做动态补丁，动态库激活操作。
 *
 * 值: 0x02001e8f
 *
 * 解决方案:LP单核复位还未完成时，非故障核不可以做动态补丁，动态库激活操作。
 */
#define OS_ERRNO_LP_RESET_NOT_ACTIVE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x8f)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位还未完成时，有组在做动态补丁，动态库去激活操作。
 *
 * 值: 0x02001e90
 *
 * 解决方案:LP单核复位还未完成时，不可以做动态补丁，动态库去激活操作。
 */
#define OS_ERRNO_LP_RESET_NOT_DEACTIVE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x90)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位还未完成时，有组在做动态补丁，动态库移除操作。
 *
 * 值: 0x02001e91
 *
 * 解决方案:LP单核复位还未完成时，不可以做动态补丁，动态库移除操作。
 */
#define OS_ERRNO_LP_RESET_NOT_REMOVE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x91)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位还未完成时，其他组进行组的注册操作。
 *
 * 值: 0x02001e92
 *
 * 解决方案:LP单核复位还未完成时，不可进行不包含故障核的组注册操作。
 */
#define OS_ERRNO_LP_RESET_NOT_GROUP_REG OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x92)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位还未完成时，有组在做去注册操作。
 *
 * 值: 0x02001e93
 *
 * 解决方案:LP单核复位还未完成时，不可进行组的去注册操作。
 */
#define OS_ERRNO_LP_RESET_NOT_GROUP_UNREG OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x93)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位操作时，用来做复位操作的核不是故障核。
 *
 * 值: 0x02001e94
 *
 * 解决方案:只能用故障核来做故障核的复位操作。
 */
#define OS_ERRNO_LP_RESET_CORE_ERR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x94)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在做注册操作的过程中发生了单核复位。
 *
 * 值: 0x03001e95
 *
 * 解决方案:致命错误，做制式复位。
 */
#define OS_ERRNO_LP_RESET_CORE_IN_REG OS_ERRNO_BUILD_FATAL(OS_MID_LP, 0x95)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在做去注册操作的过程中发生了单核复位。
 *
 * 值: 0x03001e96
 *
 * 解决方案:致命错误，做制式复位。
 */
#define OS_ERRNO_LP_RESET_CORE_IN_UNREG OS_ERRNO_BUILD_FATAL(OS_MID_LP, 0x96)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在做加载操作的过程中发生了单核复位。
 *
 * 值: 0x03001e97
 *
 * 解决方案:致命错误，做制式复位。
 */
#define OS_ERRNO_LP_RESET_CORE_IN_FETCH OS_ERRNO_BUILD_FATAL(OS_MID_LP, 0x97)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在做激活操作的过程中发生了单核复位。
 *
 * 值: 0x03001e98
 *
 * 解决方案:致命错误，做制式复位。
 */
#define OS_ERRNO_LP_RESET_CORE_IN_ACTIVE OS_ERRNO_BUILD_FATAL(OS_MID_LP, 0x98)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在做去激活操作的过程中发生了单核复位。
 *
 * 值: 0x03001e99
 *
 * 解决方案:致命错误，做制式复位。
 */
#define OS_ERRNO_LP_RESET_CORE_IN_DEACTIVE OS_ERRNO_BUILD_FATAL(OS_MID_LP, 0x99)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在做移除操作的过程中发生了单核复位。
 *
 * 值: 0x03001e9a
 *
 * 解决方案:致命错误，做制式复位。
 */
#define OS_ERRNO_LP_RESET_CORE_IN_REMOVE OS_ERRNO_BUILD_FATAL(OS_MID_LP, 0x9a)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在做完加载操作还没有做激活操作的过程中发生了单核复位；
 * 或者做完去激活还没有做移除的过程中发生了单核复位。
 *
 * 值: 0x03001e9b
 *
 * 解决方案:致命错误，做制式复位。
 */
#define OS_ERRNO_LP_CORE_RESET_FAIL OS_ERRNO_BUILD_FATAL(OS_MID_LP, 0x9b)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位时候不能创建之前未存在的组。
 *
 * 值: 0x02001e9c
 *
 * 解决方案:检查是否在LP单核复位时候创建了之前未存在的组。
 */
#define OS_ERRNO_LP_RESET_NOT_REG_NEW_GROUP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x9c)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP单核复位时候不能激活之前依赖却未加载的组。
 *
 * 值: 0x02001e9d
 *
 * 解决方案:检查是否在LP单核复位时候有依赖的组未加载。
 */
#define OS_ERRNO_LP_RESET_ACTIVE_NOT_LOAD_UNIT OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x9d)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: LP加载DP的时候，HOT PATCH TABLE空间不足。
 *
 * 值: 0x02001e9e
 *
 * 解决方案:删除一些不使用的补丁，重新加载。
 */
#define OS_ERRNO_LP_HP_TABLE_ALLOC_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x9e)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 需要操作的目的核忙，不能进行动态加载或者卸载操作。
 *
 * 值: 0x02001e9f
 *
 * 解决方案:检查是否有在同一个核上有并发的动态加载或者卸载操作。
 */
#define OS_ERRNO_LP_HANDLE_CORE_BUSY OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0x9f)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: HIDSP指令重定位失败。
 *
 * 值: 0x02001ea0
 *
 * 解决方案:检查重定位代码是否有问题。
 */
#define OS_ERRNO_LP_HIDSP_RELOC_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa0)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: SD6183、SD6185的LP模块从NSM空间申请内存失败。
 *
 * 值: 0x02001ea1
 *
 * 解决方案:扩大NSM空间内存。
 */
#define OS_ERRNO_LP_NSM_MEM_ALLOC_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa1)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: SD6183、SD6185的LP模块未完成初始化。
 *
 * 值: 0x02001ea2
 *
 * 解决方案:初始化未完成前，不能调用补丁接口。
 */
#define OS_ERRNO_LP_INITL_UNFINISH OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa2)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: DP依赖的DL没有加载。
 *
 * 值: 0x02001ea3
 *
 * 解决方案:检查代码中打补丁的动态库是否没有加载，并修改
 */
#define OS_ERRNO_LP_DP_DEPEND_DL_NOT_LOADED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa3)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: SD6183、SD6185的从核动态库或动态补丁加载失败。
 *
 * 值: 0x02001ea4
 *
 * 解决方案:通过注册SRE_ErrRegHook看从核返回的具体的错误码。
 */
#define OS_ERRNO_LP_CORE_LOAD_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa4)

/*
 * @ingroup os_lp
 * 多核LP错误码定义:主核的补丁分区MMU映射最小大小小于8K对齐。
 *
 * 值: 0x02001ea5
 *
 * 解决方案:补丁表内存在补丁分区空间中申请，其中存储打补丁时的跳转指令，SD6183x指令代码段需最小8K对齐，
 * 检查补丁空间映射大小是否小于8K。
 */
#define OS_ERRNO_LP_INIT_ALIGN_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa5)

/*
 * @ingroup os_lp
 * 多核LP错误码定义:SD6183、SD6185平台，动态补丁替换的函数不为被打补丁的动态库所有。
 *
 * 值: 0x02001ea6
 *
 * 解决方案:动态补丁只能替换对应的动态库中的函数，不可替换其他动态库函数
 */
#define OS_ERRNO_LP_DP_FUN_REPLACE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa6)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 在做完加载操作或卸载操作的过程中发生了单核复位。
 *
 * 值: 0x03001ea7
 *
 * 解决方案:致命错误，做制式复位。
 */
#define OS_ERRNO_LP_CORE_RESET_BUSY_FAIL OS_ERRNO_BUILD_FATAL(OS_MID_LP, 0xa7)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 加载动态补丁时有未定义的符号。
 *
 * 值: 0x02001ea8
 *
 * 解决方案:确保动态补丁不存在未定义的符号。
 */
#define OS_ERRNO_LP_DP_FETCH_UNDEF_SYM OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa8)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态库中未定义的符号依赖动态补丁。
 *
 * 值: 0x02001ea9
 *
 * 解决方案:确保动态库中未定义的符号不依赖动态补丁。
 */
#define OS_ERRNO_LP_DL_UNDEF_SYM_DEPEND_DP OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xa9)

/*
 * @ingroup os_lp
 * SRE MNG错误码：动态库的段信息获取，实例信息的名字空间未找到
 *
 * 值: 0x02001eaa
 *
 * 解决方案: 至少有一个实例已创建完成，并保证基础BIN和动态库是否已经加载完成.
 */
#define OS_ERRNO_LP_GET_INST_INFO_FIND_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xaa)

/*
 * @ingroup os_lp
 * SRE MNG错误码：动态库的段信息获取，未找到对应的动态库名。
 *
 * 值: 0x02001eab
 *
 * 解决方案: 检查动态库是否加载.
 */
#define OS_ERRNO_LP_DLIB_NAME_FIND_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xab)

/*
 * @ingroup os_lp
 * SRE MNG错误码：动态库的段信息获取，未找到对应的段名。
 *
 * 值: 0x02001eac
 *
 * 解决方案: 检查动态库对应的段名是否存在.
 */
#define OS_ERRNO_LP_DLIB_SEC_FIND_FAILED OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xac)

/*
 * @ingroup os_lp
 * SRE MNG错误码：从核查找主核申请的加载握手计数有名空间失败。
 *
 * 值: 0x02001ead
 *
 * 解决方案: 确认主核已经加载并且已经申请该有名空间.
 */
#define OS_ERRNO_HANDSHAKE_NAME_FIND_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xad)

/*
 * @ingroup os_lp
 * 多核LP错误码定义：LP模块正处于忙碌状态。
 *
 * 值: 0x02001eae
 *
 * 解决方案: 确认是否正在进行动态库加载/卸载动作.
 */
#define OS_ERRNO_LP_OPREATION_BUSY OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xae)

/*
 * @ingroup os_lp
 * 多核LP错误码定义：在中断中加载/卸载动态库。
 *
 * 值: 0x02001eaf
 *
 * 解决方案: 确认不在中断中做加载卸载操作.
 */
#define OS_ERRNO_LP_OP_IN_HWI OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xaf)

/*
 * @ingroup os_lp
 * 多核LP错误码定义：实例内预加载区内存不足。
 *
 * 值: 0x02001eb0
 *
 * 解决方案: 分配足够的预加载区内存.
 */
#define OS_ERRNO_LP_NO_PRELINK_MEM OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb0)

/*
 * @ingroup os_lp
 * 多核LP错误码定义：App Start Hook函数为空。
 *
 * 值: 0x02001eb1
 *
 * 解决方案: 确认调用SRE_LPAppStartHookAdd传入钩子函数不为空。
 */
#define OS_ERRNO_LP_APP_START_HOOK_NULLPTR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb1)

/*
 * @ingroup os_lp
 * 多核LP错误码定义：App Start Hook的钩子函数已满。
 *
 * 值: 0x02001eb2
 *
 * 解决方案: 目前最多支持32个钩子函数，减少钩子函数的个数.
 */
#define OS_ERRNO_LP_APP_START_HOOK_FULL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb2)

/*
 * @ingroup os_lp
 * 多核LP错误码定义：App Stop Hook函数为空。
 *
 * 值: 0x02001eb3
 *
 * 解决方案: 确认调用SRE_LPAppStopHookAdd传入钩子函数不为空。
 */
#define OS_ERRNO_LP_APP_STOP_HOOK_NULLPTR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb3)

/*
 * @ingroup os_lp
 * 多核LP错误码定义：App Stop Hook的钩子函数已满。
 *
 * 值: 0x02001eb4
 *
 * 解决方案: 目前最多支持32个钩子函数，减少钩子函数的个数.
 */
#define OS_ERRNO_LP_APP_STOP_HOOK_FULL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb4)

/*
 * @ingroup os_lp
 * 多核LP错误码定义：没找到可用的10M虚地址。
 *
 * 值: 0x02001eb5
 *
 * 解决方案: 业务确保有10M连续的空闲VA地址。
 */
#define OS_ERRNO_LP_10M_VA_NOT_AVAILABLE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb5)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 加载动态库时有未定义的符号。
 *
 * 值: 0x02001eb6
 *
 * 解决方案:确保动态库中不存在未定义的符号。
 */
#define OS_ERRNO_LP_DL_UNDEF_SYM OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb6)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 执行动态库操作时动态库所处阶段不匹配。
 *
 * 值: 0x02001eb7
 *
 * 解决方案:确保动态库处在正确的阶段。
 */
#define OS_ERRNO_LP_DL_STATE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb7)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 动态库入口/退出函数为NULL。
 *
 * 值: 0x02001eb8
 *
 * 解决方案:确保已定义动态库的入口/退出函数且不为NULL。
 */
#define OS_ERRNO_LP_DL_ENTRY_EXIT_FUNC_NULL OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb8)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 不支持的重定位类型或指令
 *
 * 值: 0x02001eb9
 *
 * 解决方案:查看动态库的重定位信息，确保不含不支持的重定位类型或指令。
 */
#define OS_ERRNO_LP_DL_INVALID_RELOC_INFO OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xb9)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 重定位结果未通过校验
 *
 * 值: 0x02001eba
 *
 * 解决方案:查看符号大小是否超出了指令支持的立即数范围。
 */
#define OS_ERRNO_LP_DL_RELOC_RES_CHECK_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xba)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 打冷补丁的架构非法
 *
 * 值: 0x02001ebb
 *
 * 解决方案:查看硬件架构是否支持打冷补丁。
 */
#define OS_ERRNO_LP_COLD_PAT_MACHINE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xbb)

/*
 * @ingroup os_lp
 * 多核LP错误码定义: 打冷补丁需要的内存不足
 *
 * 值: 0x02001ebc
 *
 * 解决方案:适当增大可用内存。
 */
#define OS_ERRNO_COLDPAT_MEM_NOT_AVAILABLE OS_ERRNO_BUILD_ERROR(OS_MID_LP, 0xbc)

/*
 * @ingroup os_lp
 * @brief 动态库入口函数定义，入口函数在动态库加载后调用
 */
#define DL_ENTRY_FUNC OsDlEntry
#define DL_ENTRY(func) \
    U32 DL_ENTRY_FUNC(void) __attribute__((alias(#func)))

/*
 * @ingroup os_lp
 * @brief 动态库退出函数定义，退出函数在动态库卸载前调用
 */
#define DL_EXIT_FUNC OsDlExit
#define DL_EXIT(func) \
    U32 DL_EXIT_FUNC(void) __attribute__((alias(#func)))

/*
 * @ingroup os_lp
 * @brief 静态补丁、动态补丁、动态加载所有镜像统一配置信息
 */
struct LpImagePubInfo {
    /* 用户配置的LP管理信息组的最大组数 */
    U32 groupNum;
    /* 用户配置的最大单元个数(静态补丁个数 + 动态补丁个数 + 动态加载库个数) */
    U32 maxLPUnitNum;
    /* 供LP使用的内存分区号 */
    U32 lpPtNo;
    /* 供LP使用的内存分区起始地址 */
    uintptr_t lpPtAddr;
    /* 供LP使用的内存分区大小 */
    uintptr_t lpPtSize;
    /* 芯片类型,仅SD6182/SD6157需要配置该项 */
    U32 chipType;
};

/*
 * @ingroup os_lp
 * @breif 动态库加载信息用户参数
 */
struct DlLoadPara {
    /* elf文件（elf文件头及非加载段）起始地址 */
    uintptr_t elfAddr;
    /* elf文件（elf文件头及非加载段）大小 */
    uintptr_t elfSize;
    /* 加载段（代码、数据、bss）起始地址 */
    uintptr_t loadSecAddr;
    /* 加载段（代码、数据、bss）大小 */
    uintptr_t loadSecSize;
    /* 是否需要注册全局符号到系统符号表 */
    bool needRegSym;
};

struct DlSecInfo {
    /* 段起始地址 */
    uintptr_t addr;
    /* 段大小 */
    uintptr_t size;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_LP_H */
