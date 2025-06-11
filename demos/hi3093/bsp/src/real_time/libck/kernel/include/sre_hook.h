/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 内核钩子模块的头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HOOK_H
#define SRE_HOOK_H

#include "sre_base.h"
#include "./common/os_hook.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_hook
 *
 * @brief 添加内核钩子。
 *
 * @par 描述
 * 添加指定类型的内核钩子，内核在相应的处理点进行钩子回调。
 * @attention
 * <ul><li>添加钩子数目不能大于对应类型静态配置(OS_HOOK_XXX_NUM)的最大值。如果没有静态配置项，默认支持1个。</li></ul>
 *
 * @param hookType [IN]  类型#enum OsHookType，添加钩子类型。
 * @param hook     [IN]  类型#OsVoidFunc，添加钩子指针。
 *
 * @retval #OS_ERRNO_HOOK_TYPE_INVALID   0x02001600，钩子类型无效。
 * @retval #OS_ERRNO_HOOK_PTR_NULL       0x02001602，钩子指针为空。
 * @retval #OS_ERRNO_HOOK_EXISTED        0x02001603，钩子已经存在。
 * @retval #OS_ERRNO_HOOK_FULL           0x02001604，钩子已满。
 * @retval #OS_ERRNO_HOOK_NOT_CFG        0x02001606，钩子个数配置为0。
 * @retval #SRE_OK                       0x00000000，添加钩子成功。
 * @par 依赖
 * <ul><li>sre_hook.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HookDel
 */
extern U32 SRE_HookAdd(enum OsHookType hookType, OsVoidFunc hook);

/*
 * @ingroup SRE_hook
 *
 * @brief 添加内核钩子，只支持临终遗留钩子注册。
 *
 * @par 描述
 * 添加指定类型的内核钩子，内核在相应的处理点根据钩子执行顺序进行钩子回调。
 *
 * @attention
 * <ul>
 * <li>添加钩子数目不能大于对应类型静态配置(OS_HOOK_XXX_NUM)的最大值。如果没有静态配置项，默认支持1个。</li>
 * <li>只支持临终遗留钩子注册，支持平台:sd6185x。</li>
 * </ul>
 *
 * @param hookType [IN]  类型#enum OsHookType，添加钩子类型。
 * @param hookMode [IN]  类型#enum OsHookMode，添加钩子执行顺序。
 * @param hook     [IN]  类型#OsVoidFunc，添加钩子指针。
 *
 * @retval #OS_ERRNO_HOOK_TYPE_INVALID   0x02001600，钩子类型无效。
 * @retval #OS_ERRNO_HOOK_MODE_INVALID   0x02001600，钩子类型无效。
 * @retval #OS_ERRNO_HOOK_PTR_NULL       0x02001602，钩子指针为空。
 * @retval #OS_ERRNO_HOOK_EXISTED        0x02001603，钩子已经存在。
 * @retval #OS_ERRNO_HOOK_FULL           0x02001604，钩子已满。
 * @retval #OS_ERRNO_HOOK_NOT_CFG        0x02001606，钩子个数配置为0。
 * @retval #SRE_OK                       0x00000000，添加钩子成功。
 * @par 依赖
 * <ul><li>sre_hook.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HookDel
 */
extern U32 SRE_HookAddEx(enum OsHookType hookType, enum OsHookMode hookMode, OsVoidFunc hook);

/*
 * @ingroup SRE_hook
 *
 * @brief 删除内核钩子。
 *
 * @par 描述
 * 删除指定类型的内核钩子。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param hookType [IN]  类型#enum OsHookType，待删除钩子类型。
 * @param hook     [IN]  类型#OsVoidFunc，待删除钩子指针。
 *
 * @retval #OS_ERRNO_HOOK_TYPE_INVALID   0x02001600，钩子类型无效。
 * @retval #OS_ERRNO_HOOK_PTR_NULL       0x02001602，钩子指针为空。
 * @retval #OS_ERRNO_HOOK_NOT_EXISTED    0x02001605，钩子不存在。
 * @retval #OS_ERRNO_HOOK_NOT_CFG        0x02001606，钩子个数配置为0。
 * @retval #SRE_OK                       0x00000000，删除钩子成功。
 * @par 依赖
 * <ul><li>sre_hook.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HookAdd
 */
extern U32 SRE_HookDel(enum OsHookType hookType, OsVoidFunc hook);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_HOOK_H */
