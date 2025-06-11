/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#ifndef __BM_CONFIG_H__
#define __BM_CONFIG_H__

#include "bm_types.h"

#define IPI_INT_NO 12
#define BM_MAX_CORE_NUM 4
#define MAX_INT_NUM 288

#define EPT_NUM 5
#define MCS_CORE_ID 2
#define CONFIG_SHUTDOWN 1
#define BM_SHUTDOWN_CMD "stop"

#define ENDPOINT_SHUTDOWN "shutdown"
#define ENDPOINT_DATA "console"

void __attribute__((weak)) mcs_peripherals_shutdown(void);
int get_shutdown_flag(void);

#endif /* __BM_CONFIG_H__ */