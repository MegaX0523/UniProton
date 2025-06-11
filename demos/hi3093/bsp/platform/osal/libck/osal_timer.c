/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: timer
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#include "sre_timer.h"
#include "sre_swtmr_external.h"
#include "sre_typedef.h"
#include "osal_inner.h"
#include "sre_sys.h"
#include "soc_osal.h"

unsigned long osal_timer_get_private_data(const void *sys_data)
{
    struct TagSwTmrCtrl *swtmr;
    swtmr = g_swtmrCBArray + (uintptr_t)OS_SWTMR_ID_2_INDEX(sys_data);
    return swtmr->arg1;
}

int osal_timer_init(osal_timer *timer)
{
    struct TimerCreatePara create_para;
    unsigned int ret;
    if (timer == NULL || timer->timer != NULL || timer->handler == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    if (timer->interval <= 0) {
        osal_log("interval:%d invalid!\n", timer->interval);
        return OSAL_FAILURE;
    }

    create_para.type= SRE_TIMER_SOFTWARE;
    create_para.mode = SRE_TIMER_LOOP;
    create_para.interval = timer->interval;
    create_para.callBackFunc = (SreTmrProcFunc)timer->handler;
    create_para.timerGroupId = OS_TICK_SWTMR_GRPID;
    create_para.arg1 = timer->data;
    ret = SRE_TimerCreate(&create_para, (TimerHandle *)&(timer->timer));
    if (ret != SRE_OK) {
        osal_log("SRE_TimerCreate Failed!, ret = %x\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

int osal_timer_start(osal_timer *timer)
{
    if (timer == NULL || timer->timer == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    unsigned int ret = SRE_TimerStart(NULL, (TimerHandle)(uintptr_t)(timer->timer));
    if (ret != SRE_OK) {
        osal_log("SRE_TimerStart Failed!, ret = %x\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

int osal_timer_stop(osal_timer *timer)
{
    if (timer == NULL || timer->timer == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    unsigned int ret = SRE_TimerStop(NULL, (TimerHandle)(uintptr_t)(timer->timer));
    if (ret != SRE_OK && ret != OS_ERRNO_SWTMR_UNSTART) {
        osal_log("SRE_TimerStop Failed!, ret = %x\n", ret);
        return OSAL_FAILURE;
    }
    return OSAL_SUCCESS;
}

int osal_timer_destroy(osal_timer *timer)
{
    if (timer == NULL || timer->timer == NULL) {
        osal_log("parameter invalid!\n");
        return OSAL_FAILURE;
    }
    unsigned int ret = SRE_TimerDelete(NULL, (TimerHandle)(uintptr_t)(timer->timer));
    if (ret != SRE_OK) {
        osal_log("SRE_TimerDelete Failed!, ret = %x\n", ret);
        return OSAL_FAILURE;
    }

    return OSAL_SUCCESS;
}

unsigned long long osal_get_jiffies(void)
{
    return SRE_TickCountGet();
}
