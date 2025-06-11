/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: event
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#include "sre_event.h"
#include "sre_task.h"
#include "sre_mem.h"
#include "sre_sys_external.h"
#include "soc_osal.h"
#include "osal_inner.h"
#include "sre_config.h"

int osal_event_init(osal_event *event_obj)
{
    U32 *task_handl = NULL;
    if (event_obj == NULL || event_obj->event != NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    
    task_handl = (U32 *)SRE_MemAlloc(OS_MID_OSAL, OS_MEM_DEFAULT_FSC_PT, sizeof(U32));
    if (task_handl == NULL) {
        osal_log("SRE_MemAlloc failed!\n");
        return OSAL_FAILURE;
    }
    event_obj->event = task_handl;
    return OSAL_SUCCESS;
}

int osal_event_write(osal_event *event_obj, unsigned int mask)
{
    unsigned int ret;

    if (event_obj == NULL || event_obj->event == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    ret = SRE_EventWrite(*(U32 *)(event_obj->event), mask);
    if (ret != SRE_OK) {
        osal_log("SRE_EventWrite failed!,ret = %x\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

int osal_event_read(osal_event *event_obj, unsigned int mask, unsigned int timeout_ms, unsigned int mode)
{
    unsigned int ret;
    unsigned int timeout;
    unsigned int real_mode;
    unsigned int read;
    if (event_obj == NULL || event_obj->event == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    ret = SRE_TaskSelf(event_obj->event);
    if (ret != SRE_OK) {
        osal_log("SRE_TaskSelf failed!\n");
        return OSAL_FAILURE;
    }
    timeout = (timeout_ms == OSAL_EVENT_FOREVER) ? OS_EVENT_WAIT_FOREVER :
     (U32)DIV64(((U64)g_tickModInfo.tickPerSecond * timeout_ms), SRE_SYS_MS_PER_SECOND);
    real_mode = (mode == OSAL_WAITMODE_AND) ? (OS_EVENT_ALL | OS_EVENT_WAIT) : (OS_EVENT_ANY | OS_EVENT_WAIT);
    ret = SRE_EventRead(mask, real_mode, timeout, &read);
    if (ret != SRE_OK) {
        osal_log("SRE_EventRead failed!,ret = %x\n", ret);
        return OSAL_FAILURE;
    }
    return read;
}

int osal_event_destroy(osal_event *event_obj)
{
    unsigned int ret;
    if (event_obj == NULL || event_obj->event == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    ret = SRE_MemFree(OS_MID_OSAL, event_obj->event);
    if (ret != SRE_OK) {
        osal_log("SRE_MemFree failed!,ret = %x\n", ret);
        return OSAL_FAILURE;
    }
    event_obj->event = NULL;
    return OSAL_SUCCESS;
}
