/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Shell模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * @defgroup sre_shell Shell功能
 * @ingroup SRE_inspect
 */

#ifndef SRE_SHELL_H
#define SRE_SHELL_H

#include "sre_base.h"
#include "sre_symbol.h"
#include "./common/os_shell.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 用户打印前缀最大长度 */
#define PRINT_PREFIX_MAX_LEN 64

/*
 * @ingroup sre_shell
 *
 * 用户打印前缀的钩子函数类型定义。
 */
typedef U32(*PrintPrefixFunc)(char *buff);

/*
 * @ingroup  sre_shell
 * @brief 接收通道输入字符处理函数
 *
 * @par 描述
 * 驱动调用该接口，向shell输入驱动接收到的字符信息。
 *
 * @attention
 * <ul>
 * <li>目前只有CORTEX_AX平台支持串行接口接收字符处理函数</li>
 * </ul>
 *
 * @param para [IN]  类型#U8，串行接口接收的字符值。
 *
 * @retval  #OS_ERRNO_IO_INTF_SENDCHR2CHL_HOOK_NULL    0x02002d32，注册的通道输出字符钩子函数为空。
 * @retval  #SRE_OK                                    0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_shell.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_SendChrToShell(U8 para);

/*
 * @ingroup  sre_shell
 * @brief 向IO端口输出字符的钩子注册函数
 *
 * @par 描述
 * 注册IO端口输出字符函数。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param sendChr2ChlHook [IN]  类型#OsSendChr2ChlHookFunc，注册的钩子函数。
 *
 * @retval  #OS_ERRNO_SHELL_SENDCHR2SHELL_PARA_NULL 0x02002d07，用户注册的钩子函数参数为空。
 * @retval  #SRE_OK                                 0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_shell.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_SendChrToChlHookReg(OsSendChr2ChlHookFunc sendChr2ChlHook);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SRE_SHELL_H */
