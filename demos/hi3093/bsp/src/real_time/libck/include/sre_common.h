/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
#ifndef __SRE_COMMIN_H__
#define __SRE_COMMIN_H__

#include "sre_typedef.h"
#include "sre_sem.h"

#define TASK_CLK_INT 30
#define OS_OPENAMP_NOTIFY_HWI_NUM 7
#define OS_OPENAMP_NOTIFY_HWI_PRIO 0

#define LIBCK_CORE_ID 3
#define EPT_NUM 5

#define CONFIG_VIRTUAL_SERIAL 0
#define CONFIG_SHUTDOWN 1

#define LIBCK_SHUTDOWN_CMD "stop"

#define ENDPOINT_VIRT_SERIAL "virt_serial"
#define ENDPOINT_SHUTDOWN "shutdown"
#define ENDPOINT_DATA "console"

U32 SRE_Printf(const char *format, ...);
int app_main(void);

#endif /* __SRE_COMMIN_H__ */