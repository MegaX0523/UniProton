/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 * UniProton is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 * 	http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * See the Mulan PSL v2 for more details.
 */

#ifndef __MMU_H__
#define __MMU_H__

/*
 * SCTLR_EL1 bits definitions
 */
#define CR_M                (1 << 0)  /* MMU enable */
#define CR_C                (1 << 2)  /* dcache enable */
#define CR_I                (1 << 12) /* icache enable */

/***************************************************************/
/*
 * memory types
 */
#define MT_DEVICE_NGNRNE    0
#define MT_DEVICE_NGNRE     1
#define MT_DEVICE_GRE       2
#define MT_NORMAL_NC        3
#define MT_NORMAL           4

#define MEMORY_ATTRIBUTES   ((0x00 << (MT_DEVICE_NGNRNE * 8)) | \
                             (0x04 << (MT_DEVICE_NGNRE * 8))   | \
                             (0x0c << (MT_DEVICE_GRE * 8))     | \
                             (0x44 << (MT_NORMAL_NC * 8))      | \
                             (0xffUL << (MT_NORMAL * 8)))

/*
 * hardware page table definitions.
 *
 */
#define PTE_TYPE_MASK           (3 << 0)
#define PTE_TYPE_FAULT          (0 << 0)
#define PTE_TYPE_TABLE          (3 << 0)
#define PTE_TYPE_PAGE           (3 << 0)
#define PTE_TYPE_BLOCK          (1 << 0)
#define PTE_TYPE_VALID          (1 << 0)

#define PTE_TABLE_PXN           (1UL << 59)
#define PTE_TABLE_XN            (1UL << 60)
#define PTE_TABLE_AP            (1UL << 61)
#define PTE_TABLE_NS            (1UL << 63)

/*
 * block
 */
#define PTE_BLOCK_MEMTYPE(x)    ((x) << 2)
#define PTE_BLOCK_NS            (1 << 5)
#define PTE_BLOCK_AP_R          (2 << 6)
#define PTE_BLOCK_AP_RW         (0 << 6)
#define PTE_BLOCK_NON_SHARE     (0 << 8)
#define PTE_BLOCK_OUTER_SHARE   (2 << 8)
#define PTE_BLOCK_INNER_SHARE   (3 << 8)
#define PTE_BLOCK_AF            (1 << 10)
#define PTE_BLOCK_NG            (1 << 11)
#define PTE_BLOCK_PXN           (1UL << 53)
#define PTE_BLOCK_UXN           (1UL << 54)

/*
 * attr_indx[2:0]
 */
#define PMD_ATTRINDX(t)     ((t) << 2)
#define PMD_ATTRINDX_MASK   (7 << 2)
#define PMD_ATTRMASK        (PTE_BLOCK_PXN      | \
                            PTE_BLOCK_UXN       | \
                            PMD_ATTRINDX_MASK   | \
                            PTE_TYPE_VALID)

/*
 * TCR flags.
 */
#define TCR_T0SZ(x)         ((64 - (x)) << 0)
#define TCR_IRGN_NC         (0 << 8)
#define TCR_IRGN_WBWA       (1 << 8)
#define TCR_IRGN_WT         (2 << 8)
#define TCR_IRGN_WBNWA      (3 << 8)
#define TCR_IRGN_MASK       (3 << 8)
#define TCR_ORGN_NC         (0 << 10)
#define TCR_ORGN_WBWA       (1 << 10)
#define TCR_ORGN_WT         (2 << 10)
#define TCR_ORGN_WBNWA      (3 << 10)
#define TCR_ORGN_MASK       (3 << 10)
#define TCR_SHARED_NON      (0 << 12)
#define TCR_SHARED_OUTER    (2 << 12)
#define TCR_SHARED_INNER    (3 << 12)
#define TCR_TG0_4K          (0 << 14)
#define TCR_TG0_64K         (1 << 14)
#define TCR_TG0_16K         (2 << 14)
#define TCR_EPD1_DISABLE    (1 << 23)

#define TCR_EL1_RSVD        (1UL << 31)
#define TCR_EL2_RSVD        (1UL << 31 | 1UL << 23)
#define TCR_EL3_RSVD        (1UL << 31 | 1UL << 23)

#define TCR_IPS(x)           ((x) << 32)

#define MAX_PTE_ENTRIES_4K      512
#define MAX_PTE_ENTRIES_64K     8192

#define PTE_TABLE_ADDR_MARK_4K  0x0000FFFFFFFFF000ULL
#define PTE_TABLE_ADDR_MARK_64K 0x0000FFFFFFFF0000ULL

typedef enum {
    MMU_LEVLE_0 = 0,
    MMU_LEVLE_1,
    MMU_LEVLE_2,
    MMU_LEVLE_3,
    MMU_LEVEL_MAX
} mmu_level_e;

typedef enum {
    MMU_PHY_ADDR_LEVEL_0 = 0,   /* 32 bits, 4GB */
    MMU_PHY_ADDR_LEVEL_1,       /* 36 bits, 64GB */
    MMU_PHY_ADDR_LEVEL_2,       /* 40 bits, 1TB */
    MMU_PHY_ADDR_LEVEL_3,       /* 42 bits, 4TB */
    MMU_PHY_ADDR_LEVEL_4,       /* 44 bits, 16TB */
    MMU_PHY_ADDR_LEVEL_5        /* 48 bits, 256TB */
} mmu_physical_addr_size_e;

typedef enum {
    MMU_BITS_9 = 9,
    MMU_BITS_12 = 12,
    MMU_BITS_13 = 13,
    MMU_BITS_16 = 16,
    MMU_BITS_32 = 32,
    MMU_BITS_36 = 36,
    MMU_BITS_39 = 39,
    MMU_BITS_40 = 40,
    MMU_BITS_42 = 42,
    MMU_BITS_44 = 44,
    MMU_BITS_48 = 48
} mmu_bits_e;

typedef struct {
    unsigned long long tlb_addr;
    unsigned long long tlb_size;
    unsigned long long tlb_fillptr;        // Pointer to the current entry
    unsigned int granule;
    unsigned int start_level;
    unsigned int va_bits;
} mmu_ctrl_s;

/*
Attr<n>[7:4]        meaning
0000                device memory. see encoding of attr<n>[3:0] for the type of device memory.
00RW, RW not 00     normal memory, outer write-Through transient
0100                normal memory, outer non-cacheable
01RW, RW not 00     normal memory, outer write-Back transient
10RW                normal memory, outer write-Through non-transient
11RW                normal memory, outer write-Back non-transient
      R = outer read-Allocate policy, W = outer write-Allocate policy.

Attr<n>[3:0]       meaning when attr<n>[7:4] is 0000        meaning when attr<n>[7:4] is not 0000
0000               device-n_gnRnE memory       UNPREDICTABLE
00RW, RW not 00    UNPREDICTABLE              normal memory, inner write-Through transient
0100               device-n_gnRE memory        normal memory, inner non-cacheable
01RW, RW not 00    UNPREDICTABLE              normal memory, inner write-Back transient
1000               device-nGRE memory         normal memory, inner write-Through non-transient (RW=00)
10RW, RW not 00    UNPREDICTABLE              normal memory, inner write-Through non-transient
1100               device-GRE memory          normal memory, inner write-Back non-transient (RW=00)
11RW, RW not 00    UNPREDICTABLE              normal memory, inner write-Back non-transient
*/

/* User-visible configuration */
#define MMU_ATTR_DEVICE_NGNRNE      (PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE))
#define MMU_ATTR_DEVICE             (PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRE))
#define MMU_ATTR_UNCACHE_UNSHARE    (PTE_BLOCK_MEMTYPE(MT_NORMAL_NC) | PTE_BLOCK_NON_SHARE)
#define MMU_ATTR_UNCACHE_SHARE      (PTE_BLOCK_MEMTYPE(MT_NORMAL_NC) | PTE_BLOCK_INNER_SHARE)
#define MMU_ATTR_CACHE_UNSHARE      (PTE_BLOCK_MEMTYPE(MT_NORMAL)    | PTE_BLOCK_NON_SHARE)
#define MMU_ATTR_CACHE_SHARE        (PTE_BLOCK_MEMTYPE(MT_NORMAL)    | PTE_BLOCK_INNER_SHARE)
#define MMU_ATTR_MASK               0x31CULL

#define MMU_ACCESS_NONE  (PTE_BLOCK_AP_RW)  /* PTE_BLOCK_AF is not set and cannot be accessed or executed. */
#define MMU_ACCESS_R     (PTE_BLOCK_AF | PTE_BLOCK_UXN | PTE_BLOCK_PXN | PTE_BLOCK_AP_R)  /* Read-only */
#define MMU_ACCESS_RW    (PTE_BLOCK_AF | PTE_BLOCK_UXN | PTE_BLOCK_PXN | PTE_BLOCK_AP_RW) /* Read-writable */
#define MMU_ACCESS_RWX   (PTE_BLOCK_AF | PTE_BLOCK_AP_RW)    /* Readable, writable, and executable */
#define MMU_ACCESS_RX    (PTE_BLOCK_AF | PTE_BLOCK_AP_R)     /* Readable and executable */
#define MMU_ACCESS_MASK  0x600000000004C0ULL

#define MMU_GRANULE_4K   0
#define MMU_GRANULE_64K  1

typedef struct {
    unsigned long long virt;
    unsigned long long phys;
    unsigned long long size;
    unsigned long long max_level;
    unsigned long long attrs;
} mmu_map_region_s;

static inline unsigned long bm_get_sctlr(void)
{
    unsigned long val;

    __asm__ __volatile__("mrs %0, sctlr_el1" : "=r" (val) : : "cc");

    return val;
}

static inline void bm_set_sctlr(unsigned long val)
{
    __asm__ __volatile__("dsb sy");
    __asm__ __volatile__("msr sctlr_el1, %0" : : "r" (val) : "cc");
    __asm__ __volatile__("dsb sy");
    __asm__ __volatile__("isb");
}

void bm_asm_invalidate_dcache_all(void);
void bm_asm_invalidate_dcache_range(unsigned long long start, unsigned long long end);
void bm_asm_flush_dcache_all(void);
void bm_asm_flush_dcache_range(unsigned long long start, unsigned long long end);
void bm_asm_invalidate_icache_all(void);
void bm_asm_invalidate_tlb_all(void);

#endif /* __MMU_H__ */