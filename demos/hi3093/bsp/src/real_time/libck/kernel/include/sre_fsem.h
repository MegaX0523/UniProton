/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 快速信号量模块的对外头文件。。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_FSEM_H
#define SRE_FSEM_H

#include "sre_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * @ingroup SRE_fsem
 * 被操作的任务的PID。
 */
typedef U32 TaskPid;

/*
 * @ingroup SRE_fsem
 * 快速信号量计数获取时，任务PID非法或者任务为IDLE任务。
 */
#define OS_FSEM_COUNT_ERR 0x80000000

/*
 * @ingroup SRE_fsem
 * 快速信号量计数获取时，任务未创建。
 */
#define OS_FSEM_TASK_NO_CREATE 0x80000001

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：用户输入的任务句柄非法（错误或未创建或为Idle任务）。
 *
 * 值: 0x02001000
 *
 * 解决方案：检查任务句柄是否为已创建。
 */
#define OS_ERRNO_FSEM_TASK_INVALID SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x00)

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：快速信号量计数值设定为负值。
 *
 * 值: 0x02001001
 *
 * 解决方案：快速信号量设定为非负值或PEND操作的等待量不小于1。
 */
#define OS_ERRNO_FSEM_SET_NEGATIVE SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x01)

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：Pend快速信号量时指定的等待量值非法。
 *
 * 值: 0x02001002
 *
 * 解决方案：确保Pend快速信号量时指定的等待量值在[1, 0x7FFFFFFF]范围内。
 */
#define OS_ERRNO_FSEM_PEND_VAL_INVALID SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x02)

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：快速信号量获取函数调用场景错误。
 *
 * 值: 0x02001003
 *
 * 解决方案：不要在中断里PEND快速信号量。
 */
#define OS_ERRNO_FSEM_PEND_INTERR SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x03)

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：任务切换锁定时，禁止任务阻塞于快速信号量。
 *
 * 值: 0x02001004
 *
 * 解决方案：不要在锁任务后PEND快速信号量。
 */
#define OS_ERRNO_FSEM_PEND_IN_LOCK SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x04)

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：POST快速信号量时，计数正值溢出（正数变负数）；
 * 若是post核间快速信号量，此错误检测在接收方进行。
 *
 * 值: 0x02001005
 *
 * 解决方案：对快速信号量进行PEND操作。
 */
#define OS_ERRNO_FSEM_OVERFLOW SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x05)

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：任务阻塞于快速信号量。
 *
 * 值: 0x02001006
 *
 * 解决方案：设置快速信号量初值时，确保任务没有被阻塞于快速信号量。
 */
#define OS_ERRNO_FSEM_PENDED SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x06)

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：核间快速信号量post失败，接收端任务不存在，
 * 此错误检测在接收方进行。
 *
 * 值: 0x02001007
 *
 * 解决方案: 查看接收端任务是否有效。
 */
#define OS_ERRNO_FSEM_MCPOST_FAIL SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x07)

/*
 * @ingroup SRE_fsem
 * 快速信号量错误码：快速信号量post失败，试图post非本核任务，却未打开核间快速信号量模块配置项开关。
 *
 * 值: 0x02001008
 *
 * 解决方案: 检查入参是否和预期相吻合，若吻合，请检查快速信号量模块配置开关是否打开。
 */
#define OS_ERRNO_FSEM_POST_FAIL SRE_ERRNO_OS_ERROR(OS_MID_FSEM, 0x08)

/*
 * @ingroup SRE_fsem
 * @brief 设置快速信号量初值。
 *
 * @par 描述
 * 设置指定任务快速信号量初值，如果此任务正为快速信号量阻塞着，则操作失败。
 * @attention
 * <ul>
 * <li>多核环境下，只能对本核任务进行此操作。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TaskPid，任务PID，来源于通过Hunt模块接口获取到的任务的PID。
 * @param fsemVal [IN]  类型#S32，设定的快速信号量数值，取值范围为[0, 0x7FFFFFFF]。
 *
 * @retval #OS_ERRNO_FSEM_TASK_INVALID 0x02001000，任务句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_FSEM_SET_NEGATIVE 0x02001001，设定初值超出规定范围。
 * @retval #OS_ERRNO_FSEM_PENDED       0x02001005，任务正在阻塞于该信号量，禁止重设。
 * @retval #SRE_OK                     0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_fsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_FsemGet
 */
extern U32 SRE_FsemSet(TaskPid taskPid, S32 fsemVal);

/*
 * @ingroup SRE_fsem
 * @brief 获取快速信号量计数器数值。
 *
 * @par 描述
 * 查询指定任务的快速信号量计数值，如果为负表示当前该任务阻塞于快速信号量。
 * @attention
 * <ul>
 * <li>多核环境下，只能对本核任务进行此操作。</li>
 * <li>快速信号量默认初始值为0。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TaskPid，任务PID，来源于Hunt模块获取到的任务的PID。
 *
 * @retval #OS_FSEM_COUNT_ERR      0x80000000，获取失败，原因是快速信号量计数获取时，任务PID非法或者任务为IDLE任务。
 * @retval #OS_FSEM_TASK_NO_CREATE 0x80000001，获取失败，原因是快速信号量计数获取时，任务未创建。
 * @retval 其他值                  (0x80000001~0x07FFFFFFF]，快速信号量计数器值。
 * @par 依赖
 * <ul><li>sre_fsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_FsemSet
 */
extern S32 SRE_FsemGet(TaskPid taskPid);

/*
 * @ingroup SRE_fsem
 * @brief 等待快速信号量。
 *
 * @par 描述
 * 等待任务自己的快速信号量，将计数减去输入的等待量。若结果为负数，则发生阻塞。
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>该接口只能在任务中调用。</li>
 * <li>快速信号量初始值默认为0。</li>
 * </ul>
 *
 * @param fsemVal [IN]  类型#S32，快速信号量的等待量。取值范围为[1, 0x7FFFFFFF]。
 *
 * @retval #OS_ERRNO_FSEM_PEND_VAL_INVALID 0x02001002，等待量非法，超出规定范围。
 * @retval #OS_ERRNO_FSEM_PEND_INTERR      0x02001003，该接口禁止中断处理函数调用。
 * @retval #OS_ERRNO_FSEM_PEND_IN_LOCK     0x02001004，任务切换锁定时，禁止任务发生阻塞。
 * @retval #SRE_OK                         0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_fsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_FsemPost
 */
extern U32 SRE_FsemPend(S32 fsemVal);

/*
 * @ingroup SRE_fsem
 * @brief 发布指定任务快速信号量。
 *
 * @par 描述
 * 将指定任务快速信号量值加1，如果计数从负数变为0则唤醒该任务。
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>如果唤醒的任务非本核，则会触发核间中断，发送唤醒任务的操作。</li>
 * <li>如果唤醒的任务在本核且高于当前任务，则会立刻发生任务切换。</li>
 * <li>该接口能调用于任何类型线程（软中断、硬中断、任务）。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TaskPid，任务PID，来源于Hunt模块获取到的任务的PID。
 *
 * @retval #OS_ERRNO_FSEM_TASK_INVALID 0x02001000，任务句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_FSEM_OVERFLOW     0x02001005，快速信号量加操作后溢出。
 * @retval #OS_ERRNO_FSEM_POST_FAIL    0x02001008，快速信号量post失败，试图post非本核任务，
 * @retval                                         却未打开核间快速信号量模块配置项开关。
 * @retval #OS_ERRNO_IPC_SHQ_FULL      0x02001486，6181和6108平台下，发送核间中断，共享队列已满。
 * @retval #SRE_OK                     0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_fsem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_FsemPend
 */
extern U32 SRE_FsemPost(TaskPid taskPid);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* SRE_FSEM_H */
