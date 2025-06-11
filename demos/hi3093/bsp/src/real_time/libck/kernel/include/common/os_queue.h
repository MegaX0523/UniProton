/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 队列模块的基础头文件，用户不能直接包含
 * Author: LibCK项目组
 * Create: 2009-08-01
 */
#ifndef OS_QUEUE_H
#define OS_QUEUE_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * @ingroup os_queue
 * 队列错误码：队列最大资源数配置成0。
 *
 * 值: 0x02002701
 *
 * 解决方案: 队列最大资源数配置大于0，如果不用队列模块，可以配置裁剪开关为NO。
 */
#define OS_ERRNO_QUEUE_MAXNUM_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x01)

/*
 * @ingroup os_queue
 * 队列错误码：初始化队列内存不足。
 *
 * 值: 0x02002702
 *
 * 解决方案: 分配更大的内存分区。
 */
#define OS_ERRNO_QUEUE_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x02)

/*
 * @ingroup os_queue
 * 队列错误码：队列创建时内存不足。
 *
 * 值: 0x02002703
 *
 * 解决方案: 可以将内存空间配大。或将创建队列的节点长度和节点个数减少。
 */
#define OS_ERRNO_QUEUE_CREATE_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x03)

/*
 * @ingroup os_queue
 * 队列错误码：没有空闲的队列资源，已经达到配置的最大队列数。
 *
 * 值: 0x02002704
 *
 * 解决方案: 增加配置项中队列资源数配置。
 */
#define OS_ERRNO_QUEUE_CB_UNAVAILABLE OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x04)

/*
 * @ingroup os_queue
 * 队列错误码：任务切换锁定时，禁止任务阻塞于队列。
 *
 * 值: 0x02002705
 *
 * 解决方案: 使用前，任务先解锁。
 */
#define OS_ERRNO_QUEUE_PEND_IN_LOCK OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x05)

/*
 * @ingroup os_queue
 * 队列错误码：队列等待超时。
 *
 * 值: 0x02002706
 *
 * 解决方案: 请查看超时时间设置是否合适。
 */
#define OS_ERRNO_QUEUE_TIMEOUT OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x06)

/*
 * @ingroup os_queue
 * 队列错误码：不能删除被任务阻塞的队列。
 *
 * 值: 0x02002707
 *
 * 解决方案: 让阻塞的任务获得资源，不阻塞在此队列上。
 */
#define OS_ERRNO_QUEUE_IN_TSKUSE OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x07)

/*
 * @ingroup os_queue
 * 队列错误码：有超时情况下写队列不能在中断中使用。
 *
 * 值: 0x02002708
 *
 * 解决方案: 同步队列超时时间配置为无等待或者使用异步队列。
 */
#define OS_ERRNO_QUEUE_IN_INTERRUPT OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x08)

/*
 * @ingroup os_queue
 * 队列错误码：队列未创建。
 *
 * 值: 0x02002709
 *
 * 解决方案: 输入正确的入参。
 */
#define OS_ERRNO_QUEUE_NOT_CREATE OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x09)

/*
 * @ingroup os_queue
 * 队列错误码：阻塞在任务上的队列被激活，但没有得到调度，不能删除
 *
 * 值: 0x0200270a
 *
 * 解决方案: 等待任务被调度后，就可以删除。
 */
#define OS_ERRNO_QUEUE_BUSY OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x0a)

/*
 * @ingroup os_queue
 * 队列错误码：队列创建时输入的指针为空。
 *
 * 值: 0x0200270b
 *
 * 解决方案: 查看队列创建时输入的指针是否为空。
 */
#define OS_ERRNO_QUEUE_CREAT_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x0b)

/*
 * @ingroup os_queue
 * 队列错误码：队列创建时入参队列长度或者队列消息结点大小为0。
 *
 * 值: 0x0200270c
 *
 * 解决方案: 输入正确的入参。
 */
#define OS_ERRNO_QUEUE_PARA_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x0c)

/*
 * @ingroup os_queue
 * 队列错误码：队列句柄非法，错误或超出队列句柄范围。
 *
 * 值: 0x0200270d
 *
 * 解决方案: 查看输入的队列句柄值是否有效。
 */
#define OS_ERRNO_QUEUE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x0d)

/*
 * @ingroup os_queue
 * 队列错误码：指针为空。
 *
 * 值: 0x0200270e
 *
 * 解决方案: 查看输入的指针是否输入为空。
 */
#define OS_ERRNO_QUEUE_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x0e)

/*
 * @ingroup os_queue
 * 队列错误码：读写队列时buffer长度为0。
 *
 * 值: 0x0200270f
 *
 * 解决方案: 输入正确的入参。
 */
#define OS_ERRNO_QUEUE_SIZE_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x0f)

/*
 * @ingroup os_queue
 * 队列错误码：写队列时，输入buffer的大小大于队列结点大小。
 *
 * 值: 0x02002710
 *
 * 解决方案: 减少buffer的大小，或者使用更大结点大小的队列。
 */
#define OS_ERRNO_QUEUE_SIZE_TOO_BIG OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x10)

/*
 * @ingroup os_queue
 * 队列错误码：读写队列时，没有资源。
 *
 * 值: 0x02002711
 *
 * 解决方案: 写队列前需保证要有空闲的节点，读队列时需保证队列里有消息。
 */
#define OS_ERRNO_QUEUE_NO_SOURCE OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x11)

/*
 * @ingroup os_queue
 * 队列错误码：队列优先级参数有误
 *
 * 值: 0x02002712
 *
 * 解决方案: 请检查参数，参数只能是0或1
 */
#define OS_ERRNO_QUEUE_PRIO_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x12)

/*
 * @ingroup os_queue
 * 队列错误码：节点长度超过最大值
 *
 * 值: 0x02002713
 *
 * 解决方案: 队列节点长度不能大于0XFFFA
 */
#define OS_ERRNO_QUEUE_NSIZE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x13)

/*
 * @ingroup os_queue
 * 队列错误码：队列的名字重名
 *
 * 值: 0x02002714
 *
 * 解决方案: 检查创建的队列名字是否已经使用
 */
#define OS_ERRNO_QUEUE_NAME_REPEATED OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x14)

/*
 * @ingroup os_queue
 * 队列错误码：输入的队列名字为空字符串
 *
 * 值: 0x02002715
 *
 * 解决方案: 队列名字不能为空字符串
 */
#define OS_ERRNO_QUEUE_NAME_EMPTY OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x15)

/*
 * @ingroup os_queue
 * 队列错误码：保序队列节点长度过大
 *
 * 值: 0x02002716
 *
 * 解决方案: 保序队列节点长度不超过128个
 */
#define OS_ERRNO_QUEUE_ORDERQUE_NODE_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x16)

/*
 * @ingroup os_queue
 * 队列错误码：保序队列缓存与节点个数不匹配
 *
 * 值: 0x02002717
 *
 * 解决方案: 先根据节点个数获取所需缓存大小，再创建
 */
#define OS_ERRNO_QUEUE_ORDERQUE_BUFF_SIZE_ERR OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x17)

/*
 * @ingroup os_queue
 * 队列错误码：保序队列接收消息时在非CR协程上下文
 *
 * 值: 0x02002718
 *
 * 解决方案: 保序队列只支持CR协程接收队列消息
 */
#define OS_ERRNO_QUEUE_ORDERQUE_THREAD_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x18)

/*
 * @ingroup os_queue
 * 队列错误码：保序队列接收消息时任务发生了核间迁移
 *
 * 值: 0x03002719
 *
 * 解决方案: 保序队列接收消息的任务在迁移等操作前必须保证队列非空
 */
#define OS_ERRNO_QUEUE_ORDERQUE_THREAD_MIGRATE OS_ERRNO_BUILD_FATAL(OS_MID_QUEUE, 0x19)

/*
 * @ingroup os_queue
 * 队列错误码：保序队列消息节点满
 *
 * 值: 0x0300271a
 *
 * 解决方案: 排查消息节点满的原因，缓存节点个数配置增大或者解决接收消息阻塞问题。
 */
#define OS_ERRNO_QUEUE_ORDERQUE_FULL OS_ERRNO_BUILD_FATAL(OS_MID_QUEUE, 0x1a)

/*
 * @ingroup os_queue
 * 队列错误码：保序队列消息读取失败
 *
 * 值: 0x0200271b
 *
 * 解决方案: 当前节点为空，或者当前首节点的目标任务不是本任务。
 */
#define OS_ERRNO_QUEUE_ORDERQUE_READ_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x1b)

/*
 * @ingroup os_queue
 * 队列错误码：保序队列操作时硬线程ID非法
 *
 * 值: 0x0300271c
 *
 * 解决方案: 保序队列仅支持创建保序队列的硬线程接收或查询保序消息。
 */
#define OS_ERRNO_QUEUE_ORDERQUE_CORE_ID_INVALID OS_ERRNO_BUILD_FATAL(OS_MID_QUEUE, 0x1c)

/*
 * @ingroup os_queue
 * 队列错误码: 删除保序队列时，队列还有消息节点
 *
 * 值: 0x0200271d
 *
 * 解决方案: 删除保序队列前，确保消息节点已读取完
 */
#define OS_ERRNO_QUEUE_ORDERQUE_NOT_EMPTY OS_ERRNO_BUILD_ERROR(OS_MID_QUEUE, 0x1d)

/*
 * @ingroup os_queue
 * 队列优先级类型
 */
enum OsQueuePrio {
    OS_QUEUE_NORMAL = 0, /* 普通消息队列 */
    OS_QUEUE_URGENT,     /* 紧急消息队列 */
    OS_QUEUE_BUTT
};

/*
 * @ingroup os_queue
 * 队列等待时间设定：表示永久等待。
 */
#define OS_QUEUE_WAIT_FOREVER OS_WAIT_FOREVER

/*
 * @ingroup os_queue
 * 队列等待时间设定：表示不等待。
 */
#define OS_QUEUE_NO_WAIT 0

/*
 * @ingroup os_queue
 * 所有PID。
 */
#define OS_QUEUE_PID_ALL 0xFFFFFFFF

/*
 * @ingroup os_queue
 * 保序队列句柄。
 */
typedef uintptr_t OsOrderQueHandle;

/*
 * @ingroup os_queue
 * 保序队列消息内容长度，当前支持发送2个word大小。
 */
#define OS_ORDER_QUE_SIZE 2
/*
 * @ingroup os_queue
 * 保序队列消息内容结构体。
 */
struct OsFusionOrderMsg {
    U32 load[OS_ORDER_QUE_SIZE];
};

/*
 * @ingroup os_queue
 * 保序队列消息发送的option。
 */
struct OsFusionOrderQueOption {
    U32 urgent : 1;         /* 是否紧急消息的标志，0(OS_QUEUE_NORMAL)表示非紧急，1(OS_QUEUE_URGENT)表示紧急 */
    U32 reserved : 15;      /* 预留 */
    U32 data : 16;          /* 其它数据 */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* OS_QUEUE_H */
