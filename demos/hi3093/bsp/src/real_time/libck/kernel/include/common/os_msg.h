/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: msg模块的基础头文件, 用户不能直接包含。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_MSG_H
#define OS_MSG_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup os_msg
 * 消息错误码：传入的消息体句柄非法（错误或已删除或句柄指针为NULL），或者消息体正在被使用
 *
 * 值: 0x02000d00
 *
 * 解决方案: 检查该消息体是否只发送一次或者是否正在使用中；检查传入的消息体指针是否正确。
 */
#define OS_ERRNO_MSG_HANDLE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x00)

/*
 * @ingroup os_msg
 * 消息错误码：收发消息时线程PID非法（线程非法、未创建或已删除，目的任务消息队列非法）。
 * 注意:若错误信息从黑匣子中导出，则为核间通信时，目的核获取到错误信息。
 *
 * 值: 0x02000d01
 *
 * 解决方案: 检查接口传入的参数是否正确。
 */
#define OS_ERRNO_MSG_PID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x01)

/*
 * @ingroup os_msg
 * 消息错误码：核内消息模块初始化过程中，从0号分区申请内存失败
 *
 * 值: 0x02000d02
 *
 * 解决方案: 增加0号分区大小。
 *
 */
#define OS_ERRNO_MSG_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x02)

/*
 * @ingroup os_msg
 * 消息错误码：注册核内消息失败，核内消息节点的个数被配置为了0
 *
 * 值: 0x02000d03
 *
 * 解决方案: 若使用核内消息通信功能，请配置大于0的核内消息节点数
 *
 */
#define OS_ERRNO_MSG_REG_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x03)

/*
 * @ingroup os_msg
 * 消息错误码：发送消息时无空闲消息节点，可增加系统支持的最大消息节点数，
 * 注意:MsgSend和MsgRecv配对使用，以确保使用后的消息节点被释放
 *
 * 值: 0x02000d04
 *
 * 解决方案: 排查本核是否有短消息，不带消息体消息未接收，若已接收，则可增加系统支持的最大消息节点数
 *
 */
#define OS_ERRNO_MSG_NO_MSGNODE OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x04)

/*
 * @ingroup os_msg
 * 消息错误码：任务消息队列号非法，大于该任务线程的最大消息队列号。
 *
 * 值: 0x02000d06
 *
 * 解决方案:确认传入的消息队列号是否合法。
 *
 */
#define OS_ERRNO_MSG_QID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x06)

/*
 * @ingroup os_msg
 * 消息错误码：接收线程非法，在硬中断中接收
 *
 * 值: 0x02000d07
 *
 * 解决方案: 请确保不要在硬中断线程中接收消息。
 *
 */
#define OS_ERRNO_MSG_THREAD_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x07)

/*
 * @ingroup os_msg
 * 消息错误码：消息接收超时，在用户设置的超时时间内没有接收到消息。
 *
 * 值: 0x02000d08
 *
 * 解决方案: 确保超时时间内指定队列上有消息发送过来，如延时时间不够，可增大延时。
 *
 */
#define OS_ERRNO_MSG_RECEIVE_TIMEOUT OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x08)

/*
 * @ingroup os_msg
 * 消息错误码：发送核间消息时，目的PID非法。
 *
 * 值: 0x02000d09
 *
 * 解决方案: 确保消息发送接口的目的PID是否正确。
 *
 */
#define OS_ERRNO_MCMSG_PID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x09)

/*
 * @ingroup os_msg
 * 消息错误码：接收队列中无消息或在IDLE线程下接收消息为空。
 *
 * 值: 0x02000d0a
 *
 * 解决方案: 请检查是否向该消息队列中发送了消息。
 *
 */
#define OS_ERRNO_MSG_QUEUE_EMPTY OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x0a)

/*
 * @ingroup os_msg
 * 消息错误码：发送核间消息时，核间消息模块未打开。
 *
 * 值: 0x02000d0b
 *
 * 解决方案: 请检查核间消息模块是否已经打开。
 *
 */
#define OS_ERRNO_MCMSG_HOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x0b)

/*
 * @ingroup os_msg
 * 消息错误码：释放消息体内存时，传入的句柄地址未32字节对齐。
 *
 * 值: 0x02000d0c
 *
 * 解决方案: 请检查传入的消息体句柄值是否有效。
 *
 */
#define OS_ERRNO_MSG_HANDLE_ADDR_UNALIGN OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x0c)

/*
 * @ingroup os_msg
 * 消息错误码：短消息未接收处理，被强制释放, 或者短消息被重复释放(例如使用MSGM消息组装后就已经释放了短消息)
 *
 * 值: 0x02000d0d
 *
 * 解决方案: 请确保该消息被接收处理后再释放，以免造成内存泄漏及不能重复释放已经释放的消息。
 *
 */
#define OS_ERRNO_MSG_SMSG_CAN_NOT_DELETE OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x0d)

/*
 * @ingroup os_msg
 * 消息错误码：申请消息体，输入的消息体内存大小参数为零
 *
 * 值: 0x02000d0e
 *
 * 解决方案: 请确保传入的消息体内存大小是否有效。
 *
 */
#define OS_ERRNO_MSG_MEM_SIZE_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x0e)

/*
 * @ingroup os_msg
 * 消息错误码：申请消息体，从指定分区中申请不到足够大小的消息体内存
 *
 * 值: 0x02000d0f
 *
 * 解决方案: 检查传入的分区号是否有效，若有效则减小申请的消息体内存或者扩大指定的分区内存大小。
 *
 */
#define OS_ERRNO_MSG_MEM_ALLOC_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x0f)

/*
 * @ingroup os_msg
 * 消息错误码：长消息未接收处理，被强制释放消息体内存；或者长消息体被重复释放。
 *
 * 值: 0x02000d10
 *
 * 解决方案: 请确保该消息被接收处理后再释放，以免造成内存泄漏；或者不要释放已经被释放的消息体。
 *
 */
#define OS_ERRNO_MSG_LMSG_CAN_NOT_DELETE OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x10)

/*
 * @ingroup os_msg
 * 消息错误码：获取本核短消息体长度值失败
 *
 * 值: 0x02000d11
 *
 * 解决方案: 在释放该短消息前，输入合理的消息句柄值以获取正确的短消息体长度
 *
 */
#define OS_ERRNO_MSG_GET_SMSG_LENGTH_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x11)

/*
 * @ingroup os_msg
 * 消息错误码：获取本核长消息体长度值失败
 *
 * 值: 0x02000d12
 *
 * 解决方案: 在释放该长消息前，输入合理的消息句柄值以获取正确的长消息体长度
 *
 */
#define OS_ERRNO_MSG_GET_LMSG_LENGTH_FAIL OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x12)

/*
 * @ingroup os_msg
 * 消息错误码：消息接收时，接收到消息类型为长消息，但长消息魔术字校验错误。
 *
 * 值: 0x02000d13
 *
 * 解决方案: 检查使用SRE_QportSend发送OS消息时，消息是否由SRE_QportMsgCreate函数创建。
 *
 */
#define OS_ERRNO_MSG_LONGMSG_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x13)

/*
 * @ingroup os_msg
 * 消息错误码：传入的消息句柄未4字节对齐
 *
 * 值: 0x02000d14
 *
 * 解决方案: 检查传入的消息句柄，修改为4字节对齐。
 *
 */
#define OS_ERRNO_MSG_HANDLE_NOT_WORD_ALIGN OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x14)

/*
 * @ingroup os_msg
 * 消息错误码：传入的消息白名单非法或者入参结构指针为空
 *
 * 值: 0x02000d15
 *
 * 解决方案: 检查传入的消息白名单列表的正确性以及入参结构指针不能为空
 *
 */
#define OS_ERRNO_MSG_CORELIST_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x15)

/*
 * @ingroup os_msg
 * 消息错误码：发送核间消息时线程PID非法。
 *
 * 值: 0x02000d16
 *
 * 解决方案: 检查发送接口传入的参数是否正确或检查消息白名单列表。
 */
#define OS_ERRNO_MSG_PID_ILLEGAL OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x16)

/*
 * @ingroup os_msg
 * 消息错误码：获取指定任务指定队列的消息数时， 任务控制块指针为空。
 *
 * 值: 0x02000d17
 *
 * 解决方案: 请确认任务是否被裁剪。
 */
#define OS_ERRNO_MSG_TASK_CTRL_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x17)

/*
 * @ingroup os_msg
 * 消息错误码：VOS PID与OS Task ID转换关系钩子函数或VOS PID优先级获取钩子函数为空。
 *
 * 值: 0x02000d18
 *
 * 解决方案: 请确认对应钩子函数是否已注册。
 */
#define OS_ERRNO_MSG_VOS_HOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x18)

/*
 * @ingroup os_msg
 * 消息错误码：接收任务消息时，任务消息钩子为空
 *
 * 值: 0x02000d19
 *
 * 解决方案: 请确认已经注册任务接收消息钩子
 */
#define OS_ERRNO_MSG_TASK_RX_HOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x19)

/*
 * @ingroup os_msg
 * 消息错误码：注册核间消息失败，核间消息节点的个数被配置为了0
 *
 * 值: 0x02000d1a
 *
 * 解决方案: 若使用核间消息通信功能，请配置大于0的核间消息节点数
 */
#define OS_ERRNO_MCMSG_REG_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x1a)

/*
 * @ingroup os_msg
 * 消息错误码：核间消息模块初始化过程中，申请共享静态内存失败
 *
 * 值: 0x02000d1b
 *
 * 解决方案: 为共享静态内存配置更大内存空间
 */
#define OS_ERRNO_MCMSG_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x1b)

/*
 * @ingroup os_msg
 * 消息错误码：核间消息忙碌
 *
 * 值: 0x02000d1c
 *
 * 解决方案: 延时发送核间消息
 */
#define OS_ERRNO_MCMSG_BUSY OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x1c)
/*
 * @ingroup os_msg
 * 消息错误码：消息体的flag非法
 *
 * 值: 0x02000d1d
 *
 * 解决方案: 检查消息体的flag魔术字是否正确
 */
#define OS_ERRNO_MSG_HANDLE_FLAG_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x1d)
/*
 * @ingroup os_msg
 * 消息错误码：消息接口的输入存在NULL指针
 *
 * 值: 0x02000d1f
 *
 * 解决方案: 检查输入
 */
#define OS_ERRNO_MSG_INPUT_NULL OS_ERRNO_BUILD_ERROR(OS_MID_MSG, 0x1f)

/*
 * @ingroup os_msg
 * 消息体句柄类型定义。
 */
typedef uintptr_t MsgHandle;

/*
 * @ingroup os_msg
 * 消息发送者或接收者的线程PID类型定义。
 */
typedef U32 MsgPid;

/*
 * @ingroup os_msg
 * 消息类型。
 */
enum KmsgType {
    MSG_URGENT = 0,   /* 是否紧急消息，紧急消息添加表头优先处理 */
    MSG_NOWAKEUP      /* 是否需要唤醒对端核或者DSS             */
};
/*
 * @ingroup os_msg
 * 消息的类型。
 *
 * 消息是紧急消息,值：0x1。
 */
#define OS_KMSG_URGENT  (1U << MSG_URGENT)

/*
 * @ingroup os_msg
 * 消息的类型。
 *
 * 消息不需要唤醒对端核或者DSS,值：0x2。
 */
#define OS_KMSG_NOWAKE  (1U << MSG_NOWAKEUP)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* OS_MSG_H */
