/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: idle线程实现
 * Author: LibCK项目组
 * Create: 2020-10-30
 * History: 2020-10-30 LibCK项目组  新建此文件
 */
#ifndef SRE_IDLE_EXTERNAL_H
#define SRE_IDLE_EXTERNAL_H

#include "sre_sys_external.h"
#include "sre_base.h"

extern OsVoidFunc g_idleSwiSwitchToTask;
extern void OsIdleTaskExe(void);
extern void OsIdleThread(void);
extern void OsIdleSetCoreSleepHook(OsVoidFunc handle);

#endif /* SRE_IDLE_EXTERNAL_H */
