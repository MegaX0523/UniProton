/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 断点模块对外头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组  华为规范整改
 */
#ifndef SRE_BREAKPOINT_EXTERNAL_H
#define SRE_BREAKPOINT_EXTERNAL_H

#include "sre_breakpoint.h"

/*
 * 模块typedef声明
 */
enum TagBpState {
    BP_STATE_FREE = 1,
    BP_STATE_USED = 2,
    BP_STATE_INVAILD
};

struct TagBpCtrl {
    enum OsBpType type;
    enum TagBpState state;
};

extern uintptr_t g_bpRegBase;

/*
 * 模块函数声明
 */
extern void OsBreakPointInit(U32 coreID);
extern U32 OsBreakPointAlloc(enum OsBpType bpType, U32 *bpIdx);
extern void OsBreakPointSet(enum OsBpType bpType, uintptr_t bpAddr, enum OsBpMonSize size, U32 idx);
extern void OsDBreakPointDisable(U32 idx);
extern void OsBreakPointRegister(void);

#if defined(OS_OPTION_POWERGATE)
extern void OsDynPwrGtOcdRegSave(void);
extern void OsDynPwrGtOcdRegRecovery(void);
#endif

/*
 * 模块内联函数定义
 */
#endif /* SRE_BREAKPOINT_EXTERNAL_H */
