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
#include "mmu.h"
#include "hi309x_memmap.h"
#include "bm_common.h"
#include "bm_ops.h"
#include "bm_mmu.h"

#define ULONG_MAX ((unsigned long long)(~0L))

volatile unsigned long long g_tcr;

static const mmu_map_region_s g_mem_map_info[] = {
    {
        .virt  = MMU_GIC_ADDR,
        .phys  = MMU_GIC_ADDR,
        .size  = 0x1000000,
        .max_level = 0x2,
        .attrs = MMU_ATTR_DEVICE_NGNRNE | MMU_ACCESS_RWX,
    }, {
        .virt  = MMU_DRIVER_ADDR,
        .phys  = MMU_DRIVER_ADDR,
        .size  = 0x17FFFF,
        .max_level = 0x2,
        .attrs = MMU_ATTR_DEVICE_NGNRNE | MMU_ACCESS_RWX,
    }, {
        .virt  = 0x0c000000,
        .phys  = 0x0c000000,
        .size  = 0x00ffffff,
        .max_level = 0x2,
        .attrs = MMU_ATTR_DEVICE_NGNRNE | MMU_ACCESS_RWX,
    }, {
        .virt  = MMU_LOCALBUS_MEM_ADDR,
        .phys  = MMU_LOCALBUS_MEM_ADDR,
        .size  = MMU_LOCALBUS_MEM_SIZE,
        .max_level = 0x2,
        .attrs = MMU_ATTR_DEVICE_NGNRNE | MMU_ACCESS_RWX,
    }, {
        .virt      = MMU_DDR_ADDR, /* DDR */
        .phys      = MMU_DDR_ADDR,
        .size      = MMU_DDR_SIZE,
        .max_level = 0x2,
        .attrs     = MMU_ATTR_CACHE_SHARE | MMU_ACCESS_RWX,
    }, {
        .virt      = MMU_OPENAMP_ADDR, /* DDR */
        .phys      = MMU_OPENAMP_ADDR,
        .size      = MMU_OPENAMP_ADDR_SIZE,
        .max_level = 0x2,
        .attrs     = MMU_ATTR_UNCACHE_UNSHARE | MMU_ACCESS_RW,
    }
};

static mmu_ctrl_s g_mmu_ctrl = { 0 };

/*
 * with 4k page granule, a virtual addr is split into 4 lookup parts
 * spanning 9 bits each
 *
 * _______________________________________________
 * |       |       |       |       |       |       |
 * |   0   |  lv0  |  lv1  |  lv2  |  lv3  |  off  |
 * |_______|_______|_______|_______|_______|_______|
 * 63-48   47-39   38-30   29-21   20-12   11-00
 *
 * mask        page size
 *
 * lv0: FF8000000000       --
 * lv1:   7FC0000000       1G
 * lv2:     3FE00000       2M
 * lv3:       1FF000       4K
 * off:          FFF
 *
 * with 64k page granule, a virtual addr is split into 4 lookup parts
 * spanning 9 bits each
 *
 * _______________________________________________
 * |       |       |       |       |       |       |
 * |   0   |   0   |  lv1  |  lv2  |  lv3  |  off  |
 * |_______|_______|_______|_______|_______|_______|
 * 63-48   63-48   47-42   41-29   28-16   15-00
 *
 * mask        page size
 *
 * lv1: FC0000000000       --
 * lv2:  3FFE0000000       512M
 * lv3:     1FFF0000       64K
 * off:         FFFF
 */
static unsigned long long bm_mmu_get_tcr(unsigned int *pips, unsigned int *pva_bits)
{
    unsigned long long max_addr = 0;
    unsigned long long ips;
    unsigned long long va_bits;
    unsigned long long tcr;
    unsigned int i;
    unsigned int mmu_table_mum = sizeof(g_mem_map_info) / sizeof(mmu_map_region_s);

    /* find the largest addr we need to support */
    for (i = 0; i < mmu_table_mum; i++) {
        if (g_mem_map_info[i].virt <= ULONG_MAX - g_mem_map_info[i].size) {
            max_addr = max(max_addr, g_mem_map_info[i].virt + g_mem_map_info[i].size);
        }
    }

    /* calculate the maximum physical (and thus virtual) addr */
    if (max_addr > (1ULL << MMU_BITS_44)) {
        /* physical addr size, 5: 48 bits, 256TB. */
        ips = MMU_PHY_ADDR_LEVEL_5;
        va_bits = MMU_BITS_48;
    } else if (max_addr > (1ULL << MMU_BITS_42)) {
        /* physical addr size, 4: 44 bits, 16TB. */
        ips = MMU_PHY_ADDR_LEVEL_4;
        va_bits = MMU_BITS_44;
    } else if (max_addr > (1ULL << MMU_BITS_40)) {
        /* physical addr size, 3: 42 bits, 4TB. */
        ips = MMU_PHY_ADDR_LEVEL_3;
        va_bits = MMU_BITS_42;
    } else if (max_addr > (1ULL << MMU_BITS_36)) {
        /* physical addr size, 2: 40 bits, 1TB. */
        ips = MMU_PHY_ADDR_LEVEL_2;
        va_bits = MMU_BITS_40;
    } else if (max_addr > (1ULL << MMU_BITS_32)) {
        /* physical addr size, 1: 36 bits, 64GB. */
        ips = MMU_PHY_ADDR_LEVEL_1;
        va_bits = MMU_BITS_36;
    } else {
        /* physical addr size, 0: 32 bits, 4GB. */
        ips = MMU_PHY_ADDR_LEVEL_0;
        va_bits = MMU_BITS_32;
    }

    tcr = TCR_EL1_RSVD | TCR_IPS(ips);

    if (g_mmu_ctrl.granule == MMU_GRANULE_4K) {
        /* PTWs cacheable, inner/outer WBWA and inner shareable */
        tcr |= TCR_TG0_4K | TCR_SHARED_INNER | TCR_ORGN_WBWA | TCR_IRGN_WBWA;
    } else {
        /* PTWs cacheable, inner/outer WBWA and inner shareable */
        tcr |= TCR_TG0_64K | TCR_SHARED_INNER | TCR_ORGN_WBWA | TCR_IRGN_WBWA;
    }

    tcr |= TCR_T0SZ(va_bits);

    if (pips != NULL) {
        *pips = ips;
    }

    if (pva_bits != NULL) {
        *pva_bits = va_bits;
    }

    return tcr;
}

static unsigned int bm_mmu_get_pte_type(unsigned long long const * pte)
{
    return (unsigned int)(*pte & PTE_TYPE_MASK);
}

/* returns the LSB number for a PTE on level <level> */
static unsigned int bm_mmu_level2shift(unsigned int level)
{
    /* 4K:  page is 12 bits wide, 9 bits per level */
    /* 64K: page is 16 bits wide, 13 bits per level */
    if (g_mmu_ctrl.granule == MMU_GRANULE_4K) {
        return (unsigned int)(MMU_BITS_12 + MMU_BITS_9 * (MMU_LEVLE_3 - level));
    } else {
        return (unsigned int)(MMU_BITS_16 + MMU_BITS_13 * (MMU_LEVLE_3 - level));
    }
}

static unsigned long long *bm_mmu_find_pte(unsigned long long addr, unsigned int level)
{
    unsigned long long *pte = NULL;
    unsigned long long idx;
    unsigned int i;

    if (level < g_mmu_ctrl.start_level) {
        return NULL;
    }

    /* walk through all page table levels to find our PTE */
    pte = (unsigned long long *)g_mmu_ctrl.tlb_addr;

    for (i = g_mmu_ctrl.start_level; i < MMU_LEVEL_MAX; i++) {
        if (g_mmu_ctrl.granule == MMU_GRANULE_4K) {
            idx = (addr >> bm_mmu_level2shift(i)) & 0x1FF;
        } else {
            idx = (addr >> bm_mmu_level2shift(i)) & 0x1FFF;
        }

        pte += idx;

        /* found it */
        if (i == level) {
            return pte;
        }

        /* PTE is no table (either invalid or block), can't traverse */
        if (bm_mmu_get_pte_type(pte) != PTE_TYPE_TABLE) {
            return NULL;
        }

        /* off to the next level */
        if (g_mmu_ctrl.granule == MMU_GRANULE_4K) {
            pte = (unsigned long long *)(*pte & PTE_TABLE_ADDR_MARK_4K);
        } else {
            pte = (unsigned long long *)(*pte & PTE_TABLE_ADDR_MARK_64K);
        }
    }

    /* should never reach here */
    return NULL;
}

/* returns and creates a new full table (512 entries) */
static unsigned long long *bm_mmu_create_table(void)
{
    unsigned int pt_len;
    unsigned int i = 0;
    unsigned long long *new_table = (unsigned long long *)g_mmu_ctrl.tlb_fillptr;

    if (g_mmu_ctrl.granule == MMU_GRANULE_4K) {
        pt_len = MAX_PTE_ENTRIES_4K * sizeof(unsigned long long);
    } else {
        pt_len = MAX_PTE_ENTRIES_64K * sizeof(unsigned long long);
    }

    /* allocate MAX_PTE_ENTRIES pte entries */
    g_mmu_ctrl.tlb_fillptr += pt_len;

    if (g_mmu_ctrl.tlb_fillptr - g_mmu_ctrl.tlb_addr > g_mmu_ctrl.tlb_size) {
        return NULL;
    }

    /* mark all entries as invalid */
    while (i < pt_len) {
        *(unsigned char *)((unsigned char *)new_table + i) = 0;
        i++;
    }

    return new_table;
}

static void bm_mmu_set_pte_table(unsigned long long *pte, unsigned long long *table)
{
    /* point *pte to the new table */
    *pte = PTE_TYPE_TABLE | (unsigned long long)(uintptr_t)table;
}

static int bm_mmu_add_map_pte_process(mmu_map_region_s const * map, unsigned long long *pte, unsigned long long phys,
    unsigned int level)
{
    unsigned long long *new_table = NULL;

    if (level < map->max_level) {
        if (bm_mmu_get_pte_type(pte) == PTE_TYPE_FAULT) {
            new_table = bm_mmu_create_table(); /* page doesn't fit, create subpages */
            if (new_table == NULL) {
                return BM_FAIL;
            }
            bm_mmu_set_pte_table(pte, new_table);
        }
    } else if (level == MMU_LEVLE_3) { /* According to the protocol requirements, level 3 cannot be set to BLOCK. */
        *pte = phys | map->attrs | PTE_TYPE_PAGE;
    } else {
        *pte = phys | map->attrs | PTE_TYPE_BLOCK;
    }

    return BM_OK;
}

static int bm_mmu_add_map(mmu_map_region_s const * map)
{
    unsigned long long virt = map->virt;
    unsigned long long phys = map->phys;
    unsigned long long max_level = map->max_level;
    unsigned long long start_level = g_mmu_ctrl.start_level;
    unsigned long long blocksize = 0;
    unsigned long long map_size = 0;
    unsigned int level;
    unsigned long long *pte = NULL;
    int ret;

    if (map->max_level <= start_level) { /* At least two levels of mapping for 4K pages */
        return BM_FAIL;
    }

    while (map_size < map->size) {
        for (level = start_level; level <= max_level; level++) {
            pte = bm_mmu_find_pte(virt, level);
            if (pte == NULL) {
                return BM_FAIL;
            }

            ret = bm_mmu_add_map_pte_process(map, pte, phys, level);
            if (ret) {
                return ret;
            }

            if (level != start_level) { /* Level 0 cannot be set to BLOCK, special processing */
                blocksize = 1ULL << bm_mmu_level2shift(level);
            }
        }

        virt += blocksize;
        phys += blocksize;
        map_size += blocksize;
    }

    return BM_OK;
}

static inline void bm_mmu_set_ttbr_tcr_mair(unsigned long long table, unsigned long long tcr, unsigned long long attr)
{
    asm volatile("dsb sy");

    asm volatile("msr ttbr0_el1, %0" : : "r"(table) : "memory");
    asm volatile("msr ttbr1_el1, %0" : : "r"(table) : "memory");
    asm volatile("msr tcr_el1, %0" : : "r"(tcr) : "memory");
    asm volatile("msr mair_el1, %0" : : "r"(attr) : "memory");

    asm volatile("isb");
}

static unsigned int bm_mmu_setup_pgtables(const mmu_map_region_s *mem_map, unsigned int mem_region_num,
    unsigned long long tlb_addr, unsigned long long tlb_len, unsigned int granule)
{
    unsigned int i;
    int ret;
    unsigned long long *new_table = NULL;

    g_mmu_ctrl.tlb_addr = tlb_addr;
    g_mmu_ctrl.tlb_size = tlb_len;
    g_mmu_ctrl.tlb_fillptr = tlb_addr;
    g_mmu_ctrl.granule = granule;
    g_mmu_ctrl.start_level = 0;

    g_tcr = bm_mmu_get_tcr(NULL, &g_mmu_ctrl.va_bits);
    bm_writel(g_tcr, CPU_RELEASE_MMU_TCR);

    if (g_mmu_ctrl.granule == MMU_GRANULE_4K) {
        /* 4K: page is 12 bits wide, A maximum of four levels are supported. */
        /* Each level converts 9 bits. If the value is less than 39, start from the first level page.. */
        if (g_mmu_ctrl.va_bits < MMU_BITS_39) {
            g_mmu_ctrl.start_level = MMU_LEVLE_1;
        } else {
            g_mmu_ctrl.start_level = MMU_LEVLE_0;
        }
    } else {
        /* 64K: page is 16 bits wide, A maximum of three levels are supported. */
        /* Each level converts 13 bits. If the value is less than 36, start from the second level page.. */
        if (g_mmu_ctrl.va_bits <= MMU_BITS_36) {
            g_mmu_ctrl.start_level = MMU_LEVLE_2;
        } else {
            g_mmu_ctrl.start_level = MMU_LEVLE_1;
            return BM_FAIL;
        }
    }

    /* Create a top-level page table */
    new_table = bm_mmu_create_table();
    if (new_table == NULL) {
        return BM_FAIL;
    }

    /* Create a subsequent page table */
    for (i = 0; i < mem_region_num; i++) {
        ret = bm_mmu_add_map(&mem_map[i]);
        if (ret) {
            return ret;
        }
    }

    return BM_OK;
}

static int bm_mmu_setup(void)
{
    int ret;
    unsigned long long page_addr;
    unsigned long long page_len;

    page_addr = MMU_PAGE_BEGIN;
    page_len = MMU_PAGE_END - MMU_PAGE_BEGIN;
    bm_writel(page_addr, CPU_RELEASE_MMU_PAGE);
    /* Configuring pages at different levels */
    ret = bm_mmu_setup_pgtables(g_mem_map_info, (sizeof(g_mem_map_info) / sizeof(mmu_map_region_s)), page_addr,
        page_len, MMU_GRANULE_4K);

    return ret;
}

/* MMU initialization main entry */
static int bm_mmu_init(void)
{
    /* Enable the MMU. */
    return bm_mmu_setup();
}

int bm_enable_caches(void)
{
    int ret = 0;

    if (!(bm_get_sctlr() & CR_M) && (bm_readl(CPU_RELEASE_BOOT_SYNC) != BOOT_SYNC_READY)) {
        ret = bm_mmu_init(); // Master Core Initialization
        if (ret) {
            return ret;
        }
    }

    bm_mmu_set_ttbr_tcr_mair(bm_readl(CPU_RELEASE_MMU_PAGE), bm_readl(CPU_RELEASE_MMU_TCR), MEMORY_ATTRIBUTES);

    /* Invalid caches. */
    bm_asm_invalidate_dcache_all();
    bm_asm_invalidate_icache_all();
    bm_asm_invalidate_tlb_all();

    /* Enabled icache、dcache、mmu */
    bm_set_sctlr(bm_get_sctlr() | CR_C | CR_M | CR_I); // Both primary and secondary cores are performed.

    return BM_OK;
}

int bm_disable_caches(void)
{
    uintptr_t sctlr;

    sctlr = bm_get_sctlr();
    if (!(sctlr & CR_M)) {
        /* if cache isn't enabled no need to disable */
        return BM_FAIL;
    }

    /* Disable icache、dcache、mmu */
    bm_set_sctlr(sctlr & ~(CR_C | CR_I));

    /* flush caches and becomes invalid. */
    bm_asm_flush_dcache_all();
    bm_asm_invalidate_icache_all();
    bm_asm_invalidate_tlb_all();

    return BM_OK;
}

int bm_disable_mmu(void);
int bm_disable_mmu(void)
{
    uintptr_t sctlr;

    sctlr = bm_get_sctlr();
    if (!(sctlr & CR_M)) {
        /* if cache isn't enabled no need to disable */
        return BM_FAIL;
    }

    /* Disable icache、dcache、mmu */
    bm_set_sctlr(sctlr & ~(CR_C | CR_I | CR_M));

    /* flush caches and becomes invalid. */
    bm_asm_flush_dcache_all();
    bm_asm_invalidate_icache_all();
    bm_asm_invalidate_tlb_all();

    return BM_OK;
}