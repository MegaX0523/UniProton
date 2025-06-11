/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: soc相关的头文件
 * Author: LibCK项目组
 * Create: 2009-10-5
 * History: 2018-10-5 LibCK项目组  华为规范整改
 */
#ifndef SRE_BOARD_EXTERNAL_H
#define SRE_BOARD_EXTERNAL_H

#include "./hw/board/os_board.h"
#include "sre_lib_external.h"
/* board type */
#define OS_PLATFORM_VC      0x0  // 85VE板
#define OS_PLATFORM_VD      0x1
#define OS_PLATFORM_VD2     0x2
#define OS_PLATFORM_SIMICS  0x3

extern U32 g_platformID;
OS_SEC_ALW_INLINE INLINE U32 OsGetPlatformID(void)
{
    return g_platformID;
}
#define OS_CHK_IS_SIMICS()   (OsGetPlatformID() == OS_PLATFORM_SIMICS)
#define OS_CHK_IS_BOARD_VC() (OsGetPlatformID() == OS_PLATFORM_VC)

#ifndef OS_CORELIST_INVALID
#define OS_CORELIST_INVALID(coreList) \
    (((coreList) == NULL) || (((coreList)->coreMask[(OS_MAX_CORE_NUM / 32)] >> (OS_MAX_CORE_NUM & 0x1fU)) != 0))
#endif

/* 检查核号是否无效, 有些芯片核号不连续 */
#ifndef OS_COREID_CHK_INVALID
#define OS_COREID_CHK_INVALID(coreId, maxNumOfCores) ((coreId) >= (maxNumOfCores))
#endif

/* 获取真实的核号, 有些芯片核号不连续 */
#ifndef OS_COREID_GET_BY_LOGIC
#define OS_COREID_GET_BY_LOGIC(coreId) (coreId)
#endif

#define OS_COMM_MAX_VALID_VMID  7

#define OS_VEC_OP_SIZE_16 16U
#define OS_VEC_OP_SIZE_64 64U
#define OS_VEC_OP_SIZE_4K 4096U

extern void OsVmodeEnable(U32 coreId);
extern U32 OsVmodeGet(U32 coreId);

extern void OsWakeUpIntEnable(U32 coreId, U32 wakeupIntPri);
extern bool OsElfMachineIsValid(U32 machine, struct SysSreCoreMask *coreMask);

#if defined(OS_OPTION_POWERGATE)
#define OS_WAKEUP_PRI 7
#else
#define OS_WAKEUP_PRI 1
#endif

#endif /* SRE_BOARD_EXTERNAL_H */
