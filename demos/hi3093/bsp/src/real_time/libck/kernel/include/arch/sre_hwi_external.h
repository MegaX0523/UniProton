/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Hardware interrupt implementation
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HWI_EXTERNAL_H
#define SRE_HWI_EXTERNAL_H

#include "sre_lib_external.h"
#include "sre_hwi.h"
#include "sre_cpu_external.h"

#define OS_HWI_MIN_SOFT_NUM 124
#define OS_HWI_NUM_MASK 0x1FU
#define OS_HWI_SOFT_BIT 0x1U

#define OS_HWI_INT_MAX_IDX   4
#define OS_HWI_SWINT_MAX_IDX 4
#define OS_HWI_MAX_LEN 8

#define ICU_EXTSWMASK0_OFFSET      0x60
#define ICU_INTMASK0_OFFSET        0x10
#define ICU_INTMASREG_DELTA        0x4
#define ICU_INTPRIOLEVEL_X_OFFSET  0x180

/*
 * 模块间全局变量声明
 */
extern U8 g_hwiNum[OS_HWI_MAX_LEN];

/*
 * 模块间函数声明
 */
extern void OsIcuInit(void);
extern void OsCoreHwiEnable(U32 coreId, HwiHandle hwiNum, U32 hwiPrio);
extern void OsCoreIcuInit(U32 coreId);
extern void OsHwiGICInit(void);
extern void OsHwiHookAttrSet(U32 hwiNum, U32 hwiPrio, uintptr_t hook);
extern U32 OsHwiPriorityGet(HwiHandle hwiNum);
extern void OsHwiPrioritySet(HwiHandle hwiNum, HwiPrior hwiPrio);
extern U32 OsHwiSetTriggerMode(HwiHandle hwiNum, U32 mode);

#if defined(OS_OPTION_HWI_VPRI)
extern U32 OsHwiVpriSet(HwiHandle hwiNum, HwiPrior hwiPrio);
extern void OsHwiVpriClear(HwiHandle hwiNum);
#endif

#if defined(OS_OPTION_POWERGATE)
extern void OsDynPwrGtIcuBitOp(void);
extern void OsDynPwrGtIcuRegSave(void);
extern void OsDynPwrGtIcuRegRecovery(void);
#endif

#endif /* SRE_HWI_EXTERNAL_H */
