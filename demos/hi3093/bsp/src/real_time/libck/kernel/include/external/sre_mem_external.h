/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 内存基本功能的内部头文件。
 * Author: LibCK项目组
 * Create: 2009-1-20
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_MEM_EXTERNAL_H
#define SRE_MEM_EXTERNAL_H

#include "sre_mem.h"
#include "sre_err_external.h"
#include "sre_hook_external.h"
#if !defined(OS_OPTION_MEM_SAFE_KEY_WORD)
#include "sre_log_external.h"
#endif
#if defined(OS_OPTION_RND)
#include "sre_rnd_external.h"
#endif
#if (defined OS_OPTION_MEM_MAP_POOL)
#include "sre_mcmappool_external.h"
#endif
#if (defined OS_OPTION_MEM_DSW)
#include "sre_dswmem_external.h"
#endif

#if defined(OS_OPTION_STATIC_FSC_MEM)
extern struct TagMemPtInfo **g_memPtInf;
#define OS_MEM_PT_INFO(ptId)  g_memPtInf[(ptId)]
#define OS_MEM_PT_IS_UNINIT(ptInfo) ((ptInfo) == NULL)
#else
extern struct TagMemPtInfo *g_memPtInfo;
#define OS_MEM_PT_INFO(ptId)  (&g_memPtInfo[(ptId)])
#define OS_MEM_PT_IS_UNINIT(ptInfo) ((ptInfo)->installed == FALSE)
#endif

#define OS_MEM_HEAD_MAGICWORD       0xABAB /* 内存块的块控制头魔术字，确保为奇数 */
#define OS_MEM_TAIL_MAGICWORD       0xCDCDCDCD

#define OS_MAX_PT_NUM               253
#define OS_MI_SHARE_MAX_PT_NUM      496
#define OS_MAX_MAP_BIT              31
#define OS_MEM_SHIFT                4
#define OS_MEM_SIZE_ALIGN           4
#define OS_MEM_MAPPOOL_ALIGN        16

#define OS_MCMEM_CTRL_STATE_VALID   0xad5678da
#define OS_MCMEM_CTRL_STATE_INVALID 0xae1234ea

#define OS_MEM_IS_SHARE_PT(ptNo) (OS_MEM_PT_INFO(ptNo)->shared)
#define OS_MEM_IS_CACHE_PT(ptNo) (OS_MEM_PT_INFO(ptNo)->cachable)

#define OS_MEM_IS_MC_FSC_PT(ptNo) ((ptNo) & 0x80000000UL)
#define OS_MEM_MI_FSC_GET_PTNO(ptNo) ((ptNo) & 0x7FFFFFFFUL)

/*
 * OS实例间共享分区分区号划分(预留给OS的只有16个，目前只支持2个实例进行划分)
 * 内存级别      inst1主核分区号       inst1从核分区号       inst2主核分区号       inst2从核分区号
 * SL2           0                    1              8                9
 * DDR64         2                    3              10               11
 */
#define OS_MIFSC_DEFAULT_PT_SL2(instId)   (((instId) - 1) * 0x8UL + OS_MI_SHARE_MEM_PT_FLAG)
#define OS_MIFSC_DEFAULT_PT_DDR64(instId) (0x2UL + ((instId) - 1) * 0x8UL + OS_MI_SHARE_MEM_PT_FLAG)
#define OS_MIFSC_DEFAULT_PT_NUM  2
#define OS_MIFSC_MAX_INSTNUM  2

#define OS_FSC_MEM_MAX_TYPE         32

#define LIBC_BUF_MAX_SIZE           0x1000

#define OS_MEM_FSC_FLAG             0xf1
#define OS_MEM_SLUB_FLAG            0xf3
#define OS_MEM_MAA_FLAG             0xf5
#define OS_MEM_SLAB_FLAG            0xfb
#define OS_MEM_MI_FSC_FLAG          0xfd
#define OS_MEM_FSC_ARITH(flag) ((flag) == OS_MEM_FSC_FLAG)
#define OS_MEM_SLUB_ARITH(flag) ((flag) == OS_MEM_SLUB_FLAG)
#define OS_MEM_MAA_ARITH(flag) ((flag) == OS_MEM_MAA_FLAG)
#define OS_MEM_SLAB_ARITH(flag) ((flag) == OS_MEM_SLAB_FLAG)
#define OS_MEM_MI_FSC_ARITH(flag) ((flag) == OS_MEM_MI_FSC_FLAG)
#define OS_MEM_ALIGN_CHECK(value) ((value) & 0x3UL)
#define OS_MEM_SHIFT_SET(value) ((value) >> 4)

#define OS_MEM_REDZONE              0xc5U
#define OS_MEM_REDEND               0xc6U

#define OS_MEM_REDZONE_U32          0xc5c5c5c5U
#define OS_MEM_REDEND_U32           0xc6c6c6c6U
#define OS_MEM_RESERVE_NUM          2

typedef void *MpaHandle;

typedef U32 (*MemAppend)(U8 ptNo, uintptr_t *addr, uintptr_t size);
typedef U32 (*MemRemove)(U8 ptNo, uintptr_t *addr, uintptr_t size);
typedef U32 (*MemCacheGetInfo)(U8 ptNo, uintptr_t size, struct OsMemCacheInfo *info);
typedef U32 (*MemCacheFill)(U8 ptNo, uintptr_t size, U32 num, uintptr_t intSave);
typedef U32 (*MemCacheGetTypeNum)(U8 ptNo, U32 *num);
typedef U32 (*MemCacheGetTypeInfo)(U8 ptNo, struct OsMemCacheTypeInfo *info, U32 num);

/* 内存分区使用信息 */
struct TagUseageHead {
    U32 hwiMemSize; /* 硬中断的内存使用长度 */
    U32 threadMemSize; /* 任务和软中断总的内存使用长度 */
    U32 memUsedSize; /* 总的内存使用长度 */
    U16 threadSize[OS_MEM_RESERVE_NUM]; /* 任务和软中断的内存使用长度，每个任务或软中断一个 */
};

/*
 * 嵌入到分区控制头的内存维测信息，DBG选项关闭时，为空结构体，SIZE为0
 */
#if defined(OS_OPTION_MEM_DBG)
struct MemDbgInsetInfo {
    /* 申请内存时的任务PID */
    U32 pid;
    /* 申请内存时调用者的LR */
    U32 cookie;
};
#endif

/* FSC算法块控制头结构，注意各成员顺序是和其他算法保持一致偏移的，不能随便改动，保持ptNo和其他算法偏移一致 */
struct TagFscMemCtrl {
    union {
        struct {
            /* 魔术字，位置固定 */
            U16 magicWord;
            /* 分区号，位置固定 */
            U8 ptNo;
            /* 算法标志，位置固定，FSC为0xfb */
            U8 arithFlag;
        } fscHead;
        // 空闲时为下一个空闲块地址
        struct TagFscMemCtrl *next;
    };
    uintptr_t size; // 块大小
    /*
     * 若前面相邻的物理块空闲，则此字段记录前面空闲块大小，否则为OS_FSC_MEM_PREV_USED
     * bit0：1表示后一块（右边块）是静态内存 bit0：0表示后一块（右边块）是动态内存
     * bit1：1表示前一块（左边块）是申请的内存 bit1：0表示前一块（左边块）是free内存
     * 其他bit 0：表示静态内存， 非0表示动态内存
     */
    uintptr_t prevSize;
#if defined(OS_OPTION_MEM_DBG)
    struct MemDbgInsetInfo dbg;
#endif
#if defined(OS_OPTION_MEM_USAGE_DETAIL_GET)
    U32 blockOwner; /* 该分区被使用大小 */
#endif
    union {
        struct {
            // slub-fsc,有用
            U16 reserved;
            /* 控制头地址相对于用户地址的偏移，位置固定为控制头的末尾 */
            U16 headOffset;
        };
        // 空闲时为上一个控制块地址
        struct TagFscMemCtrl *prev;
#if defined(OS_OPTION_MEM_APPEND)
        // 下一个不连续尾地址
        void *nextTail;
#endif
    };
};

/* 分区控制块信息 */
struct TagMemPtInfo {
    enum MemArith arith; /* 分区使用的内存算法 */
    void *ptAddr; /* 分区起始地址 */

    /* 对于共享算法，指向多核共享内存分区控制头内存起始地址，对于私有FSC，指向空闲链表头数组 */
    void *ptCtrl;

    uintptr_t ptSize; /* 分区大小，分区动态扩展该值不变 */
    uintptr_t totalSize; /* 分区总内存大小，分区动态扩展该值增加 */
    uintptr_t freeSize; /* 分区可供使用的内存大小 */
    U8 installed; /* 分区是否被使用 */
    U8 cachable; /* 分区是否cache */
    U8 shared; /* 分区是否为共享分区 */
    U8 core; /* 分区创建于哪个核 */
    U32 bitMap; /* 私有FSC的空闲链表Bitmap */
    MemAllocFunc alloc; /* 该分区申请内存接口 */
    MemFreeFunc free; /* 该分区释放内存接口 */
#if defined(OS_OPTION_MEM_APPEND)
    MemAppend append; /* 该分区扩展内存接口 */
    MemRemove remove; /* 该分区删除扩展内存接口 */
#endif
#if defined(OS_OPTION_MEM_SLUB_FSC) || defined(OS_OPTION_MEM_BLOCK)
    void *privateCtrl; /* 组合算法时，可以记录上层算法的控制块信息，例如fsc+slub的slub控制信息 */
#endif
#if defined(OS_OPTION_MEM_POOL)
    U32 sliceMaxSizeOrig; /* 原始的最大块大小 */
    U32 sliceMaxSizeAligned; /* 对齐后的最大块大小 */
#endif
#if defined(OS_OPTION_MEM_USAGE_DETAIL_GET)
    struct TagUseageHead *ptUsedInf; /* 该分区被使用大小 */
#endif
#if defined(OS_OPTION_MEM_DSSC)
    MemFreeExFunc freeEx; /* 头体分离内存释放接口 */
#endif
#if defined(OS_OPTION_MEM_CACHE_FILL)
    MemCacheGetTypeNum cacheGetTypeNum;   // 获取支持的缓存块类型个数
    MemCacheGetTypeInfo cacheGetTypeInfo; // 获取支持的缓存块类型信息
    MemCacheGetInfo cacheGetInfo;  // 获取指定缓存块的当前信息
    MemCacheFill cacheFill;        // 填充指定缓存块
#endif
};

/* 共享内存分区控制头结构体，在缺省共享静态FSC内存中分配空间 */
struct TagShareMemPtCtrl {
    /* 本分区控制块是否有效 */
    U32 state;
    /* 分区对应的算法 */
    enum MemArith arith;
    /* 分区对齐后的地址 */
    void *ptCtrl;
    /* 分区起始地址 */
    void *ptAddr;
    /* 分区大小 */
    uintptr_t ptSize;
    /* 分区是否cache */
    bool cachable;

    /* Raw内存块起始地址 */
    void *freeTop;
    /* 分区空闲内存大小 */
    uintptr_t freeSize;
};

/* 私有FSC分区控制头 */
struct TagFscMemPtCtrl {
    /* 记录分区中最靠后的申请内存快尾，用于计算该分区最大的使用量 */
    uintptr_t peakSize;
#if defined(OS_OPTION_STATIC_FSC_MEM)
    struct TagFscMemCtrl fscFreeHead[(OS_FSC_MEM_MAX_TYPE >> 1) + 1];
#else
    struct TagFscMemCtrl fscFreeHead[OS_FSC_MEM_MAX_TYPE];
#endif
#if defined(OS_OPTION_SMP)
    volatile uintptr_t spinLock;
#endif
#if defined(OS_OPTION_MEM_DBG)
    /* 该分区被使用了的内存块数 */
    U32 usedBlocks;
#endif
};

/*
 * 共享FSC内存分区控制头结构体，在缺省共享静态FSC内存分区中分配空间
 * 成员顺序不能随便动，需要与TagShareMemPtCtrl成员保持一致
 */
struct TagShareFscMemPtCtrl {
    U32 state; /* 本分区控制块是否有效 */
    enum MemArith arith; /* 分区对应的算法 */
    void *ptCtrl; /* 分区对齐后的地址 */
    void *ptAddr; /* 分区起始地址 */
    uintptr_t ptSize; /* 分区大小 */
    bool cachable; /* 分区是否cache */
    U32 spinLockId; /* 共享互斥锁 */
    uintptr_t freeSize;
    uintptr_t bitMap;
#if defined(OS_OPTION_STATIC_FSC_MEM)
    struct TagFscMemCtrl fscFreeHead[(OS_FSC_MEM_MAX_TYPE >> 1) + 1];
#else
    struct TagFscMemCtrl fscFreeHead[OS_FSC_MEM_MAX_TYPE];
#endif
    uintptr_t peakSize; /* 用于计算该分区最大的使用量 */
};

#if defined(OS_OPTION_MEM_MI_FSC)
struct MiShareMemCfg {
    uintptr_t addr;
    U32 size;
    U32 startPtNo;
    U32 maxPtNum;
    U32 resvShmSize;
    U32 osStartPtNo;
    U32 osMaxPtNum;
    U32 maxCfgPtNum;
};

/*
 * 实例间共享FSC算法块控制头结构，注意各成员顺序是和其他算法保持一致偏移的，不能随便改动，
 * 保持ptNo和其他算法偏移一致
 */
struct MiFscMemCtrl {
    union {
        struct {
            /* 魔术字 */
            U16 magicWord;
            /* 待free标记: 1表示需要free; 0表示inuse，不能free */
            U8 needFreeFlag;
            /* 算法标志，位置固定，MIFSC为0xfd */
            U8 arithFlag;
        } fscHead;
        // 空闲时为下一个空闲块地址
        struct MiFscMemCtrl *next;
    };
    U32 ptNo;
    uintptr_t size; // 块大小
    /*
     * 若前面相邻的物理块空闲，则此字段记录前面空闲块大小，否则为OS_FSC_MEM_PREV_USED
     * bit0：1表示后一块（右边块）是静态内存 bit0：0表示后一块（右边块）是动态内存
     * bit1：1表示前一块（左边块）是申请的内存 bit1：0表示前一块（左边块）是free内存
     * 其他bit 0：表示静态内存， 非0表示动态内存
     */
    uintptr_t prevSize;
#if defined(OS_OPTION_MEM_DBG)
    struct MemDbgInsetInfo dbg;
#endif
    union {
        struct {
            U32 reserved0;
        };
        // Alloc链，指向下一个alloc块
        struct MiFscMemCtrl *allocNext;
    };
    union {
        struct {
            // slub-fsc,有用
            U16 reserved1;
            /* 控制头地址相对于用户地址的偏移，位置固定为控制头的末尾 */
            U16 headOffset;
        };
        // 空闲时为上一个控制块地址
        struct MiFscMemCtrl *prev;
    };
};

/*
 * 实例间共享FSC内存分区控制头结构体，在共享内存池头部固定分配空间
 */
struct MiShareFscMemPtCtrl {
    U32 arithVer; /* 分区算法版本号 */
    U32 state; /* 本分区控制块是否有效 */
    enum MemArith arith; /* 分区对应的算法 */
    void *ptCtrl; /* 分区对齐后的地址 */
    void *ptAddr; /* 分区起始地址 */
    uintptr_t ptSize; /* 分区大小 */
    bool cachable; /* 分区是否cache */
    U32 spinLockId; /* 共享互斥锁 */
    uintptr_t freeSize;
    uintptr_t bitMap;
    struct MiFscMemCtrl fscFreeHead[(OS_FSC_MEM_MAX_TYPE >> 1) + 1];
    struct MiFscMemCtrl *allocTail; /* alloc链链尾 */
    struct MiFscMemCtrl *allocHead; /* alloc链链头 */
    U32 userInstId;
    uintptr_t peakSize; /* 用于计算该分区最大的使用量 */
#if defined(OS_OPTION_MEM_DBG)
    /* 该分区被使用了的内存块数 */
    U32 usedBlocks;
#endif
};

/* 结构体做重叠排布，1次偏移，sizeof（struct TagFscMemCtrl）/2 即8字节 */
#define MI_FSC_FREE_LIST_HEAD(freelist, index) (struct MiFscMemCtrl *)((uintptr_t)(freelist) + (uintptr_t)((index) * 8))
#define MI_FSC_VERSION_0 0x0U

/* 一次只释放1个，idle中使用 */
#define MI_FSC_FREE_NUM_ONE 0x1U
/* 一次只释放前2个，并且遇needFreeFlag=0截止 */
#define MI_FSC_FREE_NUM_TWO 0x2U
/* 连续释放多个，直到needFreeFlag=0截止 */
#define MI_FSC_FREE_CONTINUE 0xFFU
/* 遍历全部alloc链 */
#define MI_FSC_FREE_NUM_ALL 0xFFFFU

#define MI_FSC_FREE_NUM_MAX 0xFFFFFFFFU
#endif

#if defined(OS_OPTION_STATIC_FSC_MEM)
/* 结构体做重叠排布，1次偏移，sizeof（struct TagFscMemCtrl）/2 即8字节 */
#define FSC_FREE_LIST_HEAD(freelist, index) (struct TagFscMemCtrl *)((uintptr_t)(freelist) + ((uintptr_t)(index) * 8))
#else
/* 结构体未做优化，内存占用较多 */
#define FSC_FREE_LIST_HEAD(freelist, index) ((struct TagFscMemCtrl *)(freelist) + (uintptr_t)(index))
#endif

/* 创建共享内存分区时，用于记录共享内存分区信息控制体 */
struct TagMemPtCheck {
    /* 指向分区控制块位置 */
    void *ptCtrl;
};

extern struct MemArithApi g_memArithAPI[MEM_ARITH_BUTT];
extern struct TagMemPtCheck *g_memPtCheck;
extern struct TagMemPtInfo  **g_memPtInfoCheck;
/* 内存分区个数 */
extern U8 g_maxPtNum;
extern bool g_defMemPtInitFlag;
extern U8 g_ptNum;

typedef void *(*StaticMemAllocFunc)(U32 mid, U8 ptId, uintptr_t size, enum MemAlign align);

extern U8 OsGetMaxPartitionNum(void);
extern void *OsMemAlloc(enum OsMoudleId mid, U8 ptNo, U32 size);
extern void *OsMemAllocAlign(U32 mid, U8 ptNo, U32 size, enum MemAlign alignPow);
extern U32 OsMemPtParaSet(U8 ptNo, struct MemPtCreate *para, bool share, bool cache, U32 align);
extern void OsMemArithReg(enum MemArith arith, struct MemArithApi *memApi);
extern U32 OsMemPtIdentCheck(struct MemPtCreate *para, U8 ptNo);
extern void OsMemPtHeadInit(struct MemPtCreate *para, U8 ptNo);
#if defined(OS_OPTION_MEM_MAP_POOL)
extern U32 OsMcMpaOverlapCheck(struct MemPtCreate *para, U8 ptNo);
#endif
#if defined(OS_OPTION_MEM_CACHE_FILL)
extern void OsMemCacheRegHooks(U8 ptNo, MemCacheGetTypeNum getTypeNumHook, MemCacheGetTypeInfo getTypeInfoHook,
                               MemCacheGetInfo getInfoHook, MemCacheFill fillHook);
#else
OS_SEC_ALW_INLINE INLINE void OsMemCacheRegHooks(U8 ptNo, MemCacheGetTypeNum getTypeNumHook,
                                                 MemCacheGetTypeInfo getTypeInfoHook,
                                                 MemCacheGetInfo getInfoHook, MemCacheFill fillHook)
{
    (void)ptNo;
    (void)getTypeNumHook;
    (void)getTypeInfoHook;
    (void)getInfoHook;
    (void)fillHook;
    return;
}
#endif

#if defined(OS_OPTION_MEM_SPRAY_POISON)
extern void OsMemSprayPoison(const void *addr, uintptr_t size);
extern bool OsMemUafCheck(const void *addr, uintptr_t size);
#else
OS_SEC_ALW_INLINE INLINE void OsMemSprayPoison(const void *addr, uintptr_t size)
{
    (void)addr;
    (void)size;
    return;
}

OS_SEC_ALW_INLINE INLINE bool OsMemUafCheck(const void *addr, uintptr_t size)
{
    (void)addr;
    (void)size;
    return FALSE;
}
#endif

#if defined(OS_OPTION_MEM_USAGE_DETAIL_GET)
extern U32 OsMemPtUsageBufSizeGet(void);
extern void OsMemptUsageAdd(U32 mid, U8 ptNo, uintptr_t addr);
extern void OsMemptUsageReduce(U8 ptNo, struct TagFscMemCtrl *currBlk);
#endif

#if defined(OS_OPTION_SMP)
OS_SEC_ALW_INLINE INLINE void OsFscMemSpinLock(struct TagFscMemPtCtrl *fscMemPtCtrl)
{
    OsSplLock(&fscMemPtCtrl->spinLock);
}

OS_SEC_ALW_INLINE INLINE void OsFscMemSpinUnLock(struct TagFscMemPtCtrl *fscMemPtCtrl)
{
    OsSplUnlock(&fscMemPtCtrl->spinLock);
}
#else
OS_SEC_ALW_INLINE INLINE void OsFscMemSpinLock(struct TagFscMemPtCtrl *fscMemPtCtrl)
{
    (void)fscMemPtCtrl;
}

OS_SEC_ALW_INLINE INLINE void OsFscMemSpinUnLock(struct TagFscMemPtCtrl *fscMemPtCtrl)
{
    (void)fscMemPtCtrl;
}
#endif

#if defined(OS_OPTION_MEM_SAFE_OVERFLOW_CHECK)
OS_SEC_ALW_INLINE INLINE void OsMemMagicWordEncode(U16 *magicWord)
{
    *magicWord = (U16)(((U32)(*magicWord)) ^ ((U32)OsGetRndU16()));
}

OS_SEC_ALW_INLINE INLINE U16 OsMemMagicWordDecode(const U16 *magicWord)
{
    U16 decodeMagic = *magicWord;

    decodeMagic = (U16)((U32)decodeMagic ^ (U32)OsGetRndU16());

    return decodeMagic;
}

OS_SEC_ALW_INLINE INLINE void OsMemTailAdd(uintptr_t *allocsize)
{
    /* 增加4字节内存尾部魔术字 */
    *allocsize += sizeof(U32);
}
OS_SEC_ALW_INLINE INLINE void OsMemTailMagicWordSet(uintptr_t plotBlk, uintptr_t allocSize)
{
    *(uintptr_t *)(plotBlk + allocSize - sizeof(uintptr_t)) = OS_MEM_TAIL_MAGICWORD;
}
OS_SEC_ALW_INLINE INLINE bool OsMemTailMagicWordGet(uintptr_t plotBlk, uintptr_t allocSize)
{
    if (*(uintptr_t *)(plotBlk + allocSize - sizeof(uintptr_t)) != OS_MEM_TAIL_MAGICWORD) {
        return TRUE;
    }
    return FALSE;
}
#else
OS_SEC_ALW_INLINE INLINE void OsMemMagicWordEncode(U16 *magicWord)
{
    (void)magicWord;
    return;
}

OS_SEC_ALW_INLINE INLINE U16 OsMemMagicWordDecode(const U16 *magicWord)
{
    (void)magicWord;
    return *magicWord;
}

OS_SEC_ALW_INLINE INLINE void OsMemTailAdd(uintptr_t *allocsize)
{
    (void)allocsize;
}

OS_SEC_ALW_INLINE INLINE void OsMemTailMagicWordSet(uintptr_t plotBlk, uintptr_t allocSize)
{
    (void)plotBlk;
    (void)allocSize;
    return;
}
OS_SEC_ALW_INLINE INLINE bool OsMemTailMagicWordGet(uintptr_t plotBlk, uintptr_t allocSize)
{
    (void)plotBlk;
    (void)allocSize;
    return FALSE;
}
#endif

#if defined(OS_OPTION_MEM_SAFE_KEY_WORD)
/* 由于offset是个变量，加随机数只能增加其内容被猜测的难度，但不能发现是否被overflow篡改 */
OS_SEC_ALW_INLINE INLINE void OsMemCfgHeadAddr(uintptr_t usrAddr, uintptr_t ctrlAddr)
{
    U16 *headAddr = (U16 *)(usrAddr) - 1;
    U16 temp = (U16)(usrAddr - ctrlAddr);
    temp = (U16)((U32)temp ^ (U32)OsGetRndU16());
    temp = (U16)((U32)temp ^ (U32)usrAddr);
    *headAddr = temp;
    return;
}

OS_SEC_ALW_INLINE INLINE void *OsMemGetHeadAddr(uintptr_t usrAddr)
{
    U16 headOffset = *((U16 *)usrAddr - 1);

    headOffset = (U16)((U32)headOffset ^ usrAddr);
    headOffset = (U16)((U32)headOffset ^ (U32)OsGetRndU16());

    return (void *)(usrAddr - (uintptr_t)headOffset);
}
/* 防止地址泄漏，通过钩子传出的地址进行加密处理 */
OS_SEC_ALW_INLINE INLINE uintptr_t OsMemCfgHookAddr(uintptr_t addrress)
{
    uintptr_t hookAddr = addrress ^ OsGetRndU32();
    return hookAddr;
}

#else
/* 对齐之后，返回地址不一定紧跟在内存头后面，需要设置返回地址与内存头之间的差值 */
OS_SEC_ALW_INLINE INLINE void OsMemCfgHeadAddr(uintptr_t usrAddr, uintptr_t ctrlAddr)
{
    U16 *headAddr = (U16 *)(usrAddr) - 1;

    *headAddr  = (U16)(usrAddr - ctrlAddr);
    return;
}
OS_SEC_ALW_INLINE INLINE void *OsMemGetHeadAddr(uintptr_t usrAddr)
{
    U16 headOffset = *((U16 *)usrAddr - 1);

    return (void *)(usrAddr - (uintptr_t)headOffset);
}
OS_SEC_ALW_INLINE INLINE uintptr_t OsMemCfgHookAddr(uintptr_t addrress)
{
    return LOG_ADDR_DBG(addrress);
}
#endif

OS_SEC_ALW_INLINE INLINE bool OsMemRangeOverlapCheck(uintptr_t start1, uintptr_t len1,
                                                     uintptr_t start2, uintptr_t len2)
{
    if ((start1 == start2) ||
        ((start1 < start2) && ((start1 + len1) > start2)) ||
        ((start2 < start1) && ((start2 + len2) > start1))) {
        return TRUE;
    }

    return FALSE;
}

/* 静态内存申请优化对外接口，不支持静态头优化，采用动态内存申请 */
extern void *OsMemStaticAllocAndClr(U32 mid, U32 ptNo, uintptr_t size, enum MemAlign align);
#if defined(OS_OPTION_MEM_MI_FSC)
extern void *OsMiFscMemAllocAlign(U32 mid, U32 ptNo, uintptr_t size, enum MemAlign alignPow);
extern U32 OsMiShareMemFreeAll(U32 glbPtNo);
extern U32 OsMiShmRegPtCheck(U32 startPtNo, U32 maxPtNum);
extern U32 OsMiFscMemPtInfoInit(struct MiShareMemCfg *mngMemPara,
    uintptr_t *memStartAddr, uintptr_t *memSize, uintptr_t *resvShmAddr, uintptr_t *resvShmSize);
extern U32 OsMiFscMemPtCreate(U32 ptNo, U32 userInstId, uintptr_t ptAddr, uintptr_t ptSize);
extern U32 OsMiPtCreateCheck(U32 ptNo, uintptr_t ptSize);
extern U32 OsMiFscMemPtDelete(U32 ptNo, uintptr_t *ptAddr);
extern U32 OsMiFscMemPreFree(U32 mid, struct MemCtrlHead *memHead);
extern U32 OsMiShareMemPtInfoGet(U32 glbPtNo, struct MemStatInfo *memPtState);
#endif
#endif /* SRE_MEM_EXTERNAL_H */
