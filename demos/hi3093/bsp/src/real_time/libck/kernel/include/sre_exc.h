/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 异常模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * *@defgroup SRE_exc 异常接管
 * @ingroup SRE_inspect
 */

#ifndef SRE_EXC_H
#define SRE_EXC_H

#include "sre_base.h"
#include "sre_hook.h"
#include "./common/os_exc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_exc
 * @brief 异常缓冲区首地址和长度获取函数。
 *
 * @par 描述
 * 获取异常信息缓冲区的首地址与大小。
 *
 * @attention
 * <ul><li>异常发生时，可根据缓冲区的首地址和大小，进而获取到异常信息；无异常时，此间内存值为0。</li></ul>
 *
 * @param bufAddr [OUT] 类型#void **，返回缓冲区首地址。
 * @param bufLen  [OUT] 类型#U32 *，缓冲区长度。
 *
 * @retval #OS_ERRNO_EXC_PARA_INVLAID 0x02001c02，入参指针为空。
 * @retval #SRE_OK                    0x00000000，成功返回。
 *
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件</li></ul>
 * @see 无
 */
extern U32 SRE_ExcGetBufInfo(void **bufAddr, U32 *bufLen);

/*
 * @ingroup SRE_exc
 * @brief 用户注册异常处理钩子。
 *
 * @par 描述
 * 注册异常处理钩子。
 *
 * @attention
 * <ul>
 * <li>当多次注册该钩子时，最后一次注册的钩子生效。</li>
 * <li>注册的钩子函数不能为空，即一旦注册钩子函数，不能通过注册空函数将其取消。</li>
 * </ul>
 *
 * @param hook [IN]  类型#ExcProcFunc，钩子函数。
 *
 * @retval #OS_ERRNO_EXC_REG_HOOK_PTR_NULL 0x02001c09，钩子函数为空
 * @retval #SRE_OK                         0x00000000，成功返回。
 *
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件</li></ul>
 * @see 无
 */
extern U32 SRE_ExcRegHook(ExcProcFunc hook);

/*
 * @ingroup SRE_exc
 * @brief 用户注册Hac超时自清异常处理用户钩子。
 *
 * @par 描述
 * 注册Hac超时自清异常处理用户钩子。
 *
 * @attention
 * <ul>
 * <li>只支持SD6182/SD6157平台。</li>
 * <li>当多次注册该钩子时，最后一次注册的钩子生效。</li>
 * </ul>
 *
 * @param hook [IN]  类型#HacTimeoutProcFunc，钩子函数。
 *
 * @retval #OS_ERRNO_EXC_REG_HOOK_PTR_NULL 0x02001c09，钩子函数为空。
 * @retval #SRE_OK                         0x00000000，成功返回。
 *
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_HacTimeOutCheckHook(HacTimeoutProcFunc hook);

/*
 * @ingroup  SRE_exc
 * @brief 获取LibCK异常入口地址。
 *
 * @par 描述
 * Tensilica芯片以及HIDSP芯片提供有一个硬件TraceBuffer功能，可以实时循环记录指令执行流，
 * 当进入异常处理时，为避免TraceBuffer中的信息被异常处理本身的指令流覆盖，应该停止记录指令流。
 * 用户可以用这个API获取异常入口地址,设置到硬件Trace提供的一个结束寄存器，
 * 当PC与结束寄存器的值相同时，TraceBuffer自动停止指令流的记录。
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 函数地址                  函数地址，异常处理的函数地址。
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern void *SRE_ExcEntryGet(void);

/*
 * @ingroup SRE_exc
 * @brief 临终遗言钩子注册函数。
 *
 * @par 描述
 * 注册临终遗言钩子到临终遗言链表中。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param hook [IN]  类型#SreLastwordsFunc，钩子函数。
 *
 * @retval #OS_ERRNO_HOOK_TYPE_INVALID  0x02001600，入参钩子类型非法
 * @retval #OS_ERRNO_HOOK_PTR_NULL      0x02001602，入参钩子地址为NULL
 * @retval #OS_ERRNO_HOOK_FULL          0x02001604，HOOK已满或配置个数为0
 * @retval #OS_ERRNO_HOOK_EXISTED       0x02001603，HOOK已存在
 * @retval #SRE_OK                      0x00000000，成功返回。
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件</li></ul>
 * @see SRE_LastWordsDisHook
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_LastWordsRegHook(SreLastwordsFunc hook)
{
    return SRE_HookAdd(OS_HOOK_LAST_WORDS, (OsVoidFunc)hook);
}

/*
 * @ingroup SRE_exc
 * @brief 临终遗言钩子取消函数。
 *
 * @par 描述
 * 将已注册的临终遗言钩子从链表中删除。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param hook [IN]  类型#SreLastwordsFunc，钩子函数。
 *
 * @retval #OS_ERRNO_HOOK_TYPE_INVALID  0x02001600，入参钩子类型非法
 * @retval #OS_ERRNO_HOOK_PTR_NULL      0x02001602，入参钩子地址为NULL
 * @retval #OS_ERRNO_HOOK_NOT_EXISTED   0x02001605，HOOK不存在
 * @retval #SRE_OK                      0x00000000，成功返回。
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件</li></ul>
 * @see SRE_LastWordsRegHook
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_LastWordsDisHook(SreLastwordsFunc hook)
{
    return SRE_HookDel(OS_HOOK_LAST_WORDS, (OsVoidFunc)hook);
}

/*
 * @ingroup  SRE_exc
 * @brief 获取异常向量表地址。
 *
 * @par 描述
 * 获取异常向量表地址。
 *
 * @attention
 * <ul>
 * <li>该操作仅在Cortex-RX平台下支持。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval #任意值       地址，异常向量表的地址
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件。</li></ul>
 * @see NA
 */
extern void *SRE_ExcGetVectorTable(void);

/*
 * @ingroup SRE_exc
 * @brief 单核复位资源回收。
 *
 * @par 描述
 * 单核复位资源回收。
 * @attention
 * <ul>
 * <li>该接口仅支持SD6183芯片。</li>
 * <li>回收本核创建的硬件信号量资源、核间消息所占的共享内存资源、hunt、核间信号量占用的1号分区资源。
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 *
 * @par 依赖
 * <ul><li>sre_exc.h：该接口声明所在的头文件</li></ul>
 * @see 无
 */
extern void SRE_ExcSingleCoreReclaim(void);

/*
 * @ingroup SRE_exc
 * @brief 异常信息打印接口。
 *
 * @par 描述
 * 打印异常信息。
 * @attention
 * <ul>
 * <li>该接口仅支持Cortex-RX芯片。</li>
 * </ul>
 *
 * @param excInfo [IN]  类型#struct ExcInfo *，异常信息结构体指针。
 *
 * @retval #OS_ERRNO_EXC_INFO_PARA_NULL  0x02001c16，入参指针为空。
 * @retval #SRE_OK                       0x00000000，成功返回。
 *
 * @par 依赖
 * sre_exc.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_ExcInfoPrintf(struct ExcInfo *excInfo);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_EXC_H */
