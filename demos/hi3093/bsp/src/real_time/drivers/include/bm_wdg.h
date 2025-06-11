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
#ifndef __BM_WDG_H__
#define __BM_WDG_H__

typedef enum {
    WDG_ID_0 = 0,
    WDG_ID_1,
    WDG_ID_2,
    WDG_ID_3,
    WDG_ID_4,
    WDG_ID_BUTT,
} bm_wdg_ids;

#define WDG_FRQ 25000000
#define WDG_BASE_IRQ 136

/**
 * @brief  typedef callback function of wdg
 */
typedef void (*wdg_callback_func)(bm_wdg_ids id);

/**
 * @brief  time base addr and configuration structure definition
 */
typedef struct {
    bm_wdg_ids id;                  /* *<wdg id */
    unsigned int load;              /* *< 1s = WDG_FRQ */
    wdg_callback_func callback;     /* *< callback function of wdg */
} bm_wdg_cfg;

/**
 * @brief   init the wdg
 * @param   wdg cfg
 * @retval  BM_OK    success
 * @retval  BM_FAIL parameter check fail
 */
int bm_wdg_init(const bm_wdg_cfg *cfg);

/**
 * @brief   deinit the wdg
 * @param   wdg id
 * @retval  BM_OK    success
 * @retval  BM_FAIL parameter check fail
 */
int bm_wdg_deinit(bm_wdg_ids id);

/**
 * @brief   start wdg.
 * @param   wdg id
 * @retval  BM_OK    success
 * @retval  BM_FAIL parameter check fail
 */
int bm_wdg_enable(bm_wdg_ids id);

/**
 * @brief   stop wdg.
 * @param   wdg id
 * @retval  BM_OK    success
 * @retval  BM_FAIL parameter check fail
 */
int bm_wdg_disable(bm_wdg_ids id);

/**
 * @brief   feed wdg
 * @param   wdg id
 * @retval  BM_OK    success
 * @retval  BM_FAIL parameter check fail
 */
int bm_wdg_feed(bm_wdg_ids id);

/**
 * @brief   get wdg current count value
 * @in param   wdg id
 * @out param  wdg cnt value
 * @retval  BM_OK    success
 * @retval  BM_FAIL parameter check fail
 */
int bm_wdg_get_cnt_value(bm_wdg_ids id, unsigned int *val);

#endif /* __BM_WDG_H__ */
