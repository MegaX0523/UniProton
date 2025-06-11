/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef __LOADER_H__
#define __LOADER_H__

#include "sre_typedef.h"
#include "sre_mem.h"
#include "sre_sem.h"
#include "openamp/open_amp.h"

int bm_disable_mmu(void);
int mcs_init(void);
int mcs_virtual_serial_init(struct rpmsg_endpoint **ept, rpmsg_ept_cb callback);
int virtual_serial_callback(struct rpmsg_endpoint *ept, void *data, size_t len, unsigned int src, void *priv);
int mcs_deinit(void);
U32 OsHwiInit(void);

S32 OsAppMain(void);
void SRE_HardBootInit(void);
U32 SRE_HardDrvInit(void);
U32 SRE_AppInit(void);
U32 SRE_AppInit(void);
U32 SRE_PrintfInit(void);
U32 TaskClkStart(void);

#define SINGLERCOUNT    0

extern char __bss_start[], __bss_end[];
#endif /* __LOADER_H__ */