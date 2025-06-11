/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 */
#include <stddef.h>
#include <stdarg.h>
#include "bm_common.h"
#include "bm_config.h"
#include "bm_hwi.h"
#include "bm_cache.h"
#include "bm_mmu.h"
#include "bm_atomic.h"
#include "hi309x_memmap.h"
#include "bm_mcs.h"
#include "bm_ops.h"
#include "string.h"
#include "loader.h"

volatile unsigned char g_shutdown_flag = 0;

int get_shutdown_flag(void)
{
    return g_shutdown_flag;
}

#ifdef MCS_2WIHT2_HI309X
#if defined(CONFIG_SHUTDOWN) && (CONFIG_SHUTDOWN == 1)

void __attribute__((weak)) mcs_peripherals_shutdown(void)
{
    return;
}

static int mcs_shutdown_cb(struct rpmsg_endpoint *ept, void *data, size_t len, unsigned int src, void *priv)
{
    (void)ept;
    (void)src;
    (void)priv;
    if (len == strlen(BM_SHUTDOWN_CMD) && (strncmp(BM_SHUTDOWN_CMD, (char *)data, len) == 0)) {
        g_shutdown_flag = 1;
        bm_disable_irq(IPI_INT_NO);
        mcs_peripherals_shutdown();
    }

    return BM_OK;
}
#endif
#endif

INIT_TEXT void bm_loader(void)
{
    bm_hwi_init();
    bm_enable_irq(IPI_INT_NO);
    if (bm_cmp_xchg32bits((unsigned int *)(uintptr_t)CPU_RELEASE_BOOT_SYNC, BOOT_SYNC_USING, 0) == 0) {
        bm_disable_mmu();
        bm_hwi_trigger(IPI_INT_NO, HI309X_TRIGGER_MAP);
        bm_enable_caches();
        bm_msg_init();
        bm_writel(BOOT_SYNC_READY, CPU_RELEASE_BOOT_SYNC);
        bm_flush_dcache_all();
        dsb();
        isb();
        goto jump_main;
    }
    while (bm_readl(CPU_RELEASE_BOOT_SYNC) != BOOT_SYNC_READY) {
        dsb();
        isb();
    }
    bm_enable_caches();
jump_main:
#ifdef MCS_2WIHT2_HI309X
    mcs_init();
#if defined(CONFIG_SHUTDOWN) && (CONFIG_SHUTDOWN == 1)
    bm_mcs_create_ept(ENDPOINT_SHUTDOWN, mcs_shutdown_cb);
#endif
#endif
    bm_local_irq_unmask();
    main();
}

#ifndef BOOT_TEXT
#define BOOT_TEXT __attribute__((section(".boot.text")))
#endif
BOOT_TEXT WEAK void app_startup(char * const * argv);
BOOT_TEXT WEAK void app_startup(char * const * argv)
{
    (void)argv;
    char *cp = __bss_start;

    while (cp < __bss_end) {
        *cp++ = 0;
    }
#ifdef MCS_2WIHT2_HI309X
    for (int i = 0; i < CPU_RELEASE_ADDR_LEN; i++) {
        bm_writel(0, CPU_RELEASE_ADDR + i * 4);
    }
    udelay(10000);
#endif
    bm_loader();
    while (1) {
    };
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