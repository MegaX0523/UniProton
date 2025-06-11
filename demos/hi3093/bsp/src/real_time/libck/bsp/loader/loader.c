/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <stddef.h>
#include <stdarg.h>

#include "sre_task.h"
#include "sre_mem.h"
#include "sre_config.h"
#include "sre_common.h"
#include "sre_mcs.h"

#include "bm_common.h"
#include "bm_ops.h"
#include "bm_cache.h"
#include "bm_mmu.h"
#include "hi309x_memmap.h"
#include "loader.h"

static TskHandle g_testTskHandle;
static TskHandle g_testTskHandleCpuOff = -1;
SemHandle g_semHanderCpuOff = -1;

#ifndef BOOT_TEXT
#define BOOT_TEXT __attribute__((section(".boot.text")))
#endif
BOOT_TEXT WEAK void app_startup(char *const *argv);
BOOT_TEXT WEAK void app_startup(char *const *argv)
{
    (void)argv;
    char *cp = __bss_start;

    while (cp < __bss_end) {
        *cp++ = 0;
    }

    for (int i = 0; i < CPU_RELEASE_ADDR_LEN; i++) {
        bm_writel(0, CPU_RELEASE_ADDR + i * 4);
    }

    bm_enable_caches();
}

WEAK unsigned long __stack_chk_guard = 100;
INIT_TEXT WEAK void __stack_chk_fail(void);
INIT_TEXT WEAK int __vsprintf_chk(char *s, int flag, size_t slen, const char *format, va_list args);

INIT_TEXT WEAK void __stack_chk_fail(void)
{
    return;
}

INIT_TEXT WEAK int __vsprintf_chk(char *s, int flag, size_t slen, const char *format, va_list args)
{
    (void)s;
    (void)flag;
    (void)slen;
    (void)format;
    (void)args;
    return 0;
}

U32 SRE_HardDrvInit(void)
{
    U32 ret;

    ret = OsHwiInit();
    if (ret) {
        return ret;
    }

    ret = SRE_PrintfInit();
    if (ret) {
        return ret;
    }

    return OS_OK;
}

void SRE_HardBootInit(void)
{}

static int mcs_shutdown_cb(struct rpmsg_endpoint *ept, void *data, size_t len, unsigned int src, void *priv)
{
    (void)ept;
    (void)src;
    (void)priv;
    if (len == strlen(LIBCK_SHUTDOWN_CMD) && (strncmp(LIBCK_SHUTDOWN_CMD, (char *)data, len) == 0)) {
        SRE_SemPost(g_semHanderCpuOff);
    }

    return OS_OK;
}

static void InitTaskEntry(void)
{
    if (mcs_init()) {
        goto libck_run_error;
    }
#if defined(CONFIG_VIRTUAL_SERIAL) && (CONFIG_VIRTUAL_SERIAL == 1)
    if (sre_mcs_create_ept(ENDPOINT_VIRT_SERIAL, virtual_serial_callback)) {
        goto libck_run_error;
    }
#endif

#if defined(CONFIG_SHUTDOWN) && (CONFIG_SHUTDOWN == 1)
    if (sre_mcs_create_ept(ENDPOINT_SHUTDOWN, mcs_shutdown_cb)) {
        goto libck_run_error;
    }
#endif
    app_main();

libck_run_error:
    while (1) {
        SRE_Printf("libck run error\n");
        SRE_DelayMs(1000); /* delay 1000 ms */
    }
}

void __attribute__((weak)) mcs_peripherals_shutdown(void)
{
    return;
}

static void InitTaskEntryCpuOff(void)
{
    SRE_SemPend(g_semHanderCpuOff, OS_WAIT_FOREVER);
    mcs_deinit();
    SRE_HwiDisable(OS_OPENAMP_NOTIFY_HWI_NUM);
    SRE_HwiDisable(TASK_CLK_INT);
    mcs_peripherals_shutdown();
    SRE_DelayMs(10); /* 10ms后下电 */
    sre_mcs_shutdown();
}

static U32 OsTaskInit(void)
{
    U32 ret;

    struct TskInitParam param_cpu_off = {InitTaskEntryCpuOff, 2, 0, {0}, 0x1000, "InitTaskCpuOff", 0, 0};
    param_cpu_off.args[0] = g_semHanderCpuOff;
    ret = SRE_SemCreate(SINGLERCOUNT, &g_semHanderCpuOff);
    if (ret) {
        return ret;
    }

    ret = SRE_TaskCreate(&g_testTskHandleCpuOff, &param_cpu_off);
    if (ret) {
        SRE_SemDelete(g_semHanderCpuOff);
        return ret;
    }

    struct TskInitParam param_init = {InitTaskEntry, 25, 0, {0}, 0x2000, "InitTask", 0, 0};
    ret = SRE_TaskCreate(&g_testTskHandle, &param_init);
    if (ret) {
        SRE_SemDelete(g_semHanderCpuOff);
        return ret;
    }

    return OS_OK;
}

U32 SRE_AppInit(void)
{
    U32 ret;

    ret = OsTaskInit();
    if (ret) {
        return ret;
    }

    ret = TaskClkStart();
    if (ret) {
        return ret;
    }

    return OS_OK;
}

int main(void)
{
    return OsAppMain();
}