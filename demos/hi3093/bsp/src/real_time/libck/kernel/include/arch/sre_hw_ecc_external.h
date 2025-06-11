/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: ECC模块内部头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HW_ECC_EXTERNAL_H
#define SRE_HW_ECC_EXTERNAL_H

#include "sre_hw_ecc.h"
#include "sre_exc.h"

#if ((defined OS_OPTION_SREMNG) && (defined OS_OPTION_LP_PRELINK))
#include "sre_mng_mem_external.h"

/* IIU_BEGIN+8: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
#define ECC_MCCOM_ADDR_TYPE_POS 1
#define ECC_MCCOM_LIB_NAME_POS 2
#define ECC_MCCOM_LIB_ID_POS 3

#define ECC_MCCOM_HW_THREAD_POS 1
#define ECC_MCCOM_MEM_TYPE_POS 2

typedef bool(*EccPl2PaFinalizeFunc)(U32 instId, U32 phyId, U32 logicId, uintptr_t va, uintptr_t size, uintptr_t *pa);
typedef U32(*EccOwnerGet)(U32 phyEccAddr, U32 phyCoreId, U32 scanType, U32 *id, U32 *typeOut, char **libName,
                          U32 *libId);
typedef U32(*EccPrivMemQuery)(U32 instId, U32 phyCoreId, uintptr_t phyEccAddr, U32 *hwThreadId, U32 *memType);

extern EccPl2PaFinalizeFunc g_eccPl2PaFinalize;

extern EccOwnerGet g_eccMcpuOwnerGet;
extern EccPrivMemQuery g_eccMcpuPrivMemQuery;

extern U32 OsEccOwnerScanGet(struct TagMngInstInfo *instInfo, U32 phyEccAddr, U32 phyCoreId, U32 scanType,
                             U32 *instId, U32 *typeOut, char **libName, U32 *libId);
extern U32 OsEccOwnerGet(struct TagMngInstInfo *instInfo, U32 phyEccAddr, U32 phyCoreId,
                         U32 *instId, U32 *typeOut, char **libName, U32 *libId);

extern U32 OsEccPrivMemQuery(struct TagMngInstInfo *instInfo, U32 phyCoreId, uintptr_t phyEccAddr,
                             U32 *hwThreadId, U32 *memType);
#endif

extern bool OsExcEccCorrect(struct ExcInfo *excInfo);
extern void OsEccEnable(void);

#endif /* SRE_HW_ECC_EXTERNAL_H */
