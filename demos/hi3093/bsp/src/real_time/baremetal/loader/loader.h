/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
#ifndef __LOADER_H__
#define __LOADER_H__

int main(void);
int bm_disable_mmu(void);
void bm_msg_init(void);
void bm_loader(void);
int mcs_init(void);
extern char __bss_start[], __bss_end[];

#endif /* __LOADER_H__ */