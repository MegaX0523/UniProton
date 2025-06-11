/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: 定时器模块基础头文件，用户不能直接引用
 * Author: LibCK项目组
 * Create: 2021-01-16
 * History: 2021-01-16 LibCK项目组 创建文件
 */
#ifndef OS_TIMER_H
#define OS_TIMER_H

#include "os_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_timer
 * 全局定时器模块配置信息的结构体定义。
 *
 * 保存全局定时器模块的配置项信息。
 */
struct McHwtmrModInfo {
    /* 共享硬件定时器时钟频率 */
    U32 mcHwtmrClock;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_TIMER_H */
