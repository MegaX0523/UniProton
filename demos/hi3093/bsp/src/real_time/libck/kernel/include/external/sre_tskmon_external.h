/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: tskmon模块的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */

#ifndef SRE_TSKMON_EXTERNAL_H
#define SRE_TSKMON_EXTERNAL_H

#include "sre_typedef.h"

/*
 * 模块间宏定义
 */
/* ********任务检测状态标记******** */
#define OS_TSKMON_FLAG_NORMAL 0x00U  // 标记正常状态
#define OS_TSKMON_FLAG_OVERRUN 0x01U  // 标记撑死状态
#define OS_TSKMON_FLAG_STRVATION 0x02U  // 标记饿死状态
#define OS_TSKMON_FLAG_NONE_CK 0x00U  // 不进行任何状态检测
#define OS_TSKMON_FLAG_OVERRUN_CK 0x01U  // 撑死状态检测中
#define OS_TSKMON_FLAG_STRVATION_CK 0x02U  // 饿死状态检测中
#define OS_TSKMON_CK_NONE 0x00U  // 无检测标记
#define OS_TSKMON_CK_OVERRUN 0x01U  // 注册撑死标记
#define OS_TSKMON_CK_STRVATION 0x02U  // 注册饿死标记

extern void OsTskMonCancel(U32 taskPid);

/*
 * 模块间全局变量声明
 */
#endif /* SRE_TSKMON_EXTERNAL_H */
