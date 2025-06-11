/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: CPU占用率模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_CPUP_H
#define SRE_CPUP_H

#include "sre_base.h"
#include "./common/os_cpup.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup  SRE_cpup
 *
 * CPUP告警信息。
 */
#if !defined(OS_OPTION_SMP)
struct CpupWarnInfo {
    /* CPU占用率告警信息类型 */
    U16 type;
    /* 保留 */
    U16 reserve;
};
#else
struct CpupWarnInfo {
    /* CPU占用率告警信息类型 */
    U16 type;
    /* 核号 */
    U16 coreId;
};
#endif

/*
 * @ingroup SRE_cpup
 * @brief CPUP告警回调函数类型定义。
 *
 * @par 描述
 * 通过该回调函数的类型定义回调函数钩子。
 * @attention 无
 *
 * @param  #struct CpupWarnInfo*   [IN] 类型#struct CpupWarnInfo*，CPUP告警信息。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef void (*CpupHookFunc)(struct CpupWarnInfo *);

/*
 * @ingroup SRE_cpup
 * @brief 获取当前cpu占用率。
 *
 * @par 描述
 * 通过本接口获取当前cpu占用率。
 * @attention
 * <ul>
 * <li>该接口必须在CPUP模块裁剪开关打开，并在osStart之后才能调用此接口，否则返回0xffffffff。</li>
 * <li>精度为万分之一。</li>
 * <li>为了减小CPUP统计对线程调度的性能影响，OS采用了基于IDLE计数的统计算法，</li>
 * <li>统计结果会有一定误差，误差不超过百分之五。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval #0xffffffff      获取失败，CPUP裁剪开关未打开，或未初始化，或者在osStart之前调用。
 * @retval #[0,10000]       返回当前cpu占用率。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupAvg | SRE_CpupThread
 */
extern U32 SRE_CpupNow(void);

/*
 * @ingroup SRE_cpup
 * @brief 获取指定核当前的CPU占用率
 *
 * @par 描述
 * 通过用户输入的核号coreId，获取指定核当前cpu占用率。
 * @attention
 * <ul>
 * <li>OsStart之前不能调用此接口。</li>
 * <li>对于SD6183平台，支持实例间查询其他核的当前CPU占用率。</li>
 * <li>在使用该接口获取指定核当前CPU占用率时，如果其使用线程级CPUP模式，并且采样周期配置为0，</li>
 * <li>该获取功能需要依赖指定核计算其CPU占用率(调用#SRE_CpupNow或#SRE_CpupThread)。</li>
 * <li>调用此接口时，如果指定核未打开CPU占用率裁剪开关，或者未运行，或者核号非法，返回的是0xffffffff。</li>
 * </ul>
 *
 * @param coreId [IN]  类型#U32，指定核ID。
 *
 * @retval #0xffffffff         指定核号非法、指定核CPUP裁剪开关未打开或未初始化、指定核未启动情况下，返回0xffffffff。
 * @retval #[0,10000]          返回指定核当前cpu占用率。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupAvg | SRE_CpupNow
 */
extern U32 SRE_CpuUsageGet(U32 coreId);

/*
 * @ingroup SRE_cpup
 * @brief 获取平均cpu占用率接口。
 *
 * @par 描述
 * 通过本接口获取预先在配置项OS_CPUP_SAMPLE_RECORD_NUM中指定采样点的平均cpu占用率。
 * @attention
 * <ul>
 * <li>该接口必须在CPUP模块裁剪开关打开，并在osStart之后才能使用。</li>
 * <li>若系统CPUP模式配置为线程级，或在osStart之前调用，将返回0xffffffff，并且上报错误码。</li>
 * <li>当且仅当CPUP模式配置为系统级时，该接口有效。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval #0xffffffff      获取失败。
 * @retval #[0,10000]       返回平均cpu占用率。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupNow | SRE_CpupPeak
 */
extern U32 SRE_CpupAvg(void);

/*
 * @ingroup SRE_cpup
 * @brief 获取cpu占用率峰值。
 *
 * @par 描述
 * 通过本接口获取cpu占用率峰值。
 * @attention
 * <ul>
 * <li>在计算第一次系统CPU占用率之前，调用接口获取到的CPU占用率峰值为0。</li>
 * <li>当且仅当CPUP模式配置为系统级时，该接口有效。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval #[0,10000]          返回系统级CPU占用率峰值。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupAvg | SRE_CpupNow | SRE_CpupPeakClear
 */
extern U32 SRE_CpupPeak(void);

/*
 * @ingroup SRE_cpup
 * @brief 清空cpu占用率峰值。
 *
 * @par 描述
 * 通过本接口将cpu占用率峰值置零。
 * @attention
 * <ul>
 * <li>当且仅当CPUP模式配置为系统级时，该接口有效。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupPeak
 */
extern void SRE_CpupPeakClear(void);

/*
 * @ingroup SRE_cpup
 * @brief 补偿IDLE钩子执行时间。
 *
 * @par 描述
 * 根据用户输入的指定IDLE钩子补偿时间，对指定的IDLE钩子执行时间进行补偿。
 * @attention
 * <ul>
 * <li>该接口必须在CPUP模块裁剪开关打开，并在其初始化之后才能使用。</li>
 * <li>当且仅当CPUP模式配置为系统级时，该接口有效。</li>
 * <li>若对指定的IDLE钩子执行时间进行补偿，则该钩子执行时间不计算在负载内。</li>
 * <li>对单个IDLE钩子进行多次补偿，以及所有IDLE钩子的补偿值会进行累加,补偿值总大小的合法性由用户保证。</li>
 * <li>用户在对IDLE钩子进行补偿时，必须保证补偿值的准确性(需要考虑IDLE钩子执行时Cache是否命中的因素)，</li>
 * <li>否则会导致系统CPU占用率统计不准确。若IDLE钩子时间补偿值错误，</li>
 * <li>可能造成CPU占用率计算异常（超过10000或低于0）。此种情况，操作系统对计算值做容错处理，</li>
 * <li>若CPU占用率超过10000，则将计算值更正为10000。若CPU占用率低于0，则将计算值更正为0。</li>
 * </ul>
 *
 * @param hook     [IN]  类型#OsVoidFunc，输入的ILDE钩子。
 * @param addCycle [IN]  类型#U32，针对指定IDLE钩子补偿值大小。
 *
 * @retval #OS_ERRNO_CPUP_NOT_INITED             0x02000C0B，CPUP裁剪开关未打开，或者未初始化。
 * @retval #OS_ERRNO_CPUP_MODE_NOT_CORRECT       0x02000C06，CPUP模式配置为线程级。
 * @retval #OS_ERRNO_CPUP_OS_NOT_STARTED         0x02000C05，在osStart之前调用。
 * @retval #OS_ERRNO_CPUP_COMPEN_HOOK_NULL       0x02000C0E，输入的IDLE钩子为NULL。
 * @retval #OS_ERRNO_CPUP_COMPEN_VALUE_ZERO      0x02000C0F，输入的补偿值大小为0。
 * @retval #OS_ERRNO_CPUP_COMPEN_HOOK_NO_REG     0x02000C10，输入的的IDLE钩子未注册。
 * @retval #OS_ERRNO_CPUP_COMPEN_VALUE_TOO_LARGE 0x02000C11，输入的补偿值大小太大，超过了采样周期。
 * @retval #SRE_OK                               0x00000000，补偿成功。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupNow
 */
extern U32 SRE_CpupIdleHookLoadAdd(OsVoidFunc hook, U32 addCycle);

/*
 * @ingroup SRE_cpup
 * @brief 去除IDLE钩子补偿时间。
 *
 * @par 描述
 * 根据用户输入的指定IDLE钩子补偿时间，对IDLE钩子补偿时间进行去除。
 * @attention
 * <ul>
 * <li>该接口必须在CPUP模块裁剪开关打开，并在osStart之后才能使用。</li>
 * <li>当且仅当CPUP模式配置为系统级时，该接口有效。</li>
 * <li>若对指定的IDLE钩子执行时间进行补偿，则该钩子执行时间不计算在负载内。</li>
 * <li>去除ILDE钩子的补偿时间大小合法性由用户保证。</li>
 * </ul>
 *
 * @param hook     [IN]  类型#OsVoidFunc，存放中断线程的CPUP信息指针。
 * @param subCycle [IN]  类型#U32，针对指定IDLE钩子补偿值大小。
 *
 * @retval #OS_ERRNO_CPUP_NOT_INITED             0x02000C0B，CPUP裁剪开关未打开，或者未初始化。
 * @retval #OS_ERRNO_CPUP_MODE_NOT_CORRECT       0x02000C06，CPUP模式配置为线程级。
 * @retval #OS_ERRNO_CPUP_OS_NOT_STARTED         0x02000C05，在osStart之前调用。
 * @retval #OS_ERRNO_CPUP_COMPEN_HOOK_NULL       0x02000C0E，输入的IDLE钩子为NULL。
 * @retval #OS_ERRNO_CPUP_COMPEN_VALUE_ZERO      0x02000C0F，输入的去除补偿值大小为0。
 * @retval #OS_ERRNO_CPUP_COMPEN_HOOK_NO_REG     0x02000C10，输入的的IDLE钩子未注册。
 * @retval #OS_ERRNO_CPUP_COMPEN_VALUE_TOO_LARGE 0x02000C11，输入的去除补偿值大小太大，超过了IDLE钩子已补偿值总大小。
 * @retval #SRE_OK                               0x00000000，去除补偿成功。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupNow
 */
extern U32 SRE_CpupIdleHookLoadSub(OsVoidFunc hook, U32 subCycle);

/*
 * @ingroup SRE_cpup
 * @brief 获取指定个数的线程的CPU占用率。
 *
 * @par 描述
 * 根据用户输入的线程个数，获取指定个数的线程CPU占用率。
 * @attention
 * <ul>
 * <li>当且仅当CPUP模式配置为线程级时，该接口有效。</li>
 * <li>当配置项中的采样周期值等于0时，线程级CPUP采样周期为两次调用该接口或者SRE_CpupNow之间的间隔。</li>
 * <li>否则，线程级CPUP采样周期为配置项中的OS_CPUP_SAMPLE_INTERVAL大小。</li>
 * <li>输出的实际线程个数不大于系统中实际的线程个数(任务个数和一个中断线程)。</li>
 * <li>若输入的线程个数为1，则仅输出中断线程(除任务线程以外的线程统称)的CPUP信息。</li>
 * <li>若在一个采样周期内有任务被删除，则统计的任务线程和中断线程CPUP总和小于10000。</li>
 * </ul>
 *
 * @param inNum  [IN]  类型#U32，输入的线程个数。
 * @param cpup   [OUT] 类型#struct CpupThread *，缓冲区，输出参数，用于填写输出个数线程的CPUP信息。
 * @param outNum [OUT] 类型#U32 *，保存输出的实际线程个数指针。
 *
 * @retval #OS_ERRNO_CPUP_NOT_INITED           0x02000C0B，CPUP裁剪开关未打开，或者未初始化。
 * @retval #OS_ERRNO_CPUP_MODE_NOT_CORRECT     0x02000C06，CPUP模式配置为线程级。
 * @retval #OS_ERRNO_CPUP_OS_NOT_STARTED       0x02000C05，在osStart之前调用。
 * @retval #OS_ERRNO_CPUP_PTR_NULL             0x02000C02，参数指针为NULL。
 * @retval #OS_ERRNO_CPUP_THREAD_INNUM_INVALID 0x02000C0C，输入的线程个数为0。
 * @retval #SRE_OK                             0x00000000，获取成功。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupNow
 */
extern U32 SRE_CpupThread(U32 inNum, struct CpupThread *cpup, U32 *outNum);

/*
 * @ingroup SRE_cpup
 * @brief 获取指定任务的CPU占用率。
 *
 * @par 描述
 * 根据用户输入的任务ID，获取指定任务的CPU占用率。
 * @attention
 * <ul>
 * <li>当且仅当CPUP模式配置为线程级时，该接口有效。</li>
 * <li>当采样周期配置为0调用此接口时，获取到的是，上一采样周期结束到调用此接口时间内指定任务的CPU占用率。</li>
 * <li>当采样周期配置不为0时，获取到的是，上一采样周期内指定任务的CPU占用率。</li>
 * </ul>
 *
 * @param taskId   [IN]  类型#U32，输入的任务ID。
 * @param taskRate [OUT] 类型#U32 *，存放指定任务线程的CPUP信息指针。
 *
 * @retval #OS_ERRNO_CPUP_NOT_INITED           0x02000C0B，CPUP裁剪开关未打开，或者未初始化。
 * @retval #OS_ERRNO_CPUP_MODE_NOT_CORRECT     0x02000C06，CPUP模式配置不为线程级。
 * @retval #OS_ERRNO_CPUP_OS_NOT_STARTED       0x02000C05，在osStart之前调用。
 * @retval #OS_ERRNO_CPUP_PTR_NULL             0x02000C02，参数指针为NULL。
 * @retval #OS_ERRNO_CPUP_TASKID_INVALID       0x02000C09，输入的任务ID非法。
 * @retval #OS_ERRNO_CPUP_TASK_NO_CREATE       0x02000C0D，输入的任务未创建或者已被删除。
 * @retval #SRE_OK                             0x00000000，获取成功。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupThread
 */
extern U32 SRE_CpupTaskGetById(U32 taskId, U32 *taskRate);

/*
 * @ingroup SRE_cpup
 * @brief 获取中断线程的CPU占用率。
 *
 * @par 描述
 * 获取中断线程的CPU占用率。
 * @attention
 * <ul>
 * <li>当且仅当CPUP模式配置为线程级时，该接口有效。</li>
 * <li>当采样周期配置为0调用此接口时，获取到的是，上一采样周期结束到调用此接口时间内中断线程的CPU占用率。</li>
 * <li>当采样周期配置不为0时，获取到的是，上一采样周期内中断线程的CPU占用率。</li>
 * </ul>
 *
 * @param intRate [OUT] 类型#U32 *，存放中断线程的CPUP信息指针。
 *
 * @retval #OS_ERRNO_CPUP_NOT_INITED           0x02000C0B，CPUP裁剪开关未打开，或者未初始化。
 * @retval #OS_ERRNO_CPUP_MODE_NOT_CORRECT     0x02000C06，CPUP模式配置不为线程级。
 * @retval #OS_ERRNO_CPUP_OS_NOT_STARTED       0x02000C05，在osStart之前调用。
 * @retval #OS_ERRNO_CPUP_PTR_NULL             0x02000C02，参数指针为NULL。
 * @retval #SRE_OK                             0x00000000，获取成功。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupTaskGetById
 */
extern U32 SRE_CpupIntGet(U32 *intRate);

#if defined(OS_OPTION_CPUP_WARN)
/*
 * @ingroup SRE_cpup
 * @brief 设置CPU占用率告警阈值。
 *
 * @par 描述
 * 根据用户配置的CPU占用率告警阈值warn和告警恢复阈值resume，设置告警和恢复阈值。
 * @attention
 * <ul>
 * <li>OsStart之前不能调用此接口。</li>
 * </ul>
 *
 * @param warn   [IN]  类型#U32，CPUP告警阈值。
 * @param resume [IN]  类型#U32，CPUP恢复阈值。
 *
 * @retval #OS_ERRNO_CPUP_NOT_INITED            0x02000C0B，CPUP裁剪开关未打开，或者未初始化。
 * @retval #OS_ERRNO_CPUP_OS_NOT_STARTED        0x02000C05，在osStart之前调用。
 * @retval #OS_ERRNO_CPUP_INTERVAL_NOT_SUITED   0x02000C01，阈值设定不在规定范围(0,10000]内。
 * @retval #OS_ERRNO_CPUP_RESUME_NOT_SUITED     0x02000C03, 恢复阈值设定不小于告警阈值。
 * @retval #SRE_OK                              0x00000000，阈值设定设定成功。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupWarnValueGet
 */
extern U32 SRE_CpupWarnValueSet(U32 warn, U32 resume);

/*
 * @ingroup SRE_cpup
 * @brief 查询CPUP告警阈值和告警恢复阈值
 *
 * @par 描述
 * 根据用户配置的告警阈值指针warn和告警恢复阈值指针resume，查询告警阈值和告警恢复阈值
 * @attention
 * <ul>
 * <li>OsStart之前不能调用此接口。</li>
 * </ul>
 *
 * @param warn   [OUT] 类型#U32 *，CPUP告警阈值。
 * @param resume [OUT] 类型#U32 *，CPUP恢复阈值。
 *
 * @retval #OS_ERRNO_CPUP_NOT_INITED           0x02000C0B，CPUP裁剪开关未打开，或者未初始化。
 * @retval #OS_ERRNO_CPUP_OS_NOT_STARTED       0x02000C05，在osStart之前调用。
 * @retval #OS_ERRNO_CPUP_PTR_NULL             0x02000C02，参数指针为NULL。
 * @retval #SRE_OK                             0x00000000，获取成功。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_CpupWarnValueSet
 */
extern U32 SRE_CpupWarnValueGet(U32 *warn, U32 *resume);

/*
 * @ingroup SRE_cpup
 * @brief 注册CPUP告警回调函数
 *
 * @par 描述
 * 根据用户配置的回调函数hook，注册CPUP告警回调函数
 * @attention
 * <ul>
 * <li>不允许重复或覆盖注册钩子。</li>
 * <li>hook为NULL时，表示删除该钩子。</li>
 * </ul>
 *
 * @param hook [IN]  类型#CpupHookFunc，CPU告警回调函数。
 *
 * @retval #OS_ERRNO_HOOK_FULL              0x02001604，HOOK已满或配置个数为0。
 * @retval #SRE_OK                          0x00000000，注册成功。
 * @par 依赖
 * <ul><li>sre_cpup.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
extern U32 SRE_CpupWarnHookReg(CpupHookFunc hook);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_CPUP_H */
