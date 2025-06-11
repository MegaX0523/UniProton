/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: debug
 * Author: AuthorNameMagicTag
 * Create: 2021-12-16
 */

#include "sre_log_external.h"
#include "soc_osal.h"

void osal_printk(const char *fmt, ...)
{
    va_list args;

    if (fmt == NULL) {
        return;
    }

    va_start(args, fmt);
    if (g_printHook != NULL) {
        g_printHook(fmt, args);
    }
    va_end(args);
}