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

#include "mdio.h"
#include "gmac.h"
#include "bm_common.h"
#include "bm_ops.h"
#include "bm_cache.h"
#include "bm_atomic.h"
#include "io_config.h"

#if defined (__UNIPROTON__)
static int g_mdio_lock = 0;
#define GMAC_MDIO_SYNC &g_mdio_lock
#else
#include "hi309x_memmap.h"
#endif

static unsigned int mdio_ready_get(unsigned int mdio_id)
{
    mdio_cmd_data mdio_cmd;

    if (mdio_id >= MDIO_MAX_DRV_NUM) {
        return (unsigned int)-1;
    }

    mac_reg_read(mdio_id, MDIO_CMD_REG, mdio_cmd.u32);
    return (mdio_cmd.bits.mdio_go);
}

/* ****************************************************************************
 Function Name  : mdio_ready_wait
 Function Description  : Wait until the operation of the mdio interface is complete.
 Return value  : timeout_us
**************************************************************************** */
static unsigned int mdio_ready_wait(unsigned int mdio_id)
{
    unsigned int timeout_us = MDIO_WAIT_READY_CNT;

    while (--timeout_us && (mdio_ready_get(mdio_id))) {
        udelay(10); /* delay 10us */
    }

    return timeout_us;
}

/* ****************************************************************************
 Function Name  : mdio_start_phy_read
 Function Description  : Perform a read operation
 Return value  : None
**************************************************************************** */
static void mdio_start_phy_read(unsigned int mdio_id, unsigned int phy_addr, unsigned int reg_addr)
{
    mdio_cmd_data mdio_cmd;
    mdio_cmd.u32 = 0;
    mdio_reg_read(mdio_id, MDIO_CMD_REG, mdio_cmd.u32);
    mdio_cmd.bits.mdio_st = 1;
    mdio_cmd.bits.mdio_go = 1;
    mdio_cmd.bits.op_code = 0x2;
    mdio_cmd.bits.phy_addr = phy_addr;
    mdio_cmd.bits.reg_addr = reg_addr;
    mdio_reg_write(mdio_id, MDIO_CMD_REG, mdio_cmd.u32);
}

/* ****************************************************************************
 Function Name  : mdio_get_phy_read_val
 Function Description  : The result of the read operation
 Return value  : Value of the PHY register
**************************************************************************** */
static unsigned int mdio_get_phy_read_val(unsigned int mdio_id)
{
    mdio_rd_wr_data un_read_data;

    mdio_reg_read(mdio_id, MDIO_RDATA_REG, un_read_data.u32);
    udelay(5000); /* delay 5000us */
    return (un_read_data.bits.mdio_wr_data);
}

/* ****************************************************************************
 Function Name  : mdio_phy_write
 Function Description  : Perform the mdio write operation.
 Return value  : None
**************************************************************************** */
static void mdio_phy_write(unsigned int mdio_id, unsigned int phy_addr, unsigned int reg_addr, unsigned int value)
{
    mdio_cmd_data mdio_cmd;
    mdio_rd_wr_data un_write_data;
    un_write_data.u32 = 0;
    mdio_cmd.u32 = 0;

    un_write_data.bits.mdio_wr_data = value;
    mdio_reg_write(mdio_id, MDIO_WDATA_REG, un_write_data.u32);
    mdio_reg_read(mdio_id, MDIO_CMD_REG, mdio_cmd.u32);
    mdio_cmd.bits.mdio_st = 1;
    mdio_cmd.bits.mdio_go = 1;
    mdio_cmd.bits.op_code = 1;
    mdio_cmd.bits.phy_addr = phy_addr;
    mdio_cmd.bits.reg_addr = reg_addr;
    mdio_reg_write(mdio_id, MDIO_CMD_REG, mdio_cmd.u32);
}

/* ****************************************************************************
 Function Name  : mdio_init
 Function Description  : Initializing the MDIO Interface
 Return value  : BM_OK / BM_FAIL
**************************************************************************** */
int mdio_init(unsigned int mdio_id)
{
    mdio_cmd_data mdio_cmd;

    if (mdio_id >= MDIO_MAX_DRV_NUM) {
        return BM_FAIL;
    }

    /* MDIO I/O multiplexing */
    reg_write(IO_CONFIG_T_IOMG033_ADDR, 0x0);

    /* Initialize the clock frequency of the MDIO interface to 2.5 MHz. */
    mdio_cmd.u32 = 0;
    mdio_cmd.bits.mdio_clk_sel = 1;
    mdio_reg_write(mdio_id, MDIO_CMD_REG, mdio_cmd.u32);

    return BM_OK;
}

/* ****************************************************************************
 Function Name  : mdio_read
 Function Description  : Read function of the MDIO interface
 Return value  : BM_OK / BM_FAIL
**************************************************************************** */
int mdio_read(unsigned int mdio_id, unsigned int phy_addr, unsigned int reg_addr, unsigned int *value)
{
    if (mdio_id >= MDIO_MAX_DRV_NUM) {
        bm_debug(BM_ERROR_LEVEL, "mdio_id must less than 1\n");
        return BM_FAIL;
    }

    if (phy_addr > MDIO_MAX_PHY_ADDR) {
        bm_debug(BM_ERROR_LEVEL, "read phyaddr must less than 0x1f\n");
        return BM_FAIL;
    }

    if (reg_addr > MDIO_MAX_PHY_REG_ADDR) {
        bm_debug(BM_ERROR_LEVEL, "read regaddr must less than 0x30\n");
        return BM_FAIL;
    }

    if (!value) {
        bm_debug(BM_ERROR_LEVEL, "value null .\n");
        return BM_FAIL;
    }

    if (!mdio_ready_wait(mdio_id)) {
        bm_debug(BM_ERROR_LEVEL, "mdio_read ready timeout\n");
        return BM_FAIL;
    }

    while (bm_cmp_xchg32bits((unsigned int *)(uintptr_t)GMAC_MDIO_SYNC, 0xf, 0) != 0) {
        bm_flush_dcache_all();
        dsb();
        isb();
    }

    int ret = BM_OK;

    mdio_start_phy_read(mdio_id, phy_addr, reg_addr);

    if (mdio_ready_wait(mdio_id)) {
        udelay(1000); /* delay 1000us */
        *value = mdio_get_phy_read_val(mdio_id);
    } else {
        ret = BM_FAIL;
    }

    bm_writel(0, GMAC_MDIO_SYNC);

    return ret;
}

/* ****************************************************************************
 Function Name  : mdio_write
 Function Description  : Write operation function of the MDIO interface
 Return value  : BM_OK / BM_FAIL
**************************************************************************** */
int mdio_write(unsigned int mdio_id, unsigned int phy_addr, unsigned int reg_addr, unsigned int value)
{
    if (mdio_id >= MDIO_MAX_DRV_NUM) {
        bm_debug(BM_ERROR_LEVEL, "mdio_id must less than 2\n");
        return BM_FAIL;
    }

    if (phy_addr > MDIO_MAX_PHY_ADDR) {
        bm_debug(BM_ERROR_LEVEL, "write phyaddr must less than 0x1f\n");
        return BM_FAIL;
    }

    if (reg_addr > MDIO_MAX_PHY_REG_ADDR) {
        bm_debug(BM_ERROR_LEVEL, "write regaddr must less than 0x30\n");
        return BM_FAIL;
    }

    if (!mdio_ready_wait(mdio_id)) {
        bm_debug(BM_ERROR_LEVEL, "mdio_write ready timeout\n");
        return BM_FAIL;
    }

    while (bm_cmp_xchg32bits((unsigned int *)(uintptr_t)GMAC_MDIO_SYNC, 0xf, 0) != 0) {
        bm_flush_dcache_all();
        dsb();
        isb();
    }

    mdio_phy_write(mdio_id, phy_addr, reg_addr, value);
    udelay(5000); /* delay 5000us */

    bm_writel(0, GMAC_MDIO_SYNC);

    return BM_OK;
}