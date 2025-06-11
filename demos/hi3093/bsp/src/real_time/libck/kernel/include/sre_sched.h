/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 任务模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * * @defgroup SRE_sched 调度对外基本功能接口
 * @ingroup SRE_kernel
 */

#ifndef SRE_SCHED_H
#define SRE_SCHED_H

#include "sre_base.h"
#include "./common/os_sched.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @brief 获取调度域相关信息。
 *
 * @par 描述
 * 在系统初始化后获取调度域相关配置信息。
 *
 * @attention
 * <ul><li>必须在任务模块初始化之后调用。</li></ul>
 *
 * @param[out]    info 类型#struct OsScheduleDomainInfo *, 调度域信息结构体指针，存放调度域信息。
 * @param[in/out] num  类型#U32 *，元素个数，输入/输出参数，输入：可保存的调度域信息结构体个数；
 *
 * @retval #不等于SRE_OK，表示失败，返回相应的错误码
 * @retval #SRE_OK        0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_sched.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_SchedDomainInfoGet(struct OsScheduleDomainInfo *info, U32 *num);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_SCHED_H */
