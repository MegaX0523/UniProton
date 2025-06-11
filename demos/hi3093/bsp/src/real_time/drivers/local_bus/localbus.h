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

#include "bm_types.h"
#ifndef __LOCALBUS_H__
#define __LOCALBUS_H__

#define LB_BASE_ADDR                  0x8712000

// offset addr for Local_Bus registers
#define LB_CS0_BASE_ADDR_OFFSET_ADDR     0x0         /* 片选0基地址寄存器。 */
#define LB_CS1_BASE_ADDR_OFFSET_ADDR     0x4         /* 片选1基地址寄存器。 */
#define LB_CS2_BASE_ADDR_OFFSET_ADDR     0x8         /* 片选2基地址寄存器。 */
#define LB_CS3_BASE_ADDR_OFFSET_ADDR     0xC         /* 片选3基地址寄存器。 */
#define LB_CS0_CTRL_OFFSET_ADDR          0x20        /* 片选0控制寄存器。 */
#define LB_CS1_CTRL_OFFSET_ADDR          0x24        /* 片选1控制寄存器。 */
#define LB_CS2_CTRL_OFFSET_ADDR          0x28        /* 片选2控制寄存器。 */
#define LB_CS3_CTRL_OFFSET_ADDR          0x2C        /* 片选3控制寄存器。 */
#define LB_CS0_T_SET0_OFFSET_ADDR        0x40        /* 片选0的时序参数配置寄存器0。 */
#define LB_CS0_T_SET1_OFFSET_ADDR        0x44        /* 片选0的时序参数配置寄存器1。 */
#define LB_CS0_T_SET2_OFFSET_ADDR        0x48        /* 片选0的时序参数配置寄存器2。 */
#define LB_CS1_T_SET0_OFFSET_ADDR        0x4C        /* 片选1的时序参数配置寄存器0。 */
#define LB_CS1_T_SET1_OFFSET_ADDR        0x50        /* 片选1的时序参数配置寄存器1。 */
#define LB_CS1_T_SET2_OFFSET_ADDR        0x54        /* 片选1的时序参数配置寄存器2。 */
#define LB_CS2_T_SET0_OFFSET_ADDR        0x58        /* 片选2的时序参数配置寄存器0。 */
#define LB_CS2_T_SET1_OFFSET_ADDR        0x5C        /* 片选2的时序参数配置寄存器1。 */
#define LB_CS2_T_SET2_OFFSET_ADDR        0x60        /* 片选2的时序参数配置寄存器2。 */
#define LB_CS3_T_SET0_OFFSET_ADDR        0x64        /* 片选3的时序参数配置寄存器0。 */
#define LB_CS3_T_SET1_OFFSET_ADDR        0x68        /* 片选3的时序参数配置寄存器1。 */
#define LB_CS3_T_SET2_OFFSET_ADDR        0x6C        /* 片选3的时序参数配置寄存器2。 */
#define LB_INTMSK_OFFSET_ADDR            0xA0        /* LB_INTMSK为中断屏蔽寄存器。 */
#define LB_INTSTS_OFFSET_ADDR            0xA4        /* LB_INTSTS为中断状态寄存器。 */
#define LB_RINT_OFFSET_ADDR              0xA8        /* LB_RINT为原始中断状态寄存器。 */
#define LB_RDY_TOUT_0_OFFSET_ADDR        0xAC        /* LB_RDY_TOUT_0为片选0超时时间。 */
#define LB_RDY_TOUT_1_OFFSET_ADDR        0xB0        /* LB_RDY_TOUT_1为片选1超时时间。 */
#define LB_RDY_TOUT_2_OFFSET_ADDR        0xB4        /* LB_RDY_TOUT_2为片选2超时时间。 */
#define LB_RDY_TOUT_3_OFFSET_ADDR        0xB8        /* LB_RDY_TOUT_3为片选3超时时间。 */
#define LB_CTRL_OFFSET_ADDR              0xCC        /* 总体控制寄存器。 */
#define LB_PROT_ADDR_START_0_OFFSET_ADDR 0xD0        /* PROT_ADDR_START_0寄存器。 */
#define LB_PROT_ADDR_OVER_0_OFFSET_ADDR  0xD4        /* PROT_ADDR_OVER_0寄存器。 */
#define LB_PROT_ADDR_START_1_OFFSET_ADDR 0xD8        /* PROT_ADDR_START_1寄存器。 */
#define LB_PROT_ADDR_OVER_1_OFFSET_ADDR  0xDC        /* PROT_ADDR_OVER_1寄存器。 */
#define LB_PROT_ADDR_START_2_OFFSET_ADDR 0xE0        /* PROT_ADDR_START_2寄存器。 */
#define LB_PROT_ADDR_OVER_2_OFFSET_ADDR  0xE4        /* PROT_ADDR_OVER_2寄存器。 */
#define LB_PROT_ADDR_START_3_OFFSET_ADDR 0xE8        /* PROT_ADDR_START_3寄存器。 */
#define LB_PROT_ADDR_OVER_3_OFFSET_ADDR  0xEC        /* PROT_ADDR_OVER_3寄存器。 */
#define LB_PROT_ADDR_START_4_OFFSET_ADDR 0xF0        /* PROT_ADDR_START_4寄存器。 */
#define LB_PROT_ADDR_OVER_4_OFFSET_ADDR  0xF4        /* PROT_ADDR_OVER_4寄存器。 */
#define LB_PROT_ADDR_START_5_OFFSET_ADDR 0xF8        /* PROT_ADDR_START_5寄存器。 */
#define LB_PROT_ADDR_OVER_5_OFFSET_ADDR  0xFC        /* PROT_ADDR_OVER_5寄存器。 */
#define LB_PROT_ADDR_START_6_OFFSET_ADDR 0x100       /* PROT_ADDR_START_6寄存器。 */
#define LB_PROT_ADDR_OVER_6_OFFSET_ADDR  0x104       /* PROT_ADDR_OVER_6寄存器。 */
#define LB_PROT_ADDR_START_7_OFFSET_ADDR 0x108       /* PROT_ADDR_START_6寄存器。 */
#define LB_PROT_ADDR_OVER_7_OFFSET_ADDR  0x10C       /* PROT_ADDR_OVER_6寄存器。 */
#define LB_NOR_CMD0_ADDR_OFFSET_ADDR     0x110       /* nor flash 命令字0地址配置。 */
#define LB_NOR_CMD0_MASK_OFFSET_ADDR     0x114       /* nor flash 命令字0地址mask配置。 */
#define LB_NOR_CMD1_ADDR_OFFSET_ADDR     0x118       /* nor flash 命令字1地址配置。 */
#define LB_NOR_CMD1_MASK_OFFSET_ADDR     0x11C       /* nor flash 命令字1地址mask配置。 */
#define LB_NOR_CMD2_ADDR_OFFSET_ADDR     0x120       /* nor flash 命令字2地址配置。 */
#define LB_NOR_CMD2_MASK_OFFSET_ADDR     0x124       /* nor flash 命令字2地址mask配置。 */
#define LB_NOR_CMD3_ADDR_OFFSET_ADDR     0x128       /* nor flash 命令字3地址配置。 */
#define LB_NOR_CMD3_MASK_OFFSET_ADDR     0x12C       /* nor flash 命令字3地址mask配置。 */
#define LB_HL_PROT_KEY0_OFFSET_ADDR      0x130       /* 高级写保护配置字 */
#define LB_HL_PROT_KEY1_OFFSET_ADDR      0x134       /* 高级写保护配置字 */
#define LB_HL_PROT_KEY2_OFFSET_ADDR      0x138       /* 高级写保护配置字 */
#define LB_HL_PROT_KEY3_OFFSET_ADDR      0x13C       /* 高级写保护配置字 */
#define LB_DBG_ST0_OFFSET_ADDR           0x140       /* debug状态寄存器0 */
#define LB_DBG_OVER_TIME_ADDR_OFFSET_ADDR   0x144    /* debug状态寄存器1 */
#define LB_DBG_T_CFG_OFFSET_ADDR         0x148       /* debug状态寄存器2 */
#define LB_CF_CARD_ST_OFFSET_ADDR        0x150       /* CF卡状态寄存器 */
#define LB_CFG_T_UPDATE_EN_OFFSET_ADDR   0x154       /* 时序配置更新使能 */
#define LB_WP_RSP_ERR_EN_OFFSET_ADDR     0x158       /* 写保护返回error使能寄存器 */

#define LOCALBUS_IOMUX_PIN_NUM 20
#define LB_REG_CF_DATA_WID_BITS 3 /* 数据位宽[5:6] 2bit, mask=3 */
#define LB_REG_CF_DATA_WID_POS 5 /* 数据位宽起始bit位 5 */

#define localbus_readl(c) (*(volatile unsigned int *)((uintptr_t)(c)))
#define localbus_writel(v, c) (*(volatile unsigned int *)((uintptr_t)(c)) = (unsigned int)(v))

#define localbus_cs_readl(csi, desc) localbus_readl(g_localbus_driver_data.base_reg + (csi)->LOCALBUS_##desc)
#define localbus_cs_wrtel(v, csi, desc) localbus_writel((v), g_localbus_driver_data.base_reg + (csi)->LOCALBUS_##desc)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif /* ARRAY_SIZE */

#endif /* __LOCALBUS_H__ */
