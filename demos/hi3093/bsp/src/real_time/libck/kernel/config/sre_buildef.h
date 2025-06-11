/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2018. All rights reserved.
 * Description: sre_buildef.h组件功能宏所在文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组  华为规范整改
 *
 */
#ifndef SRE_BUILDEF_H
#define SRE_BUILDEF_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define OS_ARCH_ARMV8                       YES

#define OS_OPTION_CHIP_HI3093               YES

#define OS_GIC_VER                          3

#ifndef OS_HARDWARE_PLATFORM
#define OS_HARDWARE_PLATFORM                OS_ARMV8
#endif

#ifndef OS_CPU_TYPE
#define OS_CPU_TYPE                         OS_HI3093
#endif

#define OS_MAX_CORE_NUM                     4

#define OS_THIS_CORE                        3

#ifndef OS_BYTE_ORDER
#define OS_BYTE_ORDER                       OS_LITTLE_ENDIAN
#endif

#define OS_CACHE_LINE_SIZE                  64

#define OS_OPTION_EVENT                     YES

#define OS_OPTION_QUEUE                     YES

#define OS_OPTION_HWI_PRIORITY              YES

#define OS_OPTION_HWI_ATTRIBUTE             YES

#define OS_OPTION_SYS_TIME_USR              YES

#define OS_OPTION_TASK                      YES

#define OS_OPTION_CPUP                      YES

#define OS_OPTION_TASK_DELETE               YES

#define OS_OPTION_TASK_SUSPEND              YES

#define OS_OPTION_TASK_INFO                 YES

#define OS_OPTION_TASK_YIELD                YES

#define OS_TSK_PRIORITY_HIGHEST             0

#define OS_TSK_PRIORITY_LOWEST              63

#define OS_TSK_NUM_OF_PRIORITIES            64

#define OS_TSK_CORE_BYTES_IN_PID            2

#define OS_OPTION_TICK                      YES

#define OS_OPTION_CPUP_WARN                 YES

#define OS_OPTION_SHELL                     YES

#define OS_OPTION_SYMBOL                    YES

#define OS_OPTION_POSIX                     YES

#define OS_POSIX_TYPE_NEWLIB                YES

#define OS_POSIX_SET_TZDST                  YES

#define OS_OPTION_BIN_SEM                   YES

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

/* common macros's definitions */
#include "./common/os_buildef_common.h"

#endif
