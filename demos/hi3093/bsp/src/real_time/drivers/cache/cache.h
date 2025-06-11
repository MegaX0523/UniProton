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
#ifndef __CACHE_H__
#define __CACHE_H__

/**
* @defgroup bm_cache bm_cache
*/

/*
 * block/section addr mask and size definitions.
 */
#define CACHE_POS           0x2
#define CACHE_MASK          0x7
#define CACHE_ENABLE        0x4

void bm_asm_invalidate_dcache_all(void);
void bm_asm_invalidate_dcache_range(unsigned long long start, unsigned long long end);
void bm_asm_flush_dcache_all(void);
void bm_asm_flush_dcache_range(unsigned long long start, unsigned long long end);
void bm_asm_invalidate_icache_all(void);
void bm_asm_invalidate_tlb_all(void);

#endif /* __CACHE_H__ */