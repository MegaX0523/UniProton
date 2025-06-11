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

#include "i2c.h"
#include "bm_types.h"
#include "bm_dmac.h"
struct i2c_adapter g_i2c_platdrv_adap_list[] = {
    {
        .i2c_id  = 0,
        .name    = "i2c0",
        .regbase = 0x8707000,
        .irqn    = 104,
        .init_flag = 0,
    },
    {
        .i2c_id  = 1,
        .name    = "i2c1",
        .regbase = 0x8708000,
        .irqn    = 105,
        .init_flag = 0,
    },
    {
        .i2c_id  = 2,
        .name    = "i2c2",
        .regbase = 0x8709000,
        .irqn    = 106,
        .init_flag = 0,
    },
    {
        .i2c_id  = 3,
        .name    = "i2c3",
        .regbase = 0x870a000,
        .irqn    = 107,
        .init_flag = 0,
    },
    {
        .i2c_id  = 4,
        .name    = "i2c4",
        .regbase = 0x870b000,
        .irqn    = 108,
        .init_flag = 0,
    },
    {
        .i2c_id  = 5,
        .name    = "i2c5",
        .regbase = 0x870c000,
        .irqn    = 109,
        .init_flag = 0,
    },
    {
        .i2c_id  = 6,
        .name    = "i2c6",
        .regbase = 0x870d000,
        .irqn    = 110,
        .init_flag = 0,
    },
    {
        .i2c_id  = 7,
        .name    = "i2c7",
        .regbase = 0x870e000,
        .irqn    = 111,
        .init_flag = 0,
    },
    {
        .i2c_id  = 8,
        .name    = "i2c8",
        .regbase = 0x8750000,
        .irqn    = 112,
        .init_flag = 0,
    },
    {
        .i2c_id  = 9,
        .name    = "i2c9",
        .regbase = 0x8751000,
        .irqn    = 113,
        .init_flag = 0,
    },
    {
        .i2c_id  = 10,
        .name    = "i2c10",
        .regbase = 0x8752000,
        .irqn    = 114,
        .init_flag = 0,
    },
    {
        .i2c_id  = 11,
        .name    = "i2c11",
        .regbase = 0x8753000,
        .irqn    = 115,
        .init_flag = 0,
    },
    {
        .i2c_id  = 12,
        .name    = "i2c12",
        .regbase = 0x8754000,
        .irqn    = 116,
        .init_flag = 0,
    },
    {
        .i2c_id  = 13,
        .name    = "i2c13",
        .regbase = 0x8755000,
        .irqn    = 117,
        .init_flag = 0,
    },
    {
        .i2c_id  = 14,
        .name    = "i2c14",
        .regbase = 0x8756000,
        .irqn    = 118,
        .init_flag = 0,
    },
    {
        .i2c_id  = 15,
        .name    = "i2c15",
        .regbase = 0x8757000,
        .irqn    = 119,
        .init_flag = 0,
    },
};

static const int i2c_exist_array[I2C_ID_BUTT] = {
    dt_device_exists(I2C, I2C0),
    dt_device_exists(I2C, I2C1),
    dt_device_exists(I2C, I2C2),
    dt_device_exists(I2C, I2C3),
    dt_device_exists(I2C, I2C4),
    dt_device_exists(I2C, I2C5),
    dt_device_exists(I2C, I2C6),
    dt_device_exists(I2C, I2C7),
    dt_device_exists(I2C, I2C8),
    dt_device_exists(I2C, I2C9),
    dt_device_exists(I2C, I2C10),
    dt_device_exists(I2C, I2C11),
    dt_device_exists(I2C, I2C12),
    dt_device_exists(I2C, I2C13),
    dt_device_exists(I2C, I2C14),
    dt_device_exists(I2C, I2C15),
};

static struct i2c_kmsg g_i2c_msg[I2C_ID_BUTT];
static bm_i2c_callback g_i2c_callback_array[I2C_ID_BUTT] = { 0 };
static bm_i2c_callback g_i2c_dma_tx_callback_array[I2C_ID_BUTT] = { 0 };
static bm_i2c_callback g_i2c_dma_rx_callback_array[I2C_ID_BUTT] = { 0 };
static const struct i2c_iomux_info g_i2c_iomux_data_tbl[] = {
    {IO_CONFIG_T, IO_CONFIG_T_IOMG013_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG015_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG017_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG019_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG021_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG023_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG025_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG026_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG029_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG031_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG033_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG035_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG037_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG050_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x1},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG036_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG022_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x1},
};

static const struct i2c_iomux_info g_i2c_iomux_clk_tbl[] = {
    {IO_CONFIG_T, IO_CONFIG_T_IOMG014_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG016_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG018_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG020_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG022_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG024_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG025_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG026_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x0},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG028_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG030_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG032_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG034_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG036_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG050_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x1},
    {IO_CONFIG_T, IO_CONFIG_T_IOMG035_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x3},
    {IO_CONFIG_R, IO_CONFIG_R_IOMG022_OFFSET_ADDR, I2C_IO_MUX_MASK, 0x1},
};

static void i2c_set_lpsc_smbus_i2c_mux(unsigned int i2c_id)
{
    unsigned int reg_offset;
    unsigned int val;
    unsigned int reg_mask;

    reg_offset = LP_SUBSYS_CSR_LPSC_SMBUS_I2C_MUX_OFFSET_ADDR;
    val = 0x0U;
    reg_mask = (0x1U << i2c_id);

    bm_lp_subsys_reg_read_write(reg_offset, reg_mask, val);
}

static unsigned int i2c_rc_intrbits(struct i2c_adapter *adap)
{
    unsigned int irq_status = i2c_readl(adap->i2c_id, ICR_INTR_STAT);
    static const struct {
        unsigned int msk;
        unsigned int clr;
    } rcinfo[] = {
        { .msk = IC_INTR_RX_UNDER, .clr = ICR_CLR_RX_UNDER },
        { .msk = IC_INTR_RX_OVER, .clr = ICR_CLR_RX_OVER },
        { .msk = IC_INTR_TX_OVER, .clr = ICR_CLR_TX_OVER },
        { .msk = IC_INTR_RX_DONE, .clr = ICR_CLR_RX_DONE },
        { .msk = IC_INTR_ACTIVITY, .clr = ICR_CLR_ACTIVITY },
        { .msk = IC_INTR_STOP_DET, .clr = ICR_CLR_STOP_DET },
        { .msk = IC_INTR_START_DET, .clr = ICR_CLR_START_DET },
        { .msk = IC_INTR_GEN_CALL, .clr = ICR_CLR_GEN_CALL },
    };

    for (unsigned int i = 0; i < ARRAY_SIZE(rcinfo); i++) {
        if (irq_status & rcinfo[i].msk) {
            i2c_readl(adap->i2c_id, rcinfo[i].clr);
        }
    }

    if (irq_status & IC_INTR_RD_REQ) {
        i2c_readl(adap->i2c_id, ICR_CLR_RD_REQ);
    }

    return irq_status;
}

static void i2c_irq_read(struct i2c_adapter *adap)
{
    struct i2c_kmsg *msg = adap->msg;
    unsigned int rxflr = i2c_readl(adap->i2c_id, ICR_RXFLR);
    unsigned char val;
    unsigned int cnt;

    if (msg == NULL) {
        return;
    }

    for (cnt = 0; cnt < rxflr; cnt++) {
        val = (unsigned char)i2c_readl(adap->i2c_id, ICR_DATA_CMD);
        if (msg->rcnt >= msg->rlen) {
            break;
        }

        msg->rbuf[msg->rcnt++] = val;
        dsb();
    }
}

static int i2c_irq_xfer_write_byte(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    unsigned int cmd = 0;
    unsigned char val;

    if (msg->wcnt >= msg->wlen || adap->msg->wbuf == NULL) {
        return -1;
    }

    if (msg->wcnt + 1 == msg->wlen) {
        if (!(msg->flags & I2C_KMSG_RD)) {
            cmd |= IC_DATA_CMD_RECV;
        }
    }
    val = msg->wbuf[msg->wcnt++];
    dsb();

    i2c_writel(adap->i2c_id, cmd | val, ICR_DATA_CMD);
    return 0;
}

static int i2c_irq_xfer_msg_byte(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    int ret;

    ret = i2c_irq_xfer_write_byte(adap, msg);
    if (!ret) {
        return I2C_DW_MASTER_CONTINUE;
    }

    if (!(msg->flags & I2C_KMSG_RD)) {
        return I2C_DW_MASTER_BREAK;
    }

    i2c_writel(adap->i2c_id, IC_DATA_CMD_WRITE, ICR_DATA_CMD);

    return I2C_DW_MASTER_NEXT;
}

static void i2c_irq_xfer_msg(struct i2c_adapter *adap)
{
    int ret;
    int wlimit;
    int rlimit;
    unsigned int interrupt_mask = IC_INTR_MASTER_MASK;
    bm_i2c_callback i2c_callback;
    struct i2c_kmsg *msg = adap->msg;

    if (msg == NULL) {
        return;
    }

    wlimit = adap->tx_fifo_depth - i2c_readl(adap->i2c_id, ICR_TXFLR);
    rlimit = adap->rx_fifo_depth - i2c_readl(adap->i2c_id, ICR_RXFLR);

    for (; wlimit > 0 && rlimit > 0; wlimit--) {
        ret = i2c_irq_xfer_msg_byte(adap, msg);
        if (ret == I2C_DW_MASTER_BREAK && msg->wcnt >= msg->wlen) {
            i2c_callback = g_i2c_callback_array[adap->i2c_id];
            if (i2c_callback != NULL) {
                i2c_callback(adap->i2c_id, I2C_WRITE_OK);
                g_i2c_callback_array[adap->i2c_id] = NULL;
                osal_irq_disable(adap->irqn);
            }
            break;
        } else if (ret == I2C_DW_MASTER_NEXT) {
            rlimit--;
            adap->cmd_len++;
            if (adap->cmd_len >= msg->rlen) {
                break;
            }
        }
    }

    if (msg->flags & I2C_KMSG_RD) {
        if (adap->cmd_len >= msg->rlen) {
            interrupt_mask &= ~IC_INTR_TX_EMPTY;
        } else {
            interrupt_mask |= IC_INTR_TX_EMPTY;
        }
    } else if (msg->wcnt >= msg->wlen) {
        interrupt_mask &= ~IC_INTR_TX_EMPTY;
    } else {
        interrupt_mask |= IC_INTR_TX_EMPTY;
    }
    i2c_writel(adap->i2c_id, interrupt_mask, ICR_INTR_MASK);
}

static void i2c_handle_error(struct i2c_adapter *adap, bm_i2c_callback i2c_callback)
{
    unsigned int transmit_status;

    transmit_status = (adap->msg->flags & I2C_KMSG_RD) ? I2C_READ_FAIL : I2C_WRITE_FAIL;
    if (i2c_callback != NULL) {
        i2c_callback(adap->i2c_id, transmit_status);
        g_i2c_callback_array[adap->i2c_id] = NULL;
    }
}

static int i2c_interrupt_handle(int irq, void *arg)
{
    unsigned int irq_status;
    unsigned int interrupt_mask = IC_INTR_TX_ABRT | IC_INTR_SCL_STUCK_LOW;
    struct i2c_adapter *adap = arg;
    bm_i2c_callback i2c_callback;
    (void)irq;

    if (adap == NULL) {
        return BM_FAIL;
    }

    i2c_callback = g_i2c_callback_array[adap->i2c_id];
    if (!i2c_readl(adap->i2c_id, ICR_ENABLE)) {
        i2c_handle_error(adap, i2c_callback);
        return BM_FAIL;
    }

    if (!(i2c_readl(adap->i2c_id, ICR_RAW_INTR_STAT) & ~IC_INTR_ACTIVITY)) {
        i2c_handle_error(adap, i2c_callback);
        return BM_FAIL;
    }

    irq_status = i2c_rc_intrbits(adap);
    if ((irq_status & IC_INTR_RX_FULL)) {
        i2c_irq_read(adap);
        if ((i2c_callback != NULL) && (adap->msg->rcnt >= adap->msg->rlen)) {
            i2c_callback(adap->i2c_id, I2C_READ_OK);
            g_i2c_callback_array[adap->i2c_id] = NULL;
            osal_irq_disable(adap->irqn);
        }
    }

    if (irq_status & IC_INTR_TX_EMPTY) {
        i2c_irq_xfer_msg(adap);
    }

    if (irq_status & IC_INTR_STOP_DET) {
        i2c_writel(adap->i2c_id, interrupt_mask, ICR_INTR_MASK);
    }

    return BM_OK;
}

static void i2c_master_clean(struct i2c_adapter *adap)
{
    unsigned int i2c_id = adap->i2c_id;
    i2c_common_disable_int(i2c_id);
}

static void i2c_adapter_set_msgptr(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    adap->msg = msg;
    adap->msg_flags = msg ? msg->flags : 0;
}

static int i2c_adapter_set_default(struct i2c_adapter *adap)
{
    adap->control = 0;
    adap->tx_fifo_depth = 0;
    adap->rx_fifo_depth = 0;

    i2c_adapter_set_msgptr(adap, NULL);

    return 0;
}

static int i2c_wait_idle(struct i2c_adapter *adap)
{
    unsigned int udelay_conut = I2C_INIT_TIMEOUT;

    while ((i2c_readl(adap->i2c_id, ICR_STATUS) & IC_STATUS_ACTIVITY) && (udelay_conut > 0)) {
        udelay(1);
        udelay_conut--;
        dsb();
    }

    if (!udelay_conut) {
        return BM_FAIL;
    }

    i2c_common_disable(adap->i2c_id);
    return BM_OK;
}

static int i2c_master_init_speed(struct i2c_adapter *adap, struct i2c_kdepl *deploy)
{
    unsigned int i2c_id = adap->i2c_id;

    switch (deploy->flags & I2C_KDEPLOY_SPEED_MODE_MASK) {
        case I2C_KDEPLOY_SPEED_MODE_HIGH:
            adap->control |= IC_CON_SPEED_HIGH;
            adap->hcnt = deploy->scl_high ?: 0x6; /* The default register value 0x6 is used
                                                       if no input is available. */
            adap->lcnt = deploy->scl_low ?: 0x10; /* The default register value 0x10 is used
                                                       if no input is available. */
            i2c_writel(i2c_id, adap->hcnt, ICR_HS_SCL_HCNT);
            i2c_writel(i2c_id, adap->lcnt, ICR_HS_SCL_LCNT);
            break;
        case I2C_KDEPLOY_SPEED_MODE_FAST:
            adap->control |= IC_CON_SPEED_FAST;
            adap->hcnt = I2C_FS_SCL_HCNT_CYCLE;
            adap->lcnt = I2C_FS_SCL_LCNT_CYCLE;
            i2c_writel(i2c_id, adap->hcnt, ICR_FS_SCL_HCNT);
            i2c_writel(i2c_id, adap->lcnt, ICR_FS_SCL_LCNT);
            break;
        case I2C_KDEPLOY_SPEED_MODE_STD:
            adap->control |= IC_CON_SPEED_STD;
            adap->hcnt = I2C_SS_SCL_HCNT_CYCLE;
            adap->lcnt = I2C_SS_SCL_LCNT_CYCLE;
            i2c_writel(i2c_id, adap->hcnt, ICR_SS_SCL_HCNT);
            i2c_writel(i2c_id, adap->lcnt, ICR_SS_SCL_LCNT);
            break;
        default:
            return BM_FAIL;
    }

    return BM_OK;
}

static void i2c_master_set_fifo_depth(struct i2c_adapter *adap)
{
    unsigned int val;
    unsigned int tx_depth;
    unsigned int rx_depth;

    val = i2c_readl(adap->i2c_id, ICR_COMP_PARAM_1);
    tx_depth = ((val >> 16) & 0xff) + 1; /* Bit16~bit23  tx_fifo_depth */
    rx_depth = ((val >> 8) & 0xff) + 1;  /* bit8~bit15   rx_fifo_depth */

    adap->tx_fifo_depth = adap->tx_fifo_depth ?: tx_depth;
    adap->rx_fifo_depth = adap->rx_fifo_depth ?: rx_depth;
    adap->tx_fifo_depth = min(adap->tx_fifo_depth, tx_depth);
    adap->rx_fifo_depth = min(adap->rx_fifo_depth, rx_depth);
}

static void i2c_master_configure(struct i2c_adapter *adap, struct i2c_kdepl *deploy)
{
    unsigned int val;
    unsigned int i2c_id = adap->i2c_id;

    i2c_writel(i2c_id, adap->tx_fifo_depth / 2, ICR_TX_TL); /* Divided by 2 sets the FIFO threshold
                                                                to half the depth of the FIFO */
    i2c_writel(i2c_id, 0, ICR_RX_TL);
    i2c_writel(i2c_id, adap->control, ICR_CONTROL);

    val = deploy->scl_low_timeout ?: IC_SCL_LOW_TIMEOUT_DEFAULT;
    i2c_writel(i2c_id, val, ICR_SCL_LOW_TIMEOUT);
    val = deploy->sda_low_timeout ?: IC_SDA_LOW_TIMEOUT_DEFAULT;
    i2c_writel(i2c_id, val, ICR_SDA_LOW_TIMEOUT);
}

static int i2c_master_initialize(struct i2c_adapter *adap, struct i2c_kdepl *deploy)
{
    int ret;

    i2c_set_lpsc_smbus_i2c_mux(adap->i2c_id);

    if (i2c_wait_idle(adap)) {
        bm_log("Controller busy.");
        return BM_FAIL;
    }

    adap->control = IC_CON_MASTER | IC_CON_SLAVE_DISABLE | IC_CON_RESTART_EN;

    ret = i2c_master_init_speed(adap, deploy);
    if (ret) {
        return ret;
    }

    i2c_master_set_fifo_depth(adap);
    i2c_master_configure(adap, deploy);
    osal_irq_disable(adap->irqn);
    osal_irq_set_priority(adap->irqn, 6); // pri = 6
    osal_irq_request(adap->irqn, i2c_interrupt_handle, NULL, NULL, adap);

    for (int i = 0; i < I2C_ID_BUTT; i++) {
        g_i2c_callback_array[i] = NULL;
    }

    return ret;
}

static int i2c_master_init(unsigned int i2c_id, struct i2c_kdepl *deploy)
{
    int ret;
    struct i2c_adapter *adap = NULL;

    if (i2c_id >= I2C_ID_BUTT) {
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[i2c_id];
    if (!adap) {
        return BM_FAIL;
    }

    i2c_master_clean(adap);
    i2c_adapter_set_default(adap);

    ret = i2c_master_initialize(adap, deploy);
    if (ret) {
        i2c_adapter_set_default(adap);
    }

    return ret;
}

int bm_i2c_config_io_clock(bm_i2c_ids i2c_id)
{
    unsigned int clk_value;

    if (i2c_id >= I2C_ID_BUTT) {
        return BM_FAIL;
    }

    if (!i2c_exist_array[i2c_id]) {
        bm_log("i2c_id(%u) error, forbidden to access\n", i2c_id);
        return BM_FAIL;
    }

    clk_value = BIT(i2c_id + I2C_LPSC_CLKEN_CTRL0_OFFSET);

    do {
        bm_lp_subsys_reg_read_write(LP_SUBSYS_CSR_LPSC_CLKEN_CTRL0_OFFSET_ADDR, clk_value, clk_value);
    } while (!(bm_readl(LP_SUBSYS_CSR_BASE_ADDR + LP_SUBSYS_CSR_LPSC_CLKSTA_CTRL0_OFFSET_ADDR) & clk_value));

    return BM_OK;
}

int bm_i2c_config_io_reuse(bm_i2c_ids i2c_id)
{
    struct i2c_iomux_info data_info;
    struct i2c_iomux_info clk_info;

    if (i2c_id >= I2C_ID_BUTT) {
        return BM_FAIL;
    }

    if (!i2c_exist_array[i2c_id]) {
        bm_log("i2c_id(%u) error, forbidden to access\n", i2c_id);
        return BM_FAIL;
    }

    data_info = g_i2c_iomux_data_tbl[i2c_id];
    clk_info = g_i2c_iomux_clk_tbl[i2c_id];

    if (data_info.flag) {
        bm_io_config_t_read_write(data_info.offset, data_info.mask, data_info.val);
    } else {
        bm_io_config_r_read_write(data_info.offset, data_info.mask, data_info.val);
    }

    if (clk_info.flag) {
        bm_io_config_t_read_write(clk_info.offset, clk_info.mask, clk_info.val);
    } else {
        bm_io_config_r_read_write(clk_info.offset, clk_info.mask, clk_info.val);
    }

    return BM_OK;
}


static void bm_i2c_iomux_lp_config(unsigned int i2c_id)
{
    /* Setting the IOMUX */
    bm_i2c_config_io_reuse(i2c_id);

    /* Enable the clock gating of the I2C controller. */
    bm_i2c_config_io_clock(i2c_id);
}

int bm_i2c_init(const struct i2c_init_info *init)
{
    struct i2c_adapter *adap = NULL;

    if (init == NULL || init->i2c_id >= I2C_ID_BUTT) {
        return BM_FAIL;
    }

    if (!i2c_exist_array[init->i2c_id]) {
        bm_log("i2c_id(%u) error, forbidden to access\n", init->i2c_id);
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[init->i2c_id];
    if (adap->init_flag) {
        return BM_OK;
    }

    unsigned int speed = init->speed;
    struct i2c_kdepl deploy = { 0 };

    deploy.flags |= (I2C_KDEPLOY_FORCE | I2C_KDEPLOY_MASTER);

    if (speed == I2C_SPEED_100K) {
        deploy.flags |= I2C_KDEPLOY_SPEED_MODE_STD;
    } else if (speed == I2C_SPEED_400K) {
        deploy.flags |= I2C_KDEPLOY_SPEED_MODE_FAST;
    } else if (speed == I2C_SPEED_3400K) {
        deploy.flags |= I2C_KDEPLOY_SPEED_MODE_HIGH;
    } else {
        bm_log("i2c speed is inval\n");
        return BM_FAIL;
    }

    bm_i2c_iomux_lp_config(init->i2c_id);
    if (i2c_master_init(init->i2c_id, &deploy)) {
        return BM_FAIL;
    }

    int ret = osal_sem_init(&(adap->sem), 1);
    if (ret == OSAL_SUCCESS) {
        adap->init_flag = 1;
    }

    return ret;
}

int bm_i2c_deinit(bm_i2c_ids i2c_id)
{
    struct i2c_adapter *adap = NULL;

    if (i2c_id >= I2C_ID_BUTT) {
        bm_log("i2c_deinit fail\n");
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[i2c_id];
    if (!adap->init_flag) {
        return BM_FAIL;
    }

    g_i2c_callback_array[i2c_id] = NULL;
    osal_irq_disable(adap->irqn);
    osal_sem_destroy(&adap->sem);
    adap->init_flag = 0;

    return BM_OK;
}

static int i2c_check_xfer_info(const struct i2c_xfer_info *info)
{
    if (info == NULL) {
        return BM_FAIL;
    }

    if ((!info->addr) || (!info->length) || info->buffer == NULL || info->i2c_id >= I2C_ID_BUTT ||
        is_invalid_addr_len((unsigned int)(uintptr_t)(info->buffer), info->length)) {
        return BM_FAIL;
    }

    if (!i2c_exist_array[info->i2c_id]) {
        bm_log("i2c_id(%u) error, forbidden to access\n", info->i2c_id);
        return BM_FAIL;
    }

    if (info->addr > I2C_7BIT_MAX_ADDR) {
        bm_log("addr is inval!\n");
        return BM_FAIL;
    }

    return BM_OK;
}

static void i2c_master_xfer_init(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    msg->rcnt = 0;
    msg->wcnt = 0;
    adap->cmd_len = 0;

    i2c_common_disable_int(adap->i2c_id);
    i2c_readl(adap->i2c_id, ICR_CLR_INTR);
    i2c_writel(adap->i2c_id, adap->control, ICR_CONTROL);
    i2c_writel(adap->i2c_id, msg->addr, ICR_TAR);

    i2c_common_enable(adap->i2c_id);
}

static int bm_i2c_check_xfer_finish(struct i2c_adapter *adap, struct i2c_kmsg *msg, int flag)
{
    unsigned i2c_status;
    unsigned int time_out = msg->timeout;

    while (!(i2c_readl(adap->i2c_id, ICR_STATUS) & flag) && time_out > 0) {
        i2c_status = i2c_readl(adap->i2c_id, ICR_STATUS);
        if (flag == IC_STATUS_TFE && ((i2c_status >> 2) & 0x1)) { /* Shift right by 2 bits */
            break;
        } else if (flag == IC_STATUS_RFNE && !((i2c_status >> 3) & 0x1)) { /* Shift right by 3 bits */
            break;
        }

        time_out--;
        udelay(1);
    }

    if (!time_out) {
        bm_log("i2c xfer timeout\n");
        return I2C_XFER_TIMEOUT;
    }

    return BM_OK;
}

static int i2c_dw_master_write(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    int ret;
    unsigned char val;

    while (msg->wcnt < msg->wlen) {
        if ((i2c_readl(adap->i2c_id, ICR_TXFLR) == 0) &&
            (((i2c_readl(adap->i2c_id, ICR_STATUS) >> 2) & 0x1) == 1)) { /* Shift right by 2 bits valid */
            val = msg->wbuf[msg->wcnt++];
            i2c_writel(adap->i2c_id, val, ICR_DATA_CMD);
            dsb();
        }

        if (msg->timeout == 0) {
            break;
        }

        if (msg->timeout > 0) {
            msg->timeout--;
            udelay(1);
        }
    }

    ret = bm_i2c_check_xfer_finish(adap, msg, IC_STATUS_TFE);

    return ret;
}

static int i2c_dw_master_read(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    int ret;
    unsigned char val;
    unsigned int cnt = 0;
    int rlimit = adap->rx_fifo_depth - i2c_readl(adap->i2c_id, ICR_RXFLR);
    while ((cnt < msg->rlen) && (rlimit > 0)) {
        i2c_writel(adap->i2c_id, IC_DATA_CMD_WRITE, ICR_DATA_CMD);
        cnt++;
        rlimit--;
    }

    while (msg->rcnt < msg->rlen) {
        if (i2c_readl(adap->i2c_id, ICR_RXFLR)) {
            val = (unsigned char)i2c_readl(adap->i2c_id, ICR_DATA_CMD);
            msg->rbuf[msg->rcnt++] = val;
            dsb();
            if (cnt < msg->rlen) {
                i2c_writel(adap->i2c_id, IC_DATA_CMD_WRITE, ICR_DATA_CMD);
                cnt++;
            }
        }

        if (msg->timeout == 0) {
            break;
        }

        if (msg->timeout > 0) {
            msg->timeout--;
            udelay(1);
        }
    }

    ret = bm_i2c_check_xfer_finish(adap, msg, IC_STATUS_RFNE);
    return ret;
}

static int i2c_xfer(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    int ret;

    if (i2c_wait_idle(adap)) {
        bm_log("Controller busy.");
        return BM_FAIL;
    }

    i2c_master_xfer_init(adap, msg);
    if (msg->flags & I2C_KMSG_RD) {
        ret = i2c_dw_master_read(adap, msg);
    } else {
        ret = i2c_dw_master_write(adap, msg);
    }
    return ret;
}

static int i2c_transaction(unsigned int i2c_id, struct i2c_kmsg *msg)
{
    struct i2c_adapter *adap = NULL;
    unsigned short max_address;
    int ret;

    max_address = (msg->flags & I2C_KMSG_TEN) ? I2C_10BIT_MAX_ADDR : I2C_7BIT_MAX_ADDR;
    if (msg->addr > max_address) {
        bm_log("addr is inval!\n");
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[i2c_id];

    i2c_adapter_set_msgptr(adap, msg);
    ret = i2c_xfer(adap, msg);
    i2c_adapter_set_msgptr(adap, NULL);

    return ret;
}

int bm_i2c_write(const struct i2c_xfer_info *write_info)
{
    struct i2c_kmsg *msg = NULL;
    struct i2c_adapter *adap = NULL;
    int ret;

    if (i2c_check_xfer_info(write_info)) {
        bm_log("i2c_write: write_info is inval!\n");
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[write_info->i2c_id];
    if (adap->init_flag != 1) {
        bm_log("i2c_write: i2c(%d) is not init\n");
        return BM_FAIL;
    }

    if (osal_sem_down_timeout(&(adap->sem), TIME_MS_50) != OSAL_SUCCESS) {
        return BM_FAIL;
    }

    msg = &g_i2c_msg[write_info->i2c_id];
    msg->addr = write_info->addr;
    msg->wbuf = write_info->buffer;
    msg->wlen = write_info->length;
    msg->rbuf = NULL;
    msg->flags = I2C_KMSG_WE;
    msg->timeout = (write_info->timeout * I2C_USEC_PER_MSEC);
    msg->timeout = msg->timeout ?: (3 * I2C_USEC_PER_SEC); /* The default timeout period is 3 seconds. */

    ret = i2c_transaction(write_info->i2c_id, msg);
    osal_sem_up(&(adap->sem));
    return ret;
}

int bm_i2c_read(const struct i2c_xfer_info *read_info)
{
    struct i2c_kmsg *msg = NULL;
    struct i2c_adapter *adap = NULL;
    int ret;

    if (i2c_check_xfer_info(read_info)) {
        bm_log("i2c_read: read_info is inval!\n");
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[read_info->i2c_id];
    if (adap->init_flag != 1) {
        bm_log("i2c_read: i2c(%d) is not init\n");
        return BM_FAIL;
    }

    if (osal_sem_down_timeout(&(adap->sem), TIME_MS_50) != OSAL_SUCCESS) {
        return BM_FAIL;
    }

    msg = &g_i2c_msg[read_info->i2c_id];
    msg->addr = read_info->addr;
    msg->wbuf = NULL;
    msg->rbuf = read_info->buffer;
    msg->rlen = read_info->length;
    msg->flags = I2C_KMSG_RD;
    msg->timeout = (read_info->timeout * I2C_USEC_PER_MSEC);
    msg->timeout = msg->timeout ?: (3 * I2C_USEC_PER_SEC); /* The default timeout period is 3 seconds. */

    ret = i2c_transaction(read_info->i2c_id, msg);
    osal_sem_up(&(adap->sem));
    return ret;
}

static int bm_i2c_irq_xfer_start(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    int core;
    unsigned int interrupt_mask = IC_INTR_RX_FULL | IC_INTR_TX_EMPTY | IC_INTR_STOP_DET;

    i2c_adapter_set_msgptr(adap, msg);

    if (i2c_wait_idle(adap)) {
        bm_log("Controller busy.");
        return BM_FAIL;
    }

    i2c_master_xfer_init(adap, msg);
    i2c_writel(adap->i2c_id, interrupt_mask, ICR_INTR_MASK);

    core = bm_get_coreid();
    osal_irq_set_affinity(adap->irqn, NULL, (1 << core));
    osal_irq_enable(adap->irqn);

    return BM_OK;
}

int bm_i2c_write_async(const struct i2c_xfer_info *write_info, bm_i2c_callback callback)
{
    int ret;
    struct i2c_adapter *adap = NULL;
    struct i2c_kmsg *msg;

    if (i2c_check_xfer_info(write_info)) {
        bm_log("i2c_write_async: write_info is inval!\n");
        return BM_FAIL;
    }

    if (callback == NULL) {
        bm_log("i2c_write_async: callback is NULL!\n");
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[write_info->i2c_id];
    if (adap->init_flag != 1) {
        bm_log("i2c_write_async: i2c(%d) is not init\n");
        return BM_FAIL;
    }

    if (osal_sem_down_timeout(&(adap->sem), TIME_MS_50) != OSAL_SUCCESS) {
        return BM_FAIL;
    }

    g_i2c_callback_array[adap->i2c_id] = callback;

    msg = &g_i2c_msg[adap->i2c_id];
    msg->addr = write_info->addr;
    msg->wbuf = write_info->buffer;
    msg->wlen = write_info->length;
    msg->wcnt = 0;
    msg->rbuf = NULL;
    msg->flags = I2C_KMSG_WE;

    ret = bm_i2c_irq_xfer_start(adap, msg);
    osal_sem_up(&(adap->sem));
    return ret;
}

int bm_i2c_read_async(const struct i2c_xfer_info *read_info, bm_i2c_callback callback)
{
    int ret;
    struct i2c_adapter *adap = NULL;
    struct i2c_kmsg *msg = NULL;

    if (i2c_check_xfer_info(read_info)) {
        bm_log("i2c_read_async: read_info is inval!\n");
        return BM_FAIL;
    }

    if (callback == NULL) {
        bm_log("i2c_read_async: callback is NULL!\n");
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[read_info->i2c_id];
    if (adap->init_flag != 1) {
        bm_log("i2c_read_async: i2c(%d) is not init\n");
        return BM_FAIL;
    }

    if (osal_sem_down_timeout(&(adap->sem), TIME_MS_50) != OSAL_SUCCESS) {
        return BM_FAIL;
    }

    g_i2c_callback_array[adap->i2c_id] = callback;

    msg = &g_i2c_msg[adap->i2c_id];
    msg->addr = read_info->addr;
    msg->wbuf = NULL;
    msg->rbuf = read_info->buffer;
    msg->rlen = read_info->length;
    msg->flags = I2C_KMSG_RD;

    ret = bm_i2c_irq_xfer_start(adap, msg);
    osal_sem_up(&(adap->sem));
    return ret;
}

static bm_dmac_req i2c_dma_get_dmac_req(int id, int is_read)
{
    if (id <= I2C_ID_1) {
        if (is_read == 1) {
            return (DMAC_REQ_SEL_I2C0_RX + id * IS_ODD);
        } else {
            return (DMAC_REQ_SEL_I2C0_TX + id * IS_ODD);
        }
    } else {
        if (is_read == 1) {
            return (DMAC_REQ_SEL_I2C2_RX + (id - IS_ODD) * IS_ODD);
        } else {
            return (DMAC_REQ_SEL_I2C2_TX + (id - IS_ODD) * IS_ODD);
        }
    }
}
static int i2c_dma_get_id_from_req(bm_dmac_req req)
{
    if (req <= DMAC_REQ_SEL_I2C1_RX) {
        return (req - DMAC_REQ_SEL_I2C0_TX) / IS_ODD;
    } else {
        return (req - DMAC_REQ_SEL_I2C2_TX) / IS_ODD;
    }
}

static void i2c_dma_finish_func(bm_dmac_req req, bm_dmac_transmit_async_t type)
{
    bm_i2c_callback i2c_callback = NULL;
    bm_i2c_transmit_async_t state = I2C_TRANSMIT_BUTT;
    unsigned int val = 0;
    int i2c_id = i2c_dma_get_id_from_req(req);
    i2c_writel(i2c_id, val, ICR_DMA_CR);

    if (req % IS_ODD == 0) {
        if (type != DMAC_TRANSMIT_OK) {
            state = I2C_WRITE_FAIL;
        } else {
            state = I2C_WRITE_OK;
        }
        i2c_callback = g_i2c_dma_tx_callback_array[i2c_id];
    } else {
        if (type != DMAC_TRANSMIT_OK) {
            state = I2C_READ_FAIL;
        } else {
            state = I2C_READ_OK;
        }
        i2c_callback = g_i2c_dma_rx_callback_array[i2c_id];
    }
    if (i2c_callback) {
        i2c_callback(i2c_id, state);
    }
}

static int i2c_dma_write(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    int ret;
    unsigned int val = 0;
    int dma_channel = i2c_dma_get_dmac_req(adap->i2c_id, 0);
    bm_dmac_channel_cfg channel_cfg = { 0 };

    /* Start the DMA transfer. */
    channel_cfg.req = dma_channel;
    channel_cfg.src_addr = (uintptr_t)(msg->wbuf);
    channel_cfg.dest_addr = (uintptr_t)(adap->regbase + ICR_DATA_CMD);
    channel_cfg.data_len = msg->wlen;
    channel_cfg.trans_type = TRASFER_TYPE_M2P;
    ret = bm_dmac_transmit_async(&channel_cfg, i2c_dma_finish_func);
    if (ret) {
        bm_log("i2c_write_dma fail!\n");
        return BM_FAIL;
    }

    val = i2c_readl(adap->i2c_id, ICR_DMA_CR);
    val |= IC_DMA_CR_TDMAE;
    i2c_writel(adap->i2c_id, val, ICR_DMA_CR);
    return ret;
}

static int i2c_dma_read(struct i2c_adapter *adap, struct i2c_kmsg *msg)
{
    int ret;
    unsigned int val = 0;
    int dma_channel = i2c_dma_get_dmac_req(adap->i2c_id, 1);
    bm_dmac_channel_cfg channel_cfg = { 0 };

    /* Start the DMA transfer. */
    channel_cfg.req = dma_channel;
    channel_cfg.src_addr = (uintptr_t)(adap->regbase + ICR_DATA_CMD);
    channel_cfg.dest_addr = (uintptr_t)(msg->rbuf);
    channel_cfg.data_len = msg->rlen;
    channel_cfg.trans_type = TRASFER_TYPE_P2M;
    ret = bm_dmac_transmit_async(&channel_cfg, i2c_dma_finish_func);
    if (ret) {
        bm_log("i2c_read_dma fail!\n");
        return BM_FAIL;
    }

    i2c_writel(adap->i2c_id, IC_DATA_CMD_WRITE, ICR_DATA_CMD);
    val = i2c_readl(adap->i2c_id, ICR_DMA_CR);
    val |= IC_DMA_CR_TDMAE;
    val |= IC_DMA_CR_RDMAE;
    i2c_writel(adap->i2c_id, val, ICR_DMA_CR);
    return ret;
}

int bm_i2c_write_dma(const struct i2c_xfer_info *write_info, bm_i2c_callback callback)
{
    int ret;
    struct i2c_adapter *adap = NULL;
    struct i2c_kmsg *msg;

    if (i2c_check_xfer_info(write_info)) {
        bm_log("i2c_write_dma: write_info is inval!\n");
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[write_info->i2c_id];
    if (adap->init_flag != 1) {
        bm_log("i2c_write_dma: i2c(%d) is not init\n");
        return BM_FAIL;
    }

    if (osal_sem_down_timeout(&(adap->sem), TIME_MS_50) != OSAL_SUCCESS) {
        return BM_FAIL;
    }

    g_i2c_dma_tx_callback_array[adap->i2c_id] = callback;

    msg = &g_i2c_msg[adap->i2c_id];
    msg->addr = write_info->addr;
    msg->wbuf = write_info->buffer;
    msg->wlen = write_info->length;
    msg->wcnt = 0;
    msg->rbuf = NULL;
    msg->flags = I2C_KMSG_WE;
    i2c_master_xfer_init(adap, msg);
    ret = i2c_dma_write(adap, msg);
    osal_sem_up(&(adap->sem));
    return ret;
}

int bm_i2c_read_dma(const struct i2c_xfer_info *read_info, bm_i2c_callback callback)
{
    int ret;
    struct i2c_adapter *adap = NULL;
    struct i2c_kmsg *msg = NULL;

    if (i2c_check_xfer_info(read_info)) {
        bm_log("i2c_read_dma: read_info is inval!\n");
        return BM_FAIL;
    }

    adap = &g_i2c_platdrv_adap_list[read_info->i2c_id];
    if (adap->init_flag != 1) {
        bm_log("i2c_read_dma: i2c(%d) is not init\n");
        return BM_FAIL;
    }

    if (osal_sem_down_timeout(&(adap->sem), TIME_MS_50) != OSAL_SUCCESS) {
        return BM_FAIL;
    }

    g_i2c_dma_rx_callback_array[adap->i2c_id] = callback;

    msg = &g_i2c_msg[adap->i2c_id];
    msg->addr = read_info->addr;
    msg->wbuf = NULL;
    msg->rbuf = read_info->buffer;
    msg->rlen = read_info->length;
    msg->flags = I2C_KMSG_RD;
    i2c_master_xfer_init(adap, msg);
    ret = i2c_dma_read(adap, msg);
    osal_sem_up(&(adap->sem));
    return ret;
}