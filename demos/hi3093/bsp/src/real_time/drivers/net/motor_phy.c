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
#include "bm_net.h"
#include "mdio.h"
#include "motor_phy.h"

#define EXT_ADDR_OFFSET_REG     0x1E
#define EXT_DATA_REG            0x1F

#define PHY_SPEC_STATUS         0x11
#define MASK_YT85XX_SPEED       0xc000

/* yt8521 common register */
#define SMI_SDS_PHY_ADDR        0xa000
#define BROAD_CFG_ADDR          0xa005
#define LED0_CFG_ADDR           0xa00c
#define LED1_CFG_ADDR           0xa00d
#define LED2_CFG_ADDR           0xa00e
#define SDS_LINK_TIMER_ADDR     0xa5

/* yt8521 sds mii registers */
#define SDS_RESET_ADDR          0x0
#define SDS_STATUS_ADDR         0x11
#define SDS_RESET_BIT           BIT(15)
#define SDS_RX_LPI_ACTIVE_BIT   BIT(9)

#define SDS_ACCESS_MODE         0x2
#define PHY_ACCESS_MODE         0x0

int phy_yt85xx_write(unsigned int phy_addr, unsigned int reg_num, unsigned int page, unsigned int value)
{
    int ret;
    (void)page;
    if (phy_addr > GMAC_PHY_MAX_ADDR) {
        bm_debug(BM_ERROR_LEVEL, "phy_addr(0x%x) is error!", phy_addr);
        return BM_FAIL;
    }

    ret = mdio_write(0, phy_addr, reg_num, value);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "phy_write(addr 0x%x reg 0x%x) failed", phy_addr, reg_num);
    }

    return ret;
}

int phy_yt85xx_read(unsigned int phy_addr, unsigned int reg_num, unsigned int page, unsigned int *pvalue)
{
    int ret;
    (void)page;
    if (phy_addr > GMAC_PHY_MAX_ADDR) {
        bm_debug(BM_ERROR_LEVEL, "phy_addr(0x%x) is error!", phy_addr);
        return BM_FAIL;
    }

    ret = mdio_read(0, phy_addr, reg_num, pvalue);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "phy_read(addr 0x%x reg 0x%x) failed", phy_addr, reg_num);
    }

    return ret;
}

static int phy_yt85xx_read_ext_reg(unsigned int phy_addr, unsigned int reg_num, unsigned int *pvalue)
{
    int ret;

    ret = mdio_write(0, phy_addr, EXT_ADDR_OFFSET_REG, reg_num);
    ret += mdio_read(0, phy_addr, EXT_DATA_REG, pvalue);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "phy_read ext(addr 0x%x reg 0x%x) failed", phy_addr, reg_num);
    }

    return ret;
}

static int phy_yt85xx_write_ext_reg(unsigned int phy_addr, unsigned int reg_num, unsigned int value)
{
    int ret;

    ret = mdio_write(0, phy_addr, EXT_ADDR_OFFSET_REG, reg_num);
    ret += mdio_write(0, phy_addr, EXT_DATA_REG, value);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "phy_write ext(addr 0x%x reg 0x%x) failed", phy_addr, reg_num);
    }

    return ret;
}

static int phy_yt8521_fix_old_version(unsigned int phy_addr)
{
    int ret;
    unsigned int value;

    /* Read the PHY 0xf extension register to determine whether the version is the problem version. */
    ret = phy_yt85xx_read_ext_reg(phy_addr, 0xf, &value);
    if (ret) {
        return ret;
    }

    /* The value 0x10, 0x12, or 0x14 indicates an earlier version.
    The problem that the yt8521 PHY cannot connect to the MAC needs to be solved. */
    if ((value == 0x10) || (value == 0x12) || (value == 0x14)) {
        ret = phy_yt85xx_write_ext_reg(phy_addr, SMI_SDS_PHY_ADDR, SDS_ACCESS_MODE);
        if (ret) {
            return ret;
        }
        /* The SDS extended register 0x3 is set to 0x400 to prevent the problem of the PHY of an earlier version. */
        ret = phy_yt85xx_write_ext_reg(phy_addr, 0x3, 0x400);
        if (ret) {
            return ret;
        }
    }

    return BM_OK;
}

static int phy_yt8521_recover_ext_reg0_val(int phy_addr)
{
    unsigned int value;
    int ret;

    ret = phy_yt85xx_read_ext_reg(phy_addr, 0, &value);
    if (ret) {
        return ret;
    }

    /* 0x1c8d is the default value of ext reg0 and cannot be modified. If it is modified, restore it. */
    if (value == 0x1c8d) {
        return BM_OK;
    }

    ret = phy_yt85xx_write_ext_reg(phy_addr, 0, 0x1c8d);
    if (ret) {
        return ret;
    }

    ret = phy_yt85xx_read(phy_addr, 0, 0, &value);
    if (ret) {
        return ret;
    }
    value |= PHY_RESET;

    return phy_yt85xx_write(phy_addr, 0, 0, value);
}

int phy_yt8521_init(unsigned int phy_addr)
{
    int ret;
    unsigned int value = 0;

    ret = phy_yt8521_fix_old_version(phy_addr);
    if (ret) {
        return ret;
    }

    ret = phy_yt85xx_write_ext_reg(phy_addr, SMI_SDS_PHY_ADDR, SDS_ACCESS_MODE);
    /* It is recommended that the negotiated timer be set to the maximum value 0x1f. */
    ret += phy_yt85xx_write_ext_reg(phy_addr, SDS_LINK_TIMER_ADDR, 0x1f);
    if (ret) {
        return ret;
    }

    ret = phy_yt85xx_write_ext_reg(phy_addr, SMI_SDS_PHY_ADDR, PHY_ACCESS_MODE);
    if (ret) {
        return ret;
    }

    ret = phy_yt85xx_read_ext_reg(phy_addr, BROAD_CFG_ADDR, &value);
    value &= ~(BIT(6)); /* Bit 6: The value 0 indicates that the broadcast function is disabled. */
    ret += phy_yt85xx_write_ext_reg(phy_addr, BROAD_CFG_ADDR, value);
    if (ret) {
        return ret;
    }

    /* If LED0 is set to 0x1870, the link up indicator is steady on and the link down indicator is off. */
    ret = phy_yt85xx_write_ext_reg(phy_addr, LED0_CFG_ADDR, 0x1870);
    if (ret) {
        return ret;
    }
    /* When LED2 is set to 0x2600, data transmission blinks. */
    ret = phy_yt85xx_write_ext_reg(phy_addr, LED2_CFG_ADDR, 0x2600);
    if (ret) {
        return ret;
    }

    return phy_yt8521_recover_ext_reg0_val(phy_addr);
}

static int check_yt8521_phy_id(unsigned int phy_addr)
{
    unsigned int id;
    int ret;

    ret = phy_yt85xx_get_id(phy_addr, &id);
    if (ret) {
        return BM_FAIL;
    }

    if (id == IDENTIFIER_YT8521) {
        return BM_OK;
    }

    return BM_FAIL;
}

static void phy_yt8521_fix_chip_buf(unsigned int phy_addr)
{
    unsigned int value;
    int ret;

    ret = phy_yt85xx_write_ext_reg(phy_addr, SMI_SDS_PHY_ADDR, SDS_ACCESS_MODE);
    if (ret) {
        return;
    }

    ret = phy_yt85xx_read(phy_addr, SDS_STATUS_ADDR, 0, &value);
    if (ret) {
        goto exit;
    }

    /* Indicates whether to enter the RX LPI mode. If BIT9 is set to 1, the RX LPI mode is entered. */
    if ((value & SDS_RX_LPI_ACTIVE_BIT) == 0) {
        goto exit;
    }

    bm_debug(BM_INFO_LEVEL, "YT8521 SDS enter rx LPI mode");
    /* SDS RESET */
    ret = phy_yt85xx_read(phy_addr, SDS_RESET_ADDR, 0, &value);
    if (ret) {
        goto exit;
    }
    value |= SDS_RESET_BIT;
    (void)phy_yt85xx_write(phy_addr, SDS_RESET_ADDR, 0, value);

    udelay(100000); /* Sleep 100000us and wait until the reset is complete. */

exit:
    (void)phy_yt85xx_write_ext_reg(phy_addr, SMI_SDS_PHY_ADDR, PHY_ACCESS_MODE);
    return;
}

int phy_yt85xx_get_link_status(unsigned int phy_addr, unsigned int *status)
{
    unsigned int reg_value = 0;
    int ret;

    if (!status) {
        return BM_FAIL;
    }

    ret = phy_yt85xx_read(phy_addr, PHY_SPEC_STATUS, 0x0, &reg_value);
    if (ret) {
        return ret;
    }

    if ((reg_value & 0xffff) == 0xffff) {
        return BM_FAIL;
    }

    /* Bit 10: indicates the real-time link status. The value 1 indicates that the link is up,
    and the value 0 indicates that the link is down. */
    *status = (reg_value & BIT(10)) ? PHY_LINK_UP : PHY_LINK_DOWN;

    /* The Yutong 8521 SGMII enters the LPI mode by mistake. */
    if (*status == PHY_LINK_UP && (check_yt8521_phy_id(phy_addr) == BM_OK)) {
        phy_yt8521_fix_chip_buf(phy_addr);
    }

    return BM_OK;
}

int phy_yt8521_get_speed_duplex(unsigned int phy_addr, unsigned int *speed, unsigned int *duplex)
{
    int ret;
    unsigned int value = 0;

    ret = phy_yt85xx_read(phy_addr, PHY_SPEC_STATUS, 0x0, &value);
    if (ret) {
        return ret;
    }

    if ((value & 0xffff) == 0xffff) {
        return BM_FAIL;
    }

    if ((value & MASK_YT85XX_SPEED) == PHY_SPEED_1000M_MODE) {
        *speed = MAC_1000M_SGMII;
    } else if ((value & MASK_YT85XX_SPEED) == PHY_SPEED_100M_MODE) {
        *speed = MAC_100M_SGMII;
    } else if ((value & MASK_YT85XX_SPEED) == PHY_SPEED_10M_MODE) {
        *speed = MAC_10M_SGMII;
    } else {
        *speed = MAC_1000M_SGMII;
    }

    /* Bit 13: indicates the duplex mode. The value 1 indicates the full duplex mode,
    and the value 0 indicates the half duplex mode. */
    *duplex = (value & BIT(13)) ? GMAC_DUPLEX_FULL : GMAC_DUPLEX_HALF;

    return BM_OK;
}

int phy_yt8512_get_speed_duplex(unsigned int phy_addr, unsigned int *speed, unsigned int *duplex)
{
    unsigned int value = 0;
    int ret;

    ret = phy_yt85xx_read(phy_addr, PHY_SPEC_STATUS, 0x0, &value);
    if (ret) {
        return ret;
    }

    if ((value & MASK_YT85XX_SPEED) == PHY_SPEED_1000M_MODE) {
        *speed = MAC_1000M_GMII;
    } else if ((value & MASK_YT85XX_SPEED) == PHY_SPEED_100M_MODE) {
        *speed = MAC_100M_RMII;
    } else if ((value & MASK_YT85XX_SPEED) == PHY_SPEED_10M_MODE) {
        *speed = MAC_10M_RMII;
    } else {
        *speed = MAC_100M_RMII;
    }

    /* Bit 13: indicates the duplex mode. The value 1 indicates the full duplex mode,
    and the value 0 indicates the half duplex mode. */
    *duplex = (value & BIT(13)) ? GMAC_DUPLEX_FULL : GMAC_DUPLEX_HALF;

    return BM_OK;
}

int phy_yt85xx_restart_autoneg(unsigned int phy_addr)
{
    int ret;
    unsigned int value = 0;

    ret = phy_yt85xx_read(phy_addr, REG_PHY_CTRL, 0, &value);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "read control reg error %u", phy_addr);
        return BM_FAIL;
    }

    value |= MASK_PHY_CTRL_RESTART_AUTONEG;
    ret = phy_yt85xx_write(phy_addr, REG_PHY_CTRL, 0, value);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "write control reg error %u", phy_addr);
        return BM_FAIL;
    }

    udelay(10000); /* delay 10000us */

    return BM_OK;
}

int phy_yt85xx_get_id(unsigned int phy_addr, unsigned int *identifier)
{
    int ret;
    unsigned int identifier_l32 = 0;
    unsigned int identifier_h32 = 0;

    ret = phy_yt85xx_read(phy_addr, MII_PHY_ID0_REG, 0, &identifier_h32);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "yt8521(0x%x) read reg[0x%x] error!", phy_addr, MII_PHY_ID0_REG);
        return ret;
    }

    ret = phy_yt85xx_read(phy_addr, MII_PHY_ID1_REG, 0, &identifier_l32);
    if (ret) {
        bm_debug(BM_ERROR_LEVEL, "yt8521(0x%x) read reg[0x%x] error!", phy_addr, MII_PHY_ID1_REG);
        return ret;
    }

    /* The most significant bit is shifted to the left by 16 bits to form the vendor PHY ID. */
    *identifier = ((identifier_h32 << 16) | (identifier_l32 & IDENTIFIER_LOW_MASK));

    return ret;
}

int phy_yt8512_init(unsigned int phy_addr)
{
    unsigned int val = 0;
    int ret;

    ret = phy_yt85xx_restart_autoneg(phy_addr);
    if (ret) {
        return ret;
    }

    ret = phy_yt85xx_read_ext_reg(phy_addr, 0, &val);
    if (ret) {
        return ret;
    }
    val &= ~BIT(6); /* BIT6: 0 Disable Broadcast addr  */

    /* When LED1 is set to 0xc03, it indicates that the link is up and the link is down. */
    ret = phy_yt85xx_write_ext_reg(phy_addr, 0x40c3, 0xc03);
    if (ret) {
        return ret;
    }
    /* When LED0 is set to 0x1300, data transmission blinks. */
    ret = phy_yt85xx_write_ext_reg(phy_addr, 0x40c0, 0x1300);
    if (ret) {
        return ret;
    }

    return phy_yt85xx_write_ext_reg(phy_addr, 0, val);
}