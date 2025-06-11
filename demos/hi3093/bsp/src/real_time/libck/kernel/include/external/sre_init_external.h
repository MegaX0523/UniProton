/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: INIT模块的内部头文件
 * Author: LibCK项目组
 * Create: 2022-01-17
 */
#ifndef SRE_INIT_EXTERNAL_H
#define SRE_INIT_EXTERNAL_H

#include "sre_init.h"
#include "sre_preinit.h"

typedef U32 (*OsInitFunc)(void);
extern OsInitFunc __init_os_array_start;
extern OsInitFunc __init_os_array_end;
extern OsInitFunc __init_user_array_start;
extern OsInitFunc __init_user_array_end;

extern const struct OsInitRegPara *g_coreRegPara;
extern void OsConfigAddrSizeGet(U32 memType, uintptr_t addr, uintptr_t size, uintptr_t *destAddr, uintptr_t *destSize);
extern U32 OsDependModeOpen(U32 mode);
extern bool OsConfigIsSet(U32 config);

/* LibCK系统启动相关函数的声明 */
extern U32 OsActivate(void);
extern U32 OsTickStart(void);
extern U32 OsSwiActivate(void);

#endif /* SRE_INIT_EXTERNAL_H */
