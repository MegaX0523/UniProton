/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 原子操作对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_ATOMIC_H
#define OS_ATOMIC_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define OS_SPINLOCK_UNLOCK 0
#define OS_SPINLOCK_LOCK   1

/*
 * @ingroup OS_atomic
 * 锁不被任何owner持有，处于空闲状态。
 */
#define OS_INVALID_LOCK_OWNER_ID 0xFFFFFFFE

/*
 * @ingroup OS_atomic
 * 锁被非任务线程持有(中断、系统钩子等)。
 */
#define OS_LOCK_OWNER_SYS 0xFFFFFFFF
/*
 * @ingroup OS_atomic
 * SPL地址管理错误码: 初始化地址不合法。
 *
 * 值: 0x02004900
 *
 * 解决方案：请调整SPLINIT中的首地址。
 */
#define OS_ERRNO_SPL_ADDR_SIZE_ILLEGAL                       OS_ERRNO_BUILD_ERROR(OS_MID_SPL, 0x00)

/*
 * @ingroup OS_atomic
 * SPL地址管理错误码:初始化地址及大小未对齐。
 *
 * 值: 0x02004902
 *
 * 解决方案：修改初始化地址及大小使其对齐。
 */
#define OS_ERRNO_SPL_ADDR_SIZE_NOT_ALIGN                      OS_ERRNO_BUILD_ERROR(OS_MID_SPL, 0x02)

/*
 * @ingroup OS_atomic
 * SPL地址管理错误码: 地址大小配置不合法。
 *
 * 值: 0x02004903
 *
 * 解决方案：修改地址大小。
 */
#define OS_ERRNO_SPL_SIZE_INVALID                             OS_ERRNO_BUILD_ERROR(OS_MID_SPL, 0x03)

/*
 * @ingroup OS_atomic
 * SPL地址管理错误码: 输入的地址为空。
 *
 * 值: 0x02004904
 *
 * 解决方案：检查输入。
 */
#define OS_ERRNO_SPL_ALLOC_ADDR_INVALID                       OS_ERRNO_BUILD_ERROR(OS_MID_SPL, 0x04)

/*
 * @ingroup OS_atomic
 * SPL地址管理错误码:锁申请地址时已无可用地址。
 *
 * 值: 0x02004905
 *
 * 解决方案：检查锁释放是否正常，或调整初始化地址大小。
 */
#define OS_ERRNO_SPL_NO_FREE                                  OS_ERRNO_BUILD_ERROR(OS_MID_SPL, 0x05)

/*
 * @ingroup OS_atomic
 * SPL地址管理错误码: 锁释放地址为空。
 *
 * 值: 0x02004906
 *
 * 解决方案：检查入参是否正确，修改释放地址使之不为空。
 */
#define OS_ERRNO_SPL_FREE_ADDR_INVALID                        OS_ERRNO_BUILD_ERROR(OS_MID_SPL, 0x06)

/*
 * @ingroup OS_atomic
 * SPL地址管理错误码: OS内部初始化锁时出错
 *
 * 值: 0x03004907
 *
 * 解决方案：检查内部内参或者自旋锁资源是否已消耗完
 */
#define OS_ERRNO_SPL_INIT_FAIL                                OS_ERRNO_BUILD_FATAL(OS_MID_SPL, 0x07)
struct OsSpinLock {
    volatile uintptr_t rawLock; /* *< 锁地址 */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OS_ATOMIC_H */
