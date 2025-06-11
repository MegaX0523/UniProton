/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Cache implementation
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_MMU_EXTERNAL_H
#define SRE_MMU_EXTERNAL_H

#include "sre_lib_external.h"
#include "sre_raw_spinlock_external.h"
#include "sre_err_external.h"
#include "sre_sys_external.h"
#include "sre_cpu_external.h"
#include "sre_board_external.h"

#include "sre_sync.h"
#include "sre_mmu.h"
#include "sre_sys.h"

/* 目前把emmu 当做DSS 的一部分，但本质上emmu 是外设，是SOC 层级的，后续需要修正 */
#define OS_GET_MMU_REG_GLOBAL_MAP(coreId) ((struct MmuCtlReg *)(OS_GET_MMU_REG_GLOBAL_ADDR(coreId)))

#if defined(OS_OPTION_POWERGATE)
#define MMU_SBASE_OFFSET(hwThId) (0x8 + 0x8 * (hwThId))
#define MMU_UBASE_OFFSET(hwThId) (0xc + 0x8 * (hwThId))

#define L2TLB_INV_OFFSET        0x54
#define ITLB_INV_OFFSET         0x48
#define DTLB_INV_OFFSET(hwThId) (0x4c + 0x4 * (hwThId))
#endif

#define OS_MMU_MIN_PAGE_SIZE 0x1000U

#define OS_MMU_L2_PAGE_TBL_ADDR_ALIGN 0x400
#define OS_MMU_L2_PAGE_TBL_SIZE_ALIGN 0x400
#define OS_MMU_L1_PAGE_TBL_ADDR_ALIGN 0x4000

#define OS_MMU_NO_OPT_MODE 0  // 不优化
#define OS_MMU_PART_OPT_MODE 1  // 当前映射地址范围内优化
#define OS_MMU_ALL_OPT_MODE 2  // 整个页表项优化

/* 单个二级页表描述内存大小 */
#define OS_MMU_4K 0x1000
/* 两个个二级页表描述内存大小 */
#define OS_MMU_8K 0x2000
#define OS_MMU_16K 0x4000
#define OS_MMU_64K 0x10000
#define OS_MMU_256K 0x40000

/* 单个一级页表描述内存大小 */
#define OS_MMU_1M 0x100000
#define OS_MMU_4M 0x400000
#define OS_MMU_16M 0x1000000

#define OS_MMU_4K_SHIFT 12
#define OS_MMU_1M_SHIFT 20

/* 通过虚拟地址获取L1页表描述符位置 */
#define OS_MMU_GET_L1_PT_IDX_BY_VADDR(addr) ((addr) >> 20UL)
/* 通过虚拟地址获取L2页表描述符位置 */
#define OS_MMU_GET_L2_PT_IDX_BY_VADDR(addr) ((addr) >> 12UL)

/* 获取L1 PAGETABLE地址 */
#define OS_MMU_GET_L1_PT_ADDR(addr) ((addr) & 0xfff00000UL)

/* 获取L2 PAGETABLE 页表项的偏移 */
#define OS_MMU_GET_L1_PT_OFFSET(addr) (OS_MMU_GET_L1_PT_IDX_BY_VADDR(addr) * OS_MMU_PT_STRUCT_SIZE)

/* 获取L2 PAGETABLE地址 */
#define OS_MMU_GET_L2_PT_ADDR(addr) ((addr) & (~0x3ffUL))

/* 获取L1 PAGETABLE 中的映射的物理地址偏移 */
#define OS_MMU_GET_L1_PT_MAP_OFFSET(addr) ((addr) & 0xfffffUL)

/* 获取L2 PAGETABLE 页表项的偏移 */
#define OS_MMU_GET_L2_PT_OFFSET(addr) (OS_MMU_GET_L2_PT_IDX_BY_VADDR((addr) & 0xfffffUL) * OS_MMU_PT_STRUCT_SIZE)

/* 获取L2 PAGETABLE 页表项的内容 */
#define OS_MMU_GET_L2_PT_ITEM_ADDR(l1Page, addr) (uintptr_t)(OS_MMU_GET_L2_PT_ADDR(l1Page) + \
                                                             OS_MMU_GET_L2_PT_OFFSET(addr))

#define OS_MMU_VALID_VADDR(addr) ((addr) & 0xfffffc00UL)

#define OS_MMU_VALID_PADDR(addr) ((addr) & 0xfffffc00UL)

#define OS_MMU_VALID_SIZE(len) ((len) & 0xfffffc00UL)

/* MMU 页表生效BIT位 */
#define OS_MMU_EN_BIT 0x1UL
/* MMU 页表失效BIT位 */
#define OS_MMU_DIS_BIT 0x0

/* 获取反向表项总大小 */
#define OS_MMU_SIZE_TO_KBYTE(size) ((U32)(size) << 10)

/* 获取反向表项总大小 */
#define OS_MMU_SIZE_ITEM_GET(size, mapSize) (((size) >> (mapSize)) << 2)
#define OS_MMU_ADDR_ALIGN(addr, mapSize) (((addr) >> (mapSize)) << (mapSize))

#define MMU_PAGE_TYPE_NUM 8

#define PAGE_MASK_COLLECTION \
    { 0xfffff000, 0xffffe000, 0xffffc000, 0xffff0000, 0xfffc0000, 0xfff00000, 0xffc00000, 0xff000000 }

#define PAGE_SIZE_COLLECTION \
    { 0x1000, 0x2000, 0x4000, 0x10000, 0x40000, 0x100000, 0x400000, 0x1000000 }

#define MMU_MSG_REQ_ASSIGN 0xf0
#define MMU_MSG_REQ_UNASSIGN 0xf1
#define MMU_MSG_REQ_SET_ATTR 0xf2
#define MMU_MSG_REQ_REVOKE_MMU 0xf3

#define OS_MMU_MSG_ID_PACK(attr, id, type) (((attr) << 20) | ((id) << 16) | (type))
#define OS_MMU_MSG_GET_INST_ID(msgId) (((msgId) >> 16) & 0xfU)
#define OS_MMU_MSG_GET_ATTR(msgId) (((msgId) >> 20) & (~OS_MMU_STATE_MASK))

#define OS_MMU_MSG_GET_TYPE(msgId) ((msgId) & 0xffffU)
#define OS_MMU_MSG_GET_ORG_DATA(data) ((U32)((data) << 12))
#define OS_MMU_MSG_LENGTH (2 * sizeof(uintptr_t))

struct TagPhyVirMem {
    /* 映射大小 */
    U64 size : 20;
    /* 物理地址 */
    U64 physicalAddr : 20;
    /* 虚拟地址 */
    U64 virtualAddr : 20;
    U64 res : 4;
};

struct MmuP2vPageInfo {
    /* 硬线程的反向表基址 */
    uintptr_t p2vPageTbl;
    /* 硬线程的反向表空间大小 */
    uintptr_t p2vTblSize;
};

#if ((defined OS_OPTION_SREMNG) && (defined OS_OPTION_LP_PRELINK))
/* IIU_BEGIN+6: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
struct MngMmuInfo {
    bool optFlag;
    struct TagListObject l2MmuFreeList[OS_MAX_CORE_NUM];
    struct MmuP2vPageInfo mmuP2vTbl[OS_MAX_CORE_NUM];
};
#endif

/*
 * 模块间数据结构定义
 */
extern U32 OsMmuPhyToVirInit(void);
extern U32 OsGetMmuBackdbgTag(U32 *scuMmuCr, U32 *mmuRegL2TlbBackDbg, uintptr_t tempAddr, U32 tempWay, U32 *mmuPbaInfo,
                              U32 *mmuVbaInfo);
extern void OsTlbInv(U32 coreId);

extern void OsMmuEnable(U32 coreId);
extern U32 OsMmuModeGet(U32 coreId);

extern U32 OsMmuMapJudge(uintptr_t transTblAddr, MMU_VA_T virAddr, U32 *l1PageValue, U32 *l2PageValue);

extern void OsMmuL2PageUsrAddrGet(uintptr_t transTblAddr, uintptr_t *l2PageUsrAddr);

extern void OsMmuErrIntClr(void);
extern void OsMmuGetTransTblAddr(uintptr_t *transTblAddr);
extern U32 OsMmuMcAssign(U32 instID, U32 coreId, uintptr_t transTblAddr,
                         uintptr_t vStart, uintptr_t start, uintptr_t len, U32 attr);
extern U32 OsMmuMcUnAssign(U32 instID, U32 coreId, uintptr_t transTblAddr, uintptr_t vAddr, uintptr_t len);

#if (defined OS_OPTION_LP_PRELINK)
extern void OsMmuMcUnAssignForce(U32 instId, U32 coreId, uintptr_t transTblAddr, uintptr_t vAddr, uintptr_t len);
extern void OsMmuScPhyToVirAssign(U32 instId, U32 coreId, uintptr_t pa, uintptr_t va, U32 len);
extern U32 OsMmuScVirToPhyAssign(U32 instId, U32 coreId, uintptr_t transTblAddr, uintptr_t va,
                                 uintptr_t pa, uintptr_t len, U32 attr);

typedef bool(*MmuTlbActiveCheck)(U32 instId, U32 coreId);
typedef struct TagListObject *(*MmuL2PageFreeListGet)(U32 instId, U32 coreId);
typedef struct MmuP2vPageInfo *(*MmuP2vTableGet)(U32 instId, U32 coreId);
typedef U32(*MmuMcAssignFunc)(U32 instId, U32 coreId, uintptr_t va, uintptr_t pa, uintptr_t len, U32 attr);

extern MmuTlbActiveCheck g_mmuTlbActiveCheck;
extern MmuMcAssignFunc g_mmuMcpuMcAssign;
extern MmuP2vTableGet g_mmuMcpuP2vTableGet;
extern MmuL2PageFreeListGet g_mmuMcpuL2PageFreeListGet;
#endif

extern U32 OsMmuAttrBackup(MMU_VA_T vStart, U32 len, uintptr_t regStart);
extern U32 OsMmuAttrRestore(uintptr_t regStart, U32 regSize);
extern U32 OsMmuAttrBackupRegSizeGet(MMU_VA_T vStart, U32 len, U32 *regionSize);
extern U32 OsMmuDefaultAttrSet(MMU_VA_T vStart, U32 len);
extern void OsMmuPageSpaceGet(uintptr_t transTblAddr, uintptr_t virAddr, uintptr_t len,
                              MMU_VA_T *vStartOut, U32 *lenOut);
extern void OsMmuPageBoundaryGet(uintptr_t virAddr, uintptr_t len, MMU_VA_T *vStartOut, U32 *lenOut);
extern U32 OsMmuMcAssignInner(U32 instID, U32 coreId, uintptr_t transTblAddr, uintptr_t virStart,
                              uintptr_t phyStart, uintptr_t len, U32 attr);
extern U32 OsMmuMemSizeGet(MMU_VA_T virAddr, U32 *memSize);
extern U32 OsMmuCreatPhyToVirTable(U32 instID, U32 coreId, uintptr_t addr, uintptr_t vAddr, uintptr_t len);
extern U32 OsMmuMcAssignOpt(U32 instID, U32 coreId, uintptr_t vAddr, uintptr_t phyAddr, uintptr_t transTblAddr,
                            uintptr_t len, U32 attr, U32 optMode);
extern void Osp2vTblOffsetGet(uintptr_t phyAddr, U32 *tblOffsetPtr, U32 *mapSizePtr);
extern void OsMmuDirectMap(uintptr_t l1TblAddr, uintptr_t l12TblAddr,
                           uintptr_t vStart, uintptr_t phyStart, enum MmuPageSize pageSize, U32 attr);
extern void OsMmuL2PageTblAddrSet(uintptr_t l1TblAddr, uintptr_t l12TblAddr, uintptr_t vStart);
extern void OsMmuL2TblInit(struct TagListObject *l2mmuFreeList, uintptr_t mmuL2AddrStart, U32 mmuL2Size);

#if defined(OS_OPTION_MMU_PAGE_OPTIMIZE)
extern U32 OsMmuPageMergeOpt(U32 instID, U32 coreId, uintptr_t l1PageTbl, uintptr_t virtAddr, uintptr_t len);
#endif

#if defined(OS_OPTION_AC)
extern void OsMmuAuthoritySet(U32 coreId);
#if (!defined OS_OPTION_LP_PRELINK)
extern bool OsMmuAuthorityCheck(U32 destCore);
#else
extern bool OsMmuAuthorityCheck(U32 srcCore, U32 destCore);
#endif
#endif

extern void OsMmuBaseSet(U32 coreId, enum OsSysMode mode, uintptr_t baseAddr);

extern void OsMmuTblInfoSet(uintptr_t l1TlbBase, uintptr_t l2TlbAddrStart, U32 l2TlbSize, uintptr_t v2pOffset);
extern void OsMmuL2TlbOrdSet(U32 coreId, U32 ordValue);
extern bool OsMmuIsEnabled(U32 coreId);
extern uintptr_t OsMmuMegabytesGetFreeVa(uintptr_t startVa, uintptr_t endVa, U32 megabytes);
extern void OsMmuMegabytesAssign(uintptr_t va, uintptr_t pa, uintptr_t size);
extern void OsMmuMegabytesUnAssign(uintptr_t va, uintptr_t size);
extern U32 OsMmuL2PageUsrAddrClr(uintptr_t pageTblAddr, U32 pageTblLen,
                                 uintptr_t *usrL2TblAddrStart, U32 *usrL2TblRemSize);

/*
 * 模块间typedef声明
 */
/*
 * 模块间全局变量声明
 */
extern uintptr_t g_mmuL2TBLAddrStart;
extern U32 g_mmuL2TBLRecSize;
extern U32 g_msgThreadId;
extern U32 g_mmuV2PSize;
#if defined(OS_OPTION_SREMNG)
extern struct TagListObject *g_l2mmuFreeList;
#else
extern struct TagListObject g_l2mmuFreeList;
#endif
#if defined(OS_OPTION_BOOT_DIRECT)
extern uintptr_t g_mmuPageTblAddr[OS_MAX_CORE_NUM];
extern U32 g_mmuPageTblSize[OS_MAX_CORE_NUM];
extern uintptr_t g_mmuP2vAddr[OS_MAX_CORE_NUM];
extern U32 g_mmuP2vSize[OS_MAX_CORE_NUM];
#endif

OS_SEC_ALW_INLINE INLINE U32 OsMmuGetRmb(uintptr_t value)
{
    U32 index;
    for (index = OS_MMU_4K_SHIFT; index <= OS_LMB32; index++) {
        // 消除汇编器优化为LCONTT/LCONTF指令，该指令存在绝对跳转，对实模式启动若存在虚实不一致就会有问题
        NOP1();

        if ((value & (1UL << index)) != 0) {
            break;
        }
    }

    if (index == (OS_LMB32 + 1)) {
        index = OS_LMB32;
    }
    return index;
}

OS_SEC_ALW_INLINE INLINE U32 OsMmuGetLmb(U32 value)
{
    U32 index;
    for (index = OS_LMB32; index >= OS_MMU_4K_SHIFT; index--) {
        // 消除汇编器优化为LCONTT/LCONTF指令，该指令存在绝对跳转，对实模式启动若存在虚实不一致就会有问题
        NOP1();

        if ((value & (1UL << index)) != 0) {
            break;
        }
    }

    if (index == (OS_MMU_4K_SHIFT - 1)) {
        index++;
    }
    return index;
}

#if defined(OS_OPTION_POWERGATE)
/*
 * 描述: MMU 页表基地址初始化
 */
OS_SEC_ALW_INLINE INLINE void OsMmuBaseSetTiny(U32 coreId, U32 baseAddr)
{
    U32 hwThreadID = OS_HWTHRD_REG_SEC_OFFSET(coreId);
    uintptr_t mmuRegBase = OS_GET_MMU_REG_GLOBAL_ADDR(coreId);
    U32 *mmuSBaseReg = (U32 *)(mmuRegBase + MMU_SBASE_OFFSET(hwThreadID));

    /* 内核态系统模式 */
    *mmuSBaseReg = baseAddr;
}
#endif

/*
 * 描述: 映射虚拟地址到物理地址。注意该osMmuMap代码请小心修改(实模式下调用保证不产生"j@pcu")
 */
OS_SEC_ALW_INLINE INLINE U32 OsMmuMapInline(uintptr_t transTblAddr, uintptr_t virStart, uintptr_t phyStart,
                                            enum MmuPageSize pageSize, U32 attr, uintptr_t *l2TlbAddrStart,
                                            U32 *l2TlbSize)
{
    uintptr_t tmp;
    U32 writeTimes;
    uintptr_t *pageCursor = (uintptr_t *)transTblAddr;
    /* 4k, 8k, 16k, 64k, 256k, 1M, 4M, 16M */
    U32 writeCountMap[MMU_PAGE_TYPE_NUM] = { 1, 2, 4, 16, 64, 1, 4, 16 };
    uintptr_t phyItem = phyStart;

    if (pageSize >= MMU_PAGE_1M) {  // leve11 page tabel
        // 与已经映射空间重叠检测
        writeTimes = writeCountMap[pageSize];
        phyItem = OS_MMU_GET_L1_PT_ADDR(phyStart);
        // 页表结构:bit[31,20]物理地址 bit[19,12]无效  bit[11,4]属性  bit[3,1]页大小 bit[0]页表使能位
        tmp = OsMmuMakePageEntry(phyItem, attr, pageSize);
        while (writeTimes-- > 0) {
            // 消除汇编器优化为LCONTT/LCONTF指令，该指令存在绝对跳转，对实模式启动若存在虚实不一致就会有问题
            NOP1();

            *(pageCursor + OS_MMU_GET_L1_PT_IDX_BY_VADDR(virStart) + writeTimes) = tmp;
        }
    } else {
        uintptr_t lvl1Off;
        uintptr_t lvl2Off;

        writeTimes = writeCountMap[pageSize];

        // 1表示为二级页表，且已配置
        if (((*(pageCursor + OS_MMU_GET_L1_PT_IDX_BY_VADDR(virStart))) & OS_MMU_L2_EN_MASK) != OS_MMU_POINT_TO_L2_EN) {
            // 该处要修改核内全局变量，需要关中断保护
            uintptr_t intSaved = OsIntLock();

            if (*l2TlbSize < OS_MMU_L2_PT_SIZE) {
                OsIntRestore(intSaved);
                return OS_ERRNO_MMU_SIZE_ERROR;
            }
            // 配置完成后二级页表空间减少1K
            *l2TlbSize = *l2TlbSize - OS_MMU_L2_PT_SIZE;
            tmp = OS_MMU_GET_L2_PT_ADDR(*l2TlbAddrStart);
            // 配置完成后二级页表起始地址增加1K
            *l2TlbAddrStart = *l2TlbAddrStart + OS_MMU_L2_PT_SIZE;

            // 如果需要映射二级时一页表结构:bit[31,10]对应的二级页表地址 bit[9,4]无效 bit[3,1]0b000  bit[0]页表使能位
            // 配置一级页表的link
            *(pageCursor + OS_MMU_GET_L1_PT_IDX_BY_VADDR(virStart)) = OsMmuMakeEntryToL2Tbl(tmp);

            OsIntRestore(intSaved);
        }
        lvl1Off = OS_MMU_GET_L2_PT_ADDR(*(pageCursor + OS_MMU_GET_L1_PT_IDX_BY_VADDR(virStart)));
        /* virtStart & 0xfffff为配置的虚地址的低20位，即1M范围内的需要映射在L2 TLB */
        lvl2Off = OS_MMU_GET_L2_PT_OFFSET(virStart);
        while (writeTimes-- > 0) {
            // 消除汇编器优化为LCONTT/LCONTF指令，该指令存在绝对跳转，对实模式启动若存在虚实不一致就会有问题
            NOP1();

            // 页表结构:bit[31,20]物理地址 bit[19,12]无效  bit[11,4]属性  bit[3,1]页大小 bit[0]页表使能位
            *(U32 *)(lvl1Off + lvl2Off + OS_MMU_PT_STRUCT_SIZE * writeTimes) = OsMmuMakePageEntry(phyItem, attr,
                                                                                                  pageSize);
        }
    }

    return SRE_OK;
}

/*
 * 描述: MMU分配
 * 备注: 该函数必须inline，以确保实模式下被调用不产生绝对跳转指令，且函数内部实现也不能产生绝对跳转指令
 */
OS_SEC_ALW_INLINE INLINE U32 OsMmuAssignInline(uintptr_t transTblAddr, uintptr_t virtStart, uintptr_t phyStart,
                                               U32 len, U32 attr, uintptr_t *l2TlbAddrStart, U32 *l2TlbSize)
{
    U32 size;
    U32 mapType;
    U32 vSize;
    U32 vMapType;
    U32 phySize;
    U32 phyMapType;
    uintptr_t virAddr = virtStart;
    uintptr_t phyAddr = phyStart;
    U32 mapLen = len;
    U32 ret = SRE_OK;
    /*
     * 对齐大小（0 ~ 31bit）到页表大小的映射表，对齐大小已保证最小4K对齐，所以从bit12开始
     * 0 ~ 7对应（4k, 8k, 16k, 64k, 256k, 1M, 4M, 16M）
     */
    U8 optimSize[32] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0, 1, 2, 2,
        3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7
    };

    /* 4k, 8k, 16k, 64k, 256k, 1M, 4M, 16M */
    U32 pageMap[MMU_PAGE_TYPE_NUM] = PAGE_SIZE_COLLECTION;

    /* 循环映射整块地址 */
    while (mapLen != 0) {
        // 消除汇编器优化为LCONTT/LCONTF指令，该指令存在绝对跳转，对实模式启动若存在虚实不一致就会有问题
        NOP1();

        /* 虚拟地址由于最小为4K，所以从12bit开始找对应的页表大小 */
        vMapType = optimSize[OsMmuGetRmb(virAddr)];
        vSize = pageMap[vMapType];

        /* 物理地址由于最小为4K，所以从12bit开始找对应的页表大小 */
        phyMapType = optimSize[OsMmuGetRmb(phyAddr)];
        phySize = pageMap[phyMapType];

        /* 找出物理地址和虚拟地址中较小的页映射，保证映射的页地址对齐 */
        if (vSize > phySize) {
            mapType = phyMapType;
            size = phySize;
        } else {
            mapType = vMapType;
            size = vSize;
        }

        /* 如果映射长度小于根据地址算出的映射长度，按映射长度映射 */
        if (size > mapLen) {
            mapType = optimSize[OsMmuGetLmb(mapLen)];
            size = pageMap[mapType];
        }

        ret = OsMmuMapInline(transTblAddr, virAddr, phyAddr, (enum MmuPageSize)mapType, attr,
                             l2TlbAddrStart, l2TlbSize);
        if (ret != SRE_OK) {
            return ret;
        }

        virAddr += (uintptr_t)size;
        phyAddr += (uintptr_t)size;
        mapLen -= size;
    }

    /* 防止产生"j@pcu"指令, 直接j到函数最后面的"retrr@pcu" ,实模式下调用时需要注意 */
    return ret;
}

#endif /* SRE_MMU_EXTERNAL_H */
