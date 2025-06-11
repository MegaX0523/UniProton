/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 内核钩子头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_HOOK_H
#define OS_HOOK_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_hook
 * HOOK错误码：HOOK类型错误。
 *
 * 值: 0x02001600
 *
 * 解决方案：确认输入钩子是否为有效的enum OsHookType类型。
 */
#define OS_ERRNO_HOOK_TYPE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HOOK, 0x00)

/*
 * @ingroup OS_hook
 * HOOK错误码：内存不足。
 *
 * 值: 0x02001601
 *
 * 解决方案：增加缺省分区大小。
 */
#define OS_ERRNO_HOOK_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_HOOK, 0x01)

/*
 * @ingroup OS_hook
 * HOOK错误码：HOOK指针空。
 *
 * 值: 0x02001602
 *
 * 解决方案：检查入参的钩子，NULL指针不允许进行添加或删除。
 */
#define OS_ERRNO_HOOK_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_HOOK, 0x02)

/*
 * @ingroup OS_hook
 * HOOK错误码：HOOK已存在。
 *
 * 值: 0x02001603
 *
 * 解决方案：确认该钩子之前是否已经成功注册，所以再次注册失败。
 */
#define OS_ERRNO_HOOK_EXISTED OS_ERRNO_BUILD_ERROR(OS_MID_HOOK, 0x03)

/*
 * @ingroup OS_hook
 * HOOK错误码：HOOK已满。
 *
 * 值: 0x02001604
 *
 * 解决方案：若为多钩子可以增大配置，或删掉一部分钩子；若为单钩子则表示重复注册，建议先删除再注册。
 */
#define OS_ERRNO_HOOK_FULL OS_ERRNO_BUILD_ERROR(OS_MID_HOOK, 0x04)

/*
 * @ingroup OS_hook
 * HOOK错误码：HOOK不存在。
 *
 * 值: 0x02001605
 *
 * 解决方案：确认该钩子之前是否已经成功注册。
 */
#define OS_ERRNO_HOOK_NOT_EXISTED OS_ERRNO_BUILD_ERROR(OS_MID_HOOK, 0x05)

/*
 * @ingroup OS_hook
 * HOOK错误码：HOOK配置个数为0。
 *
 * 值: 0x02001606
 *
 * 解决方案：该类型钩子个数配置为0，添加或者删除某类型钩子前，需要先对其个数进行正确配置。
 */
#define OS_ERRNO_HOOK_NOT_CFG OS_ERRNO_BUILD_ERROR(OS_MID_HOOK, 0x06)

/*
 * @ingroup OS_hook
 * HOOK错误码：HOOK执行方式错误。
 *
 * 值: 0x02001607
 *
 * 解决方案：确认钩子执行方式是否为有效的enum OsHookMode类型。
 */
#define OS_ERRNO_HOOK_MODE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_HOOK, 0x07)
/*
 * @ingroup OS_hook
 * 用户可使用钩子类型枚举。
 */
enum OsHookType {
    OS_HOOK_HWI_ENTRY = 0,  // 硬中断进入钩子
    OS_HOOK_HWI_EXIT,       // 硬中断退出钩子

    OS_HOOK_SWI_ENTRY,      // 软中断进入钩子
    OS_HOOK_SWI_EXIT,       // 软中断退出钩子

    OS_HOOK_TSK_CREATE,     // 任务创建钩子
    OS_HOOK_TSK_DELETE,     // 任务删除钩子
    OS_HOOK_TSK_SWITCH,     // 任务切换钩子
#if defined(OS_OPTION_SMP)
    OS_HOOK_READY_QUE_IN,   // 就绪队列入队钩子
    OS_HOOK_READY_QUE_OUT,  // 就绪队列出队钩子
#endif
    OS_HOOK_IDLE_PERIOD,    // IDLE钩子
    OS_HOOK_LAST_WORDS,     // 临终遗言钩子

#if defined(OS_OPTION_POWERGATE)
    OS_HOOK_DYNPWRGT_ENTRY,
    OS_HOOK_DYNPWRGT_EXIT,
#endif

#if defined(OS_OPTION_FIBER)
    OS_HOOK_FIBER_ENTRY,
    OS_HOOK_FIBER_EXIT,
#endif

    OS_HOOK_TYPE_NUM        // 钩子总数
};

/*
 * @ingroup OS_hook
 * 用户钩子执行方式枚举。
 */
enum OsHookMode {
    OS_HOOK_ORDER_FORWARD, // 正序方式
    OS_HOOK_ORDER_BACKWARD // 逆序方式
};

/*
 * 错误处理钩子函数，参数说明如下：
 * fileName [IN]  类型#const char *，出错文件名
 * lineNo   [IN]  类型#U32，出错的行号
 * errorNo  [IN]  类型#U32，用户定义的错误码
 * paraLen  [IN]  类型#U32，入参para的长度
 * para     [IN]  类型#void *，记录用户对于错误的描述或其他
 */
typedef void (*OsErrorHandleFunc)(const char *fileName, U32 lineNo, U32 errorNo, U32 paraLen, void *para);

/*
 * @ingroup os_hook
 * 钩子模块配置信息的结构体定义。
 */
struct OsHookModInfo {
    U8 maxNum[OS_HOOK_TYPE_NUM]; // 钩子配置最大个数
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_HOOK_H */
