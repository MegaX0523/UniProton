/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: board外部头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * Notes: OS内部头文件，由sre_xxx.h文件包含，用户不能包含。
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_BOARD_H
#define OS_BOARD_H

#include "sre_buildef.h"

/* 当前支持MSGM IP 版本号 */
#define OS_MSGM_V100 100
#define OS_MSGM_V110 110
#define OS_MSGM_V130 130
#define OS_MSGM_V150 150
#define OS_MSGM_V160 160
#define OS_MSGM_V300 300

/* 当前支持TRING IP 版本号 */
#define OS_TRING_V200 200
#define OS_TRING_V210 210
#define OS_TRING_V220 220

/* 当前支持TIMER IP 版本号 */
#define OS_TIMER_V100 100
#define OS_TIMER_V130 130
#define OS_TIMER_V150 150

#if (OS_CPU_TYPE == OS_SD6183)
#define OS_MSGM_VER   OS_MSGM_V110
#define OS_TIMER_VER  OS_TIMER_V150
#include "os_board_sd6183.h"
#endif

#if (OS_CPU_TYPE == OS_HI1171)
#define OS_MSGM_VER   OS_MSGM_V110
#define OS_TIMER_VER  OS_TIMER_V150
#include "os_board_hi1171.h"
#endif

#if (OS_CPU_TYPE == OS_HI6881)
#include "os_board_hi6881.h"
#endif

#if (OS_CPU_TYPE == OS_SD6185)
#define OS_MSGM_VER    OS_MSGM_V130
#define OS_TRING_VER   OS_TRING_V200
#define OS_TIMER_VER  OS_TIMER_V150
#include "os_board_sd6185.h"
#endif

#if (OS_CPU_TYPE == OS_SD6186)
#define OS_MSGM_VER  OS_MSGM_V150
#define OS_TRING_VER  OS_TRING_V210
#define OS_TIMER_VER  OS_TIMER_V150
#include "os_board_sd6186.h"
#endif

#if (OS_CPU_TYPE == OS_SD6186L)
#define OS_MSGM_VER   OS_MSGM_V160
#define OS_TRING_VER  OS_TRING_V220
#define OS_TIMER_VER  OS_TIMER_V150
#include "os_board_sd6186l.h"
#endif

#if (OS_CPU_TYPE == OS_8021)
#define OS_MSGM_VER  OS_MSGM_V300
#define OS_TRING_VER  OS_TRING_V220
#define OS_TIMER_VER  OS_TIMER_V150
#include "os_board_8021.h"
#endif

#if (OS_CPU_TYPE == OS_8011)
#define OS_MSGM_VER  OS_MSGM_V300
#define OS_TRING_VER  OS_TRING_V220
#define OS_TIMER_VER  OS_TIMER_V150
#include "os_board_8011.h"
#endif

#if (OS_CPU_TYPE == OS_SD6182)
#define OS_MSGM_VER   OS_MSGM_V100
#define OS_TIMER_VER  OS_TIMER_V130
#include "os_board_sd6182.h"
#endif

#if (OS_CPU_TYPE == OS_SD6157)
#define OS_MSGM_VER   OS_MSGM_V100
#define OS_TIMER_VER  OS_TIMER_V130
#include "os_board_sd6157.h"
#endif

#if (OS_CPU_TYPE == OS_SD6155)
#define OS_TIMER_VER  OS_TIMER_V100
#include "os_board_sd6155.h"
#endif

#if (OS_CPU_TYPE == OS_8171)
#include "os_board_8171.h"
#endif

#endif /* OS_BOARD_H */
