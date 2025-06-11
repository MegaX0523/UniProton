/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: task
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#include "sre_mem.h"
#include "sre_task.h"
#include "sre_task_external.h"
#include "sre_sys.h"
#include "osal_inner.h"
#include "soc_osal.h"
#include "sre_config.h"

#define OS_TSK_PRIORITY_DEFAUT 5

osal_task *osal_kthread_create(osal_kthread_handler handler, void *data, const char *name, unsigned int stack_size)
{
    if (handler == NULL) {
        osal_log("parameter invalid!\n");
        return NULL;
    }

    osal_task *task = (osal_task *)SRE_MemAlloc(OS_MID_OSAL, OS_MEM_DEFAULT_FSC_PT, sizeof(osal_task));
    if (task == NULL) {
        osal_log("SRE_MemAlloc failed!\n");
        return NULL;
    }

    unsigned int stacksize = (stack_size <= OS_TSK_DEFAULE_STACK_SIZE) ? OS_TSK_DEFAULE_STACK_SIZE : stack_size;
    struct TskInitParam my_task = { 0 };
    my_task.name = (char *)name;
    my_task.stackSize = stacksize;
    my_task.taskPrio = OS_TSK_PRIORITY_DEFAUT;
    my_task.taskEntry = (TskEntryFunc)handler;
    my_task.args[0] = data;

    unsigned int temp_task_id = 0;
    unsigned int ret = SRE_TaskCreate(&temp_task_id, &my_task);
    if (ret != SRE_OK) {
        SRE_MemFree(OS_MID_OSAL, task);
        osal_log("SRE_TaskCreate failed! ret = %#x.\n", ret);
        return NULL;
    }
    task->task = (void *)(uintptr_t)temp_task_id;
    return task;
}

void osal_kthread_destroy(osal_task *task, unsigned int stop_flag)
{
    osal_unused(stop_flag);
    unsigned int ret;
    if (task == NULL) {
        osal_log("parameter invalid!\n");
        return;
    }
    unsigned int uwtask_id = (unsigned int)(uintptr_t)task->task;
    ret = SRE_TaskDelete(uwtask_id);
    if (ret != SRE_OK && ret != OS_ERRNO_TSK_NOT_CREATED) {
        osal_log("SRE_TaskDelete failed! ret = %#x.\n", ret);
    }
    task->task = NULL;
    SRE_MemFree(OS_MID_OSAL, task);
}

long osal_get_current_pid(void)
{
    unsigned int ret;
    unsigned int task_id;
 
    ret = SRE_TaskSelf(&task_id);
    if (ret != SRE_OK) {
        osal_log("SRE_TaskSelf failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }
 
    return (long)task_id;
}

int osal_kthread_set_priority(osal_task *task, unsigned int priority)
{
    if (task == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }

    unsigned int ret = SRE_TaskPrioritySet((unsigned int)(uintptr_t)task->task, priority);
    if (ret != SRE_OK) {
        osal_log("SRE_TaskPrioritySet failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

unsigned long osal_msleep(unsigned int msecs)
{
    unsigned int ret;
    ret = SRE_TaskDelay((U32)DIV64(((U64)g_tickModInfo.tickPerSecond * msecs), SRE_SYS_MS_PER_SECOND));
    if (ret != SRE_OK) {
        osal_log("SRE_TaskDelay failed! ret = %#x.\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

void osal_udelay(unsigned int usecs)
{
    SRE_DelayUs(usecs);
}

void osal_mdelay(unsigned int msecs)
{
    SRE_DelayMs(msecs);
}

void osal_kthread_lock(void)
{
    SRE_TaskLock();
}

void osal_kthread_unlock(void)
{
    SRE_TaskUnlock();
}