/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: CACHE模块的基础头文件，用户不能直接包含
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_CACHE_H
#define OS_CACHE_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_cache
 * cache类型为指令cache。
 */
#define OS_ICACHE_TYPE 1

/*
 * @ingroup os_cache
 * cache类型为数据cache。
 */
#define OS_DCACHE_TYPE 2

/*
 * @ingroup os_cache
 * cache大小参数设置不合法。
 */
#define OS_CACHE_SIZE_INVALID 0x00000001

/*
 * @ingroup os_cache
 * cache地址参数类型设置不合法 。
 */
#define OS_CACHE_ADDR_INVALID 0x00000002

/*
 * @ingroup os_cache
 * cache类型参数设置不合法 。
 */
#define OS_CACHE_TYPE_INVALID 0x00000003

/*
 * @ingroup os_cache
 * cache加锁余量不足。
 */
#define OS_CACHE_NOAREA_LOCK 0x00000006

/*
 * @ingroup os_cache
 * cache TAG获取接口入参地址为空。
 */
#define OS_CACHE_TAG_PTR_NULL 0x00000007

/*
 * @ingroup os_cache
 * cache操作地址被加锁。
 */
#define OS_CACHE_ADDR_LOCKED 0x00000008

/*
 * @ingroup os_cache
 * cache错误码: 使用cache接口，操作cache区域大小为0，或者加锁解锁预取接口操作的实际大小超出最大可操作范围，则返回该值。
 *
 * 值: 0x02001101
 *
 * 解决方案: 请调整操作空间大小。
 */
#define OS_ERRNO_CACHE_SIZE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, OS_CACHE_SIZE_INVALID)

/*
 * @ingroup OS_cache
 * cache错误码: 使用加锁解锁以及获取tag接口时，操作地址超出可cache的范围。
 *
 * 值: 0x02001102
 *
 * 解决方案: 请查看操作地址空间是否可cache。
 */
#define OS_ERRNO_CACHE_ADDR_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, OS_CACHE_ADDR_INVALID)

/*
 * @ingroup OS_cache
 * cache错误码
 * 调用cache加解锁、获取tag接口以及SD6183设置Icache模式时，输入的cache类型不正确，目前只支持ICACHE
 * 进行加解锁，输入其它值报错误码，获取cache
 * tag接口支持指令cache和数据cache类型，只能是PCC或者DCC。SD6183设置ICache模式时只能是Split或者UNIFY模式。
 *
 * 值: 0x02001103
 *
 * 解决方案: 请查看输入的cache类型是否支持。
 */
#define OS_ERRNO_CACHE_TYPE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, OS_CACHE_TYPE_INVALID)

/*
 * @ingroup OS_cache
 * cache错误码: cache加锁过程中，出现某一组加锁余量不足。
 *
 * 值: 0x02001106
 *
 * 解决方案: 加锁解锁接口需要配对使用，加锁空间范围有限，请查看是否进行多次加锁。
 */
#define OS_ERRNO_CACHE_NOAREA_LOCK OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, OS_CACHE_NOAREA_LOCK)

/*
 * @ingroup OS_cache
 * cache错误码: cache tag获取接口，输入的tag结构体指针为空。
 *
 * 值: 0x02001107
 *
 * 解决方案: 请输入有效的结构体指针。
 */
#define OS_ERRNO_CACHE_TAG_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, OS_CACHE_TAG_PTR_NULL)

/*
 * @ingroup OS_cache
 * cache错误码: 调用指令cache无效接口时，由于该无效空间存在加锁情况，无效不成功。
 *
 * 值: 0x02001108
 *
 * 解决方案: 无效前请检查该地址是否加锁。
 */
#define OS_ERRNO_CACHE_ADDR_LOCKED OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, OS_CACHE_ADDR_LOCKED)

/*
 * @ingroup OS_cache
 * cache错误码: 核号错误，HiDSP只能为0-33。
 *
 * 值: 0x02001109
 *
 * 解决方案: 请输入正确的核号。
 */
#define OS_ERRNO_CACHE_CORE_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x09)

/*
 * @ingroup OS_cache
 * cache错误码: PCC硬件预取策略错误。
 *
 * 值: 0x0200110a
 *
 * 解决方案: 请修改PCC硬件预取策略。
 */
#define OS_ERRNO_CACHE_PCC_HWPF_POLICY_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x0a)

/*
 * @ingroup OS_cache
 * cache错误码: 获取指定地址的cache tag信息时，输入的CACHE Tag结构体指针为空。
 *
 * 值: 0x0200110c
 *
 * 解决方案: 请输入的有效的结构体指针。
 */
#define OS_ERRNO_CACHE_INFO_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x0c)

/*
 * @ingroup OS_cache
 * cache错误码: DCC硬件预取策略错误。
 *
 * 值: 0x0200110d
 *
 * 解决方案: 请修改DCC硬件预取策略。
 */
#define OS_ERRNO_CACHE_DCC_HWPF_POLICY_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x0d)

/*
 * @ingroup OS_cache
 * CACHE错误码: PCC 锁余量不足。
 *
 * 值: 0x0200110e
 *
 * 解决方案 :4块可加锁地址区域已经使用完，需解锁才能继续加锁。
 */
#define OS_ERRNO_CACHE_PCC_LOCK_BLOCK_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x0e)

/*
 * @ingroup OS_cache
 * 消息错误码：cache锁操作的地址范围发生了反转。
 *
 * 值: 0x0200110f
 *
 * 解决方案: 请保证传入的锁虚拟地址对应的物理地址是连续的且没有发生反转。
 *
 */
#define OS_ERRNO_CACHE_REGION_REVERSE OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x0f)

/*
 * @ingroup OS_cache
 * 消息错误码：cache锁操作的地址范围已经被锁。
 *
 * 值: 0x02001110
 *
 * 解决方案: 确认当前锁操作的地址范围是否已被锁操作。
 *
 */
#define OS_ERRNO_CACHE_REGION_ALREARY_LOCK OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x10)

/*
 * @ingroup OS_cache
 * 消息错误码：异步方式操作cache后发送的MSG内容为空。
 *
 * 值: 0x02001111
 *
 * 解决方案: 请保证传入的MSG内容不为空。
 *
 */
#define OS_ERRNO_CACHE_ASYNC_MSG_NULL OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x11)

/*
 * @ingroup OS_cache
 * 消息错误码：解锁操作对应的地址区域未加锁。
 *
 * 值: 0x02001112
 *
 * 解决方案: 请检查传入解锁地址范围是否正确。
 *
 */
#define OS_ERRNO_CACHE_PCC_LOCK_REGION_UNLOCK OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x12)

/*
 * @ingroup OS_cache
 * 消息错误码：L2 Cache控制基地址为0。
 *
 * 值: 0x02001113
 *
 * 解决方案: L2 Cache控制基地址不能为0。
 *
 */
#define OS_ERRNO_L2_CACHE_CTRL_BASE_ADDR_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x13)

/*
 * @ingroup OS_cache
 * 消息错误码：调用L2 Cache低功耗Suspend和Resume操作函数时，L2 Cache未初始化。
 *
 * 值: 0x02001114
 *
 * 解决方案: 调用L2 Cache低功耗Suspend和Resume操作函数前，需要对Cache进行初始化。
 *
 */
#define OS_ERRNO_L2_CACHE_NOT_INIT OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x14)

/*
 * @ingroup OS_cache
 * 消息错误码：重复或者覆盖注册已经注册的L2 Cache优化钩子。
 *
 * 值: 0x02001115
 *
 * 解决方案: 注册L2 Cache优化钩子时在取消前只能注册一次。
 *
 */
#define OS_ERRNO_L2_CACHE_HOOK_EXIST OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x15)

/*
 * @ingroup OS_cache
 * 消息错误码：重复Suspend L2 Cache。
 *
 * 值: 0x02001116
 *
 * 解决方案: Suspend L2 Cache后，必须Resume L2 Cache，才能再次Suspend L2 Cache。
 *
 */
#define OS_ERRNO_L2_CACHE_ALREADY_SUSPENDED OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x16)

/*
 * @ingroup OS_cache
 * 消息错误码：未Suspend L2 Cache前Resume L2 Cache。
 *
 * 值: 0x02001117
 *
 * 解决方案: 必须在Suspend L2 Cache后，才能Resume L2 Cache。
 *
 */
#define OS_ERRNO_L2_CACHE_NOT_SUSPENDED OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x17)

/*
 * @ingroup OS_cache
 * 消息错误码：硬件不支持软件预取功能
 *
 * 值: 0x02001118
 *
 * 解决方案: 不调用SRE_ICachePrefetch接口
 *
 */
#define OS_ERRNO_CACHE_SWPF_UNSUPPORT                     OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x18)

/*
 * @ingroup OS_cache
 * 消息错误码：锁不连续PA的Cache，不联系PA内存块超过4块。
 *
 * 值: 0x02001119
 *
 * 解决方案: 映射的不连续PA内存块必须在4块以内。
 *
 */
#define OS_ERRNO_CACHE_PA_BLOCK_LOCK_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_CACHE, 0x19)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_CACHE_H */
