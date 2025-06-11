/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: semaphore
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#include "sre_mem.h"
#include "sre_sem.h"
#include "sre_sys_external.h"
#include "sre_sem_external.h"
#include "sre_typedef.h"
#include "soc_osal.h"
#include "osal_errno.h"
#include "osal_inner.h"
#include "sre_config.h"

int osal_sem_init(osal_semaphore *sem, int val)
{
    if (sem == NULL || val < 0) {
        osal_log("val=%d parameter invalid!\n", val);
        return OSAL_FAILURE;
    }

    unsigned int ret = SRE_SemCreate(val, (SemHandle *)&(sem->sem)); /* semid may be zero */
    if (ret != SRE_OK) {
        osal_log("SRE_SemCreate failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

void osal_sem_destroy(osal_semaphore *sem)
{
    if (sem == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    unsigned int ret = SRE_SemDelete((unsigned int)(uintptr_t)sem->sem);
    if (ret != SRE_OK) {
        osal_log("SRE_SemDelete failed! ret = %#x.\n", ret);
    }
    sem->sem = (void *)(uintptr_t)OsSemGetMaxSemNum();
}

int osal_sem_down(osal_semaphore *sem)
{
    if (sem == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    unsigned int ret = SRE_SemPend((unsigned int)(uintptr_t)sem->sem, OS_WAIT_FOREVER);
    if (ret == OS_ERRNO_SEM_TIMEOUT) {
        osal_log("OS_ERRNO_SEM_TIMEOUT!\n");
        return OSAL_FAILURE;
    } else if (ret != SRE_OK) {
        osal_log("SRE_SemPend failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    } else {
        return OSAL_SUCCESS;
    }
}

int osal_sem_down_timeout(osal_semaphore *sem, unsigned int timeout)
{
    if (sem == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    unsigned int ticks = (timeout == OS_WAIT_FOREVER) ? timeout :
     (U32)DIV64(((U64)g_tickModInfo.tickPerSecond * timeout), SRE_SYS_MS_PER_SECOND);
    unsigned int ret = SRE_SemPend((unsigned int)(uintptr_t)sem->sem, ticks);
    if (ret == OS_ERRNO_SEM_TIMEOUT) {
        osal_log("OS_ERRNO_SEM_TIMEOUT!\n");
        return OSAL_ETIME;
    } else if (ret != OSAL_SUCCESS) {
        osal_log("SRE_SemPend failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    } else {
        return OSAL_SUCCESS;
    }
}

int osal_sem_trydown(osal_semaphore *sem)
{
    if (sem == NULL) {
        osal_log("parameter invalid!\n");
        return 1;
    }

    unsigned int ret = SRE_SemPend((unsigned int)(uintptr_t)sem->sem, OS_NO_WAIT);
    if (ret != SRE_OK) {
        osal_log("SRE_SemPend failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    } else {
        return OSAL_SUCCESS;
    }
}

void osal_sem_up(osal_semaphore *sem)
{
    if (sem == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }

    unsigned int ret = SRE_SemPost((unsigned int)(uintptr_t)sem->sem);
    if (ret != SRE_OK) {
        osal_log("SRE_SemPost failed! ret = %#x.\n", ret);
    }
}
