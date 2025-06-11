/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Unwind模块的内部公共头文件
 * Author: LibCK项目组
 * Create: 2021-04-01
 * History: 2021-04-01 LibCK项目组 迭代开发
 */
#ifndef SRE_UNWIND_EXTERNAL_H
#define SRE_UNWIND_EXTERNAL_H

#include "sre_task_external.h"

extern void OsUwdGetCallStack(const struct TagTskCB *task, U32 *maxDepth, uintptr_t *list);

#endif /* SRE_UNWIND_EXTERNAL_H */
