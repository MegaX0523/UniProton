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

#include "cache.h"
#include "bm_common.h"
#include "bm_ops.h"
#include "bm_cache.h"

#if defined (__UNIPROTON__)
#define MMU_DDR_ADDR               0x93000000ULL
#define MMU_DDR_SIZE               0x7FFFFFFFULL
#else
#include "hi309x_memmap.h"
#endif

void bm_invalidate_dcache_all(void)
{
    bm_asm_invalidate_dcache_all();
}

void bm_invalidate_dcache_range(unsigned long long start, unsigned int size)
{
    if ((start > (MMU_DDR_ADDR + MMU_DDR_SIZE)) || (start < MMU_DDR_ADDR)) {
        return;
    }

    unsigned long long end = start + size;
    if ((end > (MMU_DDR_ADDR + MMU_DDR_SIZE)) || (end < MMU_DDR_ADDR) || (end <= start)) {
        return;
    }

    bm_asm_invalidate_dcache_range(start, end);
}

void bm_flush_dcache_all(void)
{
    bm_asm_flush_dcache_all();
}

void bm_flush_dcache_range(unsigned long long start, unsigned int size)
{
    if ((start > (MMU_DDR_ADDR + MMU_DDR_SIZE)) || (start < MMU_DDR_ADDR)) {
        return;
    }

    unsigned long long end = start + size;
    if ((end > (MMU_DDR_ADDR + MMU_DDR_SIZE)) || (end < MMU_DDR_ADDR) || (end <= start)) {
        return;
    }

    bm_asm_flush_dcache_range(start, end);
}
