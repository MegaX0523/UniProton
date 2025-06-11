/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: 有名空间模块基础头文件，用户不能直接引用
 * Author: LibCK项目组
 * Create: 2021-01-16
 * History: 2021-01-16 LibCK项目组 创建文件
 */
#ifndef OS_NSM_H
#define OS_NSM_H

#include "os_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_nsm
 *
 * 有名空间注册结构体定义。
 */
struct NsmModInfo {
    /* 有名空间虚拟起始地址 */
    uintptr_t va;
    /* 有名空间物理起始地址 */
    uintptr_t pa;
    /* 有名空间内存空间大小 */
    uintptr_t size;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_NSM_H */
