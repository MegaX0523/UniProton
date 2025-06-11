/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 矢量寄存器模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * *@defgroup SRE_coprocessor 矢量寄存器
 * @ingroup SRE_kernel
 */

#ifndef SRE_COPROCESSOR_H
#define SRE_COPROCESSOR_H

#include <stddef.h>
#include "sre_base.h"
#include "sre_sys.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_coprocessor
 * coprocessor_a和coprocessor_b上下文保存区域的大小。
 *
 */
#if (OS_HARDWARE_PLATFORM == OS_DSP220)
#define OS_CP_CONTEXT_A_SIZE 0xD8
#define OS_CP_CONTEXT_B_SIZE 0xD8
#elif ((OS_HARDWARE_PLATFORM == OS_RISC220) || (OS_HARDWARE_PLATFORM == OS_HACCC))
#define OS_CP_CONTEXT_A_SIZE 0x44
#define OS_CP_CONTEXT_B_SIZE 0x44
#endif

/*
 * @ingroup SRE_coprocessor
 * @brief 为指定的内存空间设置指定值。
 *
 * @par 描述
 * 通过矢量操作，将指定首地址和长度的内存空间设置为固定值。
 * @attention
 * <ul>
 * <li>该操作适用于SD6182x平台。</li>
 * <li>参数value只有低8bit有效，高位会被忽略。</li>
 * <li>函数使用不当，会有缓冲区溢出风险，用户需要自行保证。</li>
 * </ul>
 * @param dst   [IN]  类型#void *，内存空间首地址。
 * @param value [IN]  类型#U32，需设置的指定值，低8BIT有效。
 * @param size  [IN]  类型#size_t，内存空间的长度。
 *
 * @retval #void *    目的内存空间首地址。
 * @par 依赖
 * <ul><li>sre_coprocessor.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SwiCpEntry|SRE_TaskCpEntry
 */
extern void *SRE_VecMemSet(void *dst, U32 value, size_t size);

/*
 * @ingroup SRE_coprocessor
 * @brief 将一段指定长度的内存从源地址拷贝至目的地址。
 *
 * @par 描述
 * 通过矢量操作，将src起始大小为size的内存的内容拷贝至dst起始的内存空间。
 * @attention
 * <ul>
 * <li>该操作适用于SD6182x平台。</li>
 * <li>调用前，需要保证参数之间不满足以下关系src < dst < (src + size)</li>
 * <li>函数使用不当，会有缓冲区溢出风险，用户需要自行保证。</li>
 * </ul>
 * @param dst  [IN]  类型#void *，待拷贝的目的内存首地址。
 * @param src  [IN]  类型#const void *，需拷贝的源内存首地址。
 * @param size [IN]  类型#size_t，进行拷贝的长度。
 *
 * @retval #void *    目的内存空间首地址。
 * @par 依赖
 * <ul><li>sre_coprocessor.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SwiCpEntry|SRE_TaskCpEntry
 */
extern void *SRE_VecMemCpy(void *dst, const void *src, size_t size);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_COPROCESSOR_H */
