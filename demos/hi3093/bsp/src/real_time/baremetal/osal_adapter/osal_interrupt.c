/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2023. All rights reserved.
 */
#include "soc_osal.h"
#include "bm_hwi.h"
#include "bm_config.h"

struct osal_irq_info {
    osal_irq_handler handler;
    void *dev;
    unsigned int irq;
};

static struct osal_irq_info g_osal_irq_array[MAX_INT_NUM + 1];

static void osal_callback(void* args)
{
    if (args == NULL) {
        return;
    }
    struct osal_irq_info *handle = (struct osal_irq_info *)args;
    if (handle->handler != NULL) {
        handle->handler(handle->irq, handle->dev);
    }
}

int osal_irq_request(unsigned int irq, osal_irq_handler handler, osal_irq_handler thread_fn,
    const char *name, void *dev)
{
    (void)(name);
    (void)(thread_fn);

    if (handler == NULL) {
        return OSAL_FAILURE;
    }

    if (irq > MAX_INT_NUM) {
        return OSAL_FAILURE;
    }

    g_osal_irq_array[irq].irq = irq;
    g_osal_irq_array[irq].dev = dev;
    g_osal_irq_array[irq].handler = handler;

    int ret = bm_request_irq(irq, osal_callback, &g_osal_irq_array[irq]);
    return (ret == BM_FAIL) ? OSAL_FAILURE : OSAL_SUCCESS;
}

void osal_irq_free(unsigned int irq, void *dev)
{
    (void)(dev);

    if (irq > MAX_INT_NUM) {
        return;
    }

    g_osal_irq_array[irq].irq = 0;
    g_osal_irq_array[irq].dev = NULL;
    g_osal_irq_array[irq].handler = NULL;
    bm_free_irq(irq);
}

void osal_irq_enable(unsigned int irq)
{
    bm_enable_irq(irq);
}

void osal_irq_disable(unsigned int irq)
{
    bm_disable_irq(irq);
}

int osal_irq_set_priority(unsigned int irq, unsigned short priority)
{
    int ret = bm_irq_set_priority(irq, priority);
    return (ret == BM_FAIL) ? OSAL_FAILURE : OSAL_SUCCESS;
}

int osal_irq_set_affinity(unsigned int irq, const char *name, int cpu_mask)
{
    (void)(name);
    int ret = BM_FAIL;

    for (int i = 0; i < BM_MAX_CORE_NUM; i++) {
        if ((1 << i) == cpu_mask) {
            ret = bm_irq_set_affinity(irq, i);
        }
    }

    return (ret == BM_FAIL) ? OSAL_FAILURE : OSAL_SUCCESS;
}