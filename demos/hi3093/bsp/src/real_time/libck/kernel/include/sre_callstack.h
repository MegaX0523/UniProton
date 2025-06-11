/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 硬件相关模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_CALLSTACK_H
#define SRE_CALLSTACK_H

#include "sre_task.h"
#include "./common/os_callstack.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup  SRE_callstack
 * @brief 获取在线调用栈信息
 *
 * @par 描述
 * 获取当前函数或者异常时函数的调用栈信息。
 *
 * @attention
 * <ul>
 * <li>只支持Cortex-RX平台。</li>
 * <li>支持普通调用(系统正常运行)和异常钩子调用(系统发生异常)，前者获取的为当前函数的调用栈信息，</li>
 * <li>后者获取的为异常时函数的调用栈信息。</li>
 * <li>缓冲区存储各级函数PC的顺序为从最底层函数到最上层函数。</li>
 * </ul>
 *
 * @param maxNum [I/O] 类型#U32 *，最大调用层数，输入/输出参数。输入：缓冲区list可容纳的元素个数；
 * 输出：实际解析出的函数调用层数。
 * @param list   [OUT] 类型#uintptr_t *，存储函数PC信息的缓冲区首地址，用于填写各级调用函数的PC。
 *
 * @retval #OS_ERRNO_CALLSTACK_PTR_NULL         0x02003501，获取调用栈信息时，输入的参数指针为NULL。
 * @retval #OS_ERRNO_CALLSTACK_MAX_NUM_VALID    0x02003502，获取调用栈信息时，输入的最大调用层数等于0
 * @retval                                                  或者大于CALLSTACK_DEPTH_MAX_NUM。
 * @retval #SRE_OK                              0x00000000，获取调用栈信息成功。
 * @par 依赖
 * <ul><li>sre_callstack.h：该接口声明所在的头文件。</li></ul>
 * @see NONE
 */
extern U32 SRE_GetCallStack(U32 *maxNum, uintptr_t *list);

/*
 * @ingroup  SRE_GetCallStackByTaskID
 * @brief 获取指定任务调用栈信息
 *
 * @par 描述
 * 获取指定任务切换出去或被中断/异常打断时的调用栈信息。
 *
 * @attention
 * <ul>
 * <li>只支持Cortex-RX平台。</li>
 * <li>支持获取运行任务的调用栈，包括本任务和其他核运行任务。</li>
 * <li>获取调用栈时任务阻塞，但获取过程中又运行起来的任务可能不准确，因为栈的内容发生了变化。</li>
 * <li>缓冲区存储各级函数PC的顺序为从最底层函数到最上层函数。</li>
 * </ul>
 *
 * @param maxDepth [I/O] 类型#U32 *，最大调用层数，输入/输出参数。输入：缓冲区list可容纳的元素个数；
 * 输出：实际解析出的函数调用层数。
 * @param list     [OUT] 类型#uintptr_t *，存储函数PC信息的缓冲区首地址，用于填写各级调用函数的PC。
 * @param taskPID  [IN]  类型#TskHandle，需要获取调用栈信息的任务的ID。
 *
 * @retval #OS_ERRNO_CALLSTACK_PTR_NULL           0x02003501，获取调用栈信息时，输入的参数指针为NULL。
 * @retval #OS_ERRNO_CALLSTACK_MAX_NUM_VALID      0x02003502，获取调用栈信息时，输入的最大调用层数等于0
 * @retval                                                    或者大于CALLSTACK_DEPTH_MAX_NUM。
 * @retval #OS_ERRNO_CALLSTACK_TSK_ID_INVALID     0x02003504，获取调用栈信息时，输入的任务ID非法。
 * @retval #OS_ERRNO_CALLSTACK_TSK_UNUSED         0x02003505，获取调用栈信息时，输入的任务ID为未创建的任务的ID。
 * @retval #OS_ERRNO_CALLSTACK_TSK_PAUSE_FAILED   0x02003506，获取调用栈信息时，尝试暂停目标任务失败。
 * @retval #SRE_OK                                0x00000000，获取调用栈信息成功。
 * @par 依赖
 * <ul><li>sre_callstack.h：该接口声明所在的头文件。</li></ul>
 * @see NONE
 */
extern U32 SRE_GetCallStackByTaskID(U32 *maxDepth, uintptr_t *list, TskHandle taskPID);

/*
 * @ingroup  SRE_UnwindTableAdd
 * @brief 添加EXIDX段信息给OS。
 *
 * @par 描述
 * 添加额外的EXIDX段信息给OS，用于调用栈解析。
 *
 * @attention
 * <ul>
 * <li>只支持Cortex-RX平台。</li>
 * <li>使用者保证添加的EXIDX段信息起始/结束地址合法性。</li>
 * </ul>
 *
 * @param start  [IN]  类型#U32，EXIDX段的起始地址(段*内容*实际所在的地址)
 * @param end    [IN]  类型#U32，EXIDX段的结束地址(起始地址+大小)
 * @param offset [IN]  类型#U32，EXIDX段内容实际所在地址与原本应该load到的地址之差
 *
 * @retval #OS_ERRNO_CALLSTACK_NO_MEM           0x02003507，添加EXIDX段信息时，系统内存不足
 * @retval #SRE_OK                              0x00000000，信息添加成功。
 * @par 依赖
 * <ul><li>sre_callstack.h：该接口声明所在的头文件。</li></ul>
 * @see NONE
 */
extern U32 SRE_ExidxSecInfoAdd(uintptr_t start, uintptr_t end, U32 offset);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_CALLSTACK_H */
