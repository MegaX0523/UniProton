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

#ifndef __NET_GMAC_H__
#define __NET_GMAC_H__

#include "soc_osal.h"

#define GMAC_SEM_TIMEOUT 50

#define CHIP_RX_FIFO_NUM 2
#define CHIP_TX_FIFO_NUM 2

#define MAC_ADDR_LEN    6
#define RX_DESC_LEN     64

#define GMAC_DESC_PKT_LEN_HIGH 8
#define GMAC_DESC_PKT_LEN_LOW 9

#define GMAC_SRST_BIT 0x7
#define GMAC_MAC_CORE_SRST_BIT 0x2
#define GMAC_REG_CFG_SRST_BIT 0x3
#define GMAC_BUS_SYS_SRST_BIT 0x4
#define GMAC_RX_SERDES_SRST_BIT 0x5
#define GMAC_TX_SERDES_SRST_BIT 0x6

#define MPLL_LOCK_TIMES         25   /* 25us */
#define TX_CM_TIMES             230  /* 230us */
#define TX_RX_ENABLE_TIMES      4    /* 4us */

typedef struct {
    char *name;
    unsigned int phy_id;
    int (*init)(unsigned int phy_addr);
    int (*get_speed_duplex)(unsigned int phy_addr, unsigned int *speed, unsigned int *duplex);
    int (*get_link_status)(unsigned int phy_addr, unsigned int *status);
} gmac_phy_info_s;

struct gmac_platdata {
    unsigned char rsvd[4];
};

struct gmac_device {
    unsigned int mac_num;
    unsigned int phy_addr;
    unsigned char *rx_data_ring[CHIP_RX_FIFO_NUM];
    unsigned char *tx_data_ring[CHIP_TX_FIFO_NUM];
    unsigned char rx_data_num;
    unsigned char tx_data_num;
    unsigned int init_done;
    unsigned int link_status;
    unsigned int link_speed;
    unsigned int link_mode;
    gmac_phy_info_s *gmac_phy_info;
    osal_semaphore sem;
};

/* Define the union u_transmit_control */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved1 : 5;  /* [4..0] */
        unsigned int an_enable : 1;   /* [5] */
        unsigned int crc_add : 1;     /* [6] */
        unsigned int pad_enable : 1;  /* [7] */
        unsigned int reserved0 : 24; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_transmit_control;

/* Controls the storage format of received packets. */
typedef struct {
    unsigned int cf_buf_1st_skip1;        /* Number of cache lines reserved before
                                            the packet descriptor of the first receive buffer header */
    unsigned int cf_buf_n1st_skip;        /* Number of cache lines reserved for non-first receive cache headers */
    unsigned int cf_rx_align_num;         /* Blank byte aligned with the header of the
                                             data packet in the RX buffer     */
    unsigned int cf_cache_line;           /* Number of cache lines that can be cached in the data packet header */
    unsigned int cf_rxbuf_1st_skip_size2; /* Number of cache lines reserved after the
                                            packet descriptor of the first received cache header */
} femac_pkt_store_ctrl_cfg;
/* Input packet format parameters */
typedef struct {
    unsigned int cf_instr_head_mode; /* Packets are received in command header mode.         */
    unsigned int cf_skip_len;        /* Length of the skip area before the input packet     */
    unsigned int cf_parse_mode;      /* Parsing mode in non-instruction header mode, invalid in command header mode */
    unsigned int mac_skip_len;       /* Length of the field that is not parsed in the packet header     */
} femac_rx_pkt_mode_cfg;

typedef struct {
    unsigned int port_mode; /* Port Mode              */
    unsigned int port_num;                /* Port No.                */
    unsigned int max_frm_size;            /* Maximum frame length of received packets    */
    unsigned int short_runts_thr;         /* Minimum frame length of received packets    */
    unsigned int pad_enable;              /* Indicates whether to automatically add pads when the length of
                                            transmitted packets is smaller than the minimum frame length. */
    unsigned int crc_add;                 /* Enable automatic CRC adding       */
    unsigned int an_enable;               /* Auto-negotiation enabled            */
    unsigned int runt_pkt_en;             /* Enable transparent transmission of received undersized frames    */
    unsigned int strip_pad_en;            /* Enable of automatic pad stripping for RX frames */
} femac_port_mode_cfg;

/* Structure for setting MAC filtering */
typedef struct {
    unsigned int crc_err_pass : 1;   /* CRC Indicates whether to receive error packets. */
    unsigned int pause_frm_pass : 1; /* Control frame reception enable */
    unsigned int bc_drop_en : 1;     /* Broadcast frame discarding enable */
    unsigned int mc_match_en : 1;    /* Whether to discard non-local multicast frames */
    unsigned int uc_match_en : 1;    /* Whether to discard non-local unicast frames */
} femac_mac_filter_cfg;

/* Define the union u_rx_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cf_rxbuf_1st_skip_size2 : 4;   /* [3..0] */
        unsigned int cf_cache_line : 3;             /* [6..4] */
        unsigned int cf_rx_cfg_req_en : 1;          /* [7] */
        unsigned int cf_cache_line_h : 2;           /* [9..8] */
        unsigned int cf_addr_mode : 2;              /* [11..10] */
        unsigned int cf_rx_get_addr_mode : 1;       /* [12] */
        unsigned int cf_port_num : 4;               /* [16..13] */
        unsigned int cf_rx_align_num : 2;           /* [18..17] */
        unsigned int cf_pool_num : 4;               /* [22..19] */
        unsigned int cf_time_inf_en : 1;            /* [23] */
        unsigned int cf_rxbuf_no_1st_skip_size : 4; /* [27..24] */
        unsigned int cf_rxbuf_1st_skip_size : 4;    /* [31..28] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_rx_ctrl;

/* Define the union u_mac_skip_len */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mac_skip_len : 6; /* [5..0] */
        unsigned int mac_skip_crc : 1; /* [6] */
        unsigned int reserved0 : 25;  /* [31..7] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mac_skip_len;

/* Define the union u_max_frm_size */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_frm_size : 16; /* [13..0] */
        unsigned int reserved0 : 16;   /* [31..14] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_max_frm_size;

/* Define the union u_short_runts_thr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int short_runts_thr : 5; /* [4..0] */
        unsigned int reserved0 : 27;     /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_short_runts_thr;

/* Define the union u_recv_control */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved1 : 3;   /* [2..0] */
        unsigned int strip_pad_en : 1; /* [3] */
        unsigned int runt_pkt_en : 1;  /* [4] */
        unsigned int reserved0 : 27;  /* [31..5] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_recv_control;

/* Define the union u_port_mode */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int port_mode : 4;         /* [3..0]  */
        unsigned int rgmii_1000m_delay : 1; /* [4]  */
        unsigned int reserved_0 : 1;        /* [5]  */
        unsigned int fifo_err_auto_rst : 1; /* [6]  */
        unsigned int dbg_clk_los_msk : 1;   /* [7]  */
        unsigned int rmii_clk_edge : 1;     /* [8]  */
        unsigned int rmii_rst : 1;          /* [9]  */
        unsigned int reserved_1 : 22;       /* [31..10]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_port_mode;

/* Define the union u_bus_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cf_rx_alloc : 1; /* [0] */
        unsigned int cf_endian : 2;   /* [2..1] */
        unsigned int cf_buf : 1;      /* [3] */
        unsigned int noncoh_en : 1;   /* [4] */
        unsigned int huser : 1;       /* [5] */
        unsigned int cf_pd_buf : 1;   /* [6] */
        unsigned int reserved0 : 25; /* [31..7] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_bus_ctrl;

/* Define the union u_mode_change_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int mode_change_en : 1; /* [0] */
        unsigned int reserved0 : 31;    /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_mode_change_en;

/* Define the union u_rec_filt_control */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int uc_match_en : 1;    /* [0] */
        unsigned int mc_match_en : 1;    /* [1] */
        unsigned int bc_drop_en : 1;     /* [2] */
        unsigned int reserved_0 : 1;     /* [3] */
        unsigned int pause_frm_pass : 1; /* [4] */
        unsigned int crc_err_pass : 1;   /* [5] */
        unsigned int reserved0 : 26;    /* [31..6] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_rec_filt_control;

/* Define the union u_rx_max_frame_len */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int max_frame_len : 16; /* [15..0] */
        unsigned int reserved0 : 16;    /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_rx_max_frame_len;

/* Define the union u_cf_intrpt_stat */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved1 : 20;          /* [19..0] */
        unsigned int stat_sram_parity_err : 1; /* [20] */
        unsigned int stat_tx_ahb_err : 1;      /* [21] */
        unsigned int stat_buf_avl : 1;         /* [22] */
        unsigned int stat_rel_err : 1;         /* [23] */
        unsigned int stat_txcfg_avl : 1;       /* [24] */
        unsigned int stat_tx_drop : 1;         /* [25] */
        unsigned int stat_rx_drop : 1;         /* [26] */
        unsigned int stat_rx_frm : 1;          /* [27] */
        unsigned int stat_ahb_err : 1;         /* [28] */
        unsigned int stat_mac_fifo_err : 1;    /* [29] */
        unsigned int stat_rbreq_err : 1;       /* [30] */
        unsigned int stat_we_err : 1;          /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cf_intrpt_stat;

/* Define the union u_cf_intrpt_msk */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cf_intrpt_time : 6;             /* [5..0] */
        unsigned int cf_intrpt_pkt : 6;              /* [11..6] */
        unsigned int reserved1 : 8;                 /* [19..12] */
        unsigned int cf_sram_parity_err_int_msk : 1; /* [20] */
        unsigned int cf_tx_ahb_err_int_msk : 1;      /* [21] */
        unsigned int cf_buf_avl_msk : 1;             /* [22] */
        unsigned int cf_rel_buf_msk : 1;             /* [23] */
        unsigned int cf_txcfg_avl_msk : 1;           /* [24] */
        unsigned int cf_tx_drop_int_msk : 1;         /* [25] */
        unsigned int cf_rx_drop_int_msk : 1;         /* [26] */
        unsigned int cf_pkg_intrpt_msk : 1;          /* [27] */
        unsigned int cf_ahb_err_int_msk : 1;         /* [28] */
        unsigned int cf_mac_ff_err_int_msk : 1;      /* [29] */
        unsigned int cf_rbreq_err_int_msk : 1;       /* [30] */
        unsigned int cf_we_err_int_msk : 1;          /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cf_intrpt_msk;

/* Define the union DUPLEX TYPE */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int duplex_sel : 1;  /* [0] */
        unsigned int reserved0 : 31; /* [31..1] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_duplex_type;

/* Define the union u_port_en */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved1 : 1;  /* [0] */
        unsigned int rx_en : 1;       /* [1] */
        unsigned int tx_en : 1;       /* [2] */
        unsigned int reserved0 : 29; /* [31..3] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_port_en;

typedef struct {
    union {
        struct {
            unsigned int l4_cs : 1;
            unsigned int wb : 1;    /* When Wb is 1, write 0 to the Word3 address
                                    after the packet transmission is complete. */
            unsigned int l3_cs : 1; /* Checksum is required. */
            unsigned int reserved2 : 16;
            unsigned int ds : 6; /* Number of data segments. This parameter is valid only when Sd is 1. */
            unsigned int sd : 1; /* When the value is 0, the frame contains only
                                    one data segment and points directly to the data segment. When is 1 */
            /* The frame contains multiple segments, pointing to a list of segments */
            unsigned int ipoff : 5; /* Indicates the start offset of the IP packet header in the datagram. */
            unsigned int reserved1 : 1;
        } bits;
        unsigned int u32;
    } word0;

    union {
        struct {
            unsigned int pool : 4;    /* ID of the released pool. */
            unsigned int len : 16;    /* Send Length */
            unsigned int back : 6;    /* Distance between the start address of the cache line
                                        where data is located and the start address of the cache */
            unsigned int fm : 1;      /* The buffer space is released after the transmission is complete. */
            unsigned int l2cache : 1; /* L2 cache read allocation */
            unsigned int reserve : 4;
        } bits;
        unsigned int u32;
    } word1;

    unsigned int pkt_addr;   /* IP address of the packet */
    unsigned int clear_addr; /* This address is cleared after packet transmission is complete. */
} u_tx_pcu_cmd;

/* Define the union u_rx_pkt_mode */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cf_gen_id : 8;          /* [7..0] */
        unsigned int reserved1 : 4;         /* [11..8] */
        unsigned int cf_match_offset : 9;    /* [20..12] */
        unsigned int cf_parse_mode : 2;      /* [22..21] */
        unsigned int cf_skip_len : 7;        /* [29..23] */
        unsigned int reserved0 : 1;         /* [30] */
        unsigned int cf_instr_head_mode : 1; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_rx_pkt_mode;

/* Define the union u_we_addr */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved0 : 1;   /* [0] */
        unsigned int cf_we_enable : 1; /* [1] */
        unsigned int cf_we_addr : 30;  /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_we_addr;

/* Define the union CF_CFG_FIFO_THRSLD_U */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cfg_rx_p_empty_th : 8; /* [7..0] */
        unsigned int cfg_rx_p_full_th : 8;  /* [15..8] */
        unsigned int cfg_tx_p_empty_th : 8; /* [23..16] */
        unsigned int cfg_tx_p_full_th : 8;  /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cf_cfg_fifo_thrsld;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int reserved1 : 20;            /* [19..0] */
        unsigned int cf_sram_parity_err_clr : 1; /* [20] */
        unsigned int cf_tx_ahb_err_clr : 1;      /* [21] */
        unsigned int cf_buf_avl_clr : 1;         /* [22] */
        unsigned int cf_rel_buf_clr : 1;         /* [23] */
        unsigned int cf_txcfg_avl_clr : 1;       /* [24] */
        unsigned int cf_tx_drop_clr : 1;         /* [25] */
        unsigned int cf_rx_drop_clr : 1;         /* [26] */
        unsigned int cf_rx_frm_clr : 1;          /* [27] */
        unsigned int cf_rx_ahb_err_clr : 1;      /* [28] */
        unsigned int cf_mac_fifo_err_clr : 1;    /* [29] */
        unsigned int cf_rbreq_err_clr : 1;       /* [30] */
        unsigned int cf_we_err_clr : 1;          /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cf_intrpt_clr;

/* Define the union u_cf_cff_data_num */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cf_tx_cfg_num : 9; /* [8..0] */
        unsigned int reserved1 : 7;    /* [15..9] */
        unsigned int cf_rx_cfg_num : 9; /* [24..16] */
        unsigned int reserved0 : 7;    /* [31..25] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cf_cff_data_num;

/* Define the union u_cf_tx_fifo_thrsld */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int cf_tx_p_empty_th : 10; /* [9..0] */
        unsigned int reserved1 : 6;        /* [15..10] */
        unsigned int cf_tx_p_full_th : 10;  /* [25..16] */
        unsigned int reserved0 : 6;        /* [31..26] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_cf_tx_fifo_thrsld;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tff_cf_full : 1;       /* [0]  */
        unsigned int tff_cf_p_full : 1;     /* [1]  */
        unsigned int tff_cf_empty : 1;      /* [2]  */
        unsigned int tff_cf_p_empty : 1;    /* [3]  */
        unsigned int rff_cf_full : 1;       /* [4]  */
        unsigned int rff_cf_p_full : 1;     /* [5]  */
        unsigned int rff_cf_empty : 1;      /* [6]  */
        unsigned int rff_cf_p_empty : 1;    /* [7]  */
        unsigned int cff_cf_rx_empty : 1;   /* [8]  */
        unsigned int cff_cf_rx_p_empty : 1; /* [9]  */
        unsigned int cff_cf_rx_full : 1;    /* [10]  */
        unsigned int cff_cf_rx_p_full : 1;  /* [11]  */
        unsigned int cff_cf_tx_empty : 1;   /* [12]  */
        unsigned int cff_cf_tx_p_empty : 1; /* [13]  */
        unsigned int cff_cf_tx_full : 1;    /* [14]  */
        unsigned int cff_cf_tx_p_full : 1;  /* [15]  */
        unsigned int reserved_0 : 16;       /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_fifo_curr_status;


/* Define the union u_debug_st_mch */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tx_st_mch : 5;   /* [4..0] */
        unsigned int rx_st_mch : 4;   /* [8..5] */
        unsigned int reserved0 : 23; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_debug_st_mch;

/* Define the union u_hp_mac_haddr_extend */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int m0_haddr_offset : 4;    /* [3..0]  */
        unsigned int m0_addr_remap_mode : 2; /* [5..4]  */
        unsigned int m0_haddr_hi : 2;        /* [7..6]  */
        unsigned int m1_haddr_offset : 4;    /* [11..8]  */
        unsigned int m1_addr_remap_mode : 2; /* [13..12]  */
        unsigned int m1_haddr_hi : 2;        /* [15..14]  */
        unsigned int reserved_0 : 16;        /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_hp_mac_haddr_extend;

/* Define the union u_gmac_phy0_los_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int los_level_0 : 5;            /* [4..0]  */
        unsigned int los_bias_0 : 3;             /* [7..5]  */
        unsigned int rx0_los_filt_cnt_0 : 7;     /* [14..8]  */
        unsigned int tx_vboost_lvl_0 : 3;        /* [17..15]  */
        unsigned int rx_los_valid_rst_msk_0 : 2; /* [19..18]  */
        unsigned int reserved_0 : 12;            /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gmac_phy_los_cfg;

/* Define the union u_gmac_phy0_cfg */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tx0_invert_0 : 1;      /* [0]  */
        unsigned int lane_10bit_sel_0 : 1;  /* [1]  */
        unsigned int ref_ssp_en_0 : 1;      /* [2]  */
        unsigned int tx0_vboost_en_0 : 1;   /* [3]  */
        unsigned int tx0_reset_0 : 1;       /* [4]  */
        unsigned int tx0_en_0 : 1;          /* [5]  */
        unsigned int tx0_cm_en_0 : 1;       /* [6]  */
        unsigned int rx0_term_en_0 : 1;     /* [7]  */
        unsigned int rx0_reset_0 : 1;       /* [8]  */
        unsigned int rx0_pll_en_0 : 1;      /* [9]  */
        unsigned int rx0_los_en_0 : 1;      /* [10]  */
        unsigned int rx0_invert_0 : 1;      /* [11]  */
        unsigned int rx0_data_en_0 : 1;     /* [12]  */
        unsigned int rx0_align_en_0 : 1;    /* [13]  */
        unsigned int mpll_en_0 : 1;         /* [14]  */
        unsigned int lane0_loopbk_en_0 : 1; /* [15]  */
        unsigned int test_bypass_0 : 1;     /* [16]  */
        unsigned int test_burnin_0 : 1;     /* [17]  */
        unsigned int ssc_ref_clk_sel_0 : 1; /* [18]  */
        unsigned int mpll_multiplier_0 : 7; /* [25..19]  */
        unsigned int vreg_bypass_0 : 1;     /* [26]  */
        unsigned int test_powerdown_0 : 1;  /* [27]  */
        unsigned int ref_use_pad_0 : 1;     /* [28]  */
        unsigned int ref_clkdiv2_0 : 1;     /* [29]  */
        unsigned int phy_reset_0 : 1;       /* [30]  */
        unsigned int phy_rtune_req_0 : 1;   /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gmac_phy_cfg;


typedef union {
    /* Define the struct bits */
    struct {
        unsigned int phy_rx0_eq_0 : 3;          /* [2..0]  */
        unsigned int phy_tx0_term_offset_0 : 5; /* [7..3]  */
        unsigned int tx0_amplitude_0 : 7;       /* [14..8]  */
        unsigned int tx0_detect_rx_req_0 : 1;   /* [15]  */
        unsigned int tx0_preemph_0 : 7;         /* [22..16]  */
        unsigned int tx0_data_en_0 : 1;         /* [23]  */
        unsigned int phy_data_reverse_en0 : 1;  /* [24]  */
        unsigned int reserved_0 : 7;            /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gmac_phy_cfg2;

typedef union {
    /* Define the struct bits */
    struct {
        unsigned int tx0_state_0 : 1;            /* [0]  */
        unsigned int tx0_detect_rx_result_0 : 1; /* [1]  */
        unsigned int tx0_detect_rx_ack_0 : 1;    /* [2]  */
        unsigned int tx0_cm_state_0 : 1;         /* [3]  */
        unsigned int rx0_pll_state_0 : 1;        /* [4]  */
        unsigned int mpll_state_0 : 1;           /* [5]  */
        unsigned int rx_valid_0 : 1;             /* [6]  */
        unsigned int rx_los_0 : 1;               /* [7]  */
        unsigned int reserved_0 : 24;            /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int u32;
} u_gmac_phy_sts;

#define FE_RX_FIFO_FULL_THRSHLD (100)
#define FE_RX_FIFO_EMPTY_THRSHLD (80)
#define ENABLE (0x1)
#define DISABLE (0x0)

#define CPU_CACHE_LINE_SIZE (64)
#define PCU_CACHE_LINE_SIZE (32)

/* 以CACHE LINE SIZE 为单位 */
#define FE_RX_SKIP1 (0x00)
#define FE_RX_SKIP2 (0x01)

#define CACHE_LINE_SIZE (32)
#define WORD_ALIGN_SIZE (4)
#define RX_BUFFER_DEFAULT_SIZE 0x640 /* >MTU 1518, cacheline align */
#define FE_MAX_PKT_LEN 1536

#define MAC_ADDR_LEN 6
#define GMAC_MAX_DEV 4

#define GMAC_TX 1
#define GMAC_RX 0

#define MAC_BASE_ADDR (0xcd00000)
#define MAC_REG_ADDR_LEN 0x1000

#define mac_reg_addr(port, offset) (MAC_BASE_ADDR + (port) * MAC_REG_ADDR_LEN + (offset))

/* general register access macros */
#define reg_write(addr, data) (*(volatile unsigned int *)((unsigned long)(addr)) = (data))
#define reg_read(addr, data) ((data) = *(volatile unsigned int *)((unsigned long)(addr)))

#define mac_reg_write(port, offset, data) (reg_write(mac_reg_addr((port), (offset)), ((data))))
#define mac_reg_read(port, offset, data)                    \
    do {                                                    \
        (reg_read(mac_reg_addr((port), (offset)), (data))); \
        (data) = ((data));                       \
    } while (0)

#endif /* __NET_GMAC_H__ */
