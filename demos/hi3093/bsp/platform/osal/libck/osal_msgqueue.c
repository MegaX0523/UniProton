/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: msgqueue
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#include "sre_queue.h"
#include "soc_osal.h"
#include "osal_errno.h"
#include "osal_inner.h"

int osal_msg_queue_create(const char *name, unsigned short queue_len, unsigned long *queue_id,
    unsigned int flags, unsigned short max_msgsize)
{
    osal_unused(name);
    osal_unused(flags);
    unsigned int ret = SRE_QueueCreate(queue_len, max_msgsize, (U32 *)queue_id);
    if (ret != SRE_OK) {
        osal_log("SRE_QueueCreate failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }
    return (int)ret;
}

int osal_msg_queue_write_copy(unsigned long queue_id, void *buffer_addr, unsigned int buffer_size, unsigned int timeout)
{
    unsigned int ret = SRE_QueueWrite(queue_id, buffer_addr, buffer_size, timeout, OS_QUEUE_NORMAL);
    if (ret != SRE_OK) {
        osal_log("SRE_QueueWrite failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }
    return (int)ret;
}

int osal_msg_queue_read_copy(unsigned long queue_id, void *buffer_addr, unsigned int *buffer_size, unsigned int timeout)
{
    unsigned int ret = SRE_QueueRead(queue_id, buffer_addr, (U32 *)buffer_size, timeout);
    if (ret != SRE_OK) {
        osal_log("SRE_QueueRead failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

void osal_msg_queue_delete(unsigned long queue_id)
{
    unsigned int ret = SRE_QueueDelete(queue_id);
    if (ret != SRE_OK) {
        osal_log("SRE_QueueDelete failed! ret = %#x.\n", ret);
    }
}

unsigned int osal_msg_queue_get_msg_num(unsigned long queue_id)
{
    unsigned int ret;
    unsigned int queue_num;

    ret = SRE_QueueNodeNumGet(queue_id, OS_QUEUE_PID_ALL, &queue_num);
    if (ret != SRE_OK) {
        osal_log("SRE_QueueNodeNumGet failed! ret = %#x.\n", ret);
        return TRUE;
    }

    return queue_num;
}