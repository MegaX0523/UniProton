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

#ifndef __I2C_H__
#define __I2C_H__

#include <bm_common.h>
#include "bm_ops.h"
#include "lp_subsys.h"
#include "io_config.h"
#include "bm_i2c.h"
#include "i2c_config.h"
#include "soc_osal.h"

#define ICR_CONTROL           0x0000
#define ICR_TAR               0x0004
#define ICR_SAR               0x0008
#define ICR_DATA_CMD          0x0010
#define ICR_SS_SCL_HCNT       0x0014
#define ICR_SS_SCL_LCNT       0x0018
#define ICR_FS_SCL_HCNT       0x001C
#define ICR_FS_SCL_LCNT       0x0020
#define ICR_HS_SCL_HCNT       0x0024
#define ICR_HS_SCL_LCNT       0x0028
#define ICR_INTR_STAT         0x002c
#define ICR_INTR_MASK         0x0030
#define ICR_RAW_INTR_STAT     0x0034
#define ICR_RX_TL             0x0038
#define ICR_TX_TL             0x003C
#define ICR_CLR_INTR          0x0040
#define ICR_CLR_RX_UNDER      0x0044
#define ICR_CLR_RX_OVER       0x0048
#define ICR_CLR_TX_OVER       0x004C
#define ICR_CLR_RD_REQ        0x0050
#define ICR_CLR_TX_ABRT       0x0054
#define ICR_CLR_RX_DONE       0x0058
#define ICR_CLR_ACTIVITY      0x005C
#define ICR_CLR_STOP_DET      0x0060
#define ICR_CLR_START_DET     0x0064
#define ICR_CLR_GEN_CALL      0x0068
#define ICR_ENABLE            0x006C
#define ICR_STATUS            0x0070
#define ICR_TXFLR             0x0074
#define ICR_RXFLR             0x0078
#define ICR_SDA_HOLD          0x007C
#define ICR_TX_ABRT_SOURCE    0x0080
#define ICR_DMA_CR            0x0088
#define ICR_DMA_TDLR          0x008C
#define ICR_DMA_RDLR          0x0090
#define ICR_ENABLE_STATUS     0x009C
#define ICR_SCL_LOW_TIMEOUT   0x00AC
#define ICR_SDA_LOW_TIMEOUT   0x00B0
#define ICR_CLR_SCL_STUCK_DET 0x00B4
#define ICR_CLK_LOW_SEXT      0x00BC
#define ICR_CLK_LOW_MEXT      0x00C0
#define ICR_COMP_PARAM_1      0x00F4  // 存疑
#define ICR_COMP_VERSION      0x00F8

#ifndef BITS_PER_LONG
#define BITS_PER_LONG 32
#endif

#ifndef IS_ODD
#define IS_ODD 2
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif /* ARRAY_SIZE */

#define I2C_GENMASK(h, l) \
    ((((~0UL) - (1UL << (l))) + 1) & ((~0UL) >> ((BITS_PER_LONG - 1) - (h))))


#define IC_M_STATUS_WRITE_IN_PROGRESS 0x1
#define IC_M_STATUS_READ_IN_PROGRESS  0x2
#define IC_M_STATUS_WAIT4_COMPLETION  0x4
#define IC_M_STATUS_INIT              IC_M_STATUS_WAIT4_COMPLETION

#define IC_S_STATUS_IDLE            0x0
#define IC_S_STATUS_WRITE_REQUESTED 0x2
#define IC_S_STATUS_READ_REQUESTED  0x4

#define IC_S_DATA_LEN_MULTI          (2 + IC_MSG_MAX_NUM) /* req_buf + res_buf + req_queue */
#define IC_S_RX_FIFO_TL_COUNT_SMBUS  0x30
#define IC_S_TX_FIFO_TL_COUNT_SMBUS  0x8
#define IC_S_TX_FIFO_DEPTH_SMBUS     0x80

#define IC_SRST_REG_BASE_OFFSET     (9U)  /* Bit 9–bit 24 indicate the soft reset request bits of I2C0–I2C15. */
#define IC_MSG_MAX_LEN              (256U)
#define IC_MSG_TRAN_TIME_EACH_BYTE  (300U) /* Maximum transmission time of each byte */

#define IC_SDA_HOLD_MIN_VERS 0x3131312A
#define IC_SDA_HOLD_TX_SHIFT 0
#define IC_SDA_HOLD_TX_MASK  I2C_GENMASK(15, IC_SDA_HOLD_TX_SHIFT)
#define IC_SDA_HOLD_RX_SHIFT 16
#define IC_SDA_HOLD_RX_MASK  I2C_GENMASK(23, IC_SDA_HOLD_RX_SHIFT)
#define IC_SDA_HOLD_DEFAULT  0x00010001

#define IC_SDA_LOW_TIMEOUT_DEFAULT 0xFFFFFFFF
#define IC_SCL_LOW_TIMEOUT_DEFAULT 0xFFFFFFFF

#define IC_CON_MASTER                BIT(0)
#define IC_CON_SPEED_STD             BIT(1)
#define IC_CON_SPEED_FAST            BIT(2)
#define IC_CON_SPEED_HIGH            (BIT(1) | BIT(2))
#define IC_CON_SPEED_MASK            (BIT(1) | BIT(2))
#define IC_CON_10BITADDR_SLAVE       BIT(3)
#define IC_CON_10BITADDR_MASTER      BIT(4)
#define IC_CON_RESTART_EN            BIT(5)
#define IC_CON_SLAVE_DISABLE         BIT(6)
#define IC_CON_STOP_DET_IFADDRESSED  BIT(7)
#define IC_CON_TX_EMPTY_CTRL         BIT(8)
#define IC_CON_RX_FIFO_FULL_HLD_CTRL BIT(9)

#define IC_TAR_SPECIAL          BIT(11)
#define IC_TAR_10BITADDR_MASTER BIT(12)
#define IC_TAR_SMBUS_QUICK_CMD  BIT(16)

#define IC_ENA_ENABLE BIT(0)
#define IC_ENA_ABORT  BIT(1)

#define IC_STATUS_ACTIVITY        BIT(0)
#define IC_STATUS_TFE             BIT(2)
#define IC_STATUS_RFNE            BIT(3)
#define IC_STATUS_MASTER_ACTIVITY BIT(5)
#define IC_STATUS_SLAVE_ACTIVITY  BIT(6)

#define IC_INTR_RX_UNDER      BIT(0)
#define IC_INTR_RX_OVER       BIT(1)
#define IC_INTR_RX_FULL       BIT(2)
#define IC_INTR_TX_OVER       BIT(3)
#define IC_INTR_TX_EMPTY      BIT(4)
#define IC_INTR_RD_REQ        BIT(5)
#define IC_INTR_TX_ABRT       BIT(6)
#define IC_INTR_RX_DONE       BIT(7)
#define IC_INTR_ACTIVITY      BIT(8)
#define IC_INTR_STOP_DET      BIT(9)
#define IC_INTR_START_DET     BIT(10)
#define IC_INTR_GEN_CALL      BIT(11)
#define IC_INTR_RESTART_DET   BIT(12)
#define IC_INTR_SCL_STUCK_LOW BIT(14)

#define IC_INTR_DEFAULT_MASK (IC_INTR_RX_FULL | IC_INTR_TX_ABRT | IC_INTR_STOP_DET | IC_INTR_RX_UNDER |  \
                              IC_INTR_RX_OVER | IC_INTR_TX_OVER)
#define IC_INTR_MASTER_MASK     (IC_INTR_DEFAULT_MASK | IC_INTR_TX_EMPTY | IC_INTR_SCL_STUCK_LOW)
#define IC_INTR_MASTER_DMA_MASK (IC_INTR_TX_ABRT | IC_INTR_STOP_DET | IC_INTR_SCL_STUCK_LOW)
#define IC_INTR_SLAVE_MASK      (IC_INTR_DEFAULT_MASK | IC_INTR_RX_DONE | IC_INTR_RX_UNDER | \
                                 IC_INTR_RD_REQ | IC_INTR_START_DET)

#define IC_TX_ABRT_7B_ADDR_NOACK  BIT(0)
#define IC_TX_ABRT_10ADDR1_NOACK  BIT(1)
#define IC_TX_ABRT_10ADDR2_NOACK  BIT(2)
#define IC_TX_ABRT_TXDATA_NOACK   BIT(3)
#define IC_TX_ABRT_GCALL_NOACK    BIT(4)
#define IC_TX_ABRT_GCALL_READ     BIT(5)
#define IC_TX_ABRT_SBYTE_ACKDET   BIT(7)
#define IC_TX_ABRT_SBYTE_NORSTRT  BIT(8)
#define IC_TX_ABRT_10B_RD_NORSTRT BIT(10)
#define IC_TX_ABRT_MASTER_DIS     BIT(11)
#define IC_TX_ABRT_ARB_LOST       BIT(12)
#define IC_TX_ABRT_USR_ABRT       BIT(16)
#define IC_TX_ABRT_SDA_STUCK_LOW  BIT(17)

#define IC_TX_ABRT_NOACK (IC_TX_ABRT_7B_ADDR_NOACK | IC_TX_ABRT_10ADDR1_NOACK | IC_TX_ABRT_10ADDR2_NOACK |  \
                          IC_TX_ABRT_TXDATA_NOACK | IC_TX_ABRT_GCALL_NOACK)

#define IC_DMA_CR_RDMAE BIT(0)
#define IC_DMA_CR_TDMAE BIT(1)

#define IC_DMA_TDLR_CNT 0x3
#define IC_DMA_RDLR_CNT 0x0

#define IC_DATA_CMD_WRITE   BIT(8)
#define IC_DATA_CMD_RECV    BIT(9)
#define IC_DATA_CMD_RESATRT BIT(10)

#define IC_COMP_PARAM_1_SPEED_MODE_HIGH (BIT(2) | BIT(3))
#define IC_COMP_PARAM_1_SPEED_MODE_MASK I2C_GENMASK(3, 2)

#define I2C_KMSG_WE       0x0000 /* Messages need to be sended to the bus */
#define I2C_KMSG_RD       0x0001 /* Messages need to be received from the bus */
#define I2C_KMSG_SMBUS    0x0002 /* The message sent is an SMBus message. */
#define I2C_KMSG_TEN      0x0010 /* The destination addr of the message is a 10-bit addr. */
#define I2C_KMSG_RECV_LEN 0x0400 /* The length of the received message needs to be returned by the slave component. */
#define I2C_KMSG_PEC      0x0800 /* The driver needs to perform PEC error detection. */

#define TIME_MS_50 50

struct i2c_kmsg {
    unsigned short addr;    /* Message destination addr */
    unsigned short flags;   /* Message Properties */
    unsigned long timeout; /* Timeout interval for sending and receiving messages, in jiffies. */
    unsigned int rcnt;
    unsigned int wcnt;
    unsigned int wlen;   /* Message sending length */
    unsigned char *wbuf; /* Message sending cache */
    unsigned int rlen;   /* I2C_KMSG_RECV_LEN indicates the length of the message receiving buffer,
                            Otherwise, it indicates the length of the message to be received. */
    unsigned char *rbuf; /* Message receiving buffer */
};

struct i2c_adapter {
    const unsigned int i2c_id;
    const char *name;

    unsigned long regbase;
    int irqn;

    int init_flag;
    osal_semaphore sem;

    unsigned int control;
    unsigned int hcnt;
    unsigned int lcnt;
    unsigned int tx_fifo_depth;
    unsigned int rx_fifo_depth;

    struct i2c_kmsg *msg;
    unsigned short msg_flags;
    unsigned int cmd_len;
};

struct i2c_depl {
    unsigned int i2c_id;                 /* Driver ID, which is consistent with the bus ID. */
    unsigned int flags;                  /* Initializing Properties */
    unsigned int sda_hold;               /* Number of hold delay cycles of the SDA signal */
    unsigned int scl_high;               /* SCL Number of high-level cycles */
    unsigned int scl_low;                /* SCL Number of low-level cycles */
    unsigned int scl_low_timeout;        /* SCL Number of low-level timeout cycles */
    unsigned int sda_low_timeout;        /* SDL Number of low-level timeout cycles */
    unsigned int clk_strech_timeout;     /* Maximum extension time of the slave clock */
    unsigned long long extension;        /* Reserved extended fields */
};

struct i2c_kdepl {
    unsigned long flags;          /* Initializing Properties */
    unsigned int sda_hold;        /* Number of hold delay cycles of the SDA signal */
    unsigned int sda_hold_ratio;  /* Percentage of hold delay cycles to low-level cycles of the SDA signal */
    unsigned int scl_high;        /* SCL Number of high-level cycles */
    unsigned int scl_low;         /* SCL Number of low-level cycles */
    unsigned int scl_low_timeout; /* SCL Number of low-level cycles */
    unsigned int sda_low_timeout; /* SDL Number of low-level timeout cycles */
};

struct i2c_iomux_info {
    unsigned int flag;
    unsigned int offset;
    unsigned int mask;
    unsigned val;
};

enum {
    I2C_CMD_ERR_TX_ABRT = 0x01,
    I2C_CMD_ERR_SCL_STUCK_LOW = 0x02,
    I2C_CMD_ERR_NO_MSG_PTR = 0x04,
    I2C_CMD_ERR_NO_MSG_WBUF = 0x08,
    I2C_CMD_ERR_NO_MSG_RBUF = 0x10,
    I2C_CMD_ERR_NO_ACTIVITY_INT = 0x20,
    I2C_CMD_ERR_NOT_ENABLE = 0x40,
    I2C_CMD_ERR_DMA = 0x80,
    I2C_CMD_ERR_DMA_UNSUPPORTED = 0x100,
    I2C_CMD_ERR_TX_UNFINISHED = 0x200,
    I2C_CMD_ERR_RX_UNFINISHED1 = 0x400,
    I2C_CMD_ERR_RX_UNFINISHED2 = 0x800,
    I2C_CMD_ERR_CONTROLLER_BUSY = 0x1000,
    I2C_CMD_ERR_RX_UNDER = 0x2000,
    I2C_CMD_ERR_RX_OVER = 0x4000,
    I2C_CMD_ERR_TX_OVER = 0x8000,
    I2C_CMD_ERR_TIMEOUT = 0x10000,
};

enum {
    I2C_DW_MASTER_CONTINUE = 0,
    I2C_DW_MASTER_BREAK,
    I2C_DW_MASTER_NEXT,
};

enum {
    I2C_MSG_ERR_RX_EXCEED = 0x1,
    I2C_MSG_ERR_SMBUS_RECVLEN_INVALID = 0x2,
    I2C_MSG_ERR_NO_RD = 0x4,
};

extern struct i2c_adapter g_i2c_platdrv_adap_list[16];

static inline unsigned long i2c_clk_hz(void)
{
    return SYS_APB_CLOCK_FREQ;
}

static inline unsigned int i2c_dw_common_scl_hcnt(unsigned long long clk_khz, unsigned long long t_high,
    unsigned long long tf, int cond, long long offset)
{
    if (cond) {
        /* hcnt number 500000, hcnt number 1000000, hcnt number 8 */
        return (unsigned int)((((clk_khz * t_high + 500000) / 1000000) - 8) + offset);
    } else {
        /* hcnt number 500000, hcnt number 1000000, hcnt number 3 */
        return (unsigned int)((((clk_khz * (t_high + tf) + 500000) / 1000000) - 3) + offset);
    }
}

static inline unsigned int i2c_dw_common_scl_lcnt(unsigned long long clk_khz, unsigned long long t_low,
    unsigned long long tf, long long offset)
{
    /* lcnt number 500000, lcnt number 1000000, lcnt number 1 */
    return (unsigned int)((((clk_khz * (t_low + tf) + 500000) / 1000000) - 1) + offset);
}

static inline unsigned int i2c_readl(int i2c_id, unsigned int off)
{
    return bm_readl(g_i2c_platdrv_adap_list[i2c_id].regbase + off);
}

static inline void i2c_writel(int i2c_id, unsigned int val, unsigned int off)
{
    bm_writel(val, g_i2c_platdrv_adap_list[i2c_id].regbase + off);
}

static inline void i2c_common_disable_int(int i2c_id)
{
    i2c_writel(i2c_id, 0x0, ICR_INTR_MASK);
}

static inline void i2c_common_disable(int i2c_id)
{
    i2c_writel(i2c_id, 0x0, ICR_ENABLE);
}

static inline void i2c_common_enable(int i2c_id)
{
    i2c_writel(i2c_id, IC_ENA_ENABLE, ICR_ENABLE);
}

#endif /* __I2C_H__ */