/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: cache模块内部头文件
 * Author: LibCK项目组
 * Create: 2009-10-5
 * History: 2018-10-5 LibCK项目组  华为规范整改
 */
#ifndef SRE_CACHE_EXTERNAL_H
#define SRE_CACHE_EXTERNAL_H

#include "sre_mmu.h"
#include "sre_cache.h"
#include "sre_exc.h"

#define OS_PCC_REG_BASE OS_PCC_REG_ADDR_BASE
#define OS_PCC_ENABLE 0X00
#define OS_PCC_STATUS_BUSY 0X20
#define OS_PCC_STATUS_INT 0X30
#define OS_PCC_SWPF 0X60
#define OS_PCC_HWPF 0X80
#define OS_PCC_HWPF_RNG_DIS 0X90
#define OS_PCC_ECC_CTL 0XA0
#define OS_PCC_SPLIT 0XC0
#define OS_PCC_BACKDBG 0XE0
#define OS_PCC_INTSTATUS0 0X100
#define OS_PCC_INTSTATUS1 0X200
#define OS_PCC_INTMASK0 0X120
#define OS_PCC_INTMASK1 0X220
#define OS_PCC_INTCLR0 0X140
#define OS_PCC_INTCLR1 0X240
#define OS_PCC_SWEADDR0 0X1A0
#define OS_PCC_SWEADDR1 0X2A0
#define OS_PCC_INVADDR_START0 0X1C0
#define OS_PCC_INVADDR_START1 0X2C0
#define OS_PCC_INVADDR_END0 0X1D0
#define OS_PCC_INVADDR_END1 0X2D0
#define OS_PCC_INVCTL0 0X1E0
#define OS_PCC_INVCTL1 0X2E0
#define OS_PCC_VERSION 0X500

#define OS_DCC_REG_BASE OS_DCC_REG_ADDR_BASE
#define OS_DCC_ENABLE 0X00
#define OS_DCC_STATUS_BUSY 0X20
#define OS_DCC_STATUS_INT 0X30
#define OS_DCC_L2_ACC_CTL 0X40
#define OS_DCC_SWPF 0X60
#define OS_DCC_HWPF 0X80
#define OS_DCC_ECC_CTL 0XA0
#define OS_DCC_BACKDBG 0XE0
#define OS_DCC_INTSTATUS0 0X100
#define OS_DCC_INTSTATUS1 0X200
#define OS_DCC_INTMASK0 0X120
#define OS_DCC_INTMASK1 0X220
#define OS_DCC_INTCLR0 0X140
#define OS_DCC_INTCLR1 0X240
#define OS_DCC_L2WERR0 0X160
#define OS_DCC_L2WERR1 0X260
#define OS_DCC_ECCADDR0 0X180
#define OS_DCC_ECCADDR1 0X280
#define OS_DCC_INVADDR_START0 0X1C0
#define OS_DCC_INVADDR_START1 0X2C0
#define OS_DCC_INVADDR_END0 0X1D0
#define OS_DCC_INVADDR_END1 0X2D0
#define OS_DCC_INVCTL0 0X1E0
#define OS_DCC_INVCTL1 0X2E0
#define OS_DCC_WBCONTEXT 0X340

#define OS_DCC_VERSION 0x500
#define DCC_INVCTL_STATUS_MASK 0x1000
#define OS_DCC_STATUS_BUSY_0_MASK 0x2
#define OS_DCC_STATUS_BUSY_1_MASK 0x4

#define OS_PCC_STATUS_BUSY_0_MASK 0x2
#define OS_PCC_STATUS_BUSY_1_MASK 0x4

#define PCC_CACHE_LINE 256
#define DCC_CACHE_LINE 128

#define OS_CACHE_ENBALE 0x01
#define OS_CACHE_DISABLE 0x00
#define OS_CACHE_REG_OFFSET 64

#define OS_PCC_LCK_OFFSET (OS_PCC_LCKSTART2 - OS_PCC_LCKSTART1)
#define OS_PCC_LCK_ENA 1
#define OS_PCC_ADDR_MASK 0xFFFFFF00U

#define OS_CACHE_IS_HWTHREAD_0(coreId) (OS_HWTHRD_REG_SEC_OFFSET(coreId) == 0)

#define OS_GET_LOCAL_REG_ADDR(base, reg) ((uintptr_t)(base) + (uintptr_t)(reg))

#define OS_ICACHE_TAG_VALID_MASK 0x00000001U /* 获取出ICACHE TAG的无效位 */
#define OS_ICACHE_VALID 0x00000001 /* 表示ICACHE有效 */
#define OS_ICACHE_TAG_LOCK_MASK 0x00000004U /* 获取出ICACHE TAG的锁存位 */
#define OS_ICACHE_LOCK 0x00000004 /* 表示ICACHE已经上锁 */

#define OS_DCC_INTSTATUS_CACHE_ERROR_BIT OS_BIT1_MASK

#define OS_CACHE_LCK1_MASK(core) (OS_BIT0_MASK << OS_PCC_LOCK_HWTHRD_BASE_OFFSET(core))
#define OS_CACHE_LCK2_MASK(core) (OS_BIT1_MASK << OS_PCC_LOCK_HWTHRD_BASE_OFFSET(core))
#define OS_CACHE_LCK3_MASK(core) (OS_BIT2_MASK << OS_PCC_LOCK_HWTHRD_BASE_OFFSET(core))
#define OS_CACHE_LCK4_MASK(core) (OS_BIT3_MASK << OS_PCC_LOCK_HWTHRD_BASE_OFFSET(core))

struct TagPccHwPf {
    U32 ena0 : 1;
    U32 swpf0 : 1;
    U32 off0 : 2;
    U32 rsv1 : 12;
    U32 ena1 : 1;
    U32 swpf1 : 1;
    U32 off1 : 2;
    U32 rsv3 : 12;
};

struct TagPccSwPf {
    U32 ena0 : 1;
    U32 rsv0 : 15;
    U32 ena1 : 1;
    U32 rsv1 : 15;
};

struct TagPccHwpfRngDis {
    U32 reserved : 16;
    U32 hwpfRngDisAddr : 8;
    U32 reserved1 : 1;
    U32 hwpfRngDisSz : 3;
    U32 reserved2 : 3;
    U32 hwpfRngDisEna : 1;
};

struct TagDccSwPf {
    U32 ena0 : 1;
    U32 rsv0 : 15;
    U32 ena1 : 1;
    U32 rsv1 : 15;
};

struct TagDccBackDbg {
    U32 ena0 : 1;
    U32 ena1 : 1;
    U32 rsv0 : 6;
    U32 we0 : 1;
    U32 we1 : 1;
    U32 rsv1 : 22;
};

#if (defined(OS_OPTION_SREMNG) && defined(OS_OPTION_LP_PRELINK))
/* IIU_BEGIN+4: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
struct MngCacheLockBitMap {
    U8 cacheLockBitMap[BOARD_CORE_NUM];
};
#endif

extern void OsDccErrSaveInfo(struct ExcInfo *excInfo);
extern bool OsLdpErrRet(uintptr_t errVAddr);
extern void OsDccErrIntClr(void);
extern void OsPccErrIntClr(void);
#if defined(OS_OPTION_DCACHE)
extern U32 OsDCacheFlushInvAll(void);
#else
OS_SEC_ALW_INLINE INLINE U32 OsDCacheFlushInvAll(void)
{
    return SRE_OK;
}
#endif
extern void OsICacheLockInit(U32 core, U32 cacheLockValue, U8 *cacheLockBitMap);

extern void OsCacheInitPrimary(U32 coreId);
extern void OsCacheEnable(U32 coreId);
extern void OsCacheFlushByLine(uintptr_t addr, U32 size);
extern void OsCacheIntMaskSet(uintptr_t base, uintptr_t reg, U32 intMask);
extern U32 OsCacheIntMaskGet(uintptr_t base, uintptr_t reg);

extern bool g_l2CacheInitFlag;
extern void OsSysL2Disable(void);
extern void OsSysL2Enable(void);
extern U32 OsL1ICacheInvAll(void);
extern U32 OsL1DCacheInvAll(void);
extern U32 OsL1DCacheFlushInvAll(void);
#if defined(OS_OPTION_SREMNG_DDR_NO_PREFETCH)
extern void OsICacheHwpf2GDisCfg(U32 baseAddr);
#endif

#if defined(OS_OPTION_POWERGATE)
extern void OsCachePwrGtRegSave(void);
extern void OsCachePwrGtRegRecovery(void);
#endif
#endif /* SRE_CACHE_EXTERNAL_H */
