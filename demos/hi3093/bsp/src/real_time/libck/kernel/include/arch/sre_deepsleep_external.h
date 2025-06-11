/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 内部头文件。 \n
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_DEEPSLEEP_EXTERNAL_H
#define SRE_DEEPSLEEP_EXTERNAL_H

#include "sre_base.h"

#if defined(OS_OPTION_DEEPSLEEP)
extern struct SysSreCoreMask g_deepSleepModeMap; /* Core deepsleep  mode : 0, normal;  1: deep sleep */
#define OS_CORE_DEEPSLEEP_MODE(coreID) OS_SYS_CORE_MASK_BIT_VALID(&g_deepSleepModeMap, (coreID))
#else
#define OS_CORE_DEEPSLEEP_MODE(coreID) FALSE
#endif

#endif /* SRE_DEEPSLEEP_EXTERNAL_H */
