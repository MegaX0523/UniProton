/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Fsc 内存实现
 * Author: LibCK项目组
 * Create: 2009-09-17
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_FSCMEM_EXTERNAL_H
#define SRE_FSCMEM_EXTERNAL_H

#include "sre_mem_external.h"

/*
 * 模块间宏定义
 */
#define OS_FSC_SLUB_MID            0xFEDCBA21
#define OS_FSC_MEM_SLICE_HEAD_SIZE sizeof(struct TagFscMemCtrl)

#if defined(OS_OPTION_MEM_MI_FSC)
#define OS_MI_FSC_MEM_SLICE_HEAD_SIZE sizeof(struct MiFscMemCtrl)
#endif

#if defined(OS_OPTION_MEM_SLUB_FSC)
#define OS_SLUB_FSC_MID_CHK(mid) ((mid) == OS_FSC_SLUB_MID)
#else

#define OS_SLUB_FSC_MID_CHK(mid) ((void)(mid), FALSE)

#endif

#if defined(OS_OPTION_MEM_MI_FSC)
extern void OsMiShareMemPtClear(U32 startPtNo, U32 maxPtNum);
#if defined(OS_OPTION_MEM_DBG)
extern uintptr_t OsMiFscMemDbgInfoSizeGet(U32 ptNo);
extern void OsMiFscMemDbgInfoGet(U32 glbPtNo, void *infoBuff, uintptr_t *bufSize);
#endif
#endif
/*
 * 模块间函数声明
 */
extern U32 OsFscMemDefPtCreate(U8 ptNo, uintptr_t ptAddr, uintptr_t ptSize);
extern void *OsFscMemAlloc(U32 mid, U8 ptNo, uintptr_t size, uintptr_t alignPow);
extern U32 OsFscMemFree(U32 mid, U8 ptNo, struct MemCtrlHead *memHead);
extern void OsFscMemFreeInner(U32 mid, U8 ptNo, struct MemCtrlHead *memHead);
extern U32 OsFscMemPtCreate(U8 ptNo);
extern U32 OsFscMemPtDelete(U8 ptNo);
extern U32 OsFscMemMaxBlockSizeGet(struct TagFscMemCtrl *fscFreeListHead);
extern U32 OsFscMemPtInfoGet(U8 ptNo, struct MemStatInfo *memPtState);
#if defined(OS_OPTION_MEM_DBG)
extern void OsFscMemDbgInfoGet(U8 ptNo, void *infoBuff, uintptr_t *bufSize);
extern uintptr_t OsFscMemDbgInfoSizeGet(U8 ptNo);
#endif
#if defined(OS_OPTION_MEM_APPEND)
extern U32 OsFscMemAppend(U8 ptNo, uintptr_t *addr, uintptr_t size);
extern U32 OsFscMemRemove(U8 ptNo, uintptr_t *addr, uintptr_t size);
#endif

#endif /* SRE_FSCMEM_EXTERNAL_H */
