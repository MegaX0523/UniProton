/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 队列模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_QUEUE_H
#define SRE_QUEUE_H

#include "sre_buildef.h"
#include "sre_typedef.h"
#include "./common/os_queue.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_queue
 * @brief 创建队列。
 *
 * @par 描述
 * 创建一个队列，创建时可以设定队列长度和队列结点大小。
 * @attention
 * <ul>
 * <li>每个队列节点的大小的单位是BYTE。</li>
 * <li>每个队列节点的长度自动做2字节对齐。</li>
 * <li>每个队列节点的长度不能大于0xFFFA。</li>
 * </ul>
 * @param nodeNum     [IN]  类型#U16，队列节点个数，不能为0。
 * @param maxNodeSize [IN]  类型#U16，每个队列结点的大小。
 * @param queueId     [OUT] 类型#U32 *，存储队列ID，ID从1开始。
 *
 * @retval #SRE_OK                           0x00000000，操作成功。
 * @retval #OS_ERRNO_QUEUE_CREAT_PTR_NULL    0x0200270b，队列创建时输入的指针为空。
 * @retval #OS_ERRNO_QUEUE_PARA_ZERO         0x0200270c，入参队列长度或者队列消息结点大小为0。
 * @retval #OS_ERRNO_QUEUE_CB_UNAVAILABLE    0x02002704，没有空闲的队列资源数，已经达到配置的最大队列数。
 * @retval #OS_ERRNO_QUEUE_CREATE_NO_MEMORY  0x02002703，队列创建时内存不足。
 * @retval #OS_ERRNO_QUEUE_NSIZE_INVALID     0x02002713,节点长度超过最大值。
 * @par 依赖
 * <ul><li>SRE_queue.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_QueueDelete
 */
extern U32 SRE_QueueCreate(U16 nodeNum, U16 maxNodeSize, U32 *queueId);

/*
 * @ingroup SRE_queue
 * @brief 读队列。
 *
 * @par 描述
 * 读取指定队列中的数据。将读取到的数据存入bufferAddr地址，bufferAddr地址和读取数据大小由用户传入。
 * @attention
 * <ul>
 * <li>队列读取才采用FIFO模式，即先入先出，读取队列中最早写入的数据(相对于队列节点是先后顺序)。</li>
 * <li>如果bufferSize大于队列中实际消息的大小，则只返回实际大小的数据，否则只读取bufferSize大小的数据。</li>
 * <li>bufferSize大小的单位是BYTE。</li>
 * <li>阻塞模式不能在idle钩子使用，需用户保证。</li>
 * <li>在osStart之前不能调用该接口，需用户保证。</li>
 * </ul>
 * @param queueId    [IN]  类型#U32，队列ID。
 * @param bufferAddr [OUT] 类型#void *，读取存放队列中数据的起始地址。
 * @param len        [I/O] 类型#U32 *，传入BUF的大小，输出实际消息的大小。
 * @param timeOut    [IN]  类型#U32，超时时间。
 *
 * @retval #SRE_OK                       0x00000000，操作成功。
 * @retval #OS_ERRNO_QUEUE_PTR_NULL      0x0200270e，读队列时输入指针为空。
 * @retval #OS_ERRNO_QUEUE_INVALID       0x0200270d，读队列句柄非法。
 * @retval #OS_ERRNO_QUEUE_SIZE_ZERO     0x0200270f，读队列时输入存放读取的数据buffer大小为0。
 * @retval #OS_ERRNO_QUEUE_NO_SOURCE     0x02002711, 读队列时队列为空。
 * @retval #OS_ERRNO_QUEUE_NOT_CREATE    0x02002709，要读取的队列还未创建。
 * @retval #OS_ERRNO_QUEUE_IN_INTERRUPT  0x02002708, 中断不能被阻塞。
 * @retval #OS_ERRNO_QUEUE_PEND_IN_LOCK  0x02002705，锁任务下不能被阻塞。
 * @retval #OS_ERRNO_QUEUE_TIMEOUT       0x02002706，队列超时。
 *
 * @par 依赖
 * <ul><li>SRE_queue.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_QueueWrite
 */
extern U32 SRE_QueueRead(U32 queueId, void *bufferAddr, U32 *len, U32 timeOut);

/*
 * @ingroup SRE_queue
 * @brief 写队列。
 *
 * @par 描述
 * 向指定队列写数据。将bufferAddr地址中bufferSize大小的数据写入到队列中。
 * @attention
 * <ul>
 * <li>需保证bufferSize大小小于或等于队列结点大小。</li>
 * <li>bufferSize大小的单位是BYTE。</li>
 * <li>阻塞模式不能在idle钩子使用，需用户保证。</li>
 * <li>在osStart之前不能调用该接口，需用户保证。</li>
 * </ul>
 * @param queueId    [IN]  类型#U32，队列ID。
 * @param bufferAddr [IN]  类型#void *，写到队列中数据的起始地址。
 * @param bufferSize [IN]  类型#U32，写到队列中数据的大小。
 * @param timeOut    [IN]  类型#U32，超时时间。
 * @param prio       [IN]  类型#U32，优先级, 取值OS_QUEUE_NORMAL或OS_QUEUE_URGENT。
 *
 * @retval #SRE_OK                               0x00000000，操作成功。
 * @retval #OS_ERRNO_QUEUE_PTR_NULL              0x0200270e，写队列时输入指针为空。
 * @retval #OS_ERRNO_QUEUE_INVALID               0x0200270d，写队列句柄非法。
 * @retval #OS_ERRNO_QUEUE_SIZE_TOO_BIG          0x02002710，写队列时，输入buffer的大小大于队列结点大小。
 * @retval #OS_ERRNO_QUEUE_SIZE_ZERO             0x0200270f，写队列时输入要写的数据大小为0。
 * @retval #OS_ERRNO_QUEUE_PRIO_INVALID          0x02002712，队列优先级错误。
 * @retval #OS_ERRNO_QUEUE_NOT_CREATE            0x02002709，队列没有创建。
 * @retval #OS_ERRNO_QUEUE_NO_SOURCE             0x02002711，队列满，没有可写的资源。
 * @retval #OS_ERRNO_QUEUE_IN_INTERRUPT          0x02002708，在中断被阻塞。
 * @retval #OS_ERRNO_QUEUE_PEND_IN_LOCK          0x02002705，锁任务下被阻塞。
 * @retval #OS_ERRNO_QUEUE_TIMEOUT               0x02002706，队列超时。
 *
 * @par 依赖
 * <ul><li>SRE_queue.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_QueueRead
 */
extern U32 SRE_QueueWrite(U32 queueId, void *bufferAddr, U32 bufferSize, U32 timeOut, U32 prio);

/*
 * @ingroup SRE_queue
 * @brief 删除队列。
 *
 * @par 描述
 * 删除一个消息队列。删除后队列资源被回收。
 * @attention
 * <ul>
 * <li>不能删除未创建的队列。</li>
 * <li>删除同步队列时，必须确保任务阻塞于该队列，且无被激活后还没及时操作队列的任务，否则删除队列失败。</li>
 * </ul>
 * @param queueId [IN]  类型#U32，队列ID。
 *
 * @retval #SRE_OK                       0x00000000，操作成功。
 * @retval #OS_ERRNO_QUEUE_INVALID       0x0200270d，未找到要删除的队列。
 * @retval #OS_ERRNO_QUEUE_IN_TSKUSE     0x02002707，有任务阻塞于该队列，队列不能删除。
 * @retval #OS_ERRNO_QUEUE_BUSY          0x0200270a，任务被激活但没有及时写队列。
 * @retval #OS_ERRNO_QUEUE_NOT_CREATE    0x02002709，要删除的队列未创建。
 *
 * @par 依赖
 * <ul><li>SRE_queue.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_QueueCreate
 */
extern U32 SRE_QueueDelete(U32 queueId);

/*
 * @ingroup SRE_queue
 * @brief 获取队列的历史最大使用长度。
 *
 * @par 描述
 * 获取从队列创建到删除前的历史最大使用长度。
 * @attention
 * <ul>
 * <li>峰值在队列删除前，不会被清零。</li>
 * </ul>
 * @param queueId      [IN]  类型#U32，队列ID
 * @param queueUsedNum [OUT] 类型#U32 *，队列节点使用峰值
 *
 * @retval #SRE_OK                         0x00000000，操作成功。
 * @retval #OS_ERRNO_QUEUE_INVALID         0x0200270d，未找到要删除的队列。
 * @retval #OS_ERRNO_QUEUE_PTR_NULL        0x0200270e，指针为空。
 * @retval #OS_ERRNO_QUEUE_NOT_CREATE      0x02002709，队列未创建。
 *
 * @par 依赖
 * <ul><li>SRE_queue.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_QueueNodeNumGet
 */
extern U32 SRE_QueueUsedPeak(U32 queueId, U32 *queueUsedNum);

/*
 * @ingroup SRE_queue
 * @brief 获取指定源PID的待处理消息个数。
 *
 * @par 描述
 * 从指定队列中，获取指定源PID的待处理消息个数。
 * @attention
 * <ul>
 * <li>PID为OS_QUEUE_PID_ALL时，表示获取所有待处理的消息个数 </li>
 * <li>PID的合法性不做判断，不合法的PID获取的消息个数为0</li>
 * </ul>
 * @param queueId [IN]  类型#U32，队列ID。
 * @param taskPid [IN]  类型#U32，线程PID。
 * @param num     [OUT] 类型#U32 *，待处理的消息个数。
 *
 * @retval #SRE_OK                         0x00000000，操作成功。
 * @retval #OS_ERRNO_QUEUE_INVALID         0x0200270d，未找到要删除的队列。
 * @retval #OS_ERRNO_QUEUE_PTR_NULL        0x0200270e，指针为空。
 * @retval #OS_ERRNO_QUEUE_NOT_CREATE      0x02002709，队列未创建。
 *
 * @par 依赖
 * <ul><li>SRE_queue.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_QueueUsedPeak
 */
extern U32 SRE_QueueNodeNumGet(U32 queueId, U32 taskPid, U32 *queueNum);

#if defined(OS_OPTION_QUEUE_NAME)
/*
 * @ingroup SRE_queue
 * @brief 支持名字的创建队列。
 *
 * @par 描述
 * 创建一个队列，创建时支持名字，可以设定队列长度和队列结点大小。
 * @attention
 * <ul>
 * <li>每个队列的节点个数不能超过0xFFFF。</li>
 * <li>每个队列节点的大小的单位是BYTE。</li>
 * <li>每个队列节点的长度自动做2字节对齐。</li>
 * <li>每个队列节点的长度不能大于0xFFFA。</li>
 * <li>当重名时，将返回错误码OS_ERRNO_QUEUE_NAME_REPEATED, queueId记录已创建的队列ID。</li>
 * </ul>
 * @param name        [IN]  类型#const char *，队列名字，最长支持15个字符，否则会截断。
 * @param nodeNum     [IN]  类型#U32，队列节点个数，不能为0。
 * @param maxNodeSize [IN]  类型#U32，每个队列结点的大小。
 * @param queueId     [OUT] 类型#U32 *，存储队列ID，ID从1开始。
 *
 * @retval #SRE_OK                           0x00000000，操作成功。
 * @retval #OS_ERRNO_QUEUE_CREAT_PTR_NULL    0x0200270b，队列创建时输入的指针为空。
 * @retval #OS_ERRNO_QUEUE_PARA_ZERO         0x0200270c，入参队列长度或者队列消息结点大小为0。
 * @retval #OS_ERRNO_QUEUE_CB_UNAVAILABLE    0x02002704，没有空闲的队列资源数，已经达到配置的最大队列数。
 * @retval #OS_ERRNO_QUEUE_CREATE_NO_MEMORY  0x02002703，队列创建时内存不足。
 * @retval #OS_ERRNO_QUEUE_NSIZE_INVALID     0x02002713, 节点长度超过最大值。
 * @retval #OS_ERRNO_QUEUE_NAME_REPEATED     0x02002714, 队列名字重名。
 * @par 依赖
 * <ul><li>SRE_queue.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_QueueDelete
 */
extern U32 SRE_QueueCreateWithName(const char *name, U32 nodeNum, U32 maxNodeSize, U32 *queueId);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_QUEUE_H */
