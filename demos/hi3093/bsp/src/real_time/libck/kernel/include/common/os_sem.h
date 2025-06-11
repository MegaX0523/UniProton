/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 信号量模块基础头文件，用户不能直接包含
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_SEM_H
#define OS_SEM_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * @ingroup OS_sem
 * 信号量错误码：初始化信号量内存不足。
 *
 * 值: 0x02000f00
 *
 * 解决方案: 可以将私有的静态内存空间配大。
 */
#define OS_ERRNO_SEM_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x00)

/*
 * @ingroup OS_sem
 * 信号量错误码：信号量句柄非法（错误或已删除）。
 *
 * 值: 0x02000f01
 *
 * 解决方案: 查看输入的信号量句柄值是否有效。
 */
#define OS_ERRNO_SEM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x01)

/*
 * @ingroup OS_sem
 * 信号量错误码：输入指针为空。
 *
 * 值: 0x02000f02
 *
 * 解决方案: 查看指针是否输入为空。
 */
#define OS_ERRNO_SEM_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x02)

/*
 * @ingroup OS_sem
 * 信号量错误码：没有空闲信号量。
 *
 * 值: 0x02000f03
 *
 * 解决方案: 查看信号量模块是否打开，或配置更多信号量。
 */
#define OS_ERRNO_SEM_ALL_BUSY OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x03)

/*
 * @ingroup OS_sem
 * 信号量错误码：信号量没有可用资源且Pend时设为不等待(等待时间为0)时获取信号量失败。
 *
 * 值: 0x02000f04
 *
 * 解决方案: 无。
 */
#define OS_ERRNO_SEM_UNAVAILABLE OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x04)

/*
 * @ingroup OS_sem
 * 信号量错误码：禁止中断处理函数阻塞于信号量。
 *
 * 值: 0x02000f05
 *
 * 解决方案: 查看是否在中断中Pend信号量。
 */
#define OS_ERRNO_SEM_PEND_INTERR OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x05)

/*
 * @ingroup OS_sem
 * 信号量错误码：任务切换锁定时，禁止任务阻塞于信号量。
 *
 * 值: 0x02000f06
 *
 * 解决方案: 不要在锁任务时pend没有资源可用的信号量。
 */
#define OS_ERRNO_SEM_PEND_IN_LOCK OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x06)

/*
 * @ingroup OS_sem
 * 信号量错误码：信号量等待超时。
 *
 * 值: 0x02000f07
 *
 * 解决方案: 无。
 */
#define OS_ERRNO_SEM_TIMEOUT OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x07)

/*
 * @ingroup OS_sem
 * 信号量错误码：信号量发生溢出。
 *
 * 值: 0x02000f08
 *
 * 解决方案: 查看输入的信号量计数值是否有效。
 */
#define OS_ERRNO_SEM_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x08)

/*
 * @ingroup OS_sem
 * 信号量错误码：信号量删除和重设当前值时有阻塞于信号量的任务。
 *
 * 值: 0x02000f09
 *
 * 解决方案: 如果当前信号量有任务阻塞，不能进行删除和重设计数值操作。
 */
#define OS_ERRNO_SEM_PENDED OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x09)

/*
 * @ingroup OS_sem
 * 信号量错误码：在中断中释放互斥型信号量。
 *
 * 值: 0x02000f0a
 *
 * 解决方案: 只能在任务中对互斥型信号量进行PV操作。
 */
#define OS_ERRNO_SEM_MUTEX_POST_INTERR OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x0A)

/*
 * @ingroup OS_sem
 * 信号量错误码：非当前互斥信号量的持有者释放该信号量。
 *
 * 值: 0x02000f0b
 *
 * 解决方案: 互斥信号量只能由其持有者释放，即互斥信号量的PV操作必须配对使用。
 */
#define OS_ERRNO_SEM_MUTEX_NOT_OWNER_POST OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x0B)

/*
 * @ingroup OS_sem
 * 信号量错误码：注册核内信号量个数为0导致注册失败。
 *
 * 值: 0x02000f10
 *
 * 解决方案: 查看信号量模块配置的最大个数是否为0。
 */
#define OS_ERRNO_SEM_REG_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x10)

/*
 * @ingroup OS_sem
 * 信号量错误码：调用#SRE_SemPendListGet时，指定的内存空间不足，无法存入全部的阻塞任务PID。
 *
 * 值: 0x02000f11
 *
 * 解决方案: 建议将数组长度配置为(#OS_TSK_MAX_SUPPORT_NUM + 1) * 4。
 */
#define OS_ERRNO_SEM_INPUT_BUF_NOT_ENOUGH OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x11)

/*
 * @ingroup OS_sem
 * 信号量错误码：调用#SRE_SemPendListGet时，输入指针为空或者bufLen小于4。
 *
 * 值: 0x02000f12
 *
 * 解决方案: 出参不能为NULL，指定的缓存长度不能小于4。
 */
#define OS_ERRNO_SEM_INPUT_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x12)

/*
 * @ingroup OS_sem
 * 信号量错误码：信号量创建时指定的唤醒方式非法。
 *
 * 值: 0x02000f13
 *
 * 解决方案: 唤醒方式只能为SEM_MODE_FIFO,SEM_MODE_PRIOR。
 */
#define OS_ERRNO_SEM_MODE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x13)

/*
 * @ingroup OS_sem
 * 信号量错误码：创建二进制信号量的初始值只能为OS_SEM_FULL或者OS_SEM_EMPTY。
 *
 * 值: 0x02000f14
 *
 * 解决方案: 二进制信号量初始值只能为OS_SEM_FULL(1)或者OS_SEM_EMPTY(0)。
 */
#define OS_ERRNO_SEM_BIN_VALUE_ERR OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x14)

/*
 * @ingroup OS_sem
 * 信号量错误码：信号量设置计数最大值时指定的最大值非法。
 *
 * 值: 0x02000f15
 *
 * 解决方案: 指定的信号量最大计数值不能大于0xFFFF FFFE。
 */
#define OS_ERRNO_SEM_MAXCOUNT_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x15)

/*
 * @ingroup OS_sem
 * 信号量错误码：信号量设置计数最大值时指定的最大值小于当前计数值。
 *
 * 值: 0x02000f16
 *
 * 解决方案: 指定的信号量最大计数值不能小于当前计数值。
 */
#define OS_ERRNO_SEM_MAXCOUNT_GREATER_THAN_CURRENT OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x16)

/*
 * @ingroup OS_sem
 * 信号量错误码：对二进制型信号量设置最大值。
 *
 * 值: 0x02000f17
 *
 * 解决方案: 不要对二进制型信号量设置最大值。
 */
#define OS_ERRNO_SEM_BIN_NO_MAXCOUNT OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x17)

/*
 * @ingroup OS_sem
 * 信号量错误码：重设信号量的当前计数值超过了设置的最大值。
 *
 * 值: 0x02000f18
 *
 * 解决方案: 设置的当前计数值不能大于设置的最大值。
 */
#define OS_ERRNO_SEM_OVER_MAXCOUNT OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x18)

/*
 * @ingroup OS_sem
 * 信号量错误码：获取信号量详细信息时出参结构体指针为NULL。
 *
 * 值: 0x02000f19
 *
 * 解决方案: 用来保存信号量详细信息的结构体指针不能为NULL。
 */
#define OS_ERRNO_SEM_INFO_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x19)

/*
 * @ingroup OS_sem
 * 信号量错误码：对二进制型信号量重设当前计数值。
 *
 * 值: 0x02000f1a
 *
 * 解决方案: 不要对二进制型信号量重设当前计数值。
 */
#define OS_ERRNO_SEM_BIN_CANNOT_RESET OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x1A)

/*
 * @ingroup OS_sem
 * 信号量错误码：互斥型信号量的唤醒方式不为优先级方式。
 *
 * 值: 0x02000f1c
 *
 * 解决方案: 互斥型信号量的唤醒方式不能为FIFO。
 */
#define OS_ERRNO_SEM_MUTEX_SHOULD_PRIO OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x1C)

/*
 * @ingroup OS_sem
 * 信号量错误码：获取当前信号量计数时传入的出参为NULL。
 *
 * 值: 0x02000f1d
 *
 * 解决方案: 互斥型信号量的唤醒方式不能为FIFO。
 */
#define OS_ERRNO_SEM_COUNT_GET_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SEM, 0x1D)

/*
 * 二进制信号量空闲状态，互斥型信号量初始计数值。
 */
#define OS_SEM_FULL  1

/*
 * 二进制信号量占用状态，同步型信号量初始计数值。
 */
#define OS_SEM_EMPTY 0

/*
 * 计数型信号量最大计数值。
 */
#define OS_SEM_COUNT_MAX 0xFFFFFFFE

/*
 * @ingroup OS_sem
 * 信号量模块被阻塞线程唤醒方式。
 */
enum SemMode {
    SEM_MODE_FIFO,  // 信号量FIFO唤醒模式
    SEM_MODE_PRIOR, // 信号量优先级唤醒模式
    SEM_MODE_BUTT   // 信号量非法唤醒方式
};

/*
 * @ingroup os_sem
 * 信号量模块配置信息的结构体定义。
 */
struct SemModInfo {
    /* 最大支持的信号量数 */
    U16 maxNum;
    /* 保留 */
    U16 reserved;
};

/*
 * 信号量句柄
 */
typedef U32 OsSemHandle;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OS_SEM_H */
