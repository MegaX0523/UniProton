/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 符号表模块的基础头文件，用户不能直接包含
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_SYMBOL_H
#define OS_SYMBOL_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:入参指针为空。
 *
 * 值: 0x02002900
 *
 * 解决方案: 排查函数入参是否为空
 *
 */
#define OS_ERRNO_SYMBOL_PARA_POINT_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x00)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:符号表管理内存申请失败。
 *
 * 值: 0x02002901
 *
 * 解决方案: 检查符号表创建的分区内存大小是否够用或者OS内存分区不足
 *
 */
#define OS_ERRNO_SYMBOL_MEM_ALLOC_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x01)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:符号重定义。
 *
 * 值: 0x02002902
 *
 * 解决方案:检查是否有符号项重名
 *
 */
#define OS_ERRNO_SYMBOL_REDEFINE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x02)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:在符号表中没有查找到地址匹配的符号节点。
 *
 * 值: 0x02002903
 *
 * 解决方案: 请确认查找的地址是否正确
 *
 */
#define OS_ERRNO_SYMBOL_FIND_ADDR_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x03)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:在符号表中没有查找到符号名字匹配的符号节点。
 *
 * 值: 0x02002904
 *
 * 解决方案: 请确认查找的符号名是否正确
 *
 */
#define OS_ERRNO_SYMBOL_FIND_NAME_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x04)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:符号表节点删除失败，静态符号表不允许删除。
 *
 * 值: 0x02002905
 *
 * 解决方案: 查看删除的符号项对应的UnitInterNo是否是正确，UnitInterNo为0表示静态符号项，不允许删除。
 *
 */
#define OS_ERRNO_SYMBOL_REMOVE_ID_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x05)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:动态添加符号项的序列ID与静态的重复。
 *
 * 值: 0x02002906
 *
 * 解决方案: 查看动态添加符号项序列UnitInterNo是否正确，不能为0(UnitInterNo为0表示静态符号项)。
 *
 */
#define OS_ERRNO_SYMBOL_PARA_ID_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x06)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:符号表中使用的镜像ID不可用。
 *
 * 值: 0x02002907
 *
 * 解决方案: 如果在初始化时遇到该错误码，检查是否是同一镜像重复初始化或者镜像ID配置错误。
 * 如果在使用时遇到该错误码，查看对应镜像是否存在或者该镜像的符号表是否打开。
 *
 */
#define OS_ERRNO_SYMBOL_IMAGE_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x07)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:符号表配置的分区起始地址或大小不合法。
 *
 * 值: 0x02002908
 *
 * 解决方案: 用户配置的分区号不为0时，符号表模块配置的分区起始地址或者大小不能为0。
 *
 */
#define OS_ERRNO_SYMBOL_ADDR_SIZE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x08)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:符号表配置的分区号不合法。
 *
 * 值: 0x02002909
 *
 * 解决方案: 符号表模块分区号不能配置为0。
 *
 */
#define OS_ERRNO_SYMBOL_PT_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x09)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:同一镜像符号表配置的分区参数或不一致。
 *
 * 值: 0x0200290a
 *
 * 解决方案: 检查同一镜像sre_config.h中符号表模块分区配置是否相同。
 *
 */
#define OS_ERRNO_SYMBOL_PT_PARA_NOT_SAME OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x0a)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:根据符号名获取符号地址时，输入的指针参数为NULL。
 *
 * 值: 0x0200290b
 *
 * 解决方案: 根据符号名获取符号地址时，须保证输入的指针参数不能为NULL。
 *
 */
#define OS_ERRNO_SYMBOL_GET_ADDR_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x0b)

/*
 * @ingroup OS_symbol
 * 符号表管理错误码:要查找的符号的核集同符号中已定义的同名符号的bitmap冲突
 *
 * 值: 0x0200290d
 *
 * 解决方案: 排查引用符号的核集是否超出了定义符号的核集
 *
 */
#define OS_ERRNO_SYMBOL_BITMAP_CONFLICT OS_ERRNO_BUILD_ERROR(OS_MID_SYMBOL, 0x0d)

/*
 * @ingroup OS_symbol
 * 未定义的符号类型。
 *
 */
#define OS_SYM_UNDF 0x0

/*
 * @ingroup OS_symbol
 * 全局符号类型。
 *
 */
#define OS_SYM_GLOBAL 0x1U

/*
 * @ingroup OS_symbol
 * 代码段符号类型。
 *
 */
#define OS_SYM_TEXT 0x2U

/*
 * @ingroup OS_symbol
 * 数据段符号类型。
 *
 */
#define OS_SYM_DATA 0x4U

/*
 * @ingroup OS_symbol
 * absolute符号类型。
 *
 */
#define OS_SYM_ABS 0x8U

/*
 * @ingroup OS_symbol
 * 通用符号类型。
 *
 */
#define OS_SYM_COMM 0x10U

/*
 * @ingroup OS_symbol
 * 缺省的序列ID号。
 *
 */
#define OS_SYMBOL_REVID 0x0

/*
 * @ingroup OS_symbol
 * 单链表的链表节点结构体定义。
 *
 */
struct SllNode {
    /* 单链表指向下一个节点的指针 */
    struct SllNode *next;
};

/*
 * @ingroup OS_symbol
 * Hash表的节点结构体。
 *
 */
struct SymbolNode { /* HASH_SYMBOL_NODE - entry in symbol table */
    /* hash node (must come first) */
    struct SllNode nameHNode;

    /* 指向符号名的指针 */
    const char *name;

    /* 符号地址 */
    void *value;

    /* 符号类型: OS_SYM_UNDF&OS_SYM_GLOBAL&OS_SYM_ABS&OS_SYM_TEXT&OS_SYM_DATA等 */
    U8 type;

    /* 符号动态添加的组ID，0~0xfd为LP使用(值为LP的GroupID+1) */
    /* 0xff为shell使用,支持SRE_MNG功能下该字段为无效 */
    /* 0xfe为不可删除符号 */
    U8 symGroupID;

    /* 符号的所属单元ID，1~9999为LP使用(值为LP的内部补丁单元号)，0xfffe为shell使用 */
    /* 主核在初始化阶段给从核打补丁时，unitInterNo 复用作段号，方便找寻该函数的物理地址 */
    U16 unitInterNo;

    /* 符号的长度 */
    U32 size;
};

/*
 * @ingroup os_symbol
 * 设置符号表模块信息。
 *
 */
struct SymbolModInfo {
    /* 符号表初始化标记 */
    U32 initPhase;
    /* 符号表分区号 */
    U32 symRegNum;
    /* 符号表分区起始地址 */
    void *symRegAddr;
    /* 符号表分区大小 */
    uintptr_t symRegSize;
#if defined(OS_OPTION_SYMBOL_COREBITMAP)
    /* 符号表中一个进程组注册的LP单元数 */
    U32 symUnitNum;
#endif
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_SYMBOL_H */
