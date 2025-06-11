/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 通用错误码定义头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_ERRNO_H
#define SRE_ERRNO_H

#include "./common/os_errno.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_err
 * OS错误码标记位。(0x00表示Guest OS,0xFF表示DRV,0x01表示Host OS)
 *
 */
#define SRE_ERRNO_OS_ID ERRNO_OS_ID

/*
 * @ingroup SRE_err
 * 定义错误的等级:提示级别
 *
 */
#define SRE_ERRTYPE_NORMAL ERRTYPE_NORMAL

/*
 * @ingroup SRE_err
 * 定义错误的等级:告警级别
 *
 */
#define SRE_ERRTYPE_WARN ERRTYPE_WARN

/*
 * @ingroup SRE_err
 * 定义错误的等级:严重级别
 *
 */
#define SRE_ERRTYPE_ERROR ERRTYPE_ERROR

/*
 * @ingroup SRE_err
 * 定义错误的等级:致命级别
 *
 */
#define SRE_ERRTYPE_FATAL ERRTYPE_FATAL

/*
 * @ingroup  SRE_err
 * @brief 定义OS致命错误。
 *
 * @par 描述
 * 宏定义，定义OS致命错误。
 *
 * @attention 无
 * @param  mid [IN] 模块ID编号。
 * @param  errNo [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖
 * sre_errno.h: 该宏定义所在的头文件。
 * @see SRE_ERRNO_OS_ERROR | SRE_ERRNO_OS_WARN | SRE_ERRNO_OS_NORMAL
 */
#define SRE_ERRNO_OS_FATAL(mid, errNo) OS_ERRNO_BUILD_FATAL((mid), (errNo))

/*
 * @ingroup  SRE_err
 * @brief 定义OS严重错误
 *
 * @par 描述
 * 宏定义，定义OS严重错误
 *
 * @attention 无
 * @param  mid [IN] 模块ID编号。
 * @param  errNo [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖
 * sre_errno.h: 该宏定义所在的头文件。
 * @see SRE_ERRNO_OS_FATAL | SRE_ERRNO_OS_WARN | SRE_ERRNO_OS_NORMAL
 */
#define SRE_ERRNO_OS_ERROR(mid, errNo) OS_ERRNO_BUILD_ERROR((mid), (errNo))

/*
 * @ingroup  SRE_err
 * @brief 定义OS告警信息
 *
 * @par 描述
 * 宏定义，定义OS告警
 *
 * @attention 无
 * @param  mid [IN] 模块ID编号。
 * @param  errNo [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖
 * sre_errno.h: 该宏定义所在的头文件。
 * @see SRE_ERRNO_OS_FATAL | SRE_ERRNO_OS_ERROR | SRE_ERRNO_OS_NORMAL
 */
#define SRE_ERRNO_OS_WARN(mid, errNo) OS_ERRNO_BUILD_WARN((mid), (errNo))

/*
 * @ingroup  SRE_err
 * @brief 定义OS提示信息
 *
 * @par 描述
 * 宏定义，定义OS提示信息
 *
 * @attention 无
 * @param  mid [IN] 模块ID编号。
 * @param  errNo [IN] 错误码编号。
 *
 * @retval 无
 * @par 依赖
 * sre_errno.h: 该宏定义所在的头文件。
 * @see SRE_ERRNO_OS_FATAL | SRE_ERRNO_OS_ERROR | SRE_ERRNO_OS_WARN
 */
#define SRE_ERRNO_OS_NORMAL(mid, errNo) OS_ERRNO_BUILD_NORMAL((mid), (errNo))

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_ERRNO_H */
