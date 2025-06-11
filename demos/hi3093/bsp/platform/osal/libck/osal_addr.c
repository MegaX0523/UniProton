/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: mem
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#include "sre_mem.h"
#include "soc_osal.h"
#include "osal_inner.h"

void *osal_kmalloc(unsigned long size, unsigned int osal_gfp_flag)
{
    osal_unused(osal_gfp_flag);
    return SRE_MemAlloc(OS_MID_OSAL, OS_MEM_DEFAULT_FSC_PT, size);
}

void osal_kfree(void *addr)
{
    (void)SRE_MemFree(OS_MID_OSAL, (void *)addr);
}