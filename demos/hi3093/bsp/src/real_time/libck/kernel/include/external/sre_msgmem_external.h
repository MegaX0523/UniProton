/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 消息模块的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01, LibCK项目组,  华为规范整改
 */
#ifndef SRE_MSGMEM_EXTERNAL_H
#define SRE_MSGMEM_EXTERNAL_H

#include "sre_base.h"
#include "sre_mem.h"

#if defined(OS_OPTION_CORE_RESET)
struct MsgmReclaimData {
    uintptr_t msgHead;
    uintptr_t rsv1;
    uintptr_t msgMagicWord;
    uintptr_t rsv2;
};
extern void OsMcMemReclaim(struct MsgmReclaimData *data);
#endif

/* 模块间内联函数定义 */
#endif /* SRE_MSGMEM_EXTERNAL_H */
