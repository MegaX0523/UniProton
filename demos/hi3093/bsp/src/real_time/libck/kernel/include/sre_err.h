/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 错误处理模块的头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_ERR_H
#define SRE_ERR_H

#include "sre_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_err
 *
 * @brief 错误处理钩子函数。
 *
 * @par 描述
 * 错误处理的钩子函数，用户自定义的错误处理函数必须要符合该钩子函数的参数要求。
 *
 * @attention 无。
 *
 * @param fileName [IN]  类型#const char *，出错文件名。
 * @param lineNo   [IN]  类型#U32，出错的行号。
 * @param errorNo  [IN]  类型#U32，用户定义的错误码。
 * @param paraLen  [IN]  类型#U32，入参para的长度。
 * @param para     [IN]  类型#void *，记录用户对于错误的描述或其他。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_err.h：该接口声明所在的头文件。</li>
 * @see 无
 */
typedef void (*SreErrorHandleFunc)(const char *fileName, U32 lineNo, U32 errorNo, U32 paraLen, void *para);

/*
 * @ingroup SRE_err
 *
 * @brief 处理错误。
 *
 * @par 描述
 * 用户或者OS内部使用该函数回调#SRE_ErrRegHook中注册的钩子函数。另外，OS内部使用此接口时，还会记录相关错误码。
 * @attention
 * <ul>
 * <li>系统不会做入参检测，用户SRE_ErrHandle会全部当做钩子入参输入。</li>
 * <li>用户调用SRE_ErrHandle接口时，只会回调用户注册钩子函数，</li>
 * <li>不会进行错误码记录（致命错误也不会记录Trace轨迹和触发异常）。</li>
 * </ul>
 *
 * @param fileName [IN]  类型#const char *，错误发生的文件名，可以用__FILE__作为输入。
 * @param lineNo   [IN]  类型#U32，错误发生所在的行号，可以用__LINE__作为输入。
 * @param errorNo  [IN]  类型#U32，用户输入的错误码。
 * @param paraLen  [IN]  类型#U32，入参para的长度。
 * @param para     [OUT] 类型#void *，记录用户对于错误的描述或其他(地址)。
 *
 * @retval #SRE_OK      0x00000000，处理错误成功。
 * @par 依赖
 * <li>sre_err.h: 该接口声明所在的头文件。</li>
 * @see 无
 */
extern U32 SRE_ErrHandle(const char *fileName, U32 lineNo, U32 errorNo, U32 paraLen, void *para);

/*
 * @ingroup SRE_err
 *
 * @brief 获取当前线程记录的最后一个错误码。
 *
 * @par 描述
 * 获取当前线程记录的最后一个错误码。如果该线程未记录错误码，则返回0；如果记录了错误码，则返回记录的最后一个错误码。
 * @attention
 * <ul>
 * <li>用户调用SRE_ErrHandle上报的错误码不会被记录，因此最后一个错误码不包含此类错误码。</li>
 * <li>内存模块初始化完成前调用该接口，则返回0。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval #错误码 当前线程上报的最后一个错误码
 * @par 依赖
 * <li>sre_err.h: 该接口声明所在的头文件。</li>
 * @see 无
 */
extern U32 SRE_GetLastError(void);

/*
 * @ingroup SRE_err
 *
 * @brief 注册用户错误处理的钩子函数。
 *
 * @par 描述
 * 注册hook作为用户钩子函数，在调用SRE_ErrHandle接口进行错误处理时对该钩子进行调用。
 * @attention
 * <ul>
 * <li>若入参hook为NULL,则为取消钩子。</li>
 * <li>不允许重复或覆盖注册。</li>
 * <li>用户定义的钩子函数必须符合#SRE_ERRORHANDLE_FUNC定义的形式，而且只能定义一个钩子函数。</li>
 * <li>用户调用SRE_ErrRegHook注册回调钩子函数时，钩子函数里面不能有调用SRE_ErrHandle</li>
 * <li>或者调用OS函数发生上报错误码的情况，否则可能会进入死循环，需由用户保证。</li>
 * <li>用户调用SRE_ErrRegHook注册回调钩子函数时，钩子函数里面如有单次上报的错误信息（只有第一次调用会执行），</li>
 * <li>那么会先记录钩子中错误信息，再记录发生错误时的错误信息。</li>
 * </ul>
 *
 * @param hook [IN]  类型#SreErrorHandleFunc，用户钩子函数的宏定义。
 *
 * @retval #OS_ERRNO_HOOK_FULL              0x02001604，HOOK已满或配置个数为0。
 * @retval #SRE_OK                          0x00000000，注册成功。
 * @par 依赖
 * <li>sre_err.h：该接口声明所在的头文件。</li>
 * @see 无
 */
extern U32 SRE_ErrRegHook(SreErrorHandleFunc hook);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_ERR_H */
