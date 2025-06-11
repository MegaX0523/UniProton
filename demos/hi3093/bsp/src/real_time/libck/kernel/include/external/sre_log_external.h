/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: 日志处理内部头文件
 * Author: LibCK项目组
 * Create: 2021-01-09
 */
#ifndef SRE_LOG_EXTERNAL_H
#define SRE_LOG_EXTERNAL_H

#include "inttypes.h"
#include "sre_log.h"

/*
 * 模块间宏定义
 */
#define LOG_INFO    "[I]:"
#define LOG_ERROR   "[E]:"
#define LOG_FATAL   "[F]:"

/* 默认用户打印钩子函数 */
extern PrintFunc g_printHook;

/*
 * 描述：打印接口。格式化遵循printf格式化检查
 * 备注：格式化参数位置是从1开始编码的，分别表示格式化字符串的位置和“...”的位置
 */
extern void OsLogPut(const char *format, ...) __attribute__((format(printf, 1, 2)));

#if defined(OS_OPTION_LOG_REPORT)
/*
 * 描述：打印接口。格式化遵循printf格式化检查
 * 备注：格式化参数位置是从1开始编码的，分别表示格式化字符串的位置和“...”的位置
 */
extern void OsLogReport(enum OsLogLevel printLevel, const char *format, ...) __attribute__((format(printf, 2, 3)));

#define OS_LOG_REPORT(logPrintLevel, format, ...) \
    OsLogReport((logPrintLevel), (format), ##__VA_ARGS__)
#else
#define OS_LOG_REPORT(logPrintLevel, format, ...)
#endif

#if defined(OS_DBG)
#define LOG_ADDR_DBG(addr) ((uintptr_t)(addr))
#define OS_LOG_REPORT_DBG(format, ...) OS_LOG_REPORT(OS_LOG_HIGH, LOG_INFO" "format, ##__VA_ARGS__)
#else
#define LOG_ADDR_DBG(addr) ((void)(addr), 0x0U)
#define OS_LOG_REPORT_DBG(format, ...)
#endif

/*
 * LOG分为两套接口，正常接口与DBG接口，DBG接口仅会在OS_DBG开启时编译。
 * 正常接口类型形如OS_LOG_X_Y，X取值为F/H/M/L，与打印等级匹配，Y取值为F/E/I，与log信息等级匹配。
 * 正常与DBG接口类型均会打印函数名信息，因此不再打印模块信息。
 * 默认情况下，打印等级为H(IGH)，可通过SRE_SetLogLevel接口修改打印等级。
 * 接口仅供OS内部使用，不对外。需要注意，无论使用正常接口或者DBG接口记载一条日志，
 * 整体长度(包含前置'[OS][I]:'以及函数名等固定字段)不能超过255bytes，超过将不会输出。
 */
#define OS_LOG_F_F(fmt, ...) OS_LOG_REPORT(OS_LOG_FORCE, LOG_FATAL"%s "fmt, __FUNCTION__, ##__VA_ARGS__)
#define OS_LOG_F_E(fmt, ...) OS_LOG_REPORT(OS_LOG_FORCE, LOG_ERROR"%s "fmt, __FUNCTION__, ##__VA_ARGS__)
#define OS_LOG_F_I(fmt, ...) OS_LOG_REPORT(OS_LOG_FORCE, LOG_INFO"%s "fmt, __FUNCTION__, ##__VA_ARGS__)
#define OS_LOG_H_E(fmt, ...) OS_LOG_REPORT(OS_LOG_HIGH, LOG_ERROR"%s "fmt, __FUNCTION__, ##__VA_ARGS__)
#define OS_LOG_H_I(fmt, ...) OS_LOG_REPORT(OS_LOG_HIGH, LOG_INFO"%s "fmt, __FUNCTION__, ##__VA_ARGS__)
#define OS_LOG_L_I(fmt, ...) OS_LOG_REPORT(OS_LOG_LOW, LOG_INFO"%s "fmt, __FUNCTION__, ##__VA_ARGS__)

// 各模块LOG输出DBG接口
#define OS_LOG_DBG(fmt, ...) OS_LOG_REPORT_DBG("%s "fmt, __FUNCTION__, ##__VA_ARGS__)

#endif /* SRE_LOG_EXTERNAL_H */
