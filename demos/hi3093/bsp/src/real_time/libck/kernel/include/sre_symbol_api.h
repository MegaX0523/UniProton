/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 符号表模块的对外头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * Notes: 此功能仅支持Cortex Rx平台(Balong)
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * @defgroup SRE_symbol 符号表
 * @ingroup SRE_inspect
 */

#ifndef SRE_SYMBOL_API_H
#define SRE_SYMBOL_API_H

#include "sre_symbol.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup  SRE_symbol
 * @brief 获取符号地址。
 *
 * @par 描述
 * 根据输入的符号名name，获取对应的符号地址。
 *
 * @attention 无
 *
 * @param name [IN]  类型#const char *，输入的符号名。
 * @param addr [OUT] 类型#uintptr_t *，保存符号地址。
 *
 * @retval #OS_ERRNO_SYMBOL_FIND_NAME_ERROR           0x02002904，在符号表中没有查找到符号名字匹配的符号节点。
 * @retval #OS_ERRNO_SYMBOL_GET_ADDR_PTR_NULL         0x0200290b，输入的指针参数为NULL。
 * @retval #SRE_OK                                    0x00000000，获取符号地址成功。
 * @par 依赖
 * <ul><li>sre_symbol_api.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_SymFindAddrByName(const char *name, uintptr_t *addr);

/*
 * @ingroup  SRE_symbol
 * @brief 获取符号名。
 *
 * @par 描述
 * 根据输入的符号地址，获取对应的符号名。
 *
 * @attention 无
 *
 * @param symAddr [IN]  类型#const void *，输入的符号地址。
 * @param name    [OUT] 类型#char **，保存的输出符号名。
 *
 * @retval #OS_ERRNO_SYMBOL_FIND_ADDR_ERROR           0x02002903，在符号表中没有查找到地址匹配的符号节点。
 * @retval #OS_ERRNO_SYMBOL_PARA_POINT_NULL           0x02002900，输入的指针参数为NULL。
 * @retval #SRE_OK                                    0x00000000，获取符号地址成功。
 * @par 依赖
 * <ul><li>sre_symbol_api.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_SymFindNameByAddr(const void *symAddr, char **name);

/*
 * @ingroup  SRE_symbol
 * @brief 获取符号表相关地址信息。
 *
 * @par 描述
 * 获取符号表相关地址信息。
 *
 * @attention 无
 *
 * @param symbolInfo [Out]  类型#struct SymInfo *，符号地址信息。
 *
 * @retval #OS_ERRNO_SYMBOL_PARA_POINT_NULL           0x02002900，输入的指针参数为NULL。
 * @retval #SRE_OK                                    0x00000000，获取符号地址成功。
 * @par 依赖
 * <ul><li>sre_symbol_api.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_SymInfoGet(struct SymInfo *symbolInfo);

#if defined(OS_OPTION_SYMBOL_SAFE_ADD)
/*
 * @ingroup  SRE_symbol
 * @brief 添加安全的符号表信息给OS
 *
 * @par 描述
 * 添加安全的符号表信息给OS
 *
 * @attention
 * <ul>
 * <li>添加的符号表信息正确性由调用者保证，OS不作检查。</li>
 * </ul>
 *
 * @param symbolNode [IN]  类型#struct SymbolNode *，符号表结构体数组的起始地址
 * @param symbolNum  [IN]  类型#U32，符号表结构体个数
 *
 * @retval #OS_ERRNO_SYMBOL_PARA_POINT_NULL     0x02002900，入参指针为空。
 * @retval #SRE_OK                              0x00000000，获取调用栈信息成功。
 * @par 依赖
 * @see NONE
 */
extern U32 SRE_SymbolSafeTblAdd(struct SymbolNode *symbolNode, U32 symbolNum);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_SYMBOL_API_H */
