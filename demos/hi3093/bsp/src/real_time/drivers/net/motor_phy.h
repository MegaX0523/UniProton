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

#ifndef __PHY_H__
#define __PHY_H__

/* PHY link status */
#define PHY_LINK_DOWN 0
#define PHY_LINK_UP   1

#define REG_PHY_PAGE0 0
#define REG_PHY_CTRL  0

/* page any */
#define MII_PAGE_ADDR_REG   0x16
#define MASK_STATUS_LINK    0x04
#define MASK_STATUS_AUTONEG 0x20

#define MAC_10M_RMII    0
#define MAC_100M_RMII   1
#define MAC_1000M_GMII  2

#define MAC_10M_RGMII   3
#define MAC_100M_RGMII  4
#define MAC_1000M_RGMII 5

#define MAC_10M_SGMII   6
#define MAC_100M_SGMII  7
#define MAC_1000M_SGMII 8

/* Duplex, half or full. */
#define GMAC_DUPLEX_HALF 0x00
#define GMAC_DUPLEX_FULL 0x01

#define IDENTIFIER_YT8521      0x0000011a
#define IDENTIFIER_YT8512      0x00000128

#define IDENTIFIER_LOW_MASK     0xFFFF

#define SGMII_RTL_PAGE_REG    0x1f
#define ENABLE_BROADCAST_FUN  BIT(13)

#define MASK_8211_DUPLEX BIT(3)
#define PHY_RESET        BIT(15)

#define MASK_PHY_CTRL_RESTART_AUTONEG BIT(9)
#define AUTO_NEGOTIATION_ENABLE       BIT(12)

/* Standard MII Registers */
#define MII_CTRL_REG     0x00 /* MII Control Register */
#define MII_PHY_ID0_REG  0x02 /* MII PHY ID register */
#define MII_PHY_ID1_REG  0x03 /* MII PHY ID register */

#define phy_reg_page(x) (x)
#define phy_reg_num(n)  (n)

#define RTL8211_LEDON_VALUE    0xc00b
#define BYPASS_MODE_STEP_VALUE 0xdc88

#define MASK_PHY_CTRL_ANENABLE  0x1000
#define MASK_PHY_CTRL_ANRESTART 0x0200

/* Rate mode represented by bits 15 and 14 of yt8521 and marvel 1512 phy addr 0x11 */
#define PHY_SPEED_1000M_MODE        0x8000
#define PHY_SPEED_100M_MODE         0x4000
#define PHY_SPEED_10M_MODE          0x0

/* ----------------------------------------------*
 * Description of External Variables                                 *
 *---------------------------------------------- */
/* common */
int phy_yt85xx_read(unsigned int phy_addr, unsigned int reg_num, unsigned int page, unsigned int *pvalue);

int phy_yt85xx_write(unsigned int phy_addr, unsigned int reg_num, unsigned int page, unsigned int value);

int phy_yt85xx_get_link_status(unsigned int phy_addr, unsigned int *status);

int phy_yt85xx_restart_autoneg(unsigned int phy_addr);

int phy_yt85xx_get_id(unsigned int phy_addr, unsigned int *identifier);

/* YT8521 */
int phy_yt8521_init(unsigned int phy_addr);

int phy_yt8521_get_speed_duplex(unsigned int phy_addr, unsigned int *speed, unsigned int *duplex);

/* YT8512 */
int phy_yt8512_init(unsigned int phy_addr);

int phy_yt8512_get_speed_duplex(unsigned int phy_addr, unsigned int *speed, unsigned int *duplex);

#endif /* __PHY_H__ */

