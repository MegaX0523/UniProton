/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: 初始化模块的头文件。
 * Author: LibCK项目组
 * Create: 2022-01-17
 */
#ifndef SRE_INIT_H
#define SRE_INIT_H

#include "./common/os_parainit.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_init
 * @brief OS注册模块参数设置
 *
 * @par 描述
 * OS注册模块参数设置
 *
 * <ul>
 * <li>该处为地址引用，非拷贝，保证set后不会被改变</li>
 * <li>在SRE_HardBootInit后调用。</li>
 * </ul>
 *
 * @param initRegPara   [IN]  类型#struct OsInitRegPara *，注册模块参数。
 *
 * @par 依赖
 * <ul><li>sre_init.h：该接口声明所在的头文件。</li></ul>
 */
extern void SRE_InitSetPara(const struct OsInitRegPara *initRegPara);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_INIT_H */
