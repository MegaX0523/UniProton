/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 符号表模块的头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_SYMBOL_H
#define SRE_SYMBOL_H

#include "sre_typedef.h"
#include "./common/os_symbol.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_symbol
 * Hash表的每个链表头结构体定义，分别指向链表的首尾节点。
 *
 */
struct SllList {
    /* 链表头指针 */
    struct SllNode *head;
    /* 链表尾指针 */
    struct SllNode *tail;
};

/*
 * @ingroup SRE_symbol
 * Hash表表头，统计整张表的信息。
 *
 */
struct SymbolTbl {
    /* Hash表中Hash链表的个数 */
    U32 elements;
    /* Hash表的种子 */
    U32 keyArg;
    /* 符号表初始化标记 */
    U32 initPhase;
    /* 符号表管理所在的内存分区号 */
    U32 symPartId;
    /* Hash表中符号的个数 */
    U32 symbolNum;
    /* 指向Hash链表的数组指针 */
    struct SllList *hashTbl;
};

/*
 * @ingroup SRE_symbol
 * 符号表地址信息。
 *
 */
struct SymInfo {
    uintptr_t symTableAddr; // 符号表哈希表的地址
    uintptr_t othInfoAddr;  // 符号表其他信息的地址（比如：bitmap表地址）
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_SYMBOL_H */
