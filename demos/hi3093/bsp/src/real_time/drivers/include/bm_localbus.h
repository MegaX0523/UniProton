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
#ifndef __BM_LOCALBUS_H__
#define __BM_LOCALBUS_H__

#define LOCALBUS_CS_NUM 4

typedef enum localbus_cs_width {
    LOCALBUS_WIDTH_8,
    LOCALBUS_WIDTH_16,
    LOCALBUS_WIDTH_BUTT,
} localbus_cs_width;

struct localbus_reg_offset {
    unsigned int cs_control;
    unsigned int control;
};

struct localbus_csi {
    unsigned int cs;
    unsigned long start;
    unsigned int size[4]; /* [0]bankwidth8, [1]bw16, [2]bw32, [3]bw64 */
    unsigned int width;
    struct localbus_reg_offset regoff;
#define LOCALBUS_CS_CONTROL regoff.cs_control
#define LOCALBUS_CONTROL regoff.control
};

struct localbus_cs_ops;

struct localbus_ops {
    int (*read)(struct localbus_cs_ops *, long long, unsigned int, unsigned char *);
    int (*write)(struct localbus_cs_ops *, long long, unsigned int, unsigned char *);
    int (*erase)(struct localbus_cs_ops *, long long, unsigned int);
    int (*probe)(struct localbus_cs_ops *);
    void (*destroy)(struct localbus_cs_ops *);
};

struct localbus_cs_ops {
    unsigned int cs;
    struct localbus_csi *csi;
    struct localbus_ops *ops;
};

struct localbus_driver {
    void *base_reg;
    const unsigned int csi_cnt;
    struct localbus_csi *csi;
    unsigned int ops_cnt;
    struct localbus_cs_ops **cs_ops;
};

/**
 * @defgroup bm_localbus bm_localbus
 */

/**
 *
 * @ingroup bm_localbus
 * @brief localbus initialization.
 *
 * @par description:
 * localbus initialization.
 *
 * @param void
 *
 * @return void
 *
 */
void localbus_init(void);

/**
 *
 * @ingroup bm_localbus
 * @brief localbus deinitialization.
 *
 * @par description:
 * localbus deinitialization.
 *
 * @param void
 *
 * @return void
 *
 */
void localbus_deinit(void);

/**
 *
 * @ingroup bm_localbus
 * @brief localbus ops register.
 *
 * @par description:
 * localbus ops register.
 *
 * @param ops [in]: localbus ops
 * @param cs_width [in]: data width
 *
 * @return BM_OK/BM_FAIL
 *
 */

int localbus_ops_register(struct localbus_cs_ops *ops, localbus_cs_width cs_width);

/**
 *
 * @ingroup bm_localbus
 * @brief localbus ops unregister.
 *
 * @par description:
 * localbus ops unregister.
 *
 * @param ops [in]: localbus ops
 *
 * @return BM_OK/BM_FAIL
 *
 */

int localbus_ops_unregister(struct localbus_cs_ops *ops);

/**
 *
 * @ingroup bm_localbus
 * @brief destroy localbus.
 *
 * @par description:
 * destroy initialization.
 *
 * @param void
 *
 * @return void
 *
 */
void localbus_destroy(void);

/**
 *
 * @ingroup bm_localbus
 * @brief localbus read data.
 *
 * @par description:
 * localbus read data.
 *
 * @param cs [in]: Chip select, range(0<=CS<=3)
 * @param from [in]: Source addr
 * @param length [in]: Length
 * @param buffer [out]: Out buffer
 *
 * @return BM_OK/BM_FAIL
 *
 */
int localbus_read(unsigned int cs, long long from, unsigned int length, unsigned char *buffer);

/**
 *
 * @ingroup bm_localbus
 * @brief localbus write data.
 *
 * @par description:
 * localbus write data.
 *
 * @param cs [in]: Chip select, range(0<=CS<=3)
 * @param to [in]: Write addr
 * @param length [in]: Length
 * @param buffer [in]: write buffer
 *
 * @return BM_OK/BM_FAIL
 *
 */
int localbus_write(unsigned int cs, long long to, unsigned int length, unsigned char *buffer);

/**
 *
 * @ingroup bm_localbus
 * @brief localbus erase.
 *
 * @par description:
 * localbus rease data.
 *
 * @param void
 *
 * @param cs [in]: Chip select, range(0<=CS<=3)
 * @param start [in]: Erase start addr
 * @param length [in]: Erase length
 *
 * @return BM_OK/BM_FAIL
 *
 */
int localbus_erase(unsigned int cs, long long start, unsigned int length);

#endif /* __BM_LOCALBUS_H__ */

