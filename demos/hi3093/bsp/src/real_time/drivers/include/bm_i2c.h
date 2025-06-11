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
#ifndef __BM_I2C_H__
#define __BM_I2C_H__

/**
* @defgroup bm_i2c bm_i2c
*/

#define I2C_BASE_IRQ 104

typedef enum {
    I2C_ID_0 = 0,
    I2C_ID_1,
    I2C_ID_2,
    I2C_ID_3,
    I2C_ID_4,
    I2C_ID_5,
    I2C_ID_6,
    I2C_ID_7,
    I2C_ID_8,
    I2C_ID_9,
    I2C_ID_10,
    I2C_ID_11,
    I2C_ID_12,
    I2C_ID_13,
    I2C_ID_14,
    I2C_ID_15,
    I2C_ID_BUTT,
} bm_i2c_ids;

typedef enum {
    I2C_SPEED_100K  = 100,
    I2C_SPEED_400K  = 400,
    I2C_SPEED_3400K = 3400,
} bm_i2c_speed;

typedef enum {
    I2C_WRITE_OK = 0,
    I2C_WRITE_FAIL,
    I2C_READ_OK,
    I2C_READ_FAIL,
    I2C_TRANSMIT_BUTT,
} bm_i2c_transmit_async_t;

struct i2c_init_info {
    bm_i2c_ids i2c_id;               /* ID of the I2C controller. The value ranges from 0 to 15. */
    bm_i2c_speed speed;              /* Setting the I2C transfer rate(100K/400K) I2C_CLK_100K/I2C_CLK_400K */
};

struct i2c_xfer_info {
    bm_i2c_ids i2c_id;             /* ID of the I2C controller. The value ranges from 0 to 15. */
    unsigned short addr;           /* Message destination addr */
    unsigned char *buffer;         /* Write buff or Read buff */
    unsigned int length;           /* Length of the data */
    unsigned int timeout;          /* Timeout interval, in milliseconds. */
};

typedef void (*bm_i2c_callback)(bm_i2c_ids, bm_i2c_transmit_async_t);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C pin multiplexing configuration.
 *
 * @par description:
 * I2C pin multiplexing configuration.
 *
 * @param i2c_id [in]: bm_i2c_ids
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_config_io_reuse(bm_i2c_ids i2c_id);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C turn on the gating clock.
 *
 * @par description:
 * I2C turn on the gating clock.
 *
 * @param i2c_id [in]: bm_i2c_ids
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_config_io_clock(bm_i2c_ids i2c_id);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C initialization.
 *
 * @par description:
 * I2C initialization.
 *
 * @param init [in]: i2c_init_info
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_init(const struct i2c_init_info *init);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C deinit.
 *
 * @par description:
 * I2C deinit.
 *
 * @param i2c_id [in]: bm_i2c_ids
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_deinit(bm_i2c_ids i2c_id);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C write.
 *
 * @par description:
 * I2C write.
 *
 * @param write_info [in]: i2c_xfer_info
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_write(const struct i2c_xfer_info *write_info);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C read.
 *
 * @par description:
 * I2C read.
 *
 * @param read_info [in]: i2c_xfer_info
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_read(const struct i2c_xfer_info *read_info);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C write async.
 *
 * @par description:
 * I2C write.
 *
 * @param write_info [in]: i2c_xfer_info
 * @param callback [in]: bm_i2c_callback
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_write_async(const struct i2c_xfer_info *write_info, bm_i2c_callback callback);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C read async.
 *
 * @par description:
 * I2C read.
 *
 * @param read_info [in]: i2c_xfer_info
 * @param callback [in]: bm_i2c_callback
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_read_async(const struct i2c_xfer_info *read_info, bm_i2c_callback callback);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C write dma.
 *
 * @par description:
 * I2C write.
 *
 * @param write_info [in]: i2c_xfer_info
 * @param callback [in]: callback
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_write_dma(const struct i2c_xfer_info *write_info, bm_i2c_callback callback);

/**
 *
 * @ingroup bm_i2c
 * @brief I2C read dma.
 *
 * @par description:
 * I2C read.
 *
 * @param read_info [in]: i2c_xfer_info
 * @param callback [in]: bm_i2c_callback
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_i2c_read_dma(const struct i2c_xfer_info *read_info, bm_i2c_callback callback);
#endif /* __BM_I2C_H__ */