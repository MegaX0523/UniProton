/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 内存模块的基础头文件, 用户不能直接引用
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_MEM_H
#define OS_MEM_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_mem
 * 内存错误码：创建分区的分区大小太小。
 *
 * 值: 0x02000101
 *
 * 解决方案：分区大小改大。
 */
#define OS_ERRNO_MEM_PTCREATE_SIZE_TOOSMALL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x01)

/*
 * @ingroup OS_mem
 * 任务错误码：0号或1号分区大小不够。
 *
 * 值: 0x02000102
 *
 * 解决方案: 增加0号或1号分区大小。
 */
#define OS_ERRNO_MEM_DEFAULT_PT_SIZE_TOOSMALL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x02)

/*
 * @ingroup OS_mem
 * 内存错误码：释放的地址为空。
 *
 * 值: 0x02000103
 *
 * 解决方案：请检查释放的内存块地址是否正确。
 */
#define OS_ERRNO_MEM_FREE_ADDR_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x03)

/*
 * @ingroup OS_mem
 * 内存错误码：释放内存所在的分区无效(分区未创建或分区号大于分区数)。
 *
 * 值: 0x02000104
 *
 * 解决方案：请检查分区号的有效性。
 */
#define OS_ERRNO_MEM_FREE_PTNO_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x04)

/*
 * @ingroup OS_mem
 * 内存错误码：配置的最大可创建内存分区数过大(最大不能超过253)。
 *
 * 值: 0x02000105
 *
 * 解决方案: 请检查分区数(最大不能超过253)配置项。
 */
#define OS_ERRNO_MEM_PTNUMBER_OVERMANY OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x05)

/*
 * @ingroup OS_mem
 * 内存错误码：内存申请时申请的大小太大(可能为负值)。
 *
 * 值: 0x02000106
 *
 * 解决方案: 增大分区大小，或减小要申请的内存大小。
 */
#define OS_ERRNO_MEM_ALLOC_SIZETOOLARGE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x06)

/*
 * @ingroup OS_mem
 * 内存错误码：内存配置的内存块类型都是0。
 *
 * 值: 0x02000107
 *
 * 解决方案: 请检查内存块类型配置，确保内存块类型不全为0。
 */
#define OS_ERRNO_MEM_ALLSLICE_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x07)

/*
 * @ingroup OS_mem
 * 内存错误码：内存配置的内存块类型不是按递增的顺序配置的。
 *
 * 值: 0x02000108
 *
 * 解决方案: 请确保内存块类型按照递增的顺序配置。
 */
#define OS_ERRNO_MEM_MEMSLICE_NOT_INORDER OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x08)

/*
 * @ingroup OS_mem
 * 内存错误码：创建分区入参有误(分区号应大于1且小于配置最大分区个数+2，分区信息结构体和分区起始地址不为空。
 * 检查分区算法正确性、分区大小不为0,分区起始地址+分区尺寸大于4G)。
 *
 * 值: 0x02000109
 *
 * 解决方案: 请检查分区入参是否合法。
 */
#define OS_ERRNO_MEM_PTCREATE_PARAMETER_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x09)

/*
 * @ingroup OS_mem
 * 内存错误码：分区已经创建。
 *
 * 值: 0x0200010a
 *
 * 解决方案: 请确保分区未创建。
 */
#define OS_ERRNO_MEM_PT_ALREADY_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x0a)

/*
 * @ingroup OS_mem
 * 内存错误码：动态创建内存分区时，区段有重叠。
 *
 * 值: 0x0200010b
 *
 * 解决方案: 不同分区之间避免重叠。
 */
#define OS_ERRNO_MEM_PT_OVERLAP OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x0b)

/*
 * @ingroup OS_mem
 * 内存错误码：该内存算法分区没有创建或内存算法不支持该操作（如删除）。
 *
 * 值: 0x0200010c
 *
 * 解决方案: 检查指定的内存算法分区是否已经创建或者使用的内存算法是否支持该操作
 */
#define OS_ERRNO_MEM_ALGORITHM_UNINSTALLED OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x0c)

/*
 * @ingroup OS_mem
 * 内存错误码：查询内存分区的基本信息时该内存分区未被创建或者分区号非法。
 *
 * 值: 0x0200010d
 *
 * 解决方案: 检查分区号的有效性。
 */
#define OS_ERRNO_MEM_NO_USE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x0d)

/*
 * @ingroup OS_mem
 * 内存错误码：调用获取分区的使用情况接口时，该接口参数存在空指针。
 *
 * 值: 0x0200010e
 *
 * 解决方案: 请确保接口参数不为空指针。
 */
#define OS_ERRNO_MEM_INPUT_INVAILD OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x0e)

/*
 * @ingroup OS_mem
 * 内存错误码：FSC内存算法不支持可Cache空间。
 *
 * 值: 0x0200010f
 *
 * 解决方案: 改用不可Cache空间，或改用POOL算法。
 */
#define OS_ERRNO_MEM_PTCREATE_FSC_ARITH_UNCACHABLE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x0f)

/*
 * @ingroup OS_mem
 * 内存错误码：分区号不合法，分区号范围[OS_MEM_DEFAULT_PTNUM, OS_MEM_MAX_PT_NUM + OS_MEM_DEFAULT_PTNUM -
 * 1],OS_MEM_MAX_PT_NUM是用户配置的分区数，OS_MEM_DEFAULT_PTNUM系统缺省的分区数。
 *
 * 值: 0x02000110
 *
 * 解决方案：请检查入参分区号。
 */
#define OS_ERRNO_MEM_PTNO_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x10)

/*
 * @ingroup OS_mem
 * 内存错误码：申请的内存块大小为0。
 *
 * 值: 0x02000111
 *
 * 解决方案：请检查申请内存大小的有效性。
 */
#define OS_ERRNO_MEM_ALLOC_SIZE_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x11)

/*
 * @ingroup OS_mem
 * 内存错误码：操作的分区没有创建。
 *
 * 值: 0x02000112
 *
 * 解决方案：请检查该分区是否已经创建。
 */
#define OS_ERRNO_MEM_PT_NOT_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x12)

/*
 * @ingroup OS_mem
 * 内存错误码：对齐方式不合法。
 *
 * 值: 0x02000113
 *
 * 解决方案：请检查入参对齐方式。
 */
#define OS_ERRNO_MEM_ALLOC_ALIGNPOW_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x13)

/*
 * @ingroup OS_mem
 * 内存错误码：分区算法非法。OS支持的分区算法:MEM_ARITH_FSC、MEM_ARITH_POOL、MEM_ARITH_MCDYNFSC、MEM_ARITH_MCPOOL。
 *
 * 值: 0x02000114
 *
 * 解决方案：请检查分区算法参数。
 */
#define OS_ERRNO_MEM_PT_ARITH_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x14)

/*
 * @ingroup OS_mem
 * 内存错误码：查询内存使用情况时，输入的有效参数都为NULL。
 *
 * 值: 0x02000115
 *
 * 解决方案: 查询内存使用情况时，需保证输入的有效参数合法。
 */
#define OS_ERRNO_MEMINFO_CHECK_ALL_ADDR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x15)

/*
 * @ingroup OS_mem
 * 内存错误码：查询单个内存使用情况时，输入的两个有效参数其中一个为NULL。
 *
 * 值: 0x02000116
 *
 * 解决方案: 查询单个内存使用情况时，需保证输入的两个有效参数合法。
 */
#define OS_ERRNO_MEMINFO_TSK_SINGLE_ADDR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x16)

/*
 * @ingroup OS_mem
 * 内存错误码：查询内存使用情况时，输入的查询任务个数等于0。
 *
 * 值: 0x02000117
 *
 * 解决方案: 在使用内存使用情况查询功能时，需保证输入的查询任务个数在正常范围内。
 */
#define OS_ERRNO_MEMINFO_CHECK_TASKNUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x17)

/*
 * @ingroup OS_mem
 * 内存错误码：分区算法不支持。
 *
 * 值: 0x02000119
 *
 * 解决方案:该功能只支持私有FSC算法。
 */
#define OS_ERRNO_MEM_PT_ARITH_NOT_SUPPORT OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x19)

/*
 * @ingroup OS_mem
 * 动态内存错误码：动态内存释放时要释放的内存块的头被破坏，或已释放。
 *
 * 值: 0x0200011a
 *
 * 解决方案: 保证对内存写操作时，不要出现写越界。
 *
 */
#define OS_ERRNO_MEM_FREE_SH_DAMAGED OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x1a)

/*
 * @ingroup OS_mem
 * 动态内存错误码：内存算法类型非法或OS不支持该内存算法。
 *
 * 值: 0x0200011b
 *
 * 解决方案: 查看输入的算法是否正确。
 */
#define OS_ERRNO_MEM_ARITH_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x1b)

/*
 * @ingroup OS_mem
 *
 * 内存错误码：不同核创建相同编号的共享分区时参数不相同。
 *
 * 值: 0x0200011c
 *
 * 解决方案: 检查分区号，分区算法，分区起始地址，分区大小，分区块类型及大小的一致性。
 */
#define OS_ERRNO_MEM_SHARE_PARA_INCONSISTENT OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x1c)

/*
 * @ingroup OS_mem
 * 共享内存错误码：从共享静态FSC分区中申请内存时，找不到空闲内存，申请失败。
 *
 * 值: 0x0200011d
 *
 * 解决方案: 增大共享静态FSC内存分区大小。
 */
#define OS_ERRNO_MCDYN_FSCMEM_ALLOC_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x1d)

/*
 * @ingroup OS_mem
 * 共享内存错误码：不同image中对于OS缺省使用的1号分区配置不一致。
 *
 * 值: 0x02000120
 *
 * 解决方案: 请确保所有image中对于OS缺省使用的1号分区配置的起始地址一致、分区大小一致。
 */
#define OS_ERRNO_MCDYN_FSCMEM_PT_PARA_INCONSISTENT OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x20)

/*
 * @ingroup OS_mem
 * 内存错误码：调用接口#SRE_MemFree/#SRE_MemCacheFree释放不可cache内存，或调用接口#SRE_MemUncacheFree释放可cache内存。
 *
 * 值: 0x02000121
 *
 * 解决方案: 请调用接口#SRE_MemFree/#SRE_MemCacheFree释放可cache内存，或调用接口#SRE_MemUncacheFree释放不可cache内存。
 */
#define OS_ERRNO_MEM_ATTR_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x21)

/*
 * @ingroup OS_mem
 * 内存错误码：调用接口#SRE_MemUncacheFree释放非OS_MEM_DEFAULT_FSC_PT分区中的内存。
 *
 * 值: 0x02000122
 *
 * 解决方案: 请调用接口#SRE_MemFree/#SRE_MemCacheFree释放内存。
 */
#define OS_ERRNO_MEM_FREE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x22)

/*
 * @ingroup OS_mem
 * 内存错误码：mmu模块没有初始化。
 *
 * 值: 0x02000123
 *
 * 解决方案: 请检查mmu模块参数配置是否正确。
 */
#define OS_ERRNO_MEM_MMU_NOT_INIT OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x23)

/*
 * @ingroup OS_mem
 * 内存错误码：不能删除系统缺省的内存分区。
 *
 * 值: 0x02000124
 *
 * 解决方案: 请检查分区号是否为系统缺省的分区。
 */
#define OS_ERRNO_MEM_DELETE_DEFAULT_PT OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x24)

/*
 * @ingroup OS_mem
 * 内存错误码：不能删除共享内存算法分区。
 *
 * 值: 0x02000125
 *
 * 解决方案: 请确保待删除的分区为私有内存算法分区。
 */
#define OS_ERRNO_MEM_DELETE_SHARE_PT OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x25)

/*
 * @ingroup OS_mem
 * 内存错误码：分区起始地址太大。
 *
 * 值: 0x02000126
 *
 * 解决方案: 请减小分区起始地址。
 */
#define OS_ERRNO_MEM_PTCREATE_ADDR_TOOLARGE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x26)

/*
 * @ingroup OS_mem
 * 内存错误码：不支持创建共享算法内存分区。
 *
 * 值: 0x02000127
 *
 * 解决方案: 请用私有内存算法创建分区。
 */
#define OS_ERRNO_MEM_PTCREATE_ARITH_NONSUPPORT OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x27)

/*
 * @ingroup OS_mem
 * 内存错误码：slub初始化时，申请内存用于存放分区slub管理器失败，
 * 申请失败。
 * 值: 0x02000128
 *
 * 解决方案: 请用私有内存算法创建分区。
 */
#define OS_ERRNO_MEM_SLUB_INIT_ALLOC_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x28)

/*
 * @ingroup OS_mem
 * 内存错误码：slub 初次分配失败
 * 值: 0x02000129
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_SLUB_FIRST_ALLOC_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x29)

/*
 * @ingroup OS_mem
 * 内存错误码：slub初始化时，通用slub创建失败
 * 值: 0x0200012a
 *
 * 解决方案: 无
 */
#define OS_ERRNO_MEM_SLUB_GENERAL_SLUB_CREATE_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x2a)

/*
 * @ingroup OS_mem
 * 内存错误码：slub内存释放时内存块已经被释放(或者内存没有被申请过)或者内存尾被踩。
 *
 * 值: 0x0200012b
 *
 * 解决方案: 保证释放的内存块是已经被申请的,确保内存尾没有被踩
 */
#define OS_ERRNO_MEM_SLUB_ALREADY_FREE_OR_TAIL_DAMAGE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x2b)

/*
 * @ingroup OS_mem
 * 内存错误码：维测信息查看调用形参错误
 *
 * 值: 0x0200012d
 *
 * 解决方案: 请检查形参是否为空或0
 */
#define OS_ERRNO_MEM_DBG_INFO_PARA_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x2d)

/*
 * @ingroup OS_mem
 * 内存错误码：维测信息查看分区不支持维测信息获取
 *
 * 值: 0x0200012e
 *
 * 解决方案: 请检查形参是否为空或0
 */
#define OS_ERRNO_MEM_DBG_GET_INFO_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x2e)

/*
 * @ingroup OS_mem
 * OS缺省私有FSC内存错误码：从私有FSC分区中申请内存时，找不到空闲内存，申请失败。
 *
 * 值: 0x0200012f
 *
 * 解决方案: 增大私有FSC内存分区大小。
 */
#define OS_ERRNO_FSCMEM_ALLOC_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x2f)

/*
 * @ingroup OS_mem
 * 内存错误码：维测信息查看地址翻转
 *
 * 值: 0x02000130
 *
 * 解决方案: 请检查形参是否地址翻转
 */
#define OS_ERRNO_MEM_DBG_INFO_ADDR_OVERTURN OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x30)

/*
 * @ingroup OS_mem
 * 内存错误码：注册内存算法时入参非法
 *
 * 值: 0x02000131
 *
 * 解决方案: 请检查入参
 */
#define OS_ERRNO_MEM_ARITH_REG_PARAM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x31)

/*
 * @ingroup OS_mem
 * 内存错误码：增加/减少BLOCK块时非BLOCK算法分区,配置水线时释放为SLUB-FSC算法
 *
 * 值: 0x02000132
 *
 * 解决方案: 请检查对应API算法分区
 */
#define OS_ERRNO_MEM_ARITH_NOT_MATCH OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x32)

/*
 * @ingroup OS_mem
 * 内存错误码：增加/减少BLOCK块时BLOCK块大小非法
 *
 * 值: 0x02000133
 *
 * 解决方案: 请检查BLOCK块大小是否匹配
 */
#define OS_ERRNO_MEM_BLOCK_SIZE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x33)

/*
 * @ingroup OS_mem
 * 内存错误码：减少BLOCK块时失败
 *
 * 值: 0x02000134
 *
 * 解决方案: 请检查BLOCK块个数是否匹配或者有未释放的内存块
 */
#define OS_ERRNO_MEM_BLOCK_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x34)

/*
 * @ingroup OS_mem
 * 内存错误码：增加内存时，区段有重叠。
 *
 * 值: 0x02000135
 *
 * 解决方案: 不连续内存之间不要重叠。
 */
#define OS_ERRNO_MEM_APPEND_OVERLAP OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x35)

/*
 * @ingroup OS_mem
 * 内存错误码：内存被踩。
 *
 * 值: 0x02000136
 *
 * 解决方案: 保证对内存写操作时，不要出现写越界。
 *
 */
#define OS_ERRNO_MEM_DAMAGED OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x36)

/*
 * @ingroup OS_mem
 * 内存错误码：内存扩展/删除入参非法
 *
 * 值: 0x02000137
 *
 * 解决方案: 请检查入参是否合法。
 */
#define OS_ERRNO_MEM_APPEND_PARA_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x37)

/*
 * @ingroup OS_mem
 * 内存错误码：扩展内存删除地址不存在
 *
 * 值: 0x02000138
 *
 * 解决方案: 请检查内存地址是否合法。
 */
#define OS_ERRNO_MEM_APPEND_REMOVE_ADDR_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x38)

/*
 * @ingroup OS_mem
 * 内存错误码：扩展内存删除失败
 *
 * 值: 0x02000139
 *
 * 解决方案: 请检查内存扩展内存释放都已经释放。
 */
#define OS_ERRNO_MEM_APPEND_REMOVE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x39)

/*
 * @ingroup OS_mem
 * 内存错误码：SLUB-FSC内存算法水线配置时入参非法
 *
 * 值: 0x0200013a
 *
 * 解决方案: 请检查入参内存块大小超过4K或大小非16字节对齐
 */
#define OS_ERRNO_MEM_SLUB_CFG_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x3a)

/*
 * @ingroup OS_mem
 * 内存错误码：SLUB-FSC内存算法水线配置时使用之前配置
 *
 * 值: 0x0200013b
 *
 * 解决方案: 请在使用之前配置
 */
#define OS_ERRNO_MEM_SLUB_CFG_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x3b)

/*
 * @ingroup OS_mem
 * 内存错误码：申请的内存块size进行allign对齐后向上或向下溢出。
 *
 * 值: 0x0200013c
 *
 * 解决方案：请检查申请内存大小的有效性。
 */
#define OS_ERRNO_MEM_ALLOC_SIZE_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x3c)

/*
 * @ingroup OS_mem
 * 动态内存错误码：动态(POOL)内存申请时申请的内存块被破坏。
 *
 * 值: 0x0200013d
 *
 * 解决方案: 无。
 */
#define OS_ERRNO_MEM_DYNALLOC_SH_DAMAGED OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x3d)

/*
 * @ingroup OS_mem
 * 内存错误码：私有POOL内存释放时内存块已经被释放(或者内存没有被申请过)。
 *
 * 值: 0x0200013e
 *
 * 解决方案: 检查释放的内存是否已申请，或者是否已经被释放。
 */
#define OS_ERRNO_MEM_FREE_POOL_MEM_ALREADY_FREE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x3e)

/*
 * @ingroup OS_mem
 * 动态内存错误码：动态POOL内存申请时内存空间不够。
 *
 * 值: 0x0200013f
 *
 * 解决方案: 增大分区大小，或减小要申请的内存大小。
 */
#define OS_ERRNO_MEM_DYN_ALLOC_MEM_SHORTAGE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x3f)

/*
 * @ingroup OS_mem
 * 共享内存错误码：从共享静态FSC分区中申请内存时，找不到空闲内存，申请失败。
 *
 * 值: 0x02000140
 *
 * 解决方案: 增大共享静态FSC内存分区大小。
 */
#define OS_ERRNO_MCSTA_FSCMEM_ALLOC_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x40)

/*
 * @ingroup OS_mem
 * 共享内存错误码：共享动态POOL内存申请时内存空间不够。
 *
 * 值: 0x02000141
 *
 * 解决方案: 增大分区大小，或减小要申请的内存大小。
 */
#define OS_ERRNO_MEM_SHARE_ALLOC_MEM_SHORTAGE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x41)

/*
 * @ingroup OS_mem
 * 共享内存错误码：共享动态POOL内存申请时申请的内存块被破坏。
 *
 * 值: 0x02000142
 *
 * 解决方案: 保证地址是内存地址，保证用户写操作不能越界。
 */
#define OS_ERRNO_MEM_SHAREALLOC_SH_DAMAGED OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x42)

/*
 * @ingroup OS_mem
 * 共享内存错误码：共享动态POOL内存释放时内存块已经被释放(或者内存没有被申请过)。
 *
 * 值: 0x02000143
 *
 * 解决方案: 保证释放的内存块是已经被申请的。
 */
#define OS_ERRNO_MEM_FREE_SHARE_MEM_ALREADY_FREE OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x43)

/*
 * @ingroup OS_mem
 * 内存错误码：查询内存使用情况时，输入的最大个数指针为空。
 *
 * 值: 0x02000144
 *
 * 解决方案: 在使用内存使用情况查询功能时，需保证输入最大个数指针非空。
 */
#define OS_ERRNO_MEMINFO_CHECK_MAXNUM_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x44)

/*
 * @ingroup OS_mem
 * 内存错误码：MAP-POOL内存算法，释放的内存时，入参地址非法
 *
 * 值: 0x02000145
 *
 * 解决方案: 确认内存地址按最小粒度对齐并在分区地址范围内
 */
#define OS_ERRNO_MEM_MAP_POOL_INVALID_ADDR                  OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x45)

/*
 * @ingroup OS_mem
 * 内存错误码：MAP-POOL内存算法，创建内存分区时，入参非法
 *
 * 值: 0x02000146
 *
 * 解决方案: 确认分区最小粒度bit>=7, 并且内存地址和大小按最小粒度对齐，出参地址不为空
 */
#define OS_ERRNO_MEM_MAP_POOL_INVALID_PARA                  OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x46)

/*
 * @ingroup OS_mem
 * 内存错误码：MAP-POOL内存算法分区正在使用，不能删除
 *
 * 值: 0x02000147
 *
 * 解决方案: 私有MAP-POOL分区，确认所有内存块已释放
 * 共享MAP-POOL分区，不支持内存分区删除
 */
#define OS_ERRNO_MEM_MAP_POOL_DELETE_BUSY                   OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x47)

/*
 * @ingroup OS_mem
 * 内存错误码：MAP-POOL内存算法重复释放内存
 *
 * 值: 0x02000148
 *
 * 解决方案: 确保不重复释放内存
 */
#define OS_ERRNO_MEM_MAP_POOL_REPEAT_FREE                   OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x48)

/*
 * @ingroup OS_mem
 * 内存错误码：MAP-POOL内存算法致命错误
 *
 * 值: 0x02000149
 *
 * 解决方案: 确保分区控制块和内存管理块有效(未被踩踏)
 */
#define OS_ERRNO_MEM_MAP_POOL_FATAL_ERROR                   OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x49)

/*
 * @ingroup OS_mem
 * 内存错误码：MAP-POOL内存算法无可分配的1号分区
 *
 * 值: 0x0200014a
 *
 * 解决方案: 增加1号分区的内存(分区控制块和内存管理块从1号分区申请)
 */
#define OS_ERRNO_MEM_MAP_POOL_NO_CTRL_MEM                   OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x4a)

/*
 * @ingroup OS_mem
 * 内存错误码：MAP-POOL内存算法，接口参数指针非法
 *
 * 值: 0x0200014b
 *
 * 解决方案: 使用有效的接口参数
 */
#define OS_ERRNO_MEM_MAP_POOL_INVALID_PTR                   OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x4b)

/*
 * @ingroup OS_mem
 * 内存错误码：MAP-POOL内存算法无可分配的内存
 *
 * 值: 0x0200014c
 *
 * 解决方案: 增加分区的内存
 */
#define OS_ERRNO_MEM_MAP_POOL_NO_MEMORY                     OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x4c)
/*
 * @ingroup OS_mem
 * 内存错误码：节点插入删除前后对象错误。
 *
 * 值: 0x0200014d
 *
 * 解决方案: 检查插入删除的节点前后指针指向对象
 *
 */
#define OS_ERRNO_MEM_LINK                                   OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x4d)

/*
 * @ingroup OS_mem
 * 内存错误码：内存申请时申请的内存块所在local地址错误。
 *
 * 值: 0x0200014f
 *
 * 解决方案: 保证地址是内存地址，保证用户写操作不能越界。
 */
#define OS_ERRNO_MEM_LOCAL_ADDR_DAMAGED                     OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x4f)

/*
 * @ingroup OS_mem
 * 内存错误码：内存申请时要申请的内存块的头非法，或已被申请。
 *
 * 值: 0x02000150
 *
 * 解决方案: 保证内存块的头正确
 *
 */
#define OS_ERRNO_MEM_ALLOC_HEAD_ILLEGAL                     OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x50)

/*
 * @ingroup OS_mem
 * 内存错误码：写内存时超过申请内存大小，overflow。
 *
 * 值: 0x03000151
 *
 * 解决方案: 保证对内存写操作时，不要出现写越界。
 *
 */
#define OS_ERRNO_MEM_FREE_OVERFLOW                          OS_ERRNO_BUILD_FATAL(OS_MID_MEM, 0x51)

/*
 * @ingroup OS_mem
 * 内存错误码：DSSC算法在fsc申请内存控制头时，申请错误。
 *
 * 值: 0x02000152
 *
 * 解决方案: 检查0号分区的大小，确保0号分区的空间足够。
 *
 */
#define OS_ERRNO_DSSC_ALLOC_HEAD_NO_MEMORY                 OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x52)

/*
 * @ingroup OS_mem
 * 内存错误码：DSSC算法在释放内存时，内存地址非法。
 *
 * 值: 0x02000153
 *
 * 解决方案: 确保被释放内存地址正确。
 *
 */
#define OS_ERRNO_DSSC_FREE_ADDR_ILLEGAL                    OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x53)

/*
 * @ingroup OS_mem
 * 内存错误码：DSSC算法在释放内存头错误。
 *
 * 值: 0x03000154
 *
 * 解决方案: 检查是否踩内存。
 *
 */
#define OS_ERRNO_DSSC_FREE_HEAD_ERROR                      OS_ERRNO_BUILD_FATAL(OS_MID_MEM, 0x54)

/*
 * @ingroup OS_mem
 * 内存错误码：DSW算法释放已释放过的内存。
 *
 * 值: 0x02000155
 *
 * 解决方案: 不要重复释放内存。
 *
 */
#define OS_ERRNO_DSW_MEM_FREE_REPEAT                    OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x55)

/*
 * @ingroup OS_mem
 * 内存错误码：DSW算法配置的状态字内存不足。
 *
 * 值: 0x02000156
 *
 * 解决方案: 计算所需的状态字内存大小，确保配置状态字内存足够。
 *
 */
#define OS_ERRNO_DSW_SW_NOT_ENOUGH                      OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x56)

/*
 * @ingroup OS_mem
 * 内存错误码：DSW算法状态字内存配置有误。
 *
 * 值: 0x02000157
 *
 * 解决方案: 状态字内存地址和大小必须4字节对齐。
 *
 */
#define OS_ERRNO_DSW_SW_CFG_INVALID                    OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x57)

/*
 * @ingroup OS_mem
 * 内存错误码：DSW算法分区正在使用无法删除。
 *
 * 值: 0x02000158
 *
 * 解决方案: DSW算法分区不支持删除操作。
 *
 */
#define OS_ERRNO_DSW_PT_DELETE_BUSY                    OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x58)

/*
 * @ingroup OS_mem
 * 内存错误码：释放的内存非DSW算法分区内存。
 *
 * 值: 0x02000159
 *
 * 解决方案: 确保释放内存是从指定DSW分区内申请的内存。
 *
 */
#define OS_ERRNO_DSW_MEM_INVALID_ADDR                 OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x59)

/*
 * @ingroup OS_mem
 * 内存错误码：创建DSW算法分区失败，申请不到分区控制信息。
 *
 * 值: 0x0200015a
 *
 * 解决方案: 增加1号分区的内存(分区控制块从1号分区申请)。
 *
 */
#define OS_ERRNO_DSW_MEM_NO_CTRL_MEM                  OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x5a)

/*
 * @ingroup OS_mem
 * 内存错误码：DSW算法状态字内存多核配置不一致。
 *
 * 值: 0x0200015b
 *
 * 解决方案: 实例内多核配置DSW算法状态字内存确保地址和大小一致。
 *
 */
#define OS_ERRNO_DSW_SW_CFG_INCONSISTENT              OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x5b)

/*
 * @ingroup OS_mem
 * 内存错误码：补充缓存块的个数不合法，为0或者超过了支持的最大个数。
 *
 * 值: 0x0200015c
 *
 * 解决方案: 期望补充的缓存块个数应不等于0，并且不大于支持的最大个数
 *
 */
#define OS_ERRNO_MEM_CACHE_FILL_NUM_INVALID           OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x5c)

/*
 * @ingroup OS_mem
 * 内存错误码：补充缓存块过程中发生了，任务发生核迁移
 *
 * 值: 0x0200015d
 *
 * 解决方案: 如果业务不能保证，应调用绑核接口，在缓存补充前将任务绑在所在核
 *
 */
#define OS_ERRNO_MEM_CACHE_FILL_CORE_MIGRATE          OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x5d)

/*
 * @ingroup OS_mem
 * 内存错误码：补充缓存块时分区内存不足
 *
 * 值: 0x0200015e
 *
 * 解决方案: 增大分区的内存大小或者检查是否有内存泄露
 *
 */
#define OS_ERRNO_MEM_CACHE_FILL_MEM_NOT_ENOUGH        OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x5e)

/*
 * @ingroup OS_mem
 * 内存错误码：分区控制块内存申请失败
 *
 * 值: 0x0300015f
 *
 * 解决方案: 根据创建分区的类型，增加对应的控制块分区的内存，比如mcfsc是从1号分区申请内存，其他分区是0号分区申请内存。
 *
 */
#define OS_ERRNO_FSCMEM_ALLOC_NO_MEMORY_FATAL         OS_ERRNO_BUILD_FATAL(OS_MID_MEM, 0x5f)

/*
 * @ingroup OS_mem
 * 内存错误码：获取缓存块信息时，传入的个数不正确
 *
 * 值: 0x02000160
 *
 * 解决方案: 获取缓存块信息时，传入的个数需要与分区支持的缓存块类型个数相同
 *
 */
#define OS_ERRNO_MEM_CACHE_TYPE_NUM_INVALID           OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x60)

/*
 * @ingroup OS_mem
 * 内存错误码：分区控制块地址未分配
 * 值: 0x02000161
 *
 * 解决方案: 实例间未分配该pt号对应的内存池地址
 */
#define OS_ERRNO_MEM_MI_FSC_PTNO_NO_ALLOC             OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x61)

/*
 * @ingroup OS_mem
 * 内存错误码：实例间共享分区号未分配给该实例
 * 值: 0x02000162
 *
 * 解决方案: 与分区创建时指定的userInstId保持一致
 */
#define OS_ERRNO_MEM_MI_FSC_PTNO_NO_MATCH_TO_INSTID   OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x62)

/*
 * @ingroup OS_mem
 * 内存错误码：实例间共享分区号已被占用
 * 值: 0x02000163
 *
 * 解决方案: 确认分区号划分的正确性
 */
#define OS_ERRNO_MEM_MI_FSC_PTNO_NO_ALREADY_USED   OS_ERRNO_BUILD_ERROR(OS_MID_MEM, 0x63)

/*
 * @ingroup OS_mem
 * 系统缺省的内存分区为0号分区和1号分区。
 */
#define OS_MEM_DEFAULT_PT0 0
#define OS_MEM_DEFAULT_PT1 1
#define OS_MEM_DEFAULT_PTNUM 2

/*
 * @ingroup OS_mem
 * 缺省私有FSC内存分区。
 */
#define OS_MEM_DEFAULT_FSC_PT (OS_MEM_DEFAULT_PT0)

/*
 * @ingroup OS_mem
 * OS支持核间通信的前提下，缺省共享静态FSC内存分区。
 */
#define OS_MEM_DEFAULT_MCSTA_DYN_PT 1

/*
 * @ingroup OS_mem
 * 支持不可cache内存申请时的分区号。
 */
#define OS_MEM_DEFAULT_UNCACHE_PT (OS_MEM_DEFAULT_PT1)

/* block 算法支持配置的最大核个数 */
#define OS_MEM_BLOCK_MAX_CORE_NUM   16

/*
 * @ingroup OS_mem
 * 内存算法类型。
 */
enum MemArith {
    MEM_ARITH_FSC,          // 私有FSC算法
#if defined(OS_OPTION_MEM_SLUB_FSC)
    MEM_ARITH_SLUB_FSC,
#endif
#if defined(OS_OPTION_MEM_BLOCK)
    MEM_ARITH_BLOCK,
#endif
#if defined(OS_OPTION_MEM_SSC)
    MEM_ARITH_SSC,
    MEM_ARITH_LSSC,
#endif
#if defined(OS_OPTION_MEM_MCDYNFSC)
    MEM_ARITH_MCDYNFSC,     // 共享动态FSC算法
#endif
#if defined(OS_OPTION_MEM_POOL)
    MEM_ARITH_POOL,         // 私有POOL算法
    MEM_ARITH_MCPOOL,       // 共享POOL算法
#endif
#if defined(OS_OPTION_MEM_MAP_POOL)
    MEM_ARITH_MCMAPPOOL,    // 共享 MAP-POOL算法
#endif
#if defined(OS_OPTION_MEM_DSSC)
    MEM_ARITH_DSSC,
#endif
#if defined(OS_OPTION_MEM_DSW)
    MEM_ARITH_DSW,        // DSW算法
#endif
    MEM_ARITH_BUTT          // 内存算法非法
};

/*
 * @ingroup OS_mem
 * BLOCK内存算法信息结构体。
 */
struct MemBlockInfo {
    /* BLOCK块大小 */
    U32 size;
    /* BLOCK块总个数 */
    U32 num;
    /* 每核缓存的BLOCK块上限值 */
    U32 cacheMaxNum[OS_MEM_BLOCK_MAX_CORE_NUM];
    /* BLOCK块补充个数 */
    U32 appendNum;
};

/*
 * @ingroup OS_mem
 * 动态创建分区接口参数结构体。
 */
struct MemPtCreate {
    /* 分区的算法类型 */
    enum MemArith arith;
    /* 分区起始地址 */
    void *ptAddr;
    /* 分区大小 (单位：字节） */
    uintptr_t ptSize;
    /* 指定该块物理内存是否为可cache */
    bool cachable;
    /* 最大八种内存块大小(单位：字节） */
#if defined(OS_OPTION_MEM_POOL)
    U16 sliceSize[8];
#endif
#if defined(OS_OPTION_MEM_BLOCK)
    struct MemBlockInfo *blockInfo;
    U32 num;
#endif
#if ((defined OS_OPTION_MEM_MAP_POOL) || (defined OS_OPTION_MEM_DSW))
    uintptr_t unitSize;
#endif
};

/*
 * @ingroup OS_mem
 * 获取指定分区内存信息接口参数结构体。
 */
struct MemStatInfo {
    /* 空闲内存的大小（单位：字节） */
    uintptr_t freeSize;
    /* 最大块大小（单位：字节） */
    uintptr_t maxBlockSize;
    /* 分区内存使用峰值（单位：字节） */
    uintptr_t peakUsed;
    /* 内存分区总大小 */
    uintptr_t ptSize;
};

#if defined(OS_OPTION_MEM_DBG)
/*
 * @ingroup OS_mem
 * 内存块维测信息结构体。
 */
struct MemDbgInfo {
    /* 申请内存时的任务ID。 */
    U32 pid;
    /* 可存放申请内存时调用者的PC，或者一些用户指定的值 */
    U32 cookie;
    /* 内存块的总大小(包括内存控制头)。 */
    U32 size;
#if defined(OS_OPTION_MEM_MI_FSC)
    /* 待free标记: 1表示需要free; 0表示inuse，不能free */
    U32 needFreeFlag;
#endif
};
#endif

#if defined(OS_OPTION_MEM_APPEND)
struct MemAppendCfg {
    /* 内存地址，4字节对齐 */
    uintptr_t addr;
    /* 内存大小，4字节对齐 */
    uintptr_t len;
};
#endif

/*
 * @ingroup OS_mem
 * 支持的缓存内存块类型信息，只针对支持局部缓存的算法有效
 */
struct OsMemCacheTypeInfo {
    U32 size;     // block的大小
};

/*
 * @ingroup OS_mem
 * 指定缓存内存块的信息，只针对支持局部缓存的算法有效
 */
struct OsMemCacheInfo {
    U32 currentNum;     // 当前缓存个数
    U32 maxSupportNum;  // 最大支持的缓存个数
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_MEM_H */
