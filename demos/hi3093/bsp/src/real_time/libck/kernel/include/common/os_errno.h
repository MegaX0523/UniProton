/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 通用错误码定义头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_ERRNO_H
#define OS_ERRNO_H

#include "os_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_err
 * OS错误码标记位。(0x00表示Guest OS,0xFF表示DRV,0x01表示Host OS
 *
 */
#define ERRNO_OS_ID    (0x00U << 16)

/*
 * @ingroup OS_err
 * 定义错误的等级:提示级别
 *
 */
#define ERRTYPE_NORMAL (0x00U << 24)

/*
 * @ingroup OS_err
 * 定义错误的等级:告警级别
 *
 */
#define ERRTYPE_WARN   (0x01U << 24)

/*
 * @ingroup OS_err
 * 定义错误的等级:严重级别
 *
 */
#define ERRTYPE_ERROR  (0x02U << 24)

/*
 * @ingroup OS_err
 * 定义错误的等级:致命级别
 *
 */
#define ERRTYPE_FATAL  (0x03U << 24)

/*
 * @ingroup  OS_err
 * @brief 定义OS致命错误。
 *
 * @par 描述
 * 宏定义，定义OS致命错误。
 *
 * @attention 无
 *
 * @param  mid   [IN] 模块ID编号。
 * @param  errno [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖
 * os_errno.h: 该宏定义所在的头文件。
 * @see OS_ERRNO_BUILD_ERROR | OS_ERRNO_BUILD_WARN | OS_ERRNO_BUILD_NORMAL
 */
#define OS_ERRNO_BUILD_FATAL(mid, errno) (ERRTYPE_FATAL | ERRNO_OS_ID | ((U32)(mid) << 8) | (errno))

/*
 * @ingroup  OS_err
 * @brief 定义OS严重错误
 *
 * @par 描述
 * 宏定义，定义OS严重错误
 *
 * @attention 无
 * @param  mid   [IN] 模块ID编号。
 * @param  errno [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖
 * os_errno.h: 该宏定义所在的头文件。
 * @see OS_ERRNO_BUILD_FATAL | OS_ERRNO_BUILD_WARN | OS_ERRNO_BUILD_NORMAL
 */
#define OS_ERRNO_BUILD_ERROR(mid, errno) (ERRTYPE_ERROR | ERRNO_OS_ID | ((U32)(mid) << 8) | (errno))

/*
 * @ingroup  OS_err
 * @brief 定义OS告警信息
 *
 * @par 描述
 * 宏定义，定义OS告警
 *
 * @attention 无
 * @param  mid   [IN] 模块ID编号。
 * @param  errno [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖
 * os_errno.h: 该宏定义所在的头文件。
 * @see OS_ERRNO_BUILD_FATAL | OS_ERRNO_BUILD_ERROR | OS_ERRNO_BUILD_NORMAL
 */
#define OS_ERRNO_BUILD_WARN(mid, errno) (ERRTYPE_WARN | ERRNO_OS_ID | ((U32)(mid) << 8) | (errno))

/*
 * @ingroup  OS_err
 * @brief 定义OS提示信息
 *
 * @par 描述
 * 宏定义，定义OS提示信息
 *
 * @attention 无
 * @param  mid   [IN] 模块ID编号。
 * @param  errno [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖
 * os_errno.h: 该宏定义所在的头文件。
 * @see OS_ERRNO_BUILD_FATAL | OS_ERRNO_BUILD_ERROR | OS_ERRNO_BUILD_WARN
 */
#define OS_ERRNO_BUILD_NORMAL(mid, errno) (ERRTYPE_NORMAL | ERRNO_OS_ID | ((U32)(mid) << 8) | (errno))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_ERRNO_H */
