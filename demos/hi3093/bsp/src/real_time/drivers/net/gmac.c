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

#include "gmac.h"
#include "mdio.h"
#include "motor_phy.h"
#include "gmac_regs.h"
#include "securec.h"
#include "bm_net.h"
#include "bm_cache.h"
#include "bm_common.h"
#include "bm_ops.h"
#include "io_config.h"
#include "hp_subsys.h"
#include "bm_atomic.h"

#if defined (__UNIPROTON__)
static int g_gmac_lock = 0;
#define GMAC_LOCK_SYNC &g_gmac_lock
#else
#include "hi309x_memmap.h"
#endif

static __attribute__((aligned(
    CPU_CACHE_LINE_SIZE))) unsigned char g_gmac_tx_buffer[GMAC_MAX_DEV][CHIP_TX_FIFO_NUM][RX_BUFFER_DEFAULT_SIZE];
static __attribute__((aligned(
    CPU_CACHE_LINE_SIZE))) unsigned char g_gmac_rx_buffer[GMAC_MAX_DEV][CHIP_RX_FIFO_NUM][RX_BUFFER_DEFAULT_SIZE];

static __attribute__((aligned(CPU_CACHE_LINE_SIZE))) unsigned char g_packet_buf[GMAC_MAX_DEV][RX_BUFFER_DEFAULT_SIZE];
__attribute__((aligned(CPU_CACHE_LINE_SIZE))) XFER_DATA unsigned char g_default_mac_addr[GMAC_MAX_DEV][MAC_ADDR_LEN] = {
    {0x8, 0x0, 0x8, 0x8, 0x8, 0x8},
    {0x8, 0x1, 0x8, 0x8, 0x8, 0x8},
    {0x8, 0x2, 0x8, 0x8, 0x8, 0x8},
    {0x8, 0x3, 0x8, 0x8, 0x8, 0x8},
};

static const int gmac_exist_array[ETH_BUTT] = {
    dt_device_exists(GMAC, GMAC0),
    dt_device_exists(GMAC, GMAC1),
    dt_device_exists(GMAC, GMAC2),
    dt_device_exists(GMAC, GMAC3),
};

static struct gmac_device gmac_device_tbl[GMAC_MAX_DEV] = {0};

static const gmac_phy_info_s hi_gmac_phy_info_table[] = {
    {
        "YT8521",
        IDENTIFIER_YT8521,
        phy_yt8521_init,
        phy_yt8521_get_speed_duplex,
        phy_yt85xx_get_link_status,
    },
    {
        "YT8512",
        IDENTIFIER_YT8512,
        phy_yt8512_init,
        phy_yt8512_get_speed_duplex,
        phy_yt85xx_get_link_status,
    },
};

static inline void gmac_mdelay(unsigned long msec)
{
    volatile unsigned long long usec = msec * 1000;
    udelay(usec);
}

static unsigned int gmac_get_ddr_relative_addr(unsigned int dir, unsigned int gmac_id, unsigned long long phy_addr)
{
    unsigned int reg_addr;
    unsigned long long mac_haddr;
    unsigned long long relative_addr;
    u_hp_mac_haddr_extend hp_mac_haddr_extend;
    unsigned long long mx_haddr_offset = 0, mx_haddr_hi = 0;
    unsigned int mx_addr_remap_mode;

    reg_addr = HP_SUB_CSR_HP_MAC_HADDR_EXTEND_0_ADDR + (gmac_id * 0x4);
    reg_read(reg_addr, hp_mac_haddr_extend.u32);

    if (dir == GMAC_TX) {
        mx_addr_remap_mode = hp_mac_haddr_extend.bits.m1_addr_remap_mode;
        mx_haddr_offset = hp_mac_haddr_extend.bits.m1_haddr_offset;
        mx_haddr_hi = hp_mac_haddr_extend.bits.m1_haddr_hi;
    } else {
        mx_addr_remap_mode = hp_mac_haddr_extend.bits.m0_addr_remap_mode;
        mx_haddr_offset = hp_mac_haddr_extend.bits.m0_haddr_offset;
        mx_haddr_hi = hp_mac_haddr_extend.bits.m0_haddr_hi;
    }

    switch (mx_addr_remap_mode) {
        case 0x1:
            mac_haddr = (mx_haddr_offset << 28); // Move 28 to the left to obtain the physical address.
            break;
        case 0x2:
            mac_haddr = (mx_haddr_hi << 32); // Move left by 32 to obtain the physical address.
            break;
        default:
            mac_haddr = 0;
            break;
    }

    if (phy_addr < mac_haddr) {
        return (unsigned int)phy_addr;
    }

    relative_addr = phy_addr - mac_haddr;

    if ((relative_addr >> 32) != 0) { /* Shift right by 32 bits */
        return (unsigned int)phy_addr;
    }

    return (unsigned int)relative_addr;
}

static void gmac_init_extend_addr(unsigned int gmac_id)
{
    unsigned int reg_addr;
    u_hp_mac_haddr_extend hp_mac_haddr_extend;

    reg_addr = HP_SUB_CSR_HP_MAC_HADDR_EXTEND_0_ADDR + (gmac_id * 0x4);

    /* By default, the tx&rx uses address mapping mode 1 and the address offset is 0x80000000. */
    hp_mac_haddr_extend.u32 = 0;
    hp_mac_haddr_extend.bits.m0_haddr_offset = 0x8;
    hp_mac_haddr_extend.bits.m0_addr_remap_mode = 0x1;
    hp_mac_haddr_extend.bits.m1_haddr_offset = 0x8;
    hp_mac_haddr_extend.bits.m1_addr_remap_mode = 0x1;
    reg_write(reg_addr, hp_mac_haddr_extend.u32);

    return;
}

static int gmac_set_hw_mac_addr(unsigned int mac_num)
{
    if (mac_num >= GMAC_MAX_DEV) {
        return BM_FAIL;
    }

    const unsigned char* mac_addr_ptr = g_default_mac_addr[mac_num];
    unsigned int mac_addr_low, mac_addr_hi;

    /* High address:add[0]Shift left by 8 bits + add[1] */
    mac_addr_hi = (mac_addr_ptr[0] << 8) | (mac_addr_ptr[1]);
    /* High address:add[2]Shift leftwards by 24 bits + add[3]Move Left 16 +  add[4]Move Left 8 + add[5] */
    mac_addr_low = (mac_addr_ptr[2] << 24) | (mac_addr_ptr[3] << 16) | (mac_addr_ptr[4] << 8) | (mac_addr_ptr[5]);

    /* To set the MAC address of a port */
    mac_reg_write(mac_num, MAC_CORE_STATION_ADDR_HIGH_2_REG, mac_addr_hi);
    mac_reg_write(mac_num, MAC_CORE_STATION_ADDR_LOW_2_REG, mac_addr_low);

    mac_reg_write(mac_num, MAC_CORE_STATION_ADDR_LOW_MSK_0_REG, 0xFFFFFFFF);
    mac_reg_write(mac_num, MAC_CORE_STATION_ADDR_HIGH_MSK_0_REG, 0xFFFFFFFF);

    mac_reg_write(mac_num, MAC_CORE_STATION_ADDR_LOW_MSK_1_REG, 0xFFFFFFFF);
    mac_reg_write(mac_num, MAC_CORE_STATION_ADDR_HIGH_MSK_1_REG, 0xFFFFFFFF);

    return BM_OK;
}

static int gmac_hw_set_tx_pad_add(unsigned int port, unsigned int is_enable)
{
    u_transmit_control trans_ctrl_reg;
    unsigned int cmp_val;

    if (port >= GMAC_MAX_DEV) {
        return BM_FAIL;
    }

    if (is_enable > 1) {
        return BM_FAIL;
    }

    mac_reg_read(port, MAC_CORE_TRANSMIT_CONTROL_REG, trans_ctrl_reg.u32);
    trans_ctrl_reg.bits.pad_enable = is_enable;
    mac_reg_write(port, MAC_CORE_TRANSMIT_CONTROL_REG, trans_ctrl_reg.u32);

    mac_reg_read(port, MAC_CORE_TRANSMIT_CONTROL_REG, cmp_val);
    if (cmp_val != trans_ctrl_reg.u32) {
        return BM_FAIL;
    }

    return BM_OK;
}

static int gmac_rx_init_ring(struct gmac_device *priv)
{
    int ret;
    unsigned char i;
    unsigned char *tmp_buf[CHIP_RX_FIFO_NUM] = { NULL };

    for (i = 0; i < CHIP_RX_FIFO_NUM; i++) {
        if (priv->rx_data_ring[i] == NULL) {
            tmp_buf[i] = g_gmac_rx_buffer[priv->mac_num][i];
            if (tmp_buf[i] == NULL) {
                bm_debug(BM_ERROR_LEVEL, "error: malloc rx ring buffer failed %d\n", i);
                goto EXIT;
            }

            ret = memset_s(tmp_buf[i], RX_BUFFER_DEFAULT_SIZE, 0x0, RX_BUFFER_DEFAULT_SIZE);
            if (ret != BM_OK) {
                return BM_FAIL;
            }

            priv->rx_data_ring[i] = (unsigned char *)(((unsigned long)tmp_buf[i] + (CPU_CACHE_LINE_SIZE - 1)) &
                                                      (~(CPU_CACHE_LINE_SIZE - 1)));
        }
    }

    return BM_OK;

EXIT:
    for (i = 0; i < CHIP_RX_FIFO_NUM; i++) {
        if (tmp_buf[i]) {
            priv->rx_data_ring[i] = NULL;
        }
    }

    return BM_FAIL;
}

static int gmac_tx_init_ring(struct gmac_device *priv)
{
    int ret;
    unsigned char i;
    unsigned char *tmp_buf[CHIP_TX_FIFO_NUM] = { NULL };

    for (i = 0; i < CHIP_TX_FIFO_NUM; i++) {
        if (priv->tx_data_ring[i] == NULL) {
            tmp_buf[i] = g_gmac_tx_buffer[priv->mac_num][i];
            if (tmp_buf[i] == NULL) {
                bm_debug(BM_ERROR_LEVEL, "error: malloc tx ring buffer failed %d\n", i);
                goto EXIT;
            }
            ret = memset_s(tmp_buf[i], RX_BUFFER_DEFAULT_SIZE, 0x0, RX_BUFFER_DEFAULT_SIZE);
            if (ret != BM_OK) {
                return BM_FAIL;
            }
            priv->tx_data_ring[i] = (unsigned char *)(((unsigned long)tmp_buf[i] + (CPU_CACHE_LINE_SIZE - 1)) &
                                                      (~(CPU_CACHE_LINE_SIZE - 1)));
        }
    }

    return BM_OK;

EXIT:
    for (i = 0; i < CHIP_TX_FIFO_NUM; i++) {
        if (tmp_buf[i]) {
            priv->tx_data_ring[i] = NULL;
        }
    }

    return BM_FAIL;
}

static void gmac_rx_fill_all_buff(unsigned int port, struct gmac_device *priv)
{
    unsigned int i;
    unsigned int relative_addr;

    mac_reg_write(port, PCU_RX_BUF_SIZE_REG, RX_BUFFER_DEFAULT_SIZE);

    /* No delay operation is required. */
    for (i = 0; i < CHIP_RX_FIFO_NUM; i++) {
        relative_addr = gmac_get_ddr_relative_addr(GMAC_RX, port, (unsigned long long)priv->rx_data_ring[i]);
        mac_reg_write(port, PCU_RX_CFF_ADDR_REG, relative_addr);
    }

    return;
}

static int gmac_hw_set_pkt_store_format(unsigned int port, const femac_pkt_store_ctrl_cfg *pst_rx_ctrl)
{
    u_rx_ctrl rx_ctrl_reg;
    unsigned int cmp_val;

    if (!pst_rx_ctrl) {
        bm_debug(BM_ERROR_LEVEL, "input rx ctrl parameter NULL!\n");
        return BM_FAIL;
    }

    mac_reg_read(port, PCU_RX_CTRL_REG, rx_ctrl_reg.u32);

    /* Sets the RX control information. */
    rx_ctrl_reg.bits.cf_rxbuf_1st_skip_size = pst_rx_ctrl->cf_buf_1st_skip1;
    rx_ctrl_reg.bits.cf_rxbuf_no_1st_skip_size = pst_rx_ctrl->cf_buf_n1st_skip;

    /* Disabling timestamp reporting */
    rx_ctrl_reg.bits.cf_time_inf_en = 0;
    rx_ctrl_reg.bits.cf_rx_align_num = pst_rx_ctrl->cf_rx_align_num;

    /* If the address generation mode is set to 0, the upper bits of the group are configured by the register. */
    rx_ctrl_reg.bits.cf_addr_mode = 0;
    rx_ctrl_reg.bits.cf_cache_line_h = 0;
    rx_ctrl_reg.bits.cf_cache_line = pst_rx_ctrl->cf_cache_line;
    rx_ctrl_reg.bits.cf_rxbuf_1st_skip_size2 = pst_rx_ctrl->cf_rxbuf_1st_skip_size2;

    mac_reg_write(port, PCU_RX_CTRL_REG, rx_ctrl_reg.u32);

    mac_reg_read(port, PCU_RX_CTRL_REG, cmp_val);
    if (cmp_val != rx_ctrl_reg.u32) {
        return BM_FAIL;
    }

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_set_pkt_input_parse_mode
 Function Description  : Setting and querying packet input and parsing modes
 Return value  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_hw_set_pkt_input_parse_mode(unsigned int port, const femac_rx_pkt_mode_cfg *pst_rx_pkt_mode)
{
    u_rx_pkt_mode rx_mode_reg;
    u_mac_skip_len mac_skip_len_reg;
    unsigned int cmp_val;

    if (!pst_rx_pkt_mode) {
        bm_debug(BM_ERROR_LEVEL, " packet mode parameter NULL!\n");
        return BM_FAIL;
    }

    /* To set the packet input mode */
    mac_reg_read(port, PCU_RX_PKT_MODE_REG, rx_mode_reg.u32);
    rx_mode_reg.bits.cf_instr_head_mode = pst_rx_pkt_mode->cf_instr_head_mode;
    rx_mode_reg.bits.cf_skip_len = pst_rx_pkt_mode->cf_skip_len;
    rx_mode_reg.bits.cf_parse_mode = pst_rx_pkt_mode->cf_parse_mode;
    rx_mode_reg.bits.cf_gen_id = 0;
    mac_reg_write(port, PCU_RX_PKT_MODE_REG, rx_mode_reg.u32);

    mac_reg_read(port, PCU_RX_PKT_MODE_REG, cmp_val);
    if (cmp_val != rx_mode_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, "cmp_val != rx_mode_reg.u32\n");
        return BM_FAIL;
    }

    /* The length of the field in the packet header is not parsed. CRC is performed if the field is not parsed. */
    mac_reg_read(port, MAC_CORE_MAC_SKIP_LEN_REG, mac_skip_len_reg.u32);
    mac_skip_len_reg.bits.mac_skip_len = pst_rx_pkt_mode->mac_skip_len;
    mac_skip_len_reg.bits.mac_skip_crc = 1;
    mac_reg_write(port, MAC_CORE_MAC_SKIP_LEN_REG, mac_skip_len_reg.u32);

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_set_max_frame_len
 Function Description  : Setting and obtaining the maximum frame length received by a port
 Return value  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_hw_set_max_frame_len(unsigned int port, unsigned int frame_size)
{
    u_max_frm_size max_frm_size_reg;

    if (frame_size >= 0x3FFF) {
        return BM_FAIL;
    }

    mac_reg_read(port, MAC_CORE_MAX_FRM_SIZE_REG, max_frm_size_reg.u32);
    max_frm_size_reg.bits.max_frm_size = frame_size;
    mac_reg_write(port, MAC_CORE_MAX_FRM_SIZE_REG, max_frm_size_reg.u32);

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_set_short_frame_len
 Function Description  : Setting and obtaining the minimum frame length received by a port
 Return value  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_hw_set_short_frame_len(unsigned int port, unsigned int frame_size)
{
    u_short_runts_thr short_frm_thr_reg;

    if (frame_size > 0x1f) {
        bm_debug(BM_ERROR_LEVEL, "the value of frame_size is invalid, frame_size = %u\n", frame_size);
        return BM_FAIL;
    }

    mac_reg_read(port, MAC_CORE_SHORT_RUNTS_THR_REG, short_frm_thr_reg.u32);
    short_frm_thr_reg.bits.short_runts_thr = frame_size;
    mac_reg_write(port, MAC_CORE_SHORT_RUNTS_THR_REG, short_frm_thr_reg.u32);

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_set_sgmii_an_enable
 Function Description  : 设置SGMII自协商使能，参见用户指南
 Return value  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_hw_set_sgmii_an_enable(unsigned int mac_num)
{
    u_transmit_control trans_ctrl_reg;
    unsigned int tmp;

    mac_reg_write(mac_num, MAC_CORE_TX_LOCAL_PAGE, 0x1);

    mac_reg_write(mac_num, MAC_CORE_SIXTEEN_BIT_CNTR, 0x3F);

    mac_reg_write(mac_num, MAC_CORE_LD_LINK_COUNTER, 0x1);

    mac_reg_read(mac_num, MAC_CORE_TRANSMIT_CONTROL_REG, trans_ctrl_reg.u32);
    trans_ctrl_reg.bits.an_enable = 1;
    mac_reg_write(mac_num, MAC_CORE_TRANSMIT_CONTROL_REG, trans_ctrl_reg.u32);

    gmac_mdelay(20); /* delay 20ms */

    mac_reg_read(mac_num, MAC_CORE_AN_NEG_STATE, tmp);

    /* Bit 21 determines the SDS negotiation completion status. */
    if ((tmp & (1 << 21)) == 0) {
        return BM_FAIL;
    }

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_rx_enable
 Function Description  : Enable the receive channel of the GMAC.
 Return value  : void
**************************************************************************** */
static void gmac_hw_rx_enable(unsigned int port, unsigned int is_enable)
{
    u_port_en port_en_reg;
    unsigned int cmp_val;

    /* Reads the RX channel status. */
    mac_reg_read(port, MAC_CORE_PORT_EN_REG, port_en_reg.u32);
    port_en_reg.bits.rx_en = is_enable;
    mac_reg_write(port, MAC_CORE_PORT_EN_REG, port_en_reg.u32);

    mac_reg_read(port, MAC_CORE_PORT_EN_REG, cmp_val);
    if (cmp_val != port_en_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, " cmp_val != port_en_reg.u32\n");
    }
}

/* ****************************************************************************
 Function Name  : gmac_hw_tx_enable
 Function Description  : Enable the GMAC TX channel.
 Return value  : void
**************************************************************************** */
static void gmac_hw_tx_enable(unsigned int port, unsigned int is_enable)
{
    u_port_en port_en_reg;
    unsigned int cmp_val;

    /* Reads the TX channel status. */
    mac_reg_read(port, MAC_CORE_PORT_EN_REG, port_en_reg.u32);
    port_en_reg.bits.tx_en = is_enable;
    mac_reg_write(port, MAC_CORE_PORT_EN_REG, port_en_reg.u32);

    mac_reg_read(port, MAC_CORE_PORT_EN_REG, cmp_val);
    if (cmp_val != port_en_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, "cmp_val != port_en_reg.u32\n");
    }
}

static int gmac_hw_set_port_mode(unsigned int port, unsigned int mode, unsigned int duplex)
{
    u_duplex_type duplex_type_reg;
    u_mode_change_en mode_change_en_reg;
    u_port_mode port_mode_reg;
    unsigned int cmp_val;

    /* Disable the TX and RX channels. */
    gmac_hw_tx_enable(port, DISABLE);
    gmac_hw_rx_enable(port, DISABLE);

    mac_reg_read(port, MAC_CORE_DUPLEX_TYPE, duplex_type_reg.u32);
    duplex_type_reg.bits.duplex_sel = duplex;
    mac_reg_write(port, MAC_CORE_DUPLEX_TYPE, duplex_type_reg.u32);

    mac_reg_read(port, MAC_CORE_DUPLEX_TYPE, cmp_val);
    if (cmp_val != duplex_type_reg.u32) {
        return BM_FAIL;
    }

    /* Set the port mode, that is, set the speed. */
    mac_reg_read(port, MAC_CORE_PORT_MODE_REG, port_mode_reg.u32);
    port_mode_reg.bits.port_mode = mode;

    mac_reg_write(port, MAC_CORE_PORT_MODE_REG, port_mode_reg.u32);
    mac_reg_read(port, MAC_CORE_PORT_MODE_REG, cmp_val);
    if (cmp_val != port_mode_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, " cmp_val != port_mode_reg.u32\n");
        return BM_FAIL;
    }

    mac_reg_read(port, MAC_CORE_MODE_CHANGE_EN_REG, mode_change_en_reg.u32);
    mode_change_en_reg.bits.mode_change_en = 1;
    mac_reg_write(port, MAC_CORE_MODE_CHANGE_EN_REG, mode_change_en_reg.u32);

    mac_reg_read(port, MAC_CORE_MODE_CHANGE_EN_REG, cmp_val);
    if (cmp_val != mode_change_en_reg.u32) {
        return BM_FAIL;
    }

    return BM_OK;
}

static void gmac_config_mode(unsigned int port)
{
    unsigned int times = 0;
    unsigned int val = 0;

    if ((port == ETH0) || (port == ETH1)) {
        gmac_hw_set_port_mode(port, MAC_100M_RMII, GMAC_DUPLEX_FULL);
    } else {
        gmac_hw_set_port_mode(port, MAC_1000M_SGMII, GMAC_DUPLEX_FULL);
        gmac_hw_set_sgmii_an_enable(port);
        do {
            mac_reg_read(port, MAC_CORE_AN_NEG_STATE, val);

            if (times >= 200) { /* Maximum number of cycles: 200 */
                break;
            }

            gmac_mdelay(5); /* delay 5ms */
            times++;
        } while ((val & (1 << 21)) == 0); /* Check whether the negotiation of bit 21 is complete. */
    }
}

static void gmac_hw_set_pcu_ctrl(unsigned int port)
{
    u_bus_ctrl bus_ctrl_reg;

    /* Set the system to big-endian, control word to big-endian, and data to big-endian. */
    mac_reg_read(port, PCU_BUS_CTRL_REG, bus_ctrl_reg.u32);
    bus_ctrl_reg.bits.cf_pd_buf = 0;
    bus_ctrl_reg.bits.huser = 1;       /* Indicates whether the hardware is required to
                                        ensure cache consistency. 1: yes */
    bus_ctrl_reg.bits.noncoh_en = 0;   /* Indicates whether the cache line to be released needs to be written.
                                       Clears the content in the L1 cache. 0: no.         */
    bus_ctrl_reg.bits.cf_buf = 1;      /* Indicates whether the bus write operation is bufferable. 1: bufferable. */
    bus_ctrl_reg.bits.cf_endian = 0x3;   /* 0x3: The control word is little-endian, and the data is big-endian. */
    bus_ctrl_reg.bits.cf_rx_alloc = 1; /* Indicates whether the write operation of the MAC cache is
                                        write allocation. 1: write allocation */
    mac_reg_write(port, PCU_BUS_CTRL_REG, bus_ctrl_reg.u32);
}

/* ****************************************************************************
 函 数 名  : gmac_hw_set_port_mode_all
 功能描述  : 端口模式相关参数的设置及查询
 返 回 值  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_hw_set_port_mode_all(unsigned int port, const femac_port_mode_cfg *port_mode)
{
    u_rx_ctrl rx_ctrl_reg = {0};
    u_max_frm_size max_frm_size_reg = {0};
    u_short_runts_thr short_frm_thr_reg = {0};
    u_transmit_control trans_ctrl_reg = {0};
    u_recv_control recv_ctrl_reg = {0};
    u_mode_change_en mode_change_en_reg = {0};
    unsigned int cmp_val = 0;

    gmac_config_mode(port);

    mac_reg_read(port, PCU_RX_CTRL_REG, rx_ctrl_reg.u32);
    rx_ctrl_reg.bits.cf_port_num = port_mode->port_num;
    mac_reg_write(port, PCU_RX_CTRL_REG, rx_ctrl_reg.u32);
    mac_reg_read(port, PCU_RX_CTRL_REG, cmp_val);
    if (cmp_val != rx_ctrl_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, " cmp_val != rx_ctrl_reg.u32\n");
        return BM_FAIL;
    }

    mac_reg_read(port, MAC_CORE_MODE_CHANGE_EN_REG, mode_change_en_reg.u32);
    mode_change_en_reg.bits.mode_change_en = 1;
    mac_reg_write(port, MAC_CORE_MODE_CHANGE_EN_REG, mode_change_en_reg.u32);

    gmac_hw_set_pcu_ctrl(port);

    /* Sets the maximum frame length. */
    max_frm_size_reg.bits.max_frm_size = port_mode->max_frm_size;

    /* Short Frame/Extra Short Frame Limits */
    short_frm_thr_reg.bits.short_runts_thr = port_mode->short_runts_thr;

    bm_debug(BM_DEBUG_LEVEL, "short_frm_thr_reg.u32 = %u\n", short_frm_thr_reg.u32);

    /* Sets the maximum frame length. */
    if (gmac_hw_set_max_frame_len(port, max_frm_size_reg.u32)) {
        bm_debug(BM_ERROR_LEVEL, " gmac_hw_set_max_frame_len fail\n");
        return BM_FAIL;
    }

    /* Short Frame/Extra Short Frame Limits */
    if (gmac_hw_set_short_frame_len(port, short_frm_thr_reg.u32)) {
        bm_debug(BM_ERROR_LEVEL, "gmac_hw_set_short_frame_len fail\n");
        return BM_FAIL;
    }

    /* TX pad/FCS/auto-negotiation enable */
    mac_reg_read(port, MAC_CORE_TRANSMIT_CONTROL_REG, trans_ctrl_reg.u32);
    trans_ctrl_reg.bits.pad_enable = port_mode->pad_enable;
    trans_ctrl_reg.bits.crc_add = port_mode->crc_add;
    trans_ctrl_reg.bits.an_enable = port_mode->an_enable;

    /* TX pad/FCS/auto-negotiation enable */
    mac_reg_write(port, MAC_CORE_TRANSMIT_CONTROL_REG, trans_ctrl_reg.u32);

    /* Transparent transmission of undersized frames/receive PAD stripping enable */
    mac_reg_read(port, MAC_CORE_RECV_CONTROL_REG, recv_ctrl_reg.u32);
    recv_ctrl_reg.bits.runt_pkt_en = port_mode->runt_pkt_en;
    recv_ctrl_reg.bits.strip_pad_en = port_mode->strip_pad_en;
    /* Transparent transmission of undersized frames/receive PAD stripping enable */
    mac_reg_write(port, MAC_CORE_RECV_CONTROL_REG, recv_ctrl_reg.u32);

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_set_mac_addr_filter
 Function Description  : Network port filtering rule setting
**************************************************************************** */
static int gmac_hw_set_mac_addr_filter(unsigned int port, const femac_mac_filter_cfg *pst_mac_filter)
{
    u_rec_filt_control filt_ctrl_reg;
    unsigned int cmp_val;

    if (!pst_mac_filter) {
        bm_debug(BM_ERROR_LEVEL, "input mac filter parameter NULL!\n");
        return BM_FAIL;
    }

    /* Sets the filtering rule, including discarding non-local unicast frames,
    non-local multicast frames, pause frames, and CRC error frames. */
    mac_reg_read(port, MAC_CORE_REC_FILT_CONTROL_REG, filt_ctrl_reg.u32);
    filt_ctrl_reg.bits.crc_err_pass = pst_mac_filter->crc_err_pass;
    filt_ctrl_reg.bits.pause_frm_pass = pst_mac_filter->pause_frm_pass;
    filt_ctrl_reg.bits.bc_drop_en = pst_mac_filter->bc_drop_en;
    filt_ctrl_reg.bits.mc_match_en = pst_mac_filter->mc_match_en;
    filt_ctrl_reg.bits.uc_match_en = pst_mac_filter->uc_match_en;
    mac_reg_write(port, MAC_CORE_REC_FILT_CONTROL_REG, filt_ctrl_reg.u32);

    mac_reg_read(port, MAC_CORE_REC_FILT_CONTROL_REG, cmp_val);
    if (cmp_val != filt_ctrl_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, "cmp_val != filt_ctrl_reg.u32\n");
    }

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_set_rx_max_frame_len
 Function Description  : Sets the maximum number of packets allowed in the receive direction.
    If the number of packets exceeds the maximum value, the excess packets will be truncated.
 Return value  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_hw_set_rx_max_frame_len(unsigned int port, unsigned int frame_len)
{
    u_rx_max_frame_len rx_max_frm_len_reg;
    unsigned int cmp_val;

    if (frame_len >= 0xFFFF) {
        bm_debug(BM_ERROR_LEVEL, "the value of frame_len is invalid\n");
        return BM_FAIL;
    }

    mac_reg_read(port, PCU_MAX_FRAME_LEN_REG, rx_max_frm_len_reg.u32);
    rx_max_frm_len_reg.bits.max_frame_len = frame_len;
    mac_reg_write(port, PCU_MAX_FRAME_LEN_REG, rx_max_frm_len_reg.u32);

    mac_reg_read(port, PCU_MAX_FRAME_LEN_REG, cmp_val);
    if (cmp_val != rx_max_frm_len_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, " cmp_val != rx_max_frm_len_reg.u32\n");
        return BM_FAIL;
    }

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_set_bmu_close
 Function Description  : Disabling the Function for the MAC Chip to Obtain Memory from the BMU
 Return value  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_hw_set_bmu_close(unsigned int port)
{
    u_rx_ctrl rx_ctrl_reg;
    unsigned int cmp_val;

    mac_reg_read(port, PCU_RX_CTRL_REG, rx_ctrl_reg.u32);
    rx_ctrl_reg.bits.cf_rx_get_addr_mode = 1; /* The Ethernet port must be set to 1 to
                                                passively receive the bus write address. */
    mac_reg_write(port, PCU_RX_CTRL_REG, rx_ctrl_reg.u32);

    mac_reg_read(port, PCU_RX_CTRL_REG, cmp_val);
    if (cmp_val != rx_ctrl_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, " cmp_val != rx_ctrl_reg.u32\n");
        return BM_FAIL;
    }

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_hw_set_poe_close
 Function Description  : Disable the PoE-related functions of the MAC chip.
 Return value  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_hw_set_poe_close(unsigned int port)
{
    u_we_addr work_entry_reg;
    unsigned int cmp_val;

    mac_reg_read(port, PCU_WE_ADDR_REG, work_entry_reg.u32);
    work_entry_reg.bits.cf_we_enable = 0;
    mac_reg_write(port, PCU_WE_ADDR_REG, work_entry_reg.u32);

    mac_reg_read(port, PCU_WE_ADDR_REG, cmp_val);
    if (cmp_val != work_entry_reg.u32) {
        return BM_FAIL;
    }

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : gmac_set_rx_fifo_threshold
 Function Description  : To set the TX FIFO threshold and RX FIFO threshold
 Return value  : BM_OK/BM_FAIL
**************************************************************************** */
static int gmac_set_rx_fifo_threshold(unsigned int port)
{
    u_cf_cfg_fifo_thrsld fifo_thrsld_reg;
    unsigned int cmp_val;

    mac_reg_read(port, PCU_CF_CFG_FIFO_THRSLD_REG, fifo_thrsld_reg.u32);
    fifo_thrsld_reg.bits.cfg_rx_p_full_th = FE_RX_FIFO_FULL_THRSHLD;
    fifo_thrsld_reg.bits.cfg_rx_p_empty_th = FE_RX_FIFO_EMPTY_THRSHLD;
    mac_reg_write(port, PCU_CF_CFG_FIFO_THRSLD_REG, fifo_thrsld_reg.u32);

    mac_reg_read(port, PCU_CF_CFG_FIFO_THRSLD_REG, cmp_val);
    if (cmp_val != fifo_thrsld_reg.u32) {
        return BM_FAIL;
    }

    return BM_OK;
}

static int gmac_set_tx_fifo_threshold(unsigned int port)
{
    u_cf_tx_fifo_thrsld tx_fifo_reg;
    unsigned int cmp_val;

    mac_reg_read(port, PCU_CF_TX_FIFO_THRSLD_REG, tx_fifo_reg.u32);

    tx_fifo_reg.bits.cf_tx_p_full_th = 0x300;
    tx_fifo_reg.bits.cf_tx_p_empty_th = 0x80;

    mac_reg_write(port, PCU_CF_TX_FIFO_THRSLD_REG, tx_fifo_reg.u32);

    mac_reg_read(port, PCU_CF_TX_FIFO_THRSLD_REG, cmp_val);
    if (cmp_val != tx_fifo_reg.u32) {
        return BM_FAIL;
    }

    return BM_OK;
}

static int gmac_hw_int_int(unsigned int port)
{
    u_cf_intrpt_msk int_mask_reg = {0};
    u_cf_intrpt_clr int_clr_reg;
    unsigned int cmp_val;

    int_mask_reg.bits.cf_sram_parity_err_int_msk = DISABLE;
    int_mask_reg.bits.cf_we_err_int_msk = DISABLE;
    int_mask_reg.bits.cf_rbreq_err_int_msk = DISABLE;
    int_mask_reg.bits.cf_mac_ff_err_int_msk = DISABLE;
    int_mask_reg.bits.cf_ahb_err_int_msk = DISABLE;
    int_mask_reg.bits.cf_pkg_intrpt_msk = DISABLE;

    /* In actual use, the packet loss interrupt should be disabled.
    During the test, it is recommended that the interrupt be disabled. */
    int_mask_reg.bits.cf_rx_drop_int_msk = DISABLE;

    /* In actual use, the transmit packet loss interrupt should be disabled.
    During the test, it is recommended that the transmit packet loss interrupt be enabled. */
    int_mask_reg.bits.cf_tx_drop_int_msk = DISABLE;
    int_mask_reg.bits.cf_txcfg_avl_msk = DISABLE;
    int_mask_reg.bits.cf_rel_buf_msk = DISABLE;
    int_mask_reg.bits.cf_buf_avl_msk = DISABLE;
    int_mask_reg.bits.cf_sram_parity_err_int_msk = DISABLE;

    /* Updates the aggregated value of RX interrupts. 0x1 indicates the number of configured interrupts. */
    int_mask_reg.bits.cf_intrpt_pkt = 0x1;

    /* 0xF: set time */
    int_mask_reg.bits.cf_intrpt_time = 0xF;
    mac_reg_write(port, PCU_CF_INTRPT_MSK_REG, int_mask_reg.u32);

    /* Readback check */
    mac_reg_read(port, PCU_CF_INTRPT_MSK_REG, cmp_val);
    if (cmp_val != int_mask_reg.u32) {
        bm_debug(BM_ERROR_LEVEL, "PCU_CF_INTRPT_MSK_REG error!\n");
    }

    /* Clearing Rx Interrupts */
    int_clr_reg.u32 = 0;
    int_clr_reg.bits.cf_rx_frm_clr = 1;
    mac_reg_write(port, PCU_CF_INTRPT_CLR_REG, int_clr_reg.u32);

    return BM_OK;
}

static void gmac_hw_ptp_reset(void)
{
    unsigned int rst_mask;

    rst_mask = (0x1U << 29); /* Set bit 29 to 1 to perform PTP reset. */
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_REQ1_ADDR);

    gmac_mdelay(20); /* delay 20ms */

    rst_mask = (0x1U << 29); /* Set bit 29 to 1 to perform PTP reset. */
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_DIS1_ADDR);

    return;
}

static void gmac_hw_core_reset(unsigned int mac_num)
{
    unsigned int rst_mask;

    /* Soft reset of the MAC core */
    rst_mask = (0x1U << (GMAC_MAC_CORE_SRST_BIT + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_REQ1_ADDR);

    rst_mask = (0x1U << (1 + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_REQ1_ADDR);

    /* Bus interface soft reset */
    rst_mask = (0x1U << (GMAC_BUS_SYS_SRST_BIT + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_REQ1_ADDR);

    /* Soft reset of the working clock */
    rst_mask = (0x1U << (0 + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_REQ1_ADDR);

    /* Soft reset of the mac_core configuration register */
    rst_mask = (0x1U << (GMAC_REG_CFG_SRST_BIT + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_REQ1_ADDR);

    if ((mac_num == ETH2) || (mac_num == ETH3)) {
        rst_mask = (0x1U << (GMAC_TX_SERDES_SRST_BIT + mac_num * GMAC_SRST_BIT));
        bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_REQ1_ADDR);
    }

    /* Warm reset of the RX SERDES clock */
    rst_mask = (0x1U << (GMAC_RX_SERDES_SRST_BIT + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_REQ1_ADDR);

    gmac_mdelay(20); /* delay 20ms */

    /* MAC core reset deassertion */
    rst_mask = (0x1U << (GMAC_MAC_CORE_SRST_BIT + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_DIS1_ADDR);

    /* Bus interface soft reset */
    rst_mask = (0x1U << (GMAC_BUS_SYS_SRST_BIT + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_DIS1_ADDR);

    /* Soft reset of the working clock */
    rst_mask = (0x1U << (0 + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_DIS1_ADDR);

    rst_mask = (0x1U << (1 + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_DIS1_ADDR);

    /* Soft reset of the mac_core configuration register */
    rst_mask = (0x1U << (GMAC_REG_CFG_SRST_BIT + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_DIS1_ADDR);

    /* TX SERDES clock reset deassertion */
    if ((mac_num == ETH2) || (mac_num == ETH3)) {
        rst_mask = (0x1U << (GMAC_TX_SERDES_SRST_BIT + mac_num * GMAC_SRST_BIT));
        bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_DIS1_ADDR);
    }

    /* Reset deassertion of the RX SERDES clock */
    rst_mask = (0x1U << (GMAC_RX_SERDES_SRST_BIT + mac_num * GMAC_SRST_BIT));
    bm_writel(rst_mask, HP_SUB_CSR_HP_SRST_DIS1_ADDR);
}

static int gmac_reset_chip(unsigned int mac_num)
{
    /* Disabling the RX Channel */
    gmac_hw_rx_enable(mac_num, 0);

    /* Disabling the TX Channel */
    gmac_hw_tx_enable(mac_num, 0);

    gmac_hw_ptp_reset();

    gmac_hw_core_reset(mac_num);

    /* Disabling the BMU */
    return gmac_hw_set_bmu_close(mac_num);
}

static int gmac_get_phy_id(unsigned int phy_addr, unsigned int *identifier)
{
    int ret;
    unsigned int identifier_l32 = 0;
    unsigned int identifier_h32 = 0;

    ret = mdio_read(0, phy_addr, MII_PHY_ID0_REG, &identifier_h32);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "gmac get phy0's identifier h32 failed!");
        return ret;
    }

    ret = mdio_read(0, phy_addr, MII_PHY_ID1_REG, &identifier_l32);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "gmac get phy1's identifier l32 failed!");
        return ret;
    }

    /* phy id = h32 left 16 + l32 */
    *identifier = ((identifier_h32 << 16) | (identifier_l32 & 0xFFFF));

    return ret;
}

static int gmac_phy_init(struct gmac_device *priv)
{
    unsigned int i;
    int ret;
    unsigned int count = sizeof(hi_gmac_phy_info_table) / sizeof(gmac_phy_info_s);
    unsigned int identifier = 0;
    unsigned int phy_addr = priv->phy_addr;

    ret = gmac_get_phy_id(phy_addr, &identifier);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "get phy id failed\n");
        return ret;
    }

    for (i = 0; i < count; i++) {
        if (hi_gmac_phy_info_table[i].phy_id == identifier) {
            hi_gmac_phy_info_table[i].init(phy_addr);
            priv->gmac_phy_info = (gmac_phy_info_s *)&hi_gmac_phy_info_table[i];
            bm_debug(BM_DEBUG_LEVEL,
                "phy init success, eth = %d, phy_addr = %d, id = 0x%x\n",
                priv->mac_num,
                priv->phy_addr,
                identifier);
            return BM_OK;
        }
    }

    priv->gmac_phy_info = NULL;
    bm_debug(BM_ERROR_LEVEL, "phy init failed\n");
    return BM_FAIL;
}

static void gmac_hw_sds_phy_reset(unsigned int sds_phy_cfg1_offset_addr)
{
    u_gmac_phy_cfg phy_cfg;

    /* Set the PHY to the reset state. */
    reg_read(sds_phy_cfg1_offset_addr, phy_cfg.u32);
    if (phy_cfg.bits.phy_reset_0 != ENABLE) {
        phy_cfg.bits.phy_reset_0 = ENABLE;
        reg_write(sds_phy_cfg1_offset_addr, phy_cfg.u32);
    }

    udelay(10); /* delay 10us */
}

static void gmac_hw_sds_phy_los_reg_init(unsigned int sds_phy_los_cfg_offset_addr)
{
    u_gmac_phy_los_cfg phy_los_cfg;

    /* SerDes parameter configuration */
    reg_read(sds_phy_los_cfg_offset_addr, phy_los_cfg.u32);
    phy_los_cfg.bits.rx_los_valid_rst_msk_0 = 0x3;
    phy_los_cfg.bits.tx_vboost_lvl_0 = 0x4;
    phy_los_cfg.bits.rx0_los_filt_cnt_0 = 0x5;
    phy_los_cfg.bits.los_bias_0 = 0x2;
    phy_los_cfg.bits.los_level_0 = 0x9;
    reg_write(sds_phy_los_cfg_offset_addr, phy_los_cfg.u32);
}

static void gmac_hw_sds_phy_cfg2_reg_init(unsigned int sds_phy_cfg2_offset_addr)
{
    u_gmac_phy_cfg2 phy_cfg2;

    reg_read(sds_phy_cfg2_offset_addr, phy_cfg2.u32);
    phy_cfg2.bits.phy_data_reverse_en0 = 0x0;
    phy_cfg2.bits.tx0_preemph_0 = 0x13;
    phy_cfg2.bits.tx0_detect_rx_req_0 = 0x0;
    phy_cfg2.bits.tx0_amplitude_0 = 0x7f;
    phy_cfg2.bits.phy_tx0_term_offset_0 = 0x0;
    phy_cfg2.bits.phy_rx0_eq_0 = 0x0;
    reg_write(sds_phy_cfg2_offset_addr, phy_cfg2.u32);
}

static void gmac_hw_sds_phy_cfg1_reg_init(unsigned int sds_phy_cfg1_offset_addr)
{
    u_gmac_phy_cfg phy_cfg;

    reg_read(sds_phy_cfg1_offset_addr, phy_cfg.u32);
    phy_cfg.bits.phy_rtune_req_0 = 0x0;
    phy_cfg.bits.ref_clkdiv2_0 = 0x0;
    phy_cfg.bits.ref_use_pad_0 = 0x1;
    phy_cfg.bits.vreg_bypass_0 = 0x1;
    phy_cfg.bits.mpll_multiplier_0 = 0x19;
    phy_cfg.bits.ssc_ref_clk_sel_0 = 0x0;
    phy_cfg.bits.lane0_loopbk_en_0 = DISABLE;
    phy_cfg.bits.rx0_align_en_0 = ENABLE;
    phy_cfg.bits.rx0_data_en_0 = DISABLE;
    phy_cfg.bits.rx0_invert_0 = 0x0;
    phy_cfg.bits.rx0_los_en_0 = ENABLE;
    phy_cfg.bits.rx0_pll_en_0 = DISABLE;
    phy_cfg.bits.rx0_reset_0 = 0x0;
    phy_cfg.bits.rx0_term_en_0 = ENABLE;
    phy_cfg.bits.tx0_cm_en_0 = DISABLE;
    phy_cfg.bits.tx0_en_0 = DISABLE;
    phy_cfg.bits.tx0_reset_0 = 0x0;
    phy_cfg.bits.tx0_vboost_en_0 = DISABLE;
    phy_cfg.bits.ref_ssp_en_0 = 0x0;
    phy_cfg.bits.tx0_invert_0 = 0x0;
    phy_cfg.bits.test_powerdown_0 = 0x0;
    reg_write(sds_phy_cfg1_offset_addr, phy_cfg.u32);
    udelay(10); /* delay 10us */
}

static int gmac_hw_sds_phy_init(unsigned int sds_phy_cfg1_offset_addr, unsigned int sds_phy_cfg2_offset_addr,
    unsigned int sds_phy_state_offset_addr)
{
    unsigned int count = 0;
    u_gmac_phy_cfg phy_cfg;
    u_gmac_phy_cfg2 phy_cfg2;
    u_gmac_phy_sts phy_state;

    reg_read(sds_phy_cfg1_offset_addr, phy_cfg.u32);
    /* Enable reference clock */
    phy_cfg.bits.ref_ssp_en_0 = ENABLE;
    reg_write(sds_phy_cfg1_offset_addr, phy_cfg.u32);

    /* Deassert the reset of the PHY. */
    phy_cfg.bits.phy_reset_0 = DISABLE;
    reg_write(sds_phy_cfg1_offset_addr, phy_cfg.u32);

    /* Configure mpll_en to start the PLL. */
    phy_cfg.bits.mpll_en_0 = ENABLE;
    reg_write(sds_phy_cfg1_offset_addr, phy_cfg.u32);

    /* Check whether the MPL is locked. */
    do {
        if (count++ > MPLL_LOCK_TIMES) {
            bm_debug(BM_ERROR_LEVEL, "%s:%d  TIMEOUT!\n", __func__, __LINE__);
            return BM_FAIL;
        }
        udelay(1);

        reg_read(sds_phy_state_offset_addr, phy_state.u32);
    } while (!(phy_state.bits.mpll_state_0));

    /* Enabled tx0_cm_en */
    phy_cfg.bits.tx0_cm_en_0 = ENABLE;
    reg_write(sds_phy_cfg1_offset_addr, phy_cfg.u32);

    /* Check whether the common mode is established. */
    count = 0;
    do {
        if (count++ > TX_CM_TIMES) {
            return BM_FAIL;
        }
        udelay(1);
        reg_read(sds_phy_state_offset_addr, phy_state.u32);
    } while (!(phy_state.bits.tx0_cm_state_0));

    /* Start the TX and RX channels. */
    phy_cfg.bits.rx0_pll_en_0 = ENABLE;
    phy_cfg.bits.tx0_en_0 = ENABLE;
    reg_write(sds_phy_cfg1_offset_addr, phy_cfg.u32);

    /* Check whether the startup is successful. */
    count = 0;
    do {
        if (count++ > TX_RX_ENABLE_TIMES) {
            bm_debug(BM_ERROR_LEVEL, "%s:%d  TIMEOUT!\n", __func__, __LINE__);
            return BM_FAIL;
        }
        udelay(1);
        reg_read(sds_phy_state_offset_addr, phy_state.u32);
    } while (!((phy_state.bits.tx0_state_0) && (phy_state.bits.rx0_pll_state_0)));

    /* Start data transmission in the transmit direction */
    reg_read(sds_phy_cfg2_offset_addr, phy_cfg2.u32);
    phy_cfg2.bits.tx0_data_en_0 = ENABLE;
    reg_write(sds_phy_cfg2_offset_addr, phy_cfg2.u32);

    /* Start data transmission in the receive direction */
    phy_cfg.bits.rx0_data_en_0 = ENABLE;
    reg_write(sds_phy_cfg1_offset_addr, phy_cfg.u32);

    return BM_OK;
}

static int gmac_serdes_init(unsigned int mac_num)
{
    unsigned int sds_phy_los_cfg_offset_addr;
    unsigned int sds_phy_cfg1_offset_addr;
    unsigned int sds_phy_cfg2_offset_addr;
    unsigned int sds_phy_state_offset_addr;

    if (mac_num == ETH2) {
        sds_phy_los_cfg_offset_addr = HP_SUB_CSR_GMAC_PHY0_LOS_CFG_ADDR;
        sds_phy_cfg1_offset_addr = HP_SUB_CSR_GMAC_PHY0_CFG_ADDR;
        sds_phy_cfg2_offset_addr = HP_SUB_CSR_GMAC_PHY0_CFG2_ADDR;
        sds_phy_state_offset_addr = HP_SUB_CSR_GMAC_PHY0_STS_ADDR;
    } else if (mac_num == ETH3) {
        sds_phy_los_cfg_offset_addr = HP_SUB_CSR_GMAC_PHY1_LOS_CFG_ADDR;
        sds_phy_cfg1_offset_addr = HP_SUB_CSR_GMAC_PHY1_CFG_ADDR;
        sds_phy_cfg2_offset_addr = HP_SUB_CSR_GMAC_PHY1_CFG2_ADDR;
        sds_phy_state_offset_addr = HP_SUB_CSR_GMAC_PHY1_STS_ADDR;
    } else {
        return BM_OK;
    }

    gmac_hw_sds_phy_reset(sds_phy_cfg1_offset_addr);

    gmac_hw_sds_phy_los_reg_init(sds_phy_los_cfg_offset_addr);

    gmac_hw_sds_phy_cfg2_reg_init(sds_phy_cfg2_offset_addr);

    gmac_hw_sds_phy_cfg1_reg_init(sds_phy_cfg1_offset_addr);

    return gmac_hw_sds_phy_init(sds_phy_cfg1_offset_addr, sds_phy_cfg2_offset_addr, sds_phy_state_offset_addr);
}

/* ****************************************************************************
 Function Name  : gmac_set_io_cfg
 Function Description  : Configuring RMII I/O Multiplexing
 Return value  : void
**************************************************************************** */
static void gmac_set_io_cfg(unsigned int port)
{
    if (port == ETH0) {
        reg_write(IO_CONFIG_R_IOMG022_ADDR, 0x0);
        reg_write(IO_CONFIG_R_IOMG023_ADDR, 0x0);
        reg_write(IO_CONFIG_R_IOMG024_ADDR, 0x0);
        reg_write(IO_CONFIG_R_IOMG025_ADDR, 0x0);
    } else if (port == ETH1) {
        reg_write(IO_CONFIG_R_IOMG026_ADDR, 0x0);
        reg_write(IO_CONFIG_R_IOMG027_ADDR, 0x0);
    }
}

static void get_phy_speed_duplex(struct gmac_device *priv, unsigned int *speed, unsigned int *duplex)
{
    int ret;

    if (priv->gmac_phy_info && priv->gmac_phy_info->get_speed_duplex) {
        ret = priv->gmac_phy_info->get_speed_duplex(priv->phy_addr, speed, duplex);
        if (ret == 0) {
            return;
        } else {
            bm_debug(BM_ERROR_LEVEL, "get phy speed failed\n");
            goto EXIT;
        }
    }

EXIT:
    if (priv->mac_num == ETH0 || priv->mac_num == ETH1) {
        *speed = MAC_100M_RMII;
    } else {
        *speed = MAC_1000M_SGMII;
    }

    return;
}

static int gmac_chip_init(struct gmac_device *priv)
{
    femac_pkt_store_ctrl_cfg st_rx_ctrl_reg = {
        .cf_buf_1st_skip1 = 0, .cf_buf_n1st_skip = 1,  .cf_cache_line = 0,
        .cf_rxbuf_1st_skip_size2 = 1, .cf_rx_align_num = 0
    }; /* Format for storing packets in the memory */
    femac_rx_pkt_mode_cfg st_rx_pkt_mode_reg = {
        .cf_instr_head_mode = 0, .cf_skip_len = 0, .cf_parse_mode = 1, .mac_skip_len = 0
    };
    femac_port_mode_cfg st_port_mode_reg = {
        .max_frm_size = FE_MAX_PKT_LEN, .short_runts_thr = 0x1f, .pad_enable = 1,
        .crc_add = 1, .an_enable = 1, .runt_pkt_en = 1, .strip_pad_en = 1
    };
    femac_mac_filter_cfg st_mac_filter_reg = {
        .crc_err_pass = 1, .pause_frm_pass = 1, .bc_drop_en = 0, .mc_match_en = 1, .uc_match_en = 0
    };
    unsigned int mac_num, ret;

    mac_num = priv->mac_num;

    /* Initialize the packet storage format parameters and
    buffer obtaining mode in the buff based on the port number.(PCU_RX_CTRL_REG) */

    if (gmac_hw_set_pkt_store_format(mac_num, &st_rx_ctrl_reg)) {
        bm_debug(BM_ERROR_LEVEL, " gmac_hw_set_pkt_store_format fail\n");
        return BM_FAIL;
    }

    /* Set the packet input mode and parsing mode. */
    if (gmac_hw_set_pkt_input_parse_mode(mac_num, &st_rx_pkt_mode_reg)) {
        bm_debug(BM_ERROR_LEVEL, "gmac_hw_set_pkt_input_parse_mode fail\n");
        return BM_FAIL;
    }

    st_port_mode_reg.port_num = mac_num;
    gmac_hw_set_port_mode_all(mac_num, &st_port_mode_reg);

    /* Initializing Packet Filtering Rules */
    gmac_hw_set_mac_addr_filter(mac_num, &st_mac_filter_reg);

    if (gmac_hw_set_rx_max_frame_len(mac_num, FE_MAX_PKT_LEN)) {
        bm_debug(BM_ERROR_LEVEL, " gmac_hw_set_rx_max_frame_len failed\n");
        return BM_FAIL;
    }

    /* Configure the address extension function to map the 32-bit address to the 34-bit address. */
    gmac_init_extend_addr(mac_num);

    ret = gmac_hw_set_tx_pad_add(mac_num, 1);
    ret += gmac_hw_set_poe_close(mac_num);
    ret += gmac_set_rx_fifo_threshold(mac_num);
    ret += gmac_set_tx_fifo_threshold(mac_num);
    ret += gmac_set_hw_mac_addr(mac_num);
    ret += gmac_hw_int_int(mac_num);

    return ret;
}

static int gmac_eth_init(struct gmac_device *priv)
{
    if (priv->mac_num == ETH0 || priv->mac_num == ETH1) {
        gmac_set_io_cfg(priv->mac_num);
    }

    if (gmac_reset_chip(priv->mac_num)) {
        return BM_FAIL;
    }

    if (gmac_chip_init(priv)) {
        return BM_FAIL;
    }

    gmac_mdelay(1);

    gmac_rx_fill_all_buff(priv->mac_num, priv);

    return BM_OK;
}

static int gmac_update_speed_duplex(bm_eth eth)
{
    static unsigned int old_speed, old_mode;
    if (eth >= ETH_BUTT || eth < ETH0) {
        bm_debug(BM_ERROR_LEVEL, "input arg error\n");
        return BM_FAIL;
    }

    struct gmac_device *priv = &gmac_device_tbl[eth];
    if (priv->mac_num != eth || priv->init_done == 0) {
        bm_debug(BM_ERROR_LEVEL, "input arg error\n");
        return BM_FAIL;
    }

    if ((priv->gmac_phy_info == NULL) || (priv->gmac_phy_info->get_link_status == NULL) ||
        (priv->gmac_phy_info->get_link_status(priv->phy_addr, &(priv->link_status)))) {
        bm_debug(BM_ERROR_LEVEL, "error: get_link_status failed\n");
        return BM_FAIL;
    }

    get_phy_speed_duplex(priv, &(priv->link_speed), &(priv->link_mode));

    if (priv->link_speed == old_speed && priv->link_mode == old_mode && priv->link_status == PHY_LINK_UP) {
        return BM_OK;
    }

    old_speed = priv->link_speed;
    old_mode = priv->link_mode;

    bm_debug(BM_DEBUG_LEVEL, " set new speed = %d, set mode = %d\n", priv->link_speed, priv->link_mode);

    if ((eth == ETH0) || (eth == ETH1)) {
        gmac_hw_set_port_mode(eth, priv->link_speed, priv->link_mode);
    } else {
        gmac_hw_set_port_mode(eth, priv->link_speed, priv->link_mode);
    }

    gmac_hw_rx_enable(priv->mac_num, ENABLE);
    gmac_hw_tx_enable(priv->mac_num, ENABLE);
    return (priv->link_status == PHY_LINK_UP) ? BM_OK : BM_FAIL;
}

static int gmac_net_send_data(bm_eth eth, const unsigned char *packet, unsigned int length)
{
    struct gmac_device *priv = &gmac_device_tbl[eth];
    unsigned int relative_addr;
    unsigned char *buffer = priv->tx_data_ring[priv->tx_data_num];
    u_tx_pcu_cmd st_pcu_cmd_reg;

    if (memset_s((void *)&st_pcu_cmd_reg, sizeof(st_pcu_cmd_reg), 0x0, sizeof(st_pcu_cmd_reg)) != BM_OK) {
        return BM_FAIL;
    }

    if (memcpy_s(buffer, RX_BUFFER_DEFAULT_SIZE, (void *)packet, length) != BM_OK) {
        return BM_FAIL;
    }

    bm_flush_dcache_all();
    asm volatile("dsb st" ::: "memory");

    /* word0 */
    st_pcu_cmd_reg.word0.bits.ipoff = 0; /* help:checksum         */
    st_pcu_cmd_reg.word0.bits.sd = 0;    /* The value 0 indicates that only one data segment is contained. */
    st_pcu_cmd_reg.word0.bits.l3_cs = 0; /* Checksum and ipoff can be used together. */
    st_pcu_cmd_reg.word0.bits.wb = 1;    /* Supports writeback tests. */
    st_pcu_cmd_reg.word0.bits.l4_cs = 0; /* */

    /* word1 */
    st_pcu_cmd_reg.word1.bits.l2cache = 1;  /* The L2 cache does not need to be enabled. */
    st_pcu_cmd_reg.word1.bits.fm = 0;       /* The buffer space is released after the transmission is complete. */
    st_pcu_cmd_reg.word1.bits.back = 0;     /* Not required by the FE. Distance between the start address of the
                                            cache line where data isstored and the start address of the cache */
    st_pcu_cmd_reg.word1.bits.len = length; /* Send Length */
    st_pcu_cmd_reg.word1.bits.pool = 0;     /* ID of the released pool. */

    /* word2 */
    relative_addr = gmac_get_ddr_relative_addr(GMAC_TX, priv->mac_num, (unsigned long long)(uintptr_t)buffer);
    st_pcu_cmd_reg.pkt_addr = relative_addr;

    /* word3 */
    st_pcu_cmd_reg.clear_addr = relative_addr;

    mac_reg_write(priv->mac_num, PCU_TX_CFF_ADDR_0_REG, st_pcu_cmd_reg.word0.u32);
    mac_reg_write(priv->mac_num, PCU_TX_CFF_ADDR_1_REG, st_pcu_cmd_reg.word1.u32);
    mac_reg_write(priv->mac_num, PCU_TX_CFF_ADDR_2_REG, st_pcu_cmd_reg.pkt_addr);
    mac_reg_write(priv->mac_num, PCU_TX_CFF_ADDR_3_REG, st_pcu_cmd_reg.clear_addr);

    priv->tx_data_num++;
    priv->tx_data_num = priv->tx_data_num % CHIP_TX_FIFO_NUM;

    return BM_OK;
}

int bm_net_send(bm_eth eth, const unsigned char *packet, unsigned int length)
{
    if (eth >= ETH_BUTT || eth < ETH0 || packet == NULL || length >= RX_BUFFER_DEFAULT_SIZE || length <= 0 ||
        is_invalid_addr_len((unsigned int)(uintptr_t)(packet), length)) {
        bm_debug(BM_ERROR_LEVEL, "bm_net_send: input arg error\n");
        return BM_FAIL;
    }

    struct gmac_device *priv = &gmac_device_tbl[eth];
    if (priv->mac_num != eth || priv->init_done == 0 || !gmac_exist_array[eth]) {
        bm_debug(BM_ERROR_LEVEL, "bm_net_send: input arg error\n");
        return BM_FAIL;
    }

    if (gmac_update_speed_duplex(eth) != BM_OK) {
        bm_debug(BM_ERROR_LEVEL, "gmac link failed\n");
        return BM_FAIL;
    }

    if (osal_sem_down_timeout(&(priv->sem), GMAC_SEM_TIMEOUT) != OSAL_SUCCESS) {
        return BM_FAIL;
    }
    int ret = gmac_net_send_data(eth, packet, length);
    osal_sem_up(&(priv->sem));
    return ret;
}

static int gmac_net_receive_data(bm_eth eth, unsigned int *rx_len, unsigned char **packetp)
{
    struct gmac_device *priv = &gmac_device_tbl[eth];

    unsigned char *packet = g_packet_buf[eth];
    int ret = BM_FAIL;
    u_cf_intrpt_stat int_raw_reg;
    u_cf_intrpt_clr int_clr_reg = {.u32 = 0};
    unsigned char *mac_desc = priv->rx_data_ring[priv->rx_data_num]; /* MAC Packet Descriptor */

    mac_reg_read(eth, PCU_CF_INTRPT_STAT_REG, int_raw_reg.u32);

    if (int_raw_reg.bits.stat_rx_frm) {
        int_clr_reg.bits.cf_rx_frm_clr = 1;
        mac_reg_write(eth, PCU_CF_INTRPT_CLR_REG, int_clr_reg.u32);
        unsigned char *r_data = mac_desc + ((FE_RX_SKIP1 + FE_RX_SKIP2) * CACHE_LINE_SIZE + CACHE_LINE_SIZE);
        /* Move leftward by 8 as the high byte of len */
        *rx_len = (mac_desc[GMAC_DESC_PKT_LEN_HIGH] << 8) + mac_desc[GMAC_DESC_PKT_LEN_LOW];

        /* Obtains the packet length word3.Len. */
        if (*rx_len == 0) {
            return BM_FAIL;
        }

        if (*rx_len >= (RX_BUFFER_DEFAULT_SIZE - ((FE_RX_SKIP1 + FE_RX_SKIP2) * CACHE_LINE_SIZE + CACHE_LINE_SIZE))) {
            bm_debug(BM_ERROR_LEVEL, "error: rxNum = %d, *rx_len = %d\n", priv->rx_data_num, *rx_len);
            goto EXIT_RECV;
        }

        if (memcpy_s(packet, RX_BUFFER_DEFAULT_SIZE, r_data, *rx_len) != BM_OK) {
            goto EXIT_RECV;
        }

        ret = BM_OK;

        *packetp = packet;
    EXIT_RECV:
        if (memset_s(mac_desc, CPU_CACHE_LINE_SIZE, 0x0, CPU_CACHE_LINE_SIZE) != BM_OK) {
            return BM_FAIL;
        }
        unsigned int relative_addr = gmac_get_ddr_relative_addr(GMAC_RX, eth, (unsigned long long)(uintptr_t)mac_desc);
        mac_reg_write(eth, PCU_RX_CFF_ADDR_REG, relative_addr);

        bm_flush_dcache_all();
        asm volatile("dsb st" ::: "memory");

        priv->rx_data_num++;
        priv->rx_data_num = priv->rx_data_num % CHIP_RX_FIFO_NUM;
    }

    return ret;
}

int bm_net_receive(bm_eth eth, unsigned int *rx_len, unsigned char **packetp)
{
    if (eth >= ETH_BUTT || eth < ETH0 || rx_len == NULL || packetp == NULL) {
        return BM_FAIL;
    }

    struct gmac_device *priv = &gmac_device_tbl[eth];
    if (priv->init_done == 0 || priv->mac_num != eth || !gmac_exist_array[eth]) {
        return BM_FAIL;
    }

    if (gmac_update_speed_duplex(eth) != BM_OK) {
        bm_debug(BM_ERROR_LEVEL, "gmac link failed\n");
        return BM_FAIL;
    }

    if (osal_sem_down_timeout(&(priv->sem), GMAC_SEM_TIMEOUT) != OSAL_SUCCESS) {
        return BM_FAIL;
    }
    int ret = gmac_net_receive_data(eth, rx_len, packetp);
    osal_sem_up(&(priv->sem));
    return ret;
}

static int gmac_eth_start(struct gmac_device *priv)
{
    if (gmac_phy_init(priv)) {
        return BM_FAIL;
    }

    if (priv->init_done) {
        unsigned int speed, duplex;
        get_phy_speed_duplex(priv, &speed, &duplex);
        bm_debug(BM_ERROR_LEVEL, "gmac init done, speed = %d, duplex = %d\n, link success", speed, duplex);
        gmac_hw_set_port_mode(priv->mac_num, speed, duplex);
        /* Enable the RX channel. */
        gmac_hw_rx_enable(priv->mac_num, ENABLE);
        gmac_hw_tx_enable(priv->mac_num, ENABLE);
        return BM_OK;
    }

    bm_debug(BM_DEBUG_LEVEL, "Use eth%x ,phy addr is 0x%x, link success\n", priv->mac_num, priv->phy_addr);

    if (gmac_serdes_init(priv->mac_num)) {
        bm_debug(BM_ERROR_LEVEL, "serdes init failed\n");
        return BM_FAIL;
    }

    if (gmac_eth_init(priv)) {
        bm_debug(BM_ERROR_LEVEL, "gmac eth%d init failed\n", priv->mac_num);
        return BM_FAIL;
    }

    bm_debug(BM_DEBUG_LEVEL, "Boot gmac[%d] init ok!\n", priv->mac_num);

    /* Enable the RX channel. */
    gmac_hw_rx_enable(priv->mac_num, ENABLE);
    /* Enable the TX channel. */
    gmac_hw_tx_enable(priv->mac_num, ENABLE);

    gmac_mdelay(100); /* Delay 100 ms and wait for the startup to complete. */

    return BM_OK;
}

int bm_net_init(bm_eth eth, unsigned int phy_addr)
{
    if (eth >= ETH_BUTT || eth < ETH0 || phy_addr >= GMAC_PHY_MAX_ADDR) { /* 0xFF: max phy addr */
        return BM_FAIL;
    }

    if (!gmac_exist_array[eth]) {
        bm_log("eth(%u) error, forbidden to access\n", eth);
        return BM_FAIL;
    }

    struct gmac_device *priv = &gmac_device_tbl[eth];

    priv->mac_num = eth;
    priv->phy_addr = phy_addr;

    while (bm_cmp_xchg32bits((unsigned int *)(uintptr_t)GMAC_LOCK_SYNC, 0xf, 0) != 0) {
        dsb();
        isb();
        bm_flush_dcache_all();
    }

    if (mdio_init(0)) {
        bm_writel(0, GMAC_LOCK_SYNC);
        bm_debug(BM_ERROR_LEVEL, "error: mdio init failed\n");
        return BM_FAIL;
    }

    if (gmac_rx_init_ring(priv)) {
        bm_writel(0, GMAC_LOCK_SYNC);
        bm_debug(BM_ERROR_LEVEL, "error: init rx buffer fail!\n");
        return BM_FAIL;
    }

    if (gmac_tx_init_ring(priv)) {
        bm_writel(0, GMAC_LOCK_SYNC);
        bm_debug(BM_ERROR_LEVEL, "error: init tx buffer fail!\n");
        return BM_FAIL;
    }

    if (gmac_eth_start(priv)) {
        bm_writel(0, GMAC_LOCK_SYNC);
        bm_debug(BM_ERROR_LEVEL, "error: gmac_eth_start fail!\n");
        return BM_FAIL;
    }

    bm_flush_dcache_all();
    asm volatile("dsb st" ::: "memory");

    if (priv->init_done != 1) {
        if (osal_sem_init(&(priv->sem), 1) != OSAL_SUCCESS) {
            bm_writel(0, GMAC_LOCK_SYNC);
            return BM_FAIL;
        }
    }

    priv->init_done = 1;
    bm_writel(0, GMAC_LOCK_SYNC);
    return BM_OK;
}

int bm_net_deinit(bm_eth eth)
{
    if (eth >= ETH_BUTT || eth < ETH0) {
        return BM_FAIL;
    }

    if (!gmac_exist_array[eth]) {
        bm_log("eth(%u) error, forbidden to access\n", eth);
        return BM_FAIL;
    }

    int ret = bm_net_set_mac_addr(eth, g_default_mac_addr[eth]);
    return ret;
}

int bm_net_set_mac_addr(bm_eth eth, const unsigned char *mac_addr)
{
    if (eth >= ETH_BUTT || eth < ETH0 || mac_addr == NULL ||
        is_invalid_addr_len((unsigned int)(uintptr_t)(mac_addr), 6)) { /* mac length = 6 */
        return BM_FAIL;
    }

    struct gmac_device *priv = &gmac_device_tbl[eth];
    if (priv->init_done == 0 || priv->mac_num != eth || !gmac_exist_array[eth]) {
        return BM_FAIL;
    }

    unsigned int addr_low, addr_hi;
    const unsigned char *pdata = mac_addr;

    addr_hi = ((unsigned int)pdata[0] << 8) |   /* enetaddr[0] Bits 0 - 8 of the upper address */
              (pdata[1]);

    addr_low = ((unsigned int)pdata[2] << 24) | /* enetaddr[2] Bits 24 - 31 of the lower address */
               ((unsigned int)pdata[3] << 16) | /* enetaddr[3] 16 - 23 bits of the lower address */
               ((unsigned int)pdata[4] << 8) |  /* enetaddr[4] Bits 8 - 15 of the lower address */
               (pdata[5]);                      /* enetaddr[5] Bits 0 - 7 of the lower address */

    /* To set the MAC address of a port */
    mac_reg_write(eth, MAC_CORE_STATION_ADDR_HIGH_2_REG, addr_hi);
    mac_reg_write(eth, MAC_CORE_STATION_ADDR_LOW_2_REG, addr_low);

    return BM_OK;
}