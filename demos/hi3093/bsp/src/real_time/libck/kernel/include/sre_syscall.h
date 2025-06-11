/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: 系统模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2020-06-16
 * History: 2020-06-16 LibCK项目组 创建文件
 */
/*
 * @defgroup SRE_syscall 系统调用功能
 * @ingroup SRE_kernel
 */
#ifndef SRE_SYSCALL_H
#define SRE_SYSCALL_H

#include "sre_base.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_syscall
 * 系统调用的个数
 *
 */
#define OS_SYSCALL_NUM   128

/*
 * @ingroup  SRE_sys
 * @brief 发起系统调用，限用户态调用
 *
 * @par 描述
 * 发起系统调用0~OS_SYSCALL_MAX，限用户态调用。
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param syscallNo  [IN]  类型#U32，系统调用号
 * @param arg1       [IN]  类型#uintptr_t，透传的参数1
 * @param arg2       [IN]  类型#uintptr_t，透传的参数2
 * @param arg3       [IN]  类型#uintptr_t，透传的参数3
 * @param arg4       [IN]  类型#uintptr_t，透传的参数4
 *
 * @retval #SRE_FAIL               1， 系统调用号超过OS_SYSCALL_NUM(128)
 * @retval 其他                    陷入EL3后，EL3层函数的返回值。
 * @par 依赖: sre_syscall.h：该接口声明所在的头文件。
 * @since
 * @see NA
 */
extern U32 SRE_Syscall(U32 syscallNo, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t arg4);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_SYSCALL_H */
