/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: CACHE模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * * @defgroup sre_cache Cache
 * @ingroup SRE_kernel
 */

#ifndef SRE_CACHE_H
#define SRE_CACHE_H

#include "sre_base.h"
#include "./common/os_cache.h"

#if ((OS_HARDWARE_PLATFORM == OS_HIAVPV200) || (OS_HARDWARE_PLATFORM == OS_HIDSPV200) || \
     (OS_HARDWARE_PLATFORM == OS_HIDSP) || (OS_HARDWARE_PLATFORM == OS_HIAVPV280) ||     \
     (OS_HARDWARE_PLATFORM == OS_HIAVPV300LITE) || (OS_HARDWARE_PLATFORM == OS_HIDSPV300LITE))
#include "./hw/cpu/os_cache_hidsp.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_DSP220) || (OS_HARDWARE_PLATFORM == OS_HACCC) || \
     (OS_HARDWARE_PLATFORM == OS_RISC220) || (OS_HARDWARE_PLATFORM == OS_DSP100))
#include "./hw/cpu/os_cache_xtensa.h"
#endif

#if ((OS_HARDWARE_PLATFORM == OS_CORTEX_RX) || (OS_HARDWARE_PLATFORM == OS_CORTEX_AX))
#include "./hw/cpu/os_cache_armv7.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#if !defined(OS_OPTION_DCACHE)

/*
 * @ingroup sre_cache
 * 回写所有DCACHE
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_DCacheFlushAll(void)
{
    return SRE_OK;
}

/*
 * @ingroup sre_cache
 * 将数据Cache的内容回写到内存。
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_DCacheFlush(void *addr, U32 size, bool wait)
{
    (void)addr;
    (void)size;
    (void)wait;
    return SRE_OK;
}

/*
 * @ingroup sre_cache
 * 将数据Cache中的内容无效。
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_DCacheInv(const void *addr, U32 size, bool wait)
{
    (void)addr;
    (void)size;
    (void)wait;

    return SRE_OK;
}

/*
 * @ingroup sre_cache
 * 将数据Cache里的内容回写到内存并无效该数据cache。
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_DCacheFlushInv(void *addr, U32 size, bool wait)
{
    (void)addr;
    (void)size;
    (void)wait;

    return SRE_OK;
}

/*
 * @ingroup sre_cache
 * 等待DCache操作完成。
 */
OS_SEC_ALW_INLINE INLINE void SRE_DCacheWait(void)
{
    return;
}

/*
 * @ingroup sre_cache
 * 预取数据CACHE
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_DCachePrefetch(const void *virtAddr, U32 size, bool wait)
{
    (void)virtAddr;
    (void)size;
    (void)wait;

    return SRE_OK;
}
#else

/*
 * @ingroup  sre_cache
 * @brief 回写所有DCACHE
 *
 * @par 描述
 * 刷新所有DCACHE, 将其中所有数据回写到内存
 *
 * @attention
 * <ul>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，在关中断条件下进行该操作，关中断时间长短与整个Dcache大小成正比。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval #SRE_OK                        0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_cache.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_DCacheFlushInv
 */
extern U32 SRE_DCacheFlushAll(void);

/*
 * @ingroup  sre_cache
 * @brief 将数据Cache里的内容回写到内存。
 *
 * @par 描述
 * 将指定地址区域的数据Cache内容回写到内存中。
 *
 * @attention
 * <ul>
 * <li>Cache操作的最小单位为Cache line。因此，操作范围为指定地址区域所跨的所有Cache line</li>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，输入的参数#wait必须为TRUE</li>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，在关中断条件下进行该操作，关中断时间长短与Dcache回写内存大小成正比</li>
 * <li>对于SD6183及以后平台，用户传入的虚拟地址空间对应的物理地址空间必须连续且没有翻转</li>
 * <li>对于SD6183及以后平台，如果传入的虚拟地址空间是动态库的PL2段，可以支持虚地址连续物理地址不连续，不能跨段操作</li>
 * </ul>
 *
 * @param addr [IN]  类型#void *，内存块的起始地址。
 * @param size [IN]  类型#U32，内存块的大小。
 * @param wait [IN]  类型#bool，TRUE为等Cache操作完成后才返回，FALSE为执行操作指令后立即返回。
 *
 * @retval #OS_ERRNO_CACHE_SIZE_INVALID   0x02001101，操作cache区域大小不合法。
 * @retval #OS_ERRNO_MMU_VA2PA_ERROR      0x02001239，虚地址转实地址失败。
 * @retval #SRE_OK                        0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_cache.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_DCacheFlushInv
 */
extern U32 SRE_DCacheFlush(void *addr, U32 size, bool wait);

/*
 * @ingroup  sre_cache
 * @brief 数据Cache回写并无效。
 *
 * @par 描述
 * 将指定地址区域的数据Cache内容回写至内存并无效掉。
 *
 * @attention
 * <ul>
 * <li>Cache操作的最小单位为Cache line。因此，操作范围为指定地址区域所跨的所有Cache line</li>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，输入的参数#wait必须为TRUE</li>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，在关中断条件下进行该操作，</li>
 * <li>关中断时间长短与Dcache回写并无效内存大小成正比</li>
 * <li>对于SD6183及以后平台，用户传入的虚拟地址空间对应的物理地址空间必须连续且没有翻转</li>
 * <li>对于SD6183及以后平台，如果传入的虚拟地址空间是动态库的PL2段，可以支持虚地址连续物理地址不连续，不能跨段操作</li>
 * </ul>
 *
 * @param addr [IN]  类型#void *，内存块的起始地址。
 * @param size [IN]  类型#U32，内存块的大小。
 * @param wait [IN]  类型#bool，TRUE为等Cache操作完成后才返回，FALSE为执行操作指令后立即返回。
 *
 * @retval #OS_ERRNO_CACHE_SIZE_INVALID   0x02001101，操作cache区域大小不合法。
 * @retval #OS_ERRNO_MMU_VA2PA_ERROR      0x02001239，虚地址转实地址失败。
 * @retval #SRE_OK                        0x00000000，操作成功。
 *
 * @par 依赖
 * <ul><li>sre_cache.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_ICacheInv
 */
extern U32 SRE_DCacheFlushInv(void *addr, U32 size, bool wait);

/*
 * @ingroup  sre_cache
 * @brief 数据Cache无效。
 *
 * @par 描述
 * 将指定地址区域的数据Cache内容无效掉。
 *
 * @attention
 * <ul>
 * <li>Cache操作的最小单位为Cache line。因此，操作范围为指定地址区域所跨的所有Cache line</li>
 * <li>对于SD6183及以后平台，用户传入的虚拟地址空间对应的物理地址空间必须连续且没有翻转</li>
 * <li>对于SD6183及以后平台，如果传入的虚拟地址空间是动态库的PL2段，可以支持虚地址连续物理地址不连续，不能跨段操作</li>
 * <li>对于入参addr，OS已做基本校验，无法校验所有非法地址，其合法性由业务保证</li>
 * <li>对于入参size，OS已做基本校验，无法校验所有非法size，其合法性由业务保证</li>
 * </ul>
 *
 * @param addr [IN]  类型#const void *，内存块的起始地址。
 * @param size [IN]  类型#U32，内存块的大小。
 * @param wait [IN]  类型#bool，TRUE为等Cache操作完成后才返回，FALSE为执行操作指令后立即返回。
 *
 * @retval #OS_ERRNO_CACHE_SIZE_INVALID   0x02001101，操作cache区域大小不合法。
 * @retval #OS_ERRNO_MMU_VA2PA_ERROR      0x02001239，虚地址转实地址失败。
 * @retval #SRE_OK                        0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_cache.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_ICacheInv | SRE_DCacheFlushInv
 */
extern U32 SRE_DCacheInv(const void *addr, U32 size, bool wait);

#endif

/*
 * @ingroup  sre_cache
 * @brief 无效指令Cache
 *
 * @par 描述
 * 将指定地址区域的指令Cache置为无效。
 *
 * @attention
 * <ul>
 * <li>Cache操作的最小单位为Cache line。因此，操作范围为指定地址区域所跨的所有Cache line</li>
 * <li>为保证指令cache操作的安全性，Icache操作都是以等待方式操作，wait参数无效</li>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，输入的参数#wait必须为TRUE</li>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，在关中断条件下进行该操作，关中断时间长短与Icache无效内存大小成正比</li>
 * <li>对于SD6183及以后平台，用户传入的虚拟地址空间对应的物理地址空间必须连续且没有翻转</li>
 * <li>对于SD6183及以后平台，如果传入的虚拟地址空间是动态库的PL2段，可以支持虚地址连续物理地址不连续，不能跨段操作</li>
 * </ul>
 *
 * @param addr [IN]  类型#void *，内存块的起始地址。
 * @param size [IN]  类型#U32，内存块的大小。
 * @param wait [IN]  类型#bool，TRUE为等Cache操作完成后才返回，FALSE为执行操作指令后立即返回。
 *
 * @retval #OS_ERRNO_CACHE_SIZE_INVALID   0x02001101，操作cache区域大小不合法。
 * @retval #OS_ERRNO_MMU_VA2PA_ERROR      0x02001239，虚地址转实地址失败。
 * @retval #SRE_OK                        0x00000000，操作成功。
 *
 * @par 依赖
 * <ul><li>sre_cache.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_DCacheFlushInv
 */
extern U32 SRE_ICacheInv(void *addr, U32 size, bool wait);

/*
 * @ingroup  sre_cache
 * @brief 更新指令及数据Cache
 *
 * @par 描述
 * 将指定地址区域回写并无效数据Cache，然后再无效指令cache。
 *
 * @attention
 * <ul>
 * <li>Cache操作的最小单位为Cache line。因此，操作范围为指定地址区域所跨的所有Cache line</li>
 * <li>无DCACHE情况下，此接口等同SRE_ICacheInv</li>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，输入的参数#wait必须为TRUE</li>
 * <li>对于Cortex-R8平台，当L2 Cache功能打开时，在关中断条件下进行该操作，关中断时间长短与Icache无效内存大小成正比</li>
 * <li>对于SD6183及以后平台，用户传入的虚拟地址空间对应的物理地址空间必须连续且没有翻转</li>
 * <li>对于SD6183及以后平台，如果传入的虚拟地址空间是动态库的PL2段，可以支持虚地址连续物理地址不连续，不能跨段操作</li>
 * </ul>
 *
 * @param addr [IN]  类型#void *，内存块的起始地址。
 * @param size [IN]  类型#U32，内存块的大小。
 * @param wait [IN]  类型#bool，TRUE为等Cache操作完成后才返回，FALSE为执行操作指令后立即返回。
 *
 * @retval #OS_ERRNO_CACHE_SIZE_INVALID   0x02001101，操作cache区域大小不合法。
 * @retval #OS_ERRNO_MMU_VA2PA_ERROR      0x02001239，虚地址转实地址失败。
 * @retval #SRE_OK                        0x00000000，操作成功。
 *
 * @par 依赖
 * <ul><li>sre_cache.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_DCacheFlushInv | SRE_ICacheInv
 */
extern U32 SRE_CacheUpdate(void *addr, U32 size, bool wait);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_CACHE_H */
