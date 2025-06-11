/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: log处理基础头文件，用户不能直接引用。
 * Author: LibCK项目组
 * Create: 2021-01-19
 */
#ifndef OS_LOG_H
#define OS_LOG_H

#include "os_errno.h"
#include "os_module.h"
#include "stdarg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_log
 * LOG错误码：设置日志级别不正确。
 *
 * 值: 0x02005300
 *
 * 解决方案：按照OS提供的日志等级枚举修改打印的级别。
 */
#define OS_ERRNO_LOG_PARA_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_LOG, 0x00)

/*
 * @ingroup os_log
 * LOG错误码: 钩子为空时有打印任务。
 *
 * 值: 0x02005301
 *
 * 解决方案: 检查是否发生未注册打印。
 *
 */
#define OS_ERRNO_LOG_PRINT_HOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_LOG, 0x01)

/*
 * @ingroup os_log
 * LOG错误码: 用户注册钩子打印失败
 *
 * 值: 0x02005302
 *
 * 解决方案: 检查打印钩子和打印内容。
 *
 */
#define OS_ERRNO_LOG_PRINT_FAIL OS_ERRNO_BUILD_FATAL(OS_MID_LOG, 0x02)

/*
 * @ingroup os_log
 * LOG错误码: 注册打印接口函数入参为空。
 *
 * 值: 0x02005303
 *
 * 解决方案: 检查注册打印接口函数的入参否为空指针。
 *
 */
#define OS_ERRNO_LOG_PRINT_HOOK_PARA_NULL OS_ERRNO_BUILD_ERROR(OS_MID_LOG, 0x03)

/*
 * @ingroup os_log
 *
 * 用户打印钩子类型定义。
 */
typedef U32 (*PrintFunc)(const char *format, va_list vaList);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_LOG_H */
