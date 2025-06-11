/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 事件模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * *@defgroup SRE_event 事件
 * @ingroup SRE_comm
 */

#ifndef SRE_EVENT_H
#define SRE_EVENT_H

#include "sre_buildef.h"
#include "./common/os_typedef.h"
#include "./common/os_event.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

// 支持功能宏裁剪,sre_buildef.h文件中支持的功能宏
#if defined(OS_OPTION_EVENT)

/*
 * @ingroup SRE_event
 * @brief 读事件。
 *
 * @par 描述
 * 读取当前任务的指定掩码为eventMask的事件，可以一次性读取多个事件。事件读取成功后，被读取的事件将被清除。
 * @attention
 * <ul>
 * <li>读取模式可以选择读取任意事件和读取所有事件。</li>
 * <li>等待模式可以选择等待和不等待，可等待指定时间、永久等待。</li>
 * <li>当读取事件没有发生且为等待模式时，会发生任务调度，锁任务状态除外。</li>
 * <li>不能在IDLE任务中读事件，需要用户自行保证。</li>
 * <li>不能在系统初始化之前调用读事件接口。</li>
 * </ul>
 *
 * @param eventMask [IN]  类型#U32，设置要读取的事件掩码，每个bit位对应一个事件，1表示要读取。该入参不能为0。
 * @param flags     [IN]  类型#U32，读取事件所采取的策略, 为（OS_EVENT_ANY，OS_EVENT_ALL）中一个和（OS_EVENT_WAIT，
 * OS_EVENT_NOWAIT）中的一个标识或的结果。#OS_EVENT_ALL表示期望接收eventMask中的所有事件，
 * #OS_EVENT_ANY表示等待eventMask中的任何一个事件。#OS_EVENT_WAIT表示若期望事件没有发生，等待接收，
 * #OS_EVENT_NOWAIT表示若期望事件没有发生，将不会等待接收。
 * @param timeOut   [IN]  类型#U32，等待超时时间，单位为tick，取值(0~0xFFFFFFFF]。当flags标志为OS_EVENT_WAIT，
 * 这个参数才有效。若值为#OS_EVENT_WAIT_FOREVER，则表示永久等待。
 * @param events    [OUT] 类型#U32 *，用于保存接收到的事件的指针。如果不需要输出，可以填写NULL。
 *
 * @retval #OS_ERRNO_EVENT_READ_FAILED        0x02002600，事件读取失败，期望事件没有发生。
 * @retval #OS_ERRNO_EVENT_READ_TIMEOUT       0x02002601，读取事件超时。
 * @retval #OS_ERRNO_EVENT_EVENTMASK_INVALID  0x02002604，读事件时EVENTMASK入参非法，入参不能为0。
 * @retval #OS_ERRNO_EVENT_READ_NOT_IN_TASK   0x02002605，只能在任务中进行读事件操作。
 * @retval #OS_ERRNO_EVENT_FLAGS_INVALID      0x02002606，读事件接口中flags入参非法。
 * @retval #OS_ERRNO_EVENT_READ_IN_LOCK       0x02002607，在锁任务调度状态下，禁止任务阻塞于读事件。
 * @retval #SRE_OK                            0x00000000，操作成功。
 *
 * @par 依赖
 * <ul>
 * <li>sre_event.h：该接口声明所在的头文件。</li>
 * </ul>
 * @see SRE_EventWrite
 */
extern U32 SRE_EventRead(U32 eventMask, U32 flags, U32 timeOut, U32 *events);

/*
 * @ingroup SRE_event
 * @brief 写事件。
 *
 * @par 描述
 * 写任务ID为taskID的指定事件，可以一次性写多个事件，可以在LibCK接管的中断中调用。
 * @attention
 * <ul>
 * <li>若指定任务正在等待读取写入的事件，则会激活指定任务，并发生任务调度。</li>
 * <li>不能向IDLE任务写事件，需要用户自行保证。</li>
 * </ul>
 *
 * @param taskId [IN]  类型#U32，任务ID，表示要对某个任务进行写事件操作。
 * @param events [IN]  类型#U32，事件号，每个bit对应一个事件。
 *
 * @retval #OS_ERRNO_EVENT_TSK_NOT_CREATED     0x02002603，写事件时入参任务未创建。
 * @retval #OS_ERRNO_EVENT_TASKID_INVALID      0x02002602，写事件时入参任务ID非法。
 * @retval #OS_ERRNO_EVENT_INVALID             0x02002608，写事件时入参事件类型非法，不能为零。
 * @retval #SRE_OK                             0x00000000，操作成功。
 * @par 依赖
 * <ul>
 * <li>sre_event.h：该接口声明所在的头文件。</li>
 * </ul>
 * @see SRE_EventRead
 */
extern U32 SRE_EventWrite(U32 taskId, U32 events);

#if defined(OS_OPTION_VOS_IPC)
/*
 * @ingroup SRE_event
 * @brief 创建VOS事件。
 *
 * @par 描述
 * 创建VOS事件。
 *
 * @attention
 * <ul>
 * <li>只有经过创建VOS事件的任务才能读和被写 VOS事件，并且能被无条件唤醒。</li>
 * <li>底软封装层判断是否能创建VOS事件，创建的目标任务是否是合法OSA任务。</li>
 * </ul>
 *
 * @param taskId [IN]  类型#U32，任务ID，表示要对某个任务进行写事件操作。
 *
 * @retval #SRE_OK                             0x00000000，操作成功。
 * @par 依赖
 * <ul>
 * <li>sre_event.h：该接口声明所在的头文件。</li>
 * </ul>
 * @see SRE_EventRead
 */
extern U32 SRE_CreateVEvent(U32 taskId);

/*
 * @ingroup SRE_event
 * @brief 读VOS事件。
 *
 * @par 描述
 * 读VOS事件。
 *
 * @attention
 * <ul>
 * <li>容错判断与OSA相同，读出的事件类型可为0，为0时由底软封装为特殊返回值(处理消息)。</li>
 * </ul>
 *
 * @param events    [IN]  类型#U32，期望读取的事件，0表示只检查是否有事件，而不取事件。
 * @param flags     [IN]  类型#U32，读取事件的等待类型，目前只允许OS_EVENT_ANY。
 * @param timeOut   [IN]  类型#U32，读取事件超时时间，单位tick。OS_WAIT_FOREVER表示永久等待
 * @param retEvents [OUT] 类型#U32 *，读取到的事件。
 *
 * @retval #SRE_OK                             0x00000000，操作成功。
 * @par 依赖
 * <ul>
 * <li>sre_event.h：该接口声明所在的头文件。</li>
 * </ul>
 * @see SRE_EventRead
 */
extern U32 SRE_VEventRead(U32 events, U32 flags, U32 timeOut, U32 *retEvents);

/*
 * @ingroup SRE_event
 * @brief 写VOS事件。
 *
 * @par 描述
 * 写VOS事件。
 *
 * @attention
 * <ul>
 * <li>如果目标任务阻塞在VOS事件或消息上，会唤醒该任务。</li>
 * </ul>
 *
 * @param taskId [IN]  类型#U32，任务ID，表示要对某个任务进行写事件操作。
 * @param events [IN]  类型#U32，需要写入的事件。
 *
 * @retval #SRE_OK                             0x00000000，操作成功。
 * @par 依赖
 * <ul>
 * <li>sre_event.h：该接口声明所在的头文件。</li>
 * </ul>
 * @see SRE_EventRead
 */
extern U32 SRE_VEventWrite(U32 taskId, U32 events);
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_EVENT_H */
