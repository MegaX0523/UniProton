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

#ifndef __NET_GMAC_REGS_H__
#define __NET_GMAC_REGS_H__

#define MAC01_ALL_PORT    0

/******************************************************************************/
/*                      PhosphorV600 MAC_CORE Register Definition                      */
/******************************************************************************/
/* Full-duplex and half-duplex mode register */
#define MAC_CORE_DUPLEX_TYPE            0x8
/* FD_FC_TYPE is a PAUSE frame type field register. */
#define MAC_CORE_FD_FC_TYPE_REG         0xC
/* FIFO_THRSLD is a MAC application-side FIFO parameter configuration register */
#define MAC_CORE_FIFO_THRSLD_REG        0x10
/* FC_TX_TIMER is a flow control time parameter register. */
#define MAC_CORE_FC_TX_TIMER_REG        0x1C
/* FD_FC_ADDR_LOW is PAUSE frame destination address register 1. */
#define MAC_CORE_FD_FC_ADDR_LOW_REG     0x20
/* FD_FC_ADDR_HIGH is PAUSE frame destination address register 2. */
#define MAC_CORE_FD_FC_ADDR_HIGH_REG    0x24
/* IPG_TX_TIMER is a TX IFG register. */
#define MAC_CORE_IPG_TX_TIMER_REG       0x30
/* PAUSE_THR is a TX pause IFG register. */
#define MAC_CORE_PAUSE_THR_REG          0x38
/* MAX_FRM_SIZE is the maximum frame length register. */
#define MAC_CORE_MAX_FRM_SIZE_REG       0x3C
/* PORT_MODE is a port status register. */
#define MAC_CORE_PORT_MODE_REG          0x40
/* PORT_EN is a channel enable register. */
#define MAC_CORE_PORT_EN_REG            0x44
/* PAUSE_EN is a flow control enable register. */
#define MAC_CORE_PAUSE_EN_REG           0x48
/* Short_RUNTS_THR is a short frame threshold register. */
#define MAC_CORE_SHORT_RUNTS_THR_REG    0x50
/* DROP_UNK_CTL_FRM is an unknown control frame drop enable register. */
#define MAC_CORE_DROP_UNK_CTL_FRM_REG   0x54
/* Auto-negotiation status register */
#define MAC_CORE_AN_NEG_STATE           0x58
/* Auto-negotiation local configuration register */
#define MAC_CORE_TX_LOCAL_PAGE          0x5C
/* TRANSMIT_CONTROL is a common configuration register. */
#define MAC_CORE_TRANSMIT_CONTROL_REG   0x60
/* REC_FILT_CONTROL is a RX frame filtering control register. */
#define MAC_CORE_REC_FILT_CONTROL_REG   0x64
/* PORT_MC_ADDR_LOW is multicast address register 1. */
#define MAC_CORE_PORT_MC_ADDR_LOW_REG   0x68
/* PORT_MC_ADDR_High is multicast address register 2. */
#define MAC_CORE_PORT_MC_ADDR_HIGH_REG  0x6C
/* MAC_CLR is a MAC clear register. */
#define MAC_CORE_MAC_CLR_REG            0x70
/* RX_OCTETS_TOTAL_OK is a byte count register for received valid frames. */
#define RX_OCTETS_TOTAL_OK_REG          0x80
/* RX_OCTETS_BAD is a byte count register for received error frames. */
#define RX_OCTETS_BAD_REG               0x84
/* RX_UC_PKTS is a MAC RX unicast frame count register. */
#define RX_UC_PKTS_REG                  0x88
/* RX_MC_PKTS is a received multicast frame count register. */
#define RX_MC_PKTS_REG                  0x8C
/* RX_BC_PKTS is a received broadcast frame count register. */
#define RX_BC_PKTS_REG                  0x90
/* RX_PKTS_64OCTETS is a 64-byte frame count register. */
#define RX_PKTS_64OCTETS_REG            0x94
/* RX_PKTS_65TO127OCTETS is a frame count register for the received frames (65–127 bytes). */
#define RX_PKTS_65TO127OCTETS_REG       0x98
/* RX_PKTS_128TO255OCTETS is a frame count register for the received frames (128–255 bytes). */
#define RX_PKTS_128TO255OCTETS_REG      0x9C
/* RX_PKTS_255TO511OCTETS is a frame count register for the received frames ranging from 256 bytes to 511 bytes. */
#define RX_PKTS_255TO511OCTETS_REG      0xA0
/* RX_PKTS_512TO1023OCTETS is a frame count register for the received frames ranging from 512 bytes to 1023 bytes. */
#define RX_PKTS_512TO1023OCTETS_REG     0xA4
/* RX_PKTS_1024TO1518OCTETS is a frame count register for received frames ranging from 1024 bytes to 1518 bytes. */
#define RX_PKTS_1024TO1518OCTETS_REG    0xA8
/* RX_PKTS_1024TO1518OCTETS is a frame count register for received frames ranging from 1024 bytes to 1518 bytes. */
#define RX_PKTS_1519TOMAXOCTETS_REG     0xAC
/* RX_FCS_ERRORS is a frame count register with received CRC errors. */
#define RX_FCS_ERRORS_REG               0xB0
/* RX_TAGGED is a TAG frame count register. */
#define RX_TAGGED_REG                   0xB4
/* RX_DATA_ERR is an RX data error frame count register. */
#define RX_DATA_ERR_REG                 0xB8
/* RX_ALIGN_ERRORS is a frame count register with received data non-byte alignment errors. */
#define RX_ALIGN_ERRORS_REG             0xBC
/* RX_LONG_ERRORS is frame count register 1 for the received frames whose
length is greater than the allowed maximum length. */
#define RX_LONG_ERRORS_REG              0xC0
/* RX_JABBER_ERRORS is frame count register 2 for the received frames
whose length is greater than the allowed maximum length. */
#define RX_JABBER_ERRORS_REG            0xC4
/* RX_PAUSE_MACCONTROL_FRAMCOUNTER is a received pause frame count register. */
#define RX_PAUSE_FRAME_REG              0xC8
/* Count register for non-pause control frames received by the MAC */
#define RX_UNKNOWN_MACCONTROL_FRAMCOUNTER_REG 0xCC
/* RX_VERY_LONG_ERR_CNT is a received oversized frame count register. */
#define RX_VERY_LONG_ERR_CNT_REG        0xD0
/* RX_RUNT_ERR_CNT is a frame count register for the received frames whose
length is less than 64 bytes and greater than or equal to 12 bytes. */
#define RX_RUNT_ERR_CNT_REG             0xD4
/* RX_SHORT_ERR_CNT is a frame count register for the received frames whose length is less than 96 bits. */
#define RX_SHORT_ERR_CNT_REG            0xD8
/* Number of packets filtered out in the receive direction */
#define RX_FILT_PKT_CNT_REG             0xE8
/* Indicates the number of bytes filtered out in the receive direction. */
#define RX_OCTETS_TOTAL_FILT_CNT_REG    0xEC
/* OCTETS_TRANSMITTED_OK is a count register for the bytes of successfully transmitted good packets. */
#define TX_OCTETS_TOTAL_OK_REG          0x100
/* OCTETS_TRANSMITTED_BAD is a byte count register for successfully transmitted bad packets. */
#define TX_OCTETS_TRANSMITTED_BAD_REG   0x104
/* TX_UC_PKTS is a TX unicast frame count register. */
#define TX_UC_PKTS_REG                  0x108
/* TX_MC_PKTS is a TX multicast frame count register. */
#define TX_MC_PKTS_REG                  0x10C
/* TX_BC_PKTS is a transmitted broadcast frame count register. */
#define TX_BC_PKTS_REG                  0x110
/* TX_PKTS_64OCTETS is a 64-byte frame count register. */
#define TX_PKTS_64OCTETS_REG            0x114
/* TX_PKTS_65TO127OCTETS is a frame count register for the transmitted frames (65–127 bytes). */
#define TX_PKTS_65TO127OCTETS_REG       0x118
/* TX_PKTS_128TO255OCTETS is a frame count register for the transmitted frames (128–255 bytes). */
#define TX_PKTS_128TO255OCTETS_REG      0x11C
/* TX_PKTS_255TO511OCTETS is a frame count register for the transmitted frames (256–511 bytes). */
#define TX_PKTS_255TO511OCTETS_REG      0x120
/* TX_PKTS_512TO1023OCTETS is a frame count register for transmitted frames ranging from 512 bytes to 1023 bytes. */
#define TX_PKTS_512TO1023OCTETS_REG     0x124
/* TX_PKTS_1024TO1518OCTETS is a frame count register for the
    transmitted frames ranging from 1024 bytes to 1518 bytes. */
#define TX_PKTS_1024TO1518OCTETS_REG    0x128
/* TX_PKTS_1519TOMAXOCTETS is a frame count register for the TX frames longer than 1519 bytes. */
#define TX_PKTS_1519TOMAXOCTETS_REG     0x12C
/* Count register for the number of transmission failures because the maximum frame length is exceeded */
#define TX_EXCESSIVE_LENGTH_DROP_REG    0x14C
/* TX_DOWNRUN is the number of times that frames fail to be transmitted due to internal errors during transmission. */
#define TX_UNDERRUN_REG                 0x150
/* TX_TAGGED is a transmitted VLAN frame count register. */
#define TX_TAGGED_REG                   0x154
/* TX_CRC_ERROR is the frame count register for the transmitted frames with correct lengths and CRC errors. */
#define TX_CRC_ERROR_REG                0x158
/* TX_PAUSE_FRAMES is a TX PAUSE frame count register. */
#define TX_PAUSE_FRAMES_REG             0x15C
/* LED_MOD is an indicator mode control register. */
#define MAC_CORE_LED_MOD_REG            0x16C
/* LINE_LOOP_BACK is a MAC line-side loopback register. */
#define MAC_CORE_LINE_LOOP_BACK_REG     0x1A8
/* CF_CRC_STRI is a CRC stripping enable register. */
#define MAC_CORE_CF_CRC_STRIP_REG       0x1B0
/* MODE_CHANGE_EN is a port mode change enable register. */
#define MAC_CORE_MODE_CHANGE_EN_REG     0x1B4
/* Auto-negotiation link time register */
#define MAC_CORE_SIXTEEN_BIT_CNTR       0x1CC
/* Auto-negotiation link time update register */
#define MAC_CORE_LD_LINK_COUNTER        0x1D0
/* RING_REG is a loopback supplement register. */
#define MAC_CORE_LOOP_REG               0x1DC
/* RECV_CONTROL is an RX control register. */
#define MAC_CORE_RECV_CONTROL_REG       0x1E0
/* VLAN_CODE is a VLAN code register. */
#define MAC_CORE_VLAN_CODE_REG          0x1E8
/* RX_OVERRUN_CNT is a FIFO overflow count register. */
#define RX_OVERRUN_CNT_REG              0x1EC
/* RX_LENGTHFIELD_ERR_CNT is a PAD frame count register. */
#define RX_LENGTHFIELD_ERR_CNT_REG      0x1F4
/* RX_FAIL_COMMA_CNT is a byte delimitation COMMA statistics register. */
#define RX_FAIL_COMMA_CNT_REG           0x1F8
/* STATION_ADDR_LOW is local MAC address 0 register 1. */
#define MAC_CORE_STATION_ADDR_LOW_0_REG         0x200
/* STATION_ADDR_HIGH is local MAC address 0 register 2. */
#define MAC_CORE_STATION_ADDR_HIGH_0_REG        0x204
/* STATION_ADDR_LOW is local MAC address 1 register 1. */
#define MAC_CORE_STATION_ADDR_LOW_1_REG         0x208
/* STATION_ADDR_HIGH is local MAC address 1 register 2. */
#define MAC_CORE_STATION_ADDR_HIGH_1_REG        0x20C
/* STATION_ADDR_LOW is local MAC address 2 register 1. */
#define MAC_CORE_STATION_ADDR_LOW_2_REG         0x210
/* STATION_ADDR_HIGH is local MAC address 2 register 2. */
#define MAC_CORE_STATION_ADDR_HIGH_2_REG        0x214
/* STATION_ADDR_LOW is local MAC address 3 register 1. */
#define MAC_CORE_STATION_ADDR_LOW_3_REG         0x218
/* STATION_ADDR_HIGH is local MAC address 3 register 2. */
#define MAC_CORE_STATION_ADDR_HIGH_3_REG        0x21C
/* STATION_ADDR_LOW is local MAC address 4 register 1. */
#define MAC_CORE_STATION_ADDR_LOW_4_REG         0x220
/* STATION_ADDR_HIGH is local MAC address 4 register 2. */
#define MAC_CORE_STATION_ADDR_HIGH_4_REG        0x224
/* STATION_ADDR_LOW is local MAC address 5 register 1. */
#define MAC_CORE_STATION_ADDR_LOW_5_REG         0x228
/* STATION_ADDR_HIGH is local MAC address 5 register 2. */
#define MAC_CORE_STATION_ADDR_HIGH_5_REG        0x22C
/* STATION_ADDR_LOW is the mask of local MAC address 0 register 1. */
#define MAC_CORE_STATION_ADDR_LOW_MSK_0_REG     0x230
/* STATION_ADDR_HIGH is the mask of local MAC address 0 register 2. */
#define MAC_CORE_STATION_ADDR_HIGH_MSK_0_REG    0x234
/* STATION_ADDR_LOW is the mask of local MAC address 1 register 1. */
#define MAC_CORE_STATION_ADDR_LOW_MSK_1_REG     0x238
/* STATION_ADDR_HIGH is the mask of local MAC address 1 register 2. */
#define MAC_CORE_STATION_ADDR_HIGH_MSK_1_REG    0x23C
/* SkiP_LEN indicates the length of the field that is not parsed in the packet header. */
#define MAC_CORE_MAC_SKIP_LEN_REG               0x240


/******************************************************************************/
/*                      PhosphorV600 PCU Register definition                           */
/******************************************************************************/
/* CF_TX_FIFO_THRSLD is a TX FIFO threshold register. */
#define PCU_CF_TX_FIFO_THRSLD_REG    0x420
/* CF_RX_FIFO_THRSLD is an RX FIFO threshold register. */
#define PCU_CF_RX_FIFO_THRSLD_REG    0x424
/* CF_CFG_FIFO_THRSLD is a FIFO threshold configuration register. */
#define PCU_CF_CFG_FIFO_THRSLD_REG   0x428
/* CF_INTRPT_MSK is an interrupt mask register. */
#define PCU_CF_INTRPT_MSK_REG        0x42C
/* CF_FIFO_CLR is a FIFO clear register. */
#define PCU_CF_FIFO_CLR_REG          0x430
/* CF_INTRPT_STAT is an interrupt status register. */
#define PCU_CF_INTRPT_STAT_REG       0x434
/* CF_INTRPT_CLR is an interrupt clear register. */
#define PCU_CF_INTRPT_CLR_REG        0x438
/* BUS_ERR_ADDR is an error bus address register. */
#define PCU_BUS_ERR_ADDR_REG         0x43C
/* MAX_FRAME_LEN is a RX oversized frame threshold register. */
#define PCU_MAX_FRAME_LEN_REG        0x444
/* TX_DROP_CNT is a count register for the packets discarded due to empty FIFO in the TX direction. */
#define PCU_TX_DROP_CNT_REG          0x448
/* OVER_FLOW_CNT is a count register for packets discarded because the RX FIFO is full. */
#define PCU_RX_OVER_FLOW_CNT_REG     0x44C
/* DEBUG_ST_MCH state machine report register. */
#define PCU_DEBUG_ST_MCH_REG         0x450
/* FIFO_CURR_STATUS is a FIFO current status register. */
#define PCU_FIFO_CURR_STATUS_REG     0x454
/* FIFO_HIS_STATUS is a FIFO historical status register. */
#define PCU_FIFO_HIS_STATUS_REG      0x458
/* CF_CFF_DATA_NUM is a FIFO data count configuration register. */
#define PCU_CF_CFF_DATA_NUM_REG      0x45C
/* TX_CS_FAIL_CNT is a TX checksum adding failure counter register. */
#define PCU_TX_CS_FAIL_CNT_REG       0x460
/* RX_TRANS_PKG_CNT is a counter register for packets that are successfully transferred in the downlink. */
#define PCU_RX_TRANS_PKG_CNT_REG     0x464
/* TX_TRANS_PKG_CNT is a counter register for packets that are successfully transferred in the uplink. */
#define PCU_TX_TRANS_PKG_CNT_REG     0x468
/* RX_ADDR_OVERFLOW is a write line configuration FIFO overflow counter register. */
#define PCU_RX_ADDR_OVERFLOW_REG     0x46C
/* CF_TX_PAUSE is a TX pause control register. */
#define PCU_CF_TX_PAUSE_REG          0x470
/* TX_CFF_ADDR_0 is TX descriptor FIFO address register 0. */
#define PCU_TX_CFF_ADDR_0_REG        0x488
/* TX_CFF_ADDR_1 is TX descriptor FIFO address register 1. */
#define PCU_TX_CFF_ADDR_1_REG        0x48C
/* TX_CFF_ADDR_2 is TX descriptor FIFO address register 2. */
#define PCU_TX_CFF_ADDR_2_REG        0x490
/* TX_CFF_ADDR_3 is TX descriptor FIFO address register 3. */
#define PCU_TX_CFF_ADDR_3_REG        0x494
/* RX_CFF_ADDR is the RX configuration FIFO address register. */
#define PCU_RX_CFF_ADDR_REG          0x4A0
/* WE_ADDR is a packet descriptor write address register. */
#define PCU_WE_ADDR_REG              0x4E0
/* RX_BUF_SIZE is the RX buffer size register. */
#define PCU_RX_BUF_SIZE_REG          0x4E4
/* BUS_CTRL is a bus control register. */
#define PCU_BUS_CTRL_REG             0x4E8
/* RX_BUF_REQ_ADDR is an address register for applying for the RX buffer. */
#define PCU_RX_BUF_REQ_ADDR_REG      0x4EC
/* RX_CTRL is an RX control register. */
#define PCU_RX_CTRL_REG              0x4F0
/* RX_PKT_MODE is an RX packet input mode register. */
#define PCU_RX_PKT_MODE_REG          0x4F4
/* QOS_MODE indicates the QoS information generation mode. */
#define PCU_QOS_MODE_REG             0x4F8
/* VLAN_QoS is the lookup table from the priority to the QoS in the VLAN frame. */
#define PCU_VLAN_QOS_REG             0x4FC
/* IP_QOS_0 is IP frame priority-to-QoS lookup table 0. */
#define PCU_IP_QOS_0_REG             0x500
/* IP_QOS_1 is an IP frame priority-to-Qos lookup table 11. */
#define PCU_IP_QOS_1_REG             0x504
/* IP_QOS_2 is the priority-to-QoS lookup table 22 for IP frames. */
#define PCU_IP_QOS_2_REG             0x508
/* IP_QOS_3 is IP frame priority-to-QoS lookup table 3. */
#define PCU_IP_QOS_3_REG             0x50C
/* IP_QOS_4 is IP frame priority-to-QoS lookup table 4. */
#define PCU_IP_QOS_4_REG             0x510
/* IP_QOS_5 is the priority-to-QoS lookup table 55 register for IP frames. */
#define PCU_IP_QOS_5_REG             0x514
/* IP_QOS_6 is the priority-to-QoS lookup table 66 register for IP frames. */
#define PCU_IP_QOS_6_REG             0x518
/* IP_QOS_6 is IP frame priority-to-QoS lookup table 7. */
#define PCU_IP_QOS_7_REG             0x51C
/* GRP_MODE is group information generation mode register 0. */
#define PCU_GRP_MODE_0_REG           0x520
/* GRP_MODE is group information generation mode register 1. */
#define PCU_GRP_MODE_1_REG           0x524
/* TT_MODE indicates the type information generation mode. */
#define PCU_TT_MODE_REG              0x528
/* TAG_MODE is the tag information generation mode register. */
#define PCU_TAG_MODE_REG             0x52C
/* TAG_MASK_0 is packet header byte mask table 0 used in tag calculation. */
#define PCU_TAG_MASK_0_REG           0x530
/* TAG_MASK_0 is packet header byte mask table 1 used in tag calculation. */
#define PCU_TAG_MASK_1_REG           0x534
/* MATCH_MASK_0 is packet header byte mask table 0. */
#define PCU_MATCH_MASK_0_REG         0x538
/* MATCH_TBL_0 is byte table 0 for header matching. */
#define PCU_MATCH_TBL_0_REG          0x540
/* MATCH_TBL_1 is byte table 1 for header matching. */
#define PCU_MATCH_TBL_1_REG          0x544
/* MATCH_TBL_2 is byte table 2 for header matching. */
#define PCU_MATCH_TBL_2_REG          0x548
/* MATCH_TBL_3 is byte table 3 for header matching. */
#define PCU_MATCH_TBL_3_REG          0x54C
/* MATCH_TBL_4 is byte table 4 for header matching. */
#define PCU_MATCH_TBL_4_REG          0x550
/* MATCH_TBL_5 is byte table 5 for header matching. */
#define PCU_MATCH_TBL_5_REG          0x554
/* MATCH_TBL_6 is byte table 6 for header matching. */
#define PCU_MATCH_TBL_6_REG          0x558
/* MATCH_TBL_7 is byte table 7 for header matching. */
#define PCU_MATCH_TBL_7_REG          0x55C
/* RXBUF_REQ_TIMER is an RX buffer application timeout register. */
#define PCU_RXBUF_REQ_TIMER_REG      0x580
/* RX_PD_TIMER is a timeout register for receiving a work entry write failure. */
#define PCU_RX_WE_TIMER_REG          0x584
/* TX_REL_TIMER is a timeout register after buffer release fails after TX completion. */
#define PCU_TX_REL_TIMER_REG         0x588
/* RX_BUFRQ_ERR_CNT is an RX buffer application error counter register. */
#define PCU_RX_BUFRQ_ERR_CNT_REG     0x58C
/* TX_BUFRQ_ERR_CNT is a buffer release error counter register in the TX direction. */
#define PCU_TX_BUFRL_ERR_CNT_REG     0x590
/* RX_WE_ERR_CNT is a work descriptor write error counter register in the RX direction. */
#define PCU_RX_WE_ERR_CNT_REG        0x594
/* RX_GRP_TBL_0 is group matching entry 0. */
#define PCU_RX_GRP_TBL_0_REG         0x598
/* RX_GRP_TBL_1 is group matching entry 1. */
#define PCU_RX_GRP_TBL_1_REG         0x59C
/* RX_GRP_TBL_2 is group matching entry 2. */
#define PCU_RX_GRP_TBL_2_REG         0x5A0
/* RX_GRP_TBL_3 is group matching entry 3. */
#define PCU_RX_GRP_TBL_3_REG         0x5A4
/* RX_GRP_TBL_4 is group matching entry 4. */
#define PCU_RX_GRP_TBL_4_REG         0x5A8
/* RX_GRP_TBL_5 is group matching entry 5. */
#define PCU_RX_GRP_TBL_5_REG         0x5AC
/* RX_GRP_TBL_6 is group matching entry 6. */
#define PCU_RX_GRP_TBL_6_REG         0x5B0
/* RX_GRP_TBL_7 is group matching entry 7. */
#define PCU_RX_GRP_TBL_7_REG         0x5B4
/* RX_GRP_RES is a match result configuration register. */
#define PCU_RX_GRP_RES_REG           0x5B8
/* RX_GRP_MSK is a group match mask register in byte match mode. */
#define PCU_RX_GRP_MSK_REG           0x5BC
/* ADDR_FILT_S0 is the start address register for address filtering range 0. */
#define PCU_ADDR_FILT_S0_REG         0x5C0
/* ADDR_FILT_S0 is the end address register for address filtering range 0. */
#define PCU_ADDR_FILT_E0_REG         0x5C4
/* ADDR_FILT_S1 is the start address register for address filtering range 0. */
#define PCU_ADDR_FILT_S1_REG         0x5C8
/* ADDR_FILT_S1 is the end address register for address filtering range 0. */
#define PCU_ADDR_FILT_E1_REG         0x5CC
/* ADDR_FILT_S0 is the start address register for address filtering range 0. */
#define PCU_ADDR_FILT_S2_REG         0x5D0
/* ADDR_FILT_S0 is the end address register for address filtering range 0. */
#define PCU_ADDR_FILT_E2_REG         0x5D4
/* ADDR_FILT_S1 is the start address register for address filtering range 0. */
#define PCU_ADDR_FILT_S3_REG         0x5D8
/* ADDR_FILT_S1 is the end address register for address filtering range 0. */
#define PCU_ADDR_FILT_E3_REG         0x5DC

#endif /* __NET_GMAC_REGS_H__ */
