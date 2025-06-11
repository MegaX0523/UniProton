/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: syscall模块的内部公共头文件。
 * Author: LibCK项目组
 * Create: 2020-06-16
 * History: 2020-06-16 LibCK项目组 创建文件
 */
#ifndef SRE_SYSCALL_EXTERNAL_H
#define SRE_SYSCALL_EXTERNAL_H

#include "sre_syscall.h"

/* 本文件内容与HOST的sre_syscall_external.h存在重复，建议后续提取到public文件夹 */
/* OS内部使用的syscall个数 */
#define OS_INN_SYSCALL_NUM   16

/* 注册HOST异常时用来通知其他核的中断号 */
#define OS_SYSCALL_EXC_HWI   0

extern U32 OsSyscall(U32 syscallNo, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t arg4);

#endif /* SRE_SYSCALL_EXTERNAL_H */
