/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: interrupt
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#include "sre_buildef.h"
#include "sre_hwi.h"
#include "sre_typedef.h"
#include "sre_sys_external.h"
#include "sre_irq_external.h"
#include "soc_osal.h"
#include "osal_errno.h"
#include "osal_inner.h"

struct osal_irq_info {
    osal_irq_handler handler;
    void *dev;
    unsigned int irq;
};

static struct osal_irq_info g_osal_irq_array[OS_HWI_MAX_NUM + 1];

static void osal_callback(void* args)
{
    if (args == NULL) {
        return;
    }
    struct osal_irq_info* handle = (struct osal_irq_info*)args;
    if (handle->handler != NULL) {
        handle->handler(handle->irq, handle->dev);
    }
}

int osal_irq_request(unsigned int irq, osal_irq_handler handler, osal_irq_handler thread_fn,
    const char *name, void *dev)
{
    osal_unused(name);
    osal_unused(thread_fn);

    if (handler == NULL) {
        osal_log("handler is NULL !\n");
        return OSAL_FAILURE;
    }

    if (irq >= OS_HWI_MAX_NUM) {
        return OSAL_FAILURE;
    }

    g_osal_irq_array[irq].irq = irq;
    g_osal_irq_array[irq].dev = dev;
    g_osal_irq_array[irq].handler = handler;
    g_hwiModeForm[irq].mode = OS_HWI_MODE_ENGROSS;

    unsigned int ret = SRE_HwiCreate(irq, (HwiProcFunc)osal_callback, (HwiArg)&g_osal_irq_array[irq]);
    if (ret != SRE_OK) {
        osal_log("SRE_HwiCreate failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }

    return OSAL_SUCCESS;
}

void osal_irq_free(unsigned int irq, void *dev)
{
    osal_unused(dev);

    if (irq > OS_HWI_MAX_NUM) {
        return;
    }

    g_osal_irq_array[irq].irq = 0;
    g_osal_irq_array[irq].dev = NULL;
    g_osal_irq_array[irq].handler = NULL;

    unsigned int ret = SRE_HwiDelete(irq);
    if (ret != SRE_OK) {
        osal_log("SRE_HwiDelete failed! ret = %#x.\n", ret);
    }
}

int osal_in_interrupt(void)
{
    return (OS_INT_ACTIVE);
}

void osal_irq_enable(unsigned int irq)
{
    unsigned long long ret = SRE_HwiEnable(irq);
    if (ret != SRE_OK) {
        osal_log("SRE_HwiEnable failed!\n");
    }
}

void osal_irq_disable(unsigned int irq)
{
    unsigned long long ret = SRE_HwiDisable(irq);
    if (ret != SRE_OK) {
        osal_log("SRE_HwiDisable failed!\n");
    }
}

unsigned int osal_irq_lock(void)
{
    return SRE_IntLock();
}

unsigned int osal_irq_unlock(void)
{
    return SRE_IntUnLock();
}

void osal_irq_restore(unsigned int irq_status)
{
    SRE_IntRestore(irq_status);
}

int osal_irq_set_priority(unsigned int irq, unsigned short priority)
{
    unsigned int ret;
    ret = SRE_HwiSetAttr(irq, priority, OS_HWI_MODE_ENGROSS);
    return ret == SRE_OK ? OSAL_SUCCESS : OSAL_FAILURE;
}

#if defined (OS_HARDWARE_PLATFORM) && defined (OS_ARMV8) && (OS_HARDWARE_PLATFORM == OS_ARMV8)
int osal_irq_set_affinity(unsigned int irq, const char *name, int cpu_mask)
{
    (void)name;
    unsigned int ret;

    ret = OsHwiAffinitySet(irq, cpu_mask);
    return ret == SRE_OK ? OSAL_SUCCESS : OSAL_FAILURE;
}
#endif