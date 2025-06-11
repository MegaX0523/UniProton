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

#ifndef __I2C_CONFIG_H__
#define __I2C_CONFIG_H__

#define I2C_KDEPLOY_MASTER          BIT(1) /* The bus is initialized to Master. */
#define I2C_KDEPLOY_SM_MASK         (I2C_KDEPLOY_SLAVE | I2C_KDEPLOY_MASTER)
#define I2C_KDEPLOY_SPEED_MODE_STD  BIT(2) /* The bus is initialized to the standard rate mode (100 kbit/s). */
#define I2C_KDEPLOY_SPEED_MODE_FAST BIT(3) /* Bus initialization to fast rate mode (400k) */
#define I2C_KDEPLOY_SPEED_MODE_HIGH BIT(4) /* The bus is initialized to the high-speed mode (3400k). */
#define I2C_KDEPLOY_SPEED_MODE_MASK (I2C_KDEPLOY_SPEED_MODE_STD |  \
                                     I2C_KDEPLOY_SPEED_MODE_FAST | I2C_KDEPLOY_SPEED_MODE_HIGH)
#define I2C_KDEPLOY_FORCE           BIT(9) /* Forcibly initialize the bus,
                                            regardless of whether it has been initialized before. */

#define SYS_REF_CLOCK_FREQ	(25000000)
#define SYS_APB_CLOCK_FREQ	(100000000)
#define SYS_AHB_CLOCK_FREQ	(200000000)
#define I2C_HZ_TO_KHZ 1000

#define I2C_MAX_WRITE_LEN 2048
#define I2C_MAX_READ_LEN  2048

#define I2C_LPSC_CLKEN_CTRL0_OFFSET      10

#define I2C_INIT_TIMEOUT   10000
#define I2C_USEC_PER_MSEC  1000
#define I2C_USEC_PER_SEC   1000000

#define I2C_7BIT_MAX_ADDR 0x7F
#define I2C_7BIT_ADDR_MASK I2C_7BIT_MAX_ADDR
#define I2C_10BIT_MAX_ADDR 0x3FF
#define I2C_10BIT_ADDR_MASK I2C_10BIT_MAX_ADDR

#define I2C_IO_MUX_MASK 0x7

#define I2C_SS_SCL_HCNT_CYCLE 0x188
#define I2C_SS_SCL_LCNT_CYCLE 0x1D5
#define I2C_FS_SCL_HCNT_CYCLE 0x34
#define I2C_FS_SCL_LCNT_CYCLE 0x81

#define I2C_XFER_TIMEOUT (-10)

#endif /* __I2C_CONFIG_H__ */