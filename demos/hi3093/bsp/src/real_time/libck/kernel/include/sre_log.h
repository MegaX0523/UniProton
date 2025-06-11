/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: log处理模块的头文件。
 * Author: LibCK项目组
 * Create: 2021-01-09
 */
#ifndef SRE_LOG_H
#define SRE_LOG_H

#include "sre_base.h"
#include "./common/os_log.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_log
 *
 * @brief 日志处理钩子函数。
 *
 * @par 描述
 * 日志处理的钩子函数，用户自定义的日志处理函数必须要符合该钩子函数的参数要求。
 *
 * @attention 无。
 *
 * @param logContent [IN]  类型#const char *，log文件内容。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_log.h：该接口声明所在的头文件。</li>
 * @see 无
 */
typedef void (*OsLogHandleFunc)(const char *logContent);

/*
 * @ingroup SRE_log
 *
 * @brief 注册用户日志处理的钩子函数。
 *
 * @par 描述
 * 注册hook作为用户钩子函数，用于输出OS内部日志信息。
 * @attention
 * <ul>
 * <li>若入参hook为NULL,则为取消钩子。</li>
 * <li>用户定义的钩子函数必须符合#OsLogHandleFunc定义的形式。</li>
 * <li>用户需自行保证传入钩子函数的正确性。</li>
 * </ul>
 *
 * @param hook [IN]  类型#OsLogHandleFunc，用户钩子函数的宏定义。
 *
 * @retval #SRE_OK                          0x00000000，注册成功。
 * @par 依赖
 * <li>sre_log.h：该接口声明所在的头文件。</li>
 * @see 无
 */
extern U32 SRE_LogRegHook(OsLogHandleFunc hook);

enum OsLogLevel {
    OS_LOG_LOW = 0, /* 非关键信息，打印级别低，默认不打印 */
    OS_LOG_MID = 1, /* 信息打印级别中等，先预留 */
    OS_LOG_HIGH = 2, /* 信息打印级别高，中断中默认不打印 */
    OS_LOG_FORCE = 3, /* 信息打印级别最高，任何情况下都会打印 */
};

/*
 * @ingroup SRE_log
 *
 * @brief 设置打印的级别，控制日志输出，只有不低于设置的级别的日志会输出到日志区。
 *
 * @par 描述
 * 设置打印的级别。
 * @attention
 * <ul>
 * <li>可以多次设置，每一次生效的时段为设置完成后至下一次再次设置前。</li>
 * <li>每次生效过程中，只有不低于设置的级别的日志会输出到日志区。</li>
 * </ul>
 *
 * @param hook [IN]  类型enum OsLogLevel，日志打印等级枚举。
 *
 * @retval #SRE_OK                          0x00000000，设置成功。
 * @retval #OS_ERRNO_LOG_PARA_INVALID       0x02005300，设置的日志等级不正确
 * @par 依赖
 * <li>sre_log.h：该接口声明所在的头文件。</li>
 * @see 无
 */
extern U32 SRE_SetLogLevel(enum OsLogLevel logLevel);

/*
 * @ingroup  SRE_log
 * @brief os使用print函数注册。
 *
 * @par 描述
 * os使用print函数注册函数。
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param printHook [IN]  类型#PrintFunc，注册的钩子函数。
 *
 * @retval  #OS_ERRNO_LOG_PRINT_HOOK_PARA_NULL  0x02005303，用户注册的钩子函数参数为空。
 * @retval  #SRE_OK                             0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_log.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_PrintHookReg(PrintFunc printHook);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_LOG_H */
