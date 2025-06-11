/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: tskmon模块基础头文件，用户不能直接引用
 * Author: LibCK项目组
 * Create: 2021-01-16
 * History: 2021-01-16 LibCK项目组 创建文件
 */
#ifndef OS_TSKMON_H
#define OS_TSKMON_H

#include "os_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_tskmon
 * 撑死处理模式：上报致命错误
 */
#define OS_TSKMON_MODE_REPORT_ERROR 0x01

/*
 * @ingroup OS_tskmon
 * 撑死处理模式：挂起任务
 */
#define OS_TSKMON_MODE_REAVE 0x02

/*
 * *@ingroup OS_tskmon
 * 任务死循环检测配置信息数据结构类型定义
 */
struct TskMonModInfo {
    /* 阀值，以tick为单位。 */
    U32 thresHold;
    /* 处理模式 */
    U32 procType;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_TSKMON_H */
