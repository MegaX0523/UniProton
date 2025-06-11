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
#include <reg_sysctrl.h>
#include "bm_wdg.h"
#include "soc_osal.h"
#include "wdg.h"

#define wdg_param_check_with_ret(param, ret) base_func_paramcheck_with_ret(param, ret)

static const int wdg_exist_array[WDG_ID_BUTT] = {
    dt_device_exists(WATCHDOG, WATCHDOG0),
    dt_device_exists(WATCHDOG, WATCHDOG1),
    dt_device_exists(WATCHDOG, WATCHDOG2),
    dt_device_exists(WATCHDOG, WATCHDOG3),
    dt_device_exists(WATCHDOG, WATCHDOG4),
};

static const unsigned long g_wdg_reg_bases[WDG_ID_BUTT] = {
    WDG0_REG_BASE,
    WDG1_REG_BASE,
    WDG2_REG_BASE,
    WDG3_REG_BASE,
    WDG4_REG_BASE
};

struct wdg_adapter {
    unsigned int wdg_id;
    wdg_callback_func callback;
};
struct wdg_adapter g_wdg_platdrv_adap_list[WDG_ID_BUTT];

static void watchdog_sysctl_set(unsigned int wdg_id, unsigned int wdg_mask, unsigned int rst_ctrl)
{
    unsigned int bits = 0;
    unsigned int mask = 0;
    /* rst_output : bit[3]
       rst_ctrl   : bit[2:1]
       mask       : bit[0] */
    bits = (wdg_mask & 0x1) | ((rst_ctrl & WDG_RESET_MODE) << 1);

    bits = (bits << (wdg_id * WDG_CTRL_PER_BITS));
    mask = (0xfu << (wdg_id * WDG_CTRL_PER_BITS));
    bm_sysctrl_reg_read_write(SYSCTRL_CSR_WD_CTRL_REG_OFFSET_ADDR, mask, bits);
}

static bool is_wdg_id(bm_wdg_ids id)
{
    return ((id >= WDG_ID_0) && (id < WDG_ID_BUTT) && wdg_exist_array[id]);
}

static void wdg_lock(bm_wdg_ids id, unsigned int lock_on)
{
    wdg_reg_struct *regs = (wdg_reg_struct *)g_wdg_reg_bases[id];
    if (lock_on) {
        regs->wdg_lock = WDG_LOCK_VALUE;
    } else {
        regs->wdg_lock = WDG_UNLOCK_VALUE;
    }
}

static int wdg_interrupt_handle(int irq, void *args)
{
    (void)irq;
    bm_wdg_ids id = *((bm_wdg_ids *)args);
    if (g_wdg_platdrv_adap_list[id].callback) {
        g_wdg_platdrv_adap_list[id].callback(id);
    }
    wdg_reg_struct *regs = (wdg_reg_struct *)g_wdg_reg_bases[id];
    wdg_lock(id, BASE_CFG_DISABLE);
    regs->wdg_load = WDG_RESET_VALUE;
    wdg_lock(id, BASE_CFG_ENABLE);
    return BM_OK;
}

int bm_wdg_init(const bm_wdg_cfg *cfg)
{
    wdg_param_check_with_ret(cfg != NULL, BM_FAIL);
    wdg_param_check_with_ret(is_wdg_id(cfg->id), BM_FAIL);
    wdg_param_check_with_ret(cfg->callback != NULL, BM_FAIL);

    wdg_reg_struct *regs = (wdg_reg_struct *)g_wdg_reg_bases[cfg->id];

    wdg_lock(cfg->id, BASE_CFG_DISABLE);
    regs->wdg_intclr = BASE_CFG_SET;
    regs->wdg_load = cfg->load;
    regs->wdg_control.reg = BASE_CFG_UNSET;
    wdg_lock(cfg->id, BASE_CFG_ENABLE);

    /* Set the delay after the reset signal is generated to reset the chip. */
    watchdog_sysctl_set(cfg->id, BASE_CFG_UNSET, WDG_RESET_MODE);

    g_wdg_platdrv_adap_list[cfg->id].callback = cfg->callback;
    g_wdg_platdrv_adap_list[cfg->id].wdg_id = cfg->id;

    unsigned int core = bm_get_coreid();
    osal_irq_free(WDG_BASE_IRQ + cfg->id, NULL);
    osal_irq_set_priority(WDG_BASE_IRQ + cfg->id, 6); // pri = 6
    osal_irq_request(WDG_BASE_IRQ + cfg->id, wdg_interrupt_handle, NULL, NULL,
        (void *)&g_wdg_platdrv_adap_list[cfg->id].wdg_id);
    osal_irq_set_affinity(WDG_BASE_IRQ + cfg->id, NULL, (1 << core));
    osal_irq_enable(WDG_BASE_IRQ + cfg->id);

    return BM_OK;
}

int bm_wdg_deinit(bm_wdg_ids id)
{
    wdg_param_check_with_ret(is_wdg_id(id), BM_FAIL);
    wdg_reg_struct *regs = (wdg_reg_struct *)g_wdg_reg_bases[id];

    wdg_lock(id, BASE_CFG_DISABLE);
    regs->wdg_intclr = BASE_CFG_SET;
    regs->wdg_load = 0;
    regs->wdg_control.reg = BASE_CFG_UNSET;
    wdg_lock(id, BASE_CFG_ENABLE);

    g_wdg_platdrv_adap_list[id].callback = NULL;
    osal_irq_free(WDG_BASE_IRQ + id, NULL);
    return BM_OK;
}

int bm_wdg_enable(bm_wdg_ids id)
{
    wdg_param_check_with_ret(is_wdg_id(id), BM_FAIL);
    wdg_reg_struct *regs = (wdg_reg_struct *)g_wdg_reg_bases[id];

    wdg_lock(id, BASE_CFG_DISABLE);
    regs->wdg_control.bit.inten = BASE_CFG_ENABLE;
    regs->wdg_control.bit.resen = BASE_CFG_ENABLE;
    wdg_lock(id, BASE_CFG_ENABLE);
    return BM_OK;
}

int bm_wdg_disable(bm_wdg_ids id)
{
    wdg_param_check_with_ret(is_wdg_id(id), BM_FAIL);
    wdg_reg_struct *regs = (wdg_reg_struct *)g_wdg_reg_bases[id];

    wdg_lock(id, BASE_CFG_DISABLE);
    regs->wdg_control.bit.inten = BASE_CFG_DISABLE;
    regs->wdg_control.bit.resen = BASE_CFG_DISABLE;
    wdg_lock(id, BASE_CFG_ENABLE);

    return BM_OK;
}

int bm_wdg_feed(bm_wdg_ids id)
{
    wdg_param_check_with_ret(is_wdg_id(id), BM_FAIL);
    wdg_reg_struct *regs = (wdg_reg_struct *)g_wdg_reg_bases[id];

    wdg_lock(id, BASE_CFG_DISABLE);
    regs->wdg_intclr = BASE_CFG_SET;
    wdg_lock(id, BASE_CFG_ENABLE);
    return BM_OK;
}

int bm_wdg_get_cnt_value(bm_wdg_ids id, unsigned int *val)
{
    wdg_param_check_with_ret(is_wdg_id(id), BM_FAIL);
    wdg_param_check_with_ret(val != NULL, BM_FAIL);
    wdg_reg_struct *regs = (wdg_reg_struct *)g_wdg_reg_bases[id];

    *val = regs->wdg_value;
    return BM_OK;
}
