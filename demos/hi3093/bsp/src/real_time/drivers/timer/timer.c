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
#include <stdbool.h>
#include <bm_common.h>
#include "bm_timer.h"
#include "soc_osal.h"
#include "timer.h"

#define timer_param_check_with_ret(param, ret) base_func_paramcheck_with_ret(param, ret)

static const int timer_exist_array[TIMER_ID_BUTT] = {
    dt_device_exists(TIMER, TIMER0),
    dt_device_exists(TIMER, TIMER1),
    dt_device_exists(TIMER, TIMER2),
    dt_device_exists(TIMER, TIMER3),
    dt_device_exists(TIMER, TIMER4),
    dt_device_exists(TIMER, TIMER5),
};

static const unsigned long g_reg_bases[TIMER_ID_BUTT] = {
    TIMER0_REG_BASE,
    TIMER1_REG_BASE,
    TIMER2_REG_BASE,
    TIMER3_REG_BASE,
    TIMER4_REG_BASE,
    TIMER5_REG_BASE,
};

static bool is_timer_id(bm_timer_ids id)
{
    return ((id >= TIMER_ID_0) && (id < TIMER_ID_BUTT) && timer_exist_array[id]);
}

static bool is_timer_mode(timer_mode mode)
{
    return (((mode) == TIMER_MODE_RUN_FREE) || ((mode) == TIMER_MODE_RUN_PERIODIC) ||
        ((mode) == TIMER_MODE_RUN_ONTSHOT));
}

static bool is_timer_size(timer_size size)
{
    return (((size) == TIMER_SIZE_32BIT) || ((size) == TIMER_SIZE_64BIT));
}

static bool is_timer_period(unsigned int period)
{
    return ((period) >= PERIOD_MIN_VALUE);
}

static bool is_timer_div(timer_prescaler_factor div)
{
    return (((div) == TIMERPRESCALER_NO_DIV) || ((div) == TIMERPRESCALER_DIV_16) || ((div) == TIMERPRESCALER_DIV_256) ||
        ((div) == TIMERPRESCALER_DIV_RSVD));
}

static bool is_timer_interrupt(unsigned int interrupten)
{
    return (((interrupten) == BASE_CFG_SET) || ((interrupten) == BASE_CFG_UNSET));
}

static void bm_timer_register_callback(bm_timer_ids id, timer_callback_func callback, uintptr_t param)
{
    unsigned int core = bm_get_coreid();
    osal_irq_free(TIMER_BASE_IRQ + id, NULL);
    osal_irq_set_priority(TIMER_BASE_IRQ + id, 6); // pri = 6
    osal_irq_request(TIMER_BASE_IRQ + id, (osal_irq_handler)callback, NULL, NULL, (void *)param);
    osal_irq_set_affinity(TIMER_BASE_IRQ + id, NULL, (int)(1 << core));
    osal_irq_enable(TIMER_BASE_IRQ + id);
}

static int bm_timer_param_check(const bm_timer_cfg *cfg)
{
    timer_param_check_with_ret(cfg != NULL, BM_FAIL);
    timer_param_check_with_ret(is_timer_id(cfg->id), BM_FAIL);
    timer_param_check_with_ret(is_timer_period(cfg->load_l), BM_FAIL);
    timer_param_check_with_ret(is_timer_mode(cfg->mode), BM_FAIL);
    timer_param_check_with_ret(is_timer_size(cfg->size), BM_FAIL);
    timer_param_check_with_ret(is_timer_div(cfg->prescaler), BM_FAIL);
    timer_param_check_with_ret(is_timer_interrupt(cfg->interrupten), BM_FAIL);
    if (cfg->interrupten) {
        timer_param_check_with_ret(cfg->callback != NULL, BM_FAIL);
    }
    return BM_OK;
}

static void bm_timer_config(const bm_timer_cfg *cfg)
{
    timer_reg_struct *regs = (timer_reg_struct *)g_reg_bases[cfg->id];

    regs->timer_intclr = BASE_CFG_SET;
    regs->timer_load_l = cfg->load_l;
    regs->timer_load_h = cfg->load_h;
    regs->timer_bgload_l = cfg->bgload_l;
    regs->timer_bgload_h = cfg->bgload_h;

    regs->timer_control.bit.intenable = cfg->interrupten;
    regs->timer_control.bit.timerpre = cfg->prescaler;
    regs->timer_control.bit.timersize = cfg->size;
    if (cfg->mode == TIMER_MODE_RUN_ONTSHOT) {
        regs->timer_control.bit.oneshot = BASE_CFG_SET;
    } else {
        regs->timer_control.bit.oneshot = BASE_CFG_UNSET;
        regs->timer_control.bit.timermode = (cfg->mode == TIMER_MODE_RUN_FREE) ? BASE_CFG_UNSET : BASE_CFG_SET;
    }
    if (cfg->interrupten) {
        bm_timer_register_callback(cfg->id, cfg->callback, cfg->param);
    }
}

int bm_timer_init(const bm_timer_cfg *cfg)
{
    int ret = bm_timer_param_check(cfg);
    if (ret) {
        bm_log("timer param error!\n");
        return ret;
    }
    bm_timer_config(cfg);
    return BM_OK;
}

int bm_timer_deinit(bm_timer_ids id)
{
    timer_param_check_with_ret(is_timer_id(id), BM_FAIL);
    timer_reg_struct *regs = (timer_reg_struct *)g_reg_bases[id];

    regs->timer_intclr = BASE_CFG_SET;
    regs->timer_load_l = 0;
    regs->timer_load_h = 0;
    regs->timer_bgload_l = 0;
    regs->timer_bgload_h = 0;
    regs->timer_control.reg = 0;
    osal_irq_free(TIMER_BASE_IRQ + id, NULL);
    return BM_OK;
}

int bm_timer_start(bm_timer_ids id)
{
    timer_param_check_with_ret(is_timer_id(id), BM_FAIL);
    timer_reg_struct *regs = (timer_reg_struct *)g_reg_bases[id];
    regs->timer_control.bit.timeren = BASE_CFG_SET;

    return BM_OK;
}

int bm_timer_stop(bm_timer_ids id)
{
    timer_param_check_with_ret(is_timer_id(id), BM_FAIL);
    timer_reg_struct *regs = (timer_reg_struct *)g_reg_bases[id];

    regs->timer_control.bit.timeren = BASE_CFG_UNSET;
    regs->timer_intclr = BASE_CFG_SET;

    return BM_OK;
}

int bm_timer_modify_load_value(bm_timer_ids id, unsigned int load_l, unsigned int load_h)
{
    timer_param_check_with_ret(is_timer_id(id), BM_FAIL);
    timer_param_check_with_ret(is_timer_period(load_l), BM_FAIL);
    timer_reg_struct *regs = (timer_reg_struct *)g_reg_bases[id];

    regs->timer_load_l = load_l;
    regs->timer_load_h = load_h;
    return BM_OK;
}

int bm_timer_modify_bgload_value(bm_timer_ids id, unsigned int bgload_l, unsigned int bgload_h)
{
    timer_param_check_with_ret(is_timer_id(id), BM_FAIL);
    timer_reg_struct *regs = (timer_reg_struct *)g_reg_bases[id];

    regs->timer_bgload_l = bgload_l;
    regs->timer_bgload_h = bgload_h;
    return BM_OK;
}

int bm_timer_get_cnt_value(bm_timer_ids id, unsigned int *val_l, unsigned int *val_h)
{
    timer_param_check_with_ret(is_timer_id(id), BM_FAIL);
    timer_param_check_with_ret(val_l != NULL, BM_FAIL);
    timer_param_check_with_ret(val_h != NULL, BM_FAIL);
    timer_reg_struct *regs = (timer_reg_struct *)g_reg_bases[id];

    *val_l = regs->timer_value_l;
    *val_h = regs->timer_value_h;
    return BM_OK;
}

int bm_timer_irq_clear(bm_timer_ids id)
{
    timer_param_check_with_ret(is_timer_id(id), BM_FAIL);
    timer_reg_struct *regs = (timer_reg_struct *)g_reg_bases[id];

    regs->timer_intclr = BASE_CFG_SET;

    return BM_OK;
}