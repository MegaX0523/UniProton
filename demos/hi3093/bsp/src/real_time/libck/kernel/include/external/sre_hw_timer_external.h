/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Hardware timer implementation
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_HW_TIMER_EXTERNAL_H
#define SRE_HW_TIMER_EXTERNAL_H

#include "sre_base.h"
#include "sre_cpu_external.h"

/*
 * 模块间宏定义
 */
#define OS_HWTMR_MAX_CYCLE 0xFFFFFFFF

#define OS_HW_TIMER_FREE 0
#define OS_HW_TIMER_USED 1

/*
 * 模块间数据结构定义
 */
struct TagHwTmrPHYCB {
    /* 定时器状态，USED or FREE */
    U8 status;
    /* 定时器优先级 */
    U8 priority;
    /* 定时器对应的中断号，硬件固定 */
    U16 hwiNum;
};
/*
 * 模块间全局变量声明
 */
extern struct TagHwTmrPHYCB g_hwTimerPhy[OS_HWTMR_MAX_NUM];

/*
 * 模块间函数声明
 */
extern U32 OsHwTimerPhyAlloc(U16 *hwTimerNum);
extern void OsHwTimerPhyFree(U16 hwTimerNum);
extern U16 OsHwTimerPhyHwiIdGet(U16 phyTimerID);

/*
 * 模块间内联函数定义
 */
#endif /* SRE_HW_TIMER_EXTERNAL_H */
