/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Hardware timer implementation
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HWTMR_EXTERNAL_H
#define SRE_HWTMR_EXTERNAL_H

#include "sre_exc.h"
#include "sre_timer.h"
#include "sre_cpu_external.h"

/*
 * 定时器中断模式枚举类型
 */
enum TagHidspTmrMode {
    TMR_REG_PERIODIC_MODE = 0x2,  // Timer的控制寄存器Bit1表示定时器工作模式，0表示单次模式，1表示周期模式。
    TMR_REG_ONE_SHOT_MODE = 0x0
};

/*
 * 定时器使能枚举类型
 */
enum TagHidspTmrEnable {
    TMR_REG_ENABLE = 0x1,  // Timer的控制寄存器Bit0表示定时器工作模式，0表示去使能，1表示使能。
    TMR_REG_DISABLE = 0x0
};

/*
 * 定时器中断使能枚举类型
 */
enum TagHidspTmrIntEnable {
    TMR_REG_INT_ENABLE =
        0x4,  // Timer的控制寄存器Bit2表示定时器工作模式，0表示计数值减到0时不产生中断，1表示计数值减到0时会产生中断。
    TMR_REG_INT_IDSABLE = 0x0
};

struct TagHwTmrCB {
    /* 硬件定时器状态 :运行 停止 未创建 超时处理 */
    U8 status;
    /* 硬件定时器模式 :单次 周期 */
    U8 mode;
    /* 硬件定时器优先级 */
    U16 priority;
    /* 硬件定时器对应中断号 */
    U32 hwiNum;
    /* 硬件定时器对应物理编号 */
    U32 hwTmrNum;
    /* 硬件定时器触发时长的低32位 */
    U32 intervalLow;
    /* 硬件定时器触发时长的高32位 */
    U32 intervalHigh;
    /* 硬件定时器触发剩余时长的低32位 */
    U32 remainIntervalLow;
    /* 硬件定时器触发剩余时长的高32位 */
    U32 remainIntervalHigh;
    /* 硬件定时逻辑ID */
    U32 hwtmrID;
    /* 硬件定时处理函数参数1 */
    U32 arg1;
    /* 硬件定时处理函数参数2 */
    U32 arg2;
    /* 硬件定时处理函数参数3 */
    U32 arg3;
    /* 硬件定时处理函数参数4 */
    U32 arg4;
    /* 硬件定时处理函数钩子 */
    SreTmrProcFunc handler;
    /* 硬件定时处理次数 */
    U32 tickOutCount;
    /* 硬件定时器触发时间点 */
    U32 compareValue;
};

#define TMR_REG_INT_CLR             0x1

extern struct TagHwTmrCB g_hwTmrCB[OS_HWTMR_MAX_NUM];

extern void OsHwtmrRegInit(void);
extern void OsHwTmrDispatch(U32 hwTmrNum);
extern void OsHwTmrCreateCtrlReg(struct TagHwTmrCB *hwTmrCB, U16 hwTimerNum);
extern void OsHwTmrDeleteCtrlReg(struct TagHwTmrCB *hwTmrCB);
extern void OsHwTmrStartCtrlReg(struct TagHwTmrCB *hwTmrCB);
extern void OsHwTmrReStartCtrlReg(struct TagHwTmrCB *hwTmrCB);
extern void OsHwTmrStopCtrlReg(struct TagHwTmrCB *hwTmrCB);
extern U32 OsHwTmrSetIntervalCtrlReg(struct TagHwTmrCB *hwTmrCB, U64 intervalTime);
extern U64 OsHwTmrGetRunningRemainCycle(struct TagHwTmrCB *hwTmrCB);

extern void OsHwTmrStartCcompare(struct TagHwTmrCB *hwTmrCB);
extern void OsHwTmrReStartCcompare(struct TagHwTmrCB *hwTmrCB);
extern void OsHwTmrStopCcompare(struct TagHwTmrCB *hwTmrCB);
extern U32 OsHwTmrSetIntervalByCcompare(struct TagHwTmrCB *hwTmrCB, U64 intervalTime);
extern void OsHwtrmDispatchExtend(struct TagHwTmrCB *hwTmrCB);

#if defined(OS_OPTION_POWERGATE)
extern void OsDynPwrGtTimerRegSave(void);
extern void OsDynPwrGtTimerRegRecovery(void);
#endif
/*
 * 模块间内联函数定义
 */
#endif /* SRE_HWTMR_EXTERNAL_H */
