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

#include "bm_common.h"
#include "bm_localbus.h"
#include "localbus.h"

static int localbus_exist_array[1] = {
    dt_device_exists(LOCALBUS, LOCALBUS0),
};

static struct localbus_csi g_localbus_csi[] = {
    {.cs = 0,
     .start = 0x30000000UL,
     .size = {0x10000000UL, 0x8000000UL, 0x0UL, 0x0UL},
     .regoff = {.cs_control = 0x20, .control = 0xCC}},
    {.cs = 1,
     .start = 0x40000000UL,
     .size = {0x10000000UL, 0x8000000UL, 0x0UL, 0x0UL},
     .regoff = {.cs_control = 0x24, .control = 0xCC}},
    {.cs = 2,
     .start = 0x50000000UL,
     .size = {0x8000000UL, 0x4000000UL, 0x0UL, 0x0UL},
     .regoff = {.cs_control = 0x28, .control = 0xCC}},
    {.cs = 3,
     .start = 0x58000000UL,
     .size = {0x8000000UL, 0x4000000UL, 0x0UL, 0x0UL},
     .regoff = {.cs_control = 0x2C, .control = 0xCC}},
};

static const unsigned int g_localbus_iocfg[LOCALBUS_CS_NUM][LOCALBUS_IOMUX_PIN_NUM] = {
    {0x876e000, 0x876e004, 0x876e00c, 0x876e010, 0x876e014, 0x876e018, 0x876e044, 0x876e048, 0x876e04c, 0x876e050,
        0x876e054, 0x876e074, 0x876e078, 0x876e07c, 0x876e080, 0x876e084, 0x876e088, 0x876e08c, 0x876e090, 0x876e098},
    {0x876e000, 0x876e008, 0x876e00c, 0x876e010, 0x876e014, 0x876e018, 0x876e044, 0x876e048, 0x876e04c, 0x876e050,
        0x876e054, 0x876e074, 0x876e078, 0x876e07c, 0x876e080, 0x876e084, 0x876e088, 0x876e08c, 0x876e090, 0x876e098},
    {0x876e000, 0x876e00c, 0x876e010, 0x876e014, 0x876e018, 0x876e044, 0x876e048, 0x876e04c, 0x876e050, 0x876e054,
        0x876e074, 0x876e078, 0x876e07c, 0x876e080, 0x876e084, 0x876e088, 0x876e08c, 0x876e090, 0x876e094, 0x876e098},
    {0x876e000, 0x876e00c, 0x876e010, 0x876e014, 0x876e018, 0x876e044, 0x876e048, 0x876e04c, 0x876e050, 0x876e054,
        0x876e070, 0x876e074, 0x876e078, 0x876e07c, 0x876e080, 0x876e084, 0x876e088, 0x876e08c, 0x876e090, 0x876e098},
};

static struct localbus_cs_ops *g_localbus_cs_ops[LOCALBUS_CS_NUM] = {
};

static struct localbus_driver g_localbus_driver_data = {
    .base_reg = (void *)LB_BASE_ADDR,
    .csi_cnt = ARRAY_SIZE(g_localbus_csi),
    .csi = g_localbus_csi,
    .ops_cnt = ARRAY_SIZE(g_localbus_cs_ops),
    .cs_ops = g_localbus_cs_ops,
};

static void localbus_init_csi(struct localbus_driver *drv)
{
    struct localbus_csi *csi = NULL;
    unsigned int val;
    unsigned int cnt;
    unsigned int csi_shift = 5;

    for (cnt = 0; cnt < drv->csi_cnt; cnt++) {
        csi = &drv->csi[cnt];
        val = localbus_cs_readl(csi, CS_CONTROL);
        csi->width = (val >> csi_shift) & 0x3;
    }
}

static void locabus_set_io_cfg(unsigned int cs)
{
    int i = 0;
    for (i = 0; i < LOCALBUS_IOMUX_PIN_NUM; i++) {
        localbus_writel(0x0, g_localbus_iocfg[cs][i]);
    }
}

static int localbus_reg_read_write(unsigned int offset, unsigned int mask, unsigned int val)
{
    unsigned int value;

    value = localbus_readl(LB_BASE_ADDR + offset);
    value &= ~mask;
    value |= (val & mask);
    localbus_writel(value, LB_BASE_ADDR + offset);

    return BM_OK;
}

void localbus_init(void)
{
    if (!localbus_exist_array[0]) {
        bm_log("localbus0, forbidden to access\n");
        return;
    }

    struct localbus_driver *drv = &g_localbus_driver_data;
    localbus_init_csi(drv);
}

void localbus_deinit(void)
{
    if (!localbus_exist_array[0]) {
        bm_log("localbus0, forbidden to access\n");
    }

    return;
}

int localbus_ops_register(struct localbus_cs_ops *ops, localbus_cs_width cs_width)
{
    if (!localbus_exist_array[0]) {
        bm_log("localbus0, forbidden to access\n");
        return BM_FAIL;
    }

    struct localbus_driver *drv = &g_localbus_driver_data;
    if (!ops)
        return BM_FAIL;
    if (ops->cs >= LOCALBUS_CS_NUM)
        return BM_FAIL;
    if (cs_width != LOCALBUS_WIDTH_8 && cs_width != LOCALBUS_WIDTH_16) {
        bm_log("cs width %d not support\n", cs_width);
        return BM_FAIL;
    }
    locabus_set_io_cfg(ops->cs);
    if (ops->ops) {
        g_localbus_cs_ops[ops->cs] = ops;
        g_localbus_cs_ops[ops->cs]->csi = &drv->csi[ops->cs];
        if (g_localbus_cs_ops[ops->cs]->csi->width != cs_width) {
            localbus_reg_read_write(LB_CS1_CTRL_OFFSET_ADDR, LB_REG_CF_DATA_WID_BITS << LB_REG_CF_DATA_WID_POS,
                cs_width);
            g_localbus_cs_ops[ops->cs]->csi->width = cs_width;
        }
        if (ops->ops->probe) {
            g_localbus_cs_ops[ops->cs]->ops->probe(ops);
        }
        return BM_OK;
    }
    return BM_FAIL;
}

int localbus_ops_unregister(struct localbus_cs_ops *ops)
{
    if (!localbus_exist_array[0]) {
        bm_log("localbus0, forbidden to access\n");
        return BM_FAIL;
    }

    if (!ops)
        return BM_FAIL;
    if (ops->cs >= LOCALBUS_CS_NUM)
        return BM_FAIL;
    if (ops->ops) {
        g_localbus_cs_ops[ops->cs] = NULL;
        return BM_OK;
    }
    return BM_FAIL;
}

static int localbus_para_check(struct localbus_cs_ops *cs_ops, unsigned int cs, long long to, unsigned int length)
{
    if (to > g_localbus_csi[cs].size[0])
        return BM_FAIL;
    if ((length + to) > g_localbus_csi[cs].size[0])
        return BM_FAIL;
    if (!cs_ops)
        return BM_FAIL;
    if (!cs_ops->ops)
        return BM_FAIL;
    return BM_OK;
}

void localbus_destroy(void)
{
    if (!localbus_exist_array[0]) {
        bm_log("localbus0, forbidden to access\n");
        return;
    }

    struct localbus_driver *drv = &g_localbus_driver_data;
    unsigned int cnt;
    for (cnt = 0; cnt < LOCALBUS_CS_NUM; cnt++) {
        struct localbus_cs_ops *cs_ops = drv->cs_ops[cnt];
        if (!cs_ops) {
            continue;
        }
        if (!cs_ops->ops) {
            continue;
        }
        if (cs_ops->ops->destroy) {
            cs_ops->ops->destroy(cs_ops);
        }
    }
}

static int localbus_transmit_check(unsigned int cs, unsigned char *buffer)
{
    if (!localbus_exist_array[0]) {
        bm_log("localbus0, forbidden to access\n");
        return BM_FAIL;
    }

    if (cs >= LOCALBUS_CS_NUM || !buffer) {
        return BM_FAIL;
    }

    return BM_OK;
}

int localbus_read(unsigned int cs, long long from, unsigned int length, unsigned char *buffer)
{
    if (localbus_transmit_check(cs, buffer)) {
        return BM_FAIL;
    }

    if (is_invalid_addr_len((unsigned int)(uintptr_t)buffer, length)) {
        return BM_FAIL;
    }

    struct localbus_driver *drv = &g_localbus_driver_data;
    struct localbus_cs_ops *cs_ops = drv->cs_ops[cs];

    int ret = localbus_para_check(cs_ops, cs, from, length);
    if (ret != 0) {
        bm_log("read: localbus_para_check err\n");
        return ret;
    }

    if (!cs_ops->ops->read) {
        bm_log("no read function\n");
        return BM_FAIL;
    }
    return cs_ops->ops->read(cs_ops, from, length, buffer);
}

int localbus_write(unsigned int cs, long long to, unsigned int length, unsigned char *buffer)
{
    if (localbus_transmit_check(cs, buffer)) {
        return BM_FAIL;
    }

    if (is_invalid_addr_len((unsigned int)(uintptr_t)buffer, length)) {
        return BM_FAIL;
    }

    struct localbus_driver *drv = &g_localbus_driver_data;
    struct localbus_cs_ops *cs_ops = drv->cs_ops[cs];

    int ret = localbus_para_check(cs_ops, cs, to, length);
    if (ret != 0) {
        bm_log("write: localbus_para_check err\n");
        return ret;
    }

    if (!cs_ops->ops->write) {
        bm_log("no write function\n");
        return BM_FAIL;
    }
    return cs_ops->ops->write(cs_ops, to, length, buffer);
}

int localbus_erase(unsigned int cs, long long start, unsigned int length)
{
    if (!localbus_exist_array[0]) {
        bm_log("localbus0, forbidden to access\n");
        return BM_FAIL;
    }

    if (cs >= LOCALBUS_CS_NUM)
        return BM_FAIL;
    struct localbus_driver *drv = &g_localbus_driver_data;
    struct localbus_cs_ops *cs_ops = drv->cs_ops[cs];

    int ret = localbus_para_check(cs_ops, cs, start, length);
    if (ret != 0) {
        bm_log("localbus_para_check err\n");
        return ret;
    }
    if (!cs_ops->ops->erase) {
        bm_log("no erase function\n");
        return BM_FAIL;
    }
    return cs_ops->ops->erase(cs_ops, start, length);
}
