/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: 系统模块基础头文件，用户不能直接引用
 * Author: LibCK项目组
 * Create: 2021-01-16
 * History: 2021-01-16 LibCK项目组 创建文件
 */
#ifndef OS_TRACE_H
#define OS_TRACE_H

#include "os_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup  OS_trace
 *
 * 设置TRACE模块的配置信息。
 */
struct TraceModInfo {
    /* 系统TRACE空间起始地址, 按16字节对齐 */
    uintptr_t traceBufAddr;
    /* 系统TRACE空间大小,含控制头,扣除控制头大小应为2的N次幂且不小于16 */
    uintptr_t traceBufSize;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_TRACE_H */
