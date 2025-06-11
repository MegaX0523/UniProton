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

#ifndef __BM__MMU_H__
#define __BM__MMU_H__


/**
*
* @ingroup bm_mmu
* @brief enable data cache and instruction cache.
*
* @par description:
* enable data cache and instruction cache.
*
* @param void
*
* @return BM_OK/BM_FAIL
*
*/
int bm_enable_caches(void);

/**
*
* @ingroup bm_mmu
* @brief disable data cache and instruction cache.
*
* @par description:
* disable data cache and instruction cache.
*
* @param void
*
* @return BM_OK/BM_FAIL
*
*/
int bm_disable_caches(void);

#endif