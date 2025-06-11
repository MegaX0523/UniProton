/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: CPUP模块的头文件, 用户不能直接包含
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_CPUP_H
#define OS_CPUP_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_CPUP
 * CPUP错误码：CPUP模式配置不正确。
 *
 * 值: 0x02000c00
 *
 * 解决方案: CPUP模式必须配置为OS_CPUP_MODE_SYS或者OS_CPUP_MODE_THREAD。
 */
#define OS_ERRNO_CPUP_MODE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x00)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：参数(CPUP告警阈值)设定不在规定范围(0,10000]。
 *
 * 值: 0x02000c01
 *
 * 解决方案: CPUP告警阈值设定在规定范围(0,10000]内。
 */
#define OS_ERRNO_CPUP_INTERVAL_NOT_SUITED OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x01)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：指针参数为NULL。
 *
 * 值: 0x02000c02
 *
 * 解决方案: 传入非0的有效地址。
 */
#define OS_ERRNO_CPUP_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x02)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：恢复阈值设定不小于告警阈值。
 *
 * 值: 0x02000c03
 *
 * 解决方案: 恢复阈值小于告警阈值。
 */
#define OS_ERRNO_CPUP_RESUME_NOT_SUITED OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x03)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：CPUP初始化申请内存失败。
 *
 * 值: 0x02000c04
 *
 * 解决方案: 确认缺省静态内存是否足够，以及采样个数(参见配置宏OS_CPUP_SAMPLE_RECORD_NUM)是否过大。
 */
#define OS_ERRNO_CPUP_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x04)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：在osStart之前调用CPUP模块相关功能接口。
 *
 * 值: 0x02000c05
 *
 * 解决方案: 调用CPUP模块功能接口时，请查看是否在osStart之后。
 */
#define OS_ERRNO_CPUP_OS_NOT_STARTED OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x05)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：CPUP配置模式不正确。
 *
 * 值: 0x02000c06
 *
 * 解决方案: CPUP模块配置模式必须配置正确的模式，才能调用相对应的接口。
 */
#define OS_ERRNO_CPUP_MODE_NOT_CORRECT OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x06)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：CPUP采样时间间隔为0。
 *
 * 值: 0x02000c07
 *
 * 解决方案: 系统级CPUP设置采样时间间隔必须大于0，当CPUP告警功能打开时，线程级CPUP采样时间间隔也必须大于0。
 */
#define OS_ERRNO_CPUP_SAMPLE_TIME_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x07)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：指定核号非法。
 *
 * 值: 0x02000c08
 *
 * 解决方案: 指定核小于最大核数。
 */
#define OS_ERRNO_CPUP_COREID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x08)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：获取CPUP时入参任务ID非法。
 *
 * 值: 0x02000c09
 *
 * 解决方案: 请输入合法任务ID。
 */
#define OS_ERRNO_CPUP_TASKID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x09)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：CPUP采样个数为零。
 *
 * 值: 0x02000c0a
 *
 * 解决方案: 设置采样地个数在(0,30]之间。
 */
#define OS_ERRNO_CPUP_SAMPLE_NUM_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x0A)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：CPUP功能开关未打开或者未初始化时，获取CPUP、设置告警阈值或者补偿IDLE钩子执行时间。
 *
 * 值: 0x02000c0b
 *
 * 解决方案: 需保证在打开功能开关,在osStart之后才能获取CPUP、设置告警阈值或者补偿IDLE钩子执行时间。
 */
#define OS_ERRNO_CPUP_NOT_INITED OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x0B)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：使用获取线程级所有线程的CPUP接口时，输入的线程数为0。
 *
 * 值: 0x02000c0c
 *
 * 解决方案: 使用获取线程级所有线程的CPUP接口时，输入的线程数必须大于0
 */
#define OS_ERRNO_CPUP_THREAD_INNUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x0C)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：获取指定任务CPU占用率时，该任务还未创建或已删除。
 *
 * 值: 0x02000c0d
 *
 * 解决方案: 需保证获取指定任务CPU占用率时，该任务已经创建且未被删除。
 */
#define OS_ERRNO_CPUP_TASK_NO_CREATE OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x0D)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：补偿系统级CPUP时，输入的钩子为NULL。
 *
 * 值: 0x02000c0e
 *
 * 解决方案: 补偿系统级CPUP时，需保证输入的钩子为非NULL。
 */
#define OS_ERRNO_CPUP_COMPEN_HOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x0E)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：补偿系统级CPUP时，输入的补偿值为0，或者去除IDLE钩子补偿时，输入的值为0。
 *
 * 值: 0x02000c0f
 *
 * 解决方案: 当补偿系统级CPUP时，输入的补偿值大于0；当去除IDLE钩子补偿时，输入的值不能等于0。
 */
#define OS_ERRNO_CPUP_COMPEN_VALUE_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x0F)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：补偿系统级CPUP时，输入的钩子未注册。
 *
 * 值: 0x02000c10
 *
 * 解决方案: 补偿系统级CPUP时，输入的钩子已注册。
 */
#define OS_ERRNO_CPUP_COMPEN_HOOK_NO_REG OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x10)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：补偿系统级CPUP
 * IDLE钩子时，输入的补偿值大于等于采样周期；去除IDLE钩子补偿时，输入的值大于已经补偿值的总大小。
 *
 * 值: 0x02000c11
 *
 * 解决方案: 补偿系统级CPUP时，需保证输入的补偿值小于采样周期；
 * 当去除IDLE钩子补偿值时，输入的去除值不能大于已经补偿值的总大小。
 */
#define OS_ERRNO_CPUP_COMPEN_VALUE_TOO_LARGE OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x11)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：CPUP告警恢复及CPUP告警阈值配置错误。
 *
 * 值: 0x02000c12
 *
 * 解决方案: CPUP告警阈值设定应该小于CPUP告警恢复阈值。
 */
#define OS_ERRNO_CPUP_RESUME_VALUE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_CPUP, 0x12)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：g_baseValue变量所在内存被踩。
 *
 * 值: 0x03000c13
 *
 * 解决方案: 定位g_baseValue变量何时被踩，修正踩内存错误。
 */
#define OS_ERRNO_CPUP_MEM_DAMAGED_ERROR OS_ERRNO_BUILD_FATAL(OS_MID_CPUP, 0x13)

/*
 * @ingroup OS_CPUP
 * CPUP错误码：CPUP名字申请失败。
 *
 * 值: 0x03000c14
 *
 * 解决方案: 增大共享名字空间大小。
 */
#define OS_ERRNO_CPUP_NAME_ALLOC_FAILED OS_ERRNO_BUILD_FATAL(OS_MID_CPUP, 0x14)

/*
 * @ingroup OS_CPUP
 * CPU占用率告警标志。
 */
#define CPUP_INFO_TYPE_OVERLOAD 0x01

/*
 * @ingroup OS_CPUP
 * CPU占用率恢复告警标志。
 */
#define CPUP_INFO_TYPE_RECONVERT 0x02

/*
 * @ingroup OS_CPUP
 * CPU占用率使用模式：系统级CPUP。
 */
#define OS_CPUP_MODE_SYS 0x01

/*
 * @ingroup OS_CPUP
 * CPU占用率使用模式：线程级CPUP。
 */
#define OS_CPUP_MODE_THREAD 0x02

/*
 * @ingroup  OS_CPUP
 * 设置cpu占用率的注册信息结构体。
 */
struct CpupModInfo {
    /* CPUP告警标志 */
    bool cpupWarnFlag;
    /* 保存的采样个数 */
    U32 sampleNum;
    /* 采样间隔，单位tick */
    U32 sampleTime;
    /* CPU占用率告警阈值 */
    U32 warn;
    /* CPU占用率告警恢复阈值 */
    U32 resume;
};

/*
 * @ingroup os_cpup
 * 线程级CPU占用率结构体。
 */
struct CpupThread {
    /* 线程ID */
    U32 id;
    /* 占用率，取值[0,10000] */
    U16 usage;
    /* 保留 */
    U16 resv;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_CPUP_H */
