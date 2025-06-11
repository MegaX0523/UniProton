/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 符号表模块的模块间头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组     华为规范整改
 */
#ifndef SRE_SYMBOL_EXTERNAL_H
#define SRE_SYMBOL_EXTERNAL_H

#include "sre_hw_external.h"
#include "sre_base.h"
#include "sre_symbol.h"
#include "sre_symbol_api.h"
#include "sre_mem.h"
#if defined(OS_OPTION_HWSEM)
#include "sre_hwsem.h"
#endif
#if defined(OS_OPTION_LP_COLD_PAT)
#include "sre_mng_external.h"
/* 从核基础bin临时符号表 */
extern struct SymbolTbl g_secondarySymbolTbl;
#endif

/*
 * 83
 * SRE_MNG下进程组内自己查找符号，不会实例内跨进程组添加或查找符号，故不用添加实例间共享变量g_symbolTbl，
 * 只需要进程组内共享变量即可g_symbolTbl
 */
#if defined(OS_OPTION_SYMBOL)
#if defined(OS_OPTION_SYMBOL_COREBITMAP)
/* 一个实例中的符号表的进程组数，主核启动创建从核进程组后传入到从核配置参数中 */
#define OS_SYMBOL_CUR_GROUP_ID g_pgID
#define OS_SYMBOL_PT_ID 0 /* SRE_MNG下一个实例支持一个符号表分区 */
#define OS_SYMBOL_PT_NUM 1 /* SRE_MNG下一个实例支持一个符号表分区 */
#else /* 非SRE_MNG下(SD6183)一个实例支持可能有多个基础BIN组成，每个基础bin(不同镜像)各自维护符号表分区 */
#define OS_SYMBOL_CUR_GROUP_ID g_imageId
#define OS_SYMBOL_PT_ID OS_SYMBOL_CUR_GROUP_ID
#define OS_SYMBOL_PT_NUM g_imageNum
#endif
#else
#define OS_SYMBOL_CUR_GROUP_ID 0U
#endif

typedef U32(*SymbolFindnameF)(const void *symAddr, char **name);

/* 符号表内存分区管理结构体 */
struct SymbolMemPtInfo {
    /* 存分区起始地址 */
    uintptr_t memPtAddr;
    /* 存分区大小 */
    uintptr_t memPtSize;
    /* 内存分区编号 */
    U32 memPtNo;
    /* 内存分区创建标识, 对应bit为镜像ID  0:未创建 1:创建 */
    U32 memPtFlag;
};

extern U32 OsSysImageInfoReg(U32 imageId, U32 imageNum);
extern U32 OsSymRemoveByModID(U32 imageId, U8 symGroupID, U16 unitInterNo);
extern U32 OsSymRemoveByAddr(U32 imageId, const void *symAddr);
extern U16 OsSymGetSeries(struct SymbolNode *symbolId);
extern char *OsSymGetName(struct SymbolNode *symbolId);
extern void *OsSymGetAddr(struct SymbolNode *symbolId);
extern U32 OsSymCheckFunc(struct SymbolNode *symbolId);
extern U32 OsSymCheckVar(struct SymbolNode *symbolId);
extern U32 OsSymGetSize(struct SymbolNode *symbolId);
extern U32 OsSymbolTblCreate(U32 imageId, U32 symsNum, struct SymbolNode *symsArray);
extern U32 OsSymFindByNameV2(U32 imageId, const U8 *othBuf, U32 othBufLen, const char *name,
                             struct SymbolNode **symId);
extern U32 OsSymDynAddV2(U32 imageId, const U8 *othBuf, U32 othBufLen, U16 unitInterNo, const char *name,
                         uintptr_t *value, U8 type, U32 size);
extern U32 OsSymFindByAddr(U32 imageId, const void *symAddr, struct SymbolNode **symId);
extern U32 OsSymFindByName(U32 imageId, const char *name, struct SymbolNode **symId);
extern U32 OsHashTblIdxGet(U32 elements, const char *name, U32 seed);
extern void OsHashTblPut(struct SymbolTbl *symbolTbl, struct SymbolNode *symbolNode);

extern SymbolFindnameF g_symGetNameByAddr;
extern U32 g_imageNum;
extern U32 g_imageId;

#if defined(OS_OPTION_LP_COLD_PAT)
extern U32 OsSymbolTblSecondaryInit(U32 hashSizeLog2);
extern U32 OsSymbolTblSecondaryCreate(U8 *elf);
extern U32 OsPatSymFind(const char *name, struct SymbolNode **symId);
#endif

#if defined(OS_OPTION_SYMBOL_QUICK_FIND)
extern struct SymbolNode *OsStaSymFindByAddr(uintptr_t addr);
#endif

// 镜像注册在sys模块，故需要放到模块对外头文件
#if defined(OS_OPTION_SYMBOL_OVER_IMAGE_FIND)
INLINE OS_SEC_ALW_INLINE U32 OsSysImageInfoRegInline(U32 imageId, U32 imageNum)
{
    return OsSysImageInfoReg(imageId, imageNum);
}
#else

INLINE OS_SEC_ALW_INLINE U32 OsSysImageInfoRegInline(U32 imageId, U32 imageNum)
{
    (void)imageId;
    (void)imageNum;
    return SRE_OK;
}

#endif

#endif /* SRE_SYMBOL_EXTERNAL_H */
