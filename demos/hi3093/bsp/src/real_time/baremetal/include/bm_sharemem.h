/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
#include "hi309x_memmap.h"
#ifndef __BM_SHAREMEM_H__
#define __BM_SHAREMEM_H__

/* 消息通信共享内存 */
#define SHARE_MSG_BASE_ADDR HI_SHARED_BASE_ADDR
#define SHARE_MSG_LEN       0x5000 // 20K 用于消息通信的共享内存
#define SHARE_MSG_END_ADDR (SHARE_MSG_BASE_ADDR + SHARE_MSG_LEN)

#endif /* __BM_SHAREMEM_H__ */