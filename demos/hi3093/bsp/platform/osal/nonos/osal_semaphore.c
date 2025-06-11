/*
 * Copyright (c) CompanyNameMagicTag 2012-2021. All rights reserved.
 */

#include "soc_osal.h"
#include "osal_inner.h"

int osal_sem_init(osal_semaphore *sem, int val)
{
    osal_unused(sem, val);
    return 0;
}

void osal_sem_destroy(osal_semaphore *sem)
{
    osal_unused(sem);
    return;
}

int osal_sem_down(osal_semaphore *sem)
{
    (void)(sem);
    return 0;
}

int osal_sem_down_timeout(osal_semaphore *sem, unsigned int timeout)
{
    (void)(sem);
    (void)(timeout);
    return 0;
}

int osal_sem_trydown(osal_semaphore *sem)
{
    (void)(sem);
    return 0;
}

void osal_sem_up(osal_semaphore *sem)
{
    (void)(sem);
}