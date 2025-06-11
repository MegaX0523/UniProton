/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 消息模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * @defgroup SRE_msg 消息
 * @ingroup SRE_comm
 */

#ifndef SRE_MSG_H
#define SRE_MSG_H

#include "sre_base.h"
#include "sre_typedef.h"
#include "./common/os_msg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_msg
 * 无消息体发送消息时，第一个参数传入该值。
 */
#define OS_MSG_NO_PAYLOAD 0

/*
 * @ingroup SRE_msg
 * 使用于接收消息接口，表示消息的发送者为硬中断。
 */
#define OS_MSG_HWI_SENDER 0x000000FF

/*
 * @ingroup SRE_msg
 * 消息错误码：申请消息体失败，输入alignPow入参非法，输入的目的PID非法，分区内存已用尽或者本核消息节点资源不足
 *
 * 值: 0x00000000
 *
 * 解决方案:检查申请消息体接口入参是否合法，指定的分区内存是否充足等。
 */
#define OS_MSG_CREATE_ERR 0

/*
 * @ingroup SRE_msg
 * 消息错误码：消息接收函数中接收消息失败，
 * 可能原因是在硬中断中接收消息、在无消息的软中断中接收消息、任务中接收消息超时、或者正处于锁任务时进行超时等待。
 *
 * 值: 0xffffffff
 *
 * 解决方案:检查接收消息的线程类型，硬中断中不允许接收消息；确保在任务中超时等待消息时，系统不处于锁任务状态。
 */
#define OS_MSG_RECEIVE_ERR 0xFFFFFFFF

/*
 * @ingroup SRE_msg
 * 消息错误码：查询指定任务的指定消息队列失败。
 *
 * 值: 0xffffffff
 *
 * 解决方案: 只能查询本核已创建的任务；系统的idle任务不支持查询；只能查看任务的指定队列，且不能大于最大队列号。
 */
#define OS_MSGQ_NUM_GET_ERR 0xFFFFFFFF

/*
 * @ingroup SRE_msg
 * 消息错误码：消息长度获取函数(SRE_MsgLength)的错误返回值,消息句柄非法。
 *
 * 值: 0x00000000
 *
 * 解决方案: 检查传入的消息体指针是否正确。
 */
#define OS_MSG_LENGTH_ERR 0x00000000

/*
 * @ingroup SRE_msg
 * @brief 消息体的申请函数。
 *
 * @par 描述
 * 申请指定大小的消息体内存。
 * @attention
 * <ul><li>该接口与消息体释放接口#SRE_MsgFree配对使用，发送端申请，接收端释放。</li></ul>
 * <ul><li>如果消息只需要传递消息ID号，则可省略申请、释放消息体步骤。核内消息通信可以是私有内存分区</li></ul>
 * <ul><li>或共享内存分区，但建议使用私有内存以提高效率；核间消息通信必须是共享内存分区。</li></ul>
 * <ul><li>共享和私有pool算法实际消耗：用户对齐字节数（至少32对齐）+</li></ul>
 * <ul><li>内存分配消耗32或64字节（mcpool算法管理可cache分区时消耗64字节，其他为32字节） + 实际用户size。</li></ul>
 * <ul><li>共享和私有fsc算法最坏消耗：用户对齐字节数（至少32对齐）+ 内存分配消耗44字节 + 实际用户size。</li></ul>
 *
 * @param ptNo     [IN]  类型#U32，分区号，低8字节有效(若超过0xff截取为低8字节)，用户需保证分区号不超过255。
 * @param size     [IN]  类型#U16，需要申请的消息体大小（单位Byte）。
 * @param alignPow [IN]  类型#U32，消息体内存的对齐方式。取值范围为[0, 8]。采用2的幂次方形式表示，比如需要64对齐，
 * 则该参数应为6（2的6次方为64）。当传入的数据alignPow小于5时均按32字节对齐。
 *
 * @retval #OS_MSG_CREATE_ERR 0x00000000，申请失败，失败原因很可能是对齐方式大于8、无效分区、分区或消息节点耗尽等。
 * @retval 非0值              表示申请到的消息体句柄。
 *
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MsgFree
 */
extern MsgHandle SRE_MsgAlloc(U32 ptNo, U16 size, U32 alignPow);

/*
 * @ingroup SRE_msg
 * @brief 消息释放函数
 *
 * @par 描述
 * 消息体释放函数，用在消息体成功接收并使用完成之后或者发送失败之后。
 * @attention
 * <ul><li>此函数需要与接收消息体的#SRE_MsgQRecv或者#SRE_MsgReceive成对使用，先接收然后释放。</li></ul>
 * <ul><li>消息的接收和发送需准确配对，这样才能保证消息体的正常释放。</li></ul>
 * <ul><li>无消息体的消息，不需要调用此函数释放消息体。</li></ul>
 * <ul><li>该函数入参正确性由用户保证，OS只对是否为0地址进行判断。</li></ul>
 *
 * @param msgHandle [IN]  类型#MsgHandle，消息体句柄。
 *
 * @retval #SRE_OK                                       0x00000000，操作成功。
 * @retval #OS_ERRNO_MSG_HANDLE_INVALID                  0x02000d00，消息体句柄为非法值或者消息体正在被使用。
 * @retval #OS_ERRNO_MSG_SMSG_CAN_NOT_DELETE             0x02000d0d，短消息不能被删除，可能未被接收处理或者未被使用。
 * @retval #OS_ERRNO_MSG_HANDLE_ADDR_UNALIGN             0x02000d0c，输入的消息体句柄地址值未32字节对齐。
 * @retval #OS_ERRNO_MSG_LMSG_CAN_NOT_DELETE             0x02000d10，长消息不能被删除，可能未被接收处理或者未被使用。
 * @retval #OS_ERRNO_MEM_FREE_PTNO_INVALID               0x02000104，释放内存所在的分区无效
 * @retval                                                           (分区未创建或分区号大于分区数)。
 * @retval #OS_ERRNO_MEM_FREE_SH_DAMAGED                 0x02000311，内存释放时要释放的内存块的头被破坏，或已释放。
 * @retval #OS_ERRNO_MEM_FREE_POOL_MEM_ALREADY_FREE      0x0200013e，私有POOL内存释放时内存块已经被释放
 * @retval                                                           (或者内存没有被申请过)。
 * @retval #OS_ERRNO_MEM_FREE_SHARE_MEM_ALREADY_FREE     0x02000143，共享动态POOL内存释放时内存块已经被释放
 * @retval                                                           (或者内存没有被申请过)。
 * @retval #OS_ERRNO_MSG_HANDLE_NOT_WORD_ALIGN           0x02000d14，传入的消息句柄未4字节对齐。
 * @retval #OS_ERRNO_MEM_FREE_ADDR_INVALID               0x02000103，释放地址为空。
 * @par 依赖
 * <ul><li>li SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MsgAlloc
 */
extern U32 SRE_MsgDelete(MsgHandle msgHandle);

/*
 * @ingroup SRE_msg
 * @brief 释放创建成功的消息体。
 *
 * @par 描述
 * 释放创建成功的消息体。
 *
 * @attention
 * <ul><li>无</li></ul>
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MsgDelete
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_MsgFree(MsgHandle handle)
{
    return SRE_MsgDelete(handle);
}

/*
 * @ingroup SRE_msg
 * @brief 释放接收成功的消息体
 *
 * @par 描述
 * 释放接收成功的消息体。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @par 依赖
 * @li SRE_msg.h：该接口声明所在的头文件。
 * @see SRE_MsgDelete
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_MsgRelease(MsgHandle handle)
{
    return SRE_MsgDelete(handle);
}

/*
 * @ingroup SRE_msg
 * @brief 向指定任务或软中断的指定队列发消息。
 *
 * @par 描述
 * 发送消息到指定的任务的指定队列，支持核内和核间消息的发送。
 * 发送消息到软中断的0队列，支持核内和核间消息的发送。
 * 当任务因为接收该队列的消息而阻塞，则会唤醒该任务。
 * @attention
 * <ul><li>目的线程只能是任务和软中断，不能是硬中断。目的线程请勿是系统占用的线程，比如IDLE任务。</li></ul>
 * <ul><li>如果目的任务正在为接收该队列的消息而阻塞，则会唤醒该任务。</li></ul>
 * <ul><li>若目的任务优先级高于其所在核中正在运行的任务，则会发生任务切换。</li></ul>
 * <ul><li>如果目的线程是软中断，则直接激活该软中断，使其处于就绪态。</li></ul>
 * <ul><li>消息可以不包含消息体，若发送不带消息体消息，则MsgHandle入参为OS_MSG_NO_PAYLOAD，</li></ul>
 * <ul><li>若需要附带消息体，则消息体应来源于#SRE_MsgAlloc创建的消息体或#SRE_MsgRecv接收的其他消息体。</li></ul>
 * <ul><li>其中带消息体发送，SD6182又有长短消息之分，用户传入数据不大于8字节，</li></ul>
 * <ul><li>OS内部走短消息流程以提高通信效率。反之，OS内部走长消息流程，与SD6181/SD6108相似。</li></ul>
 * <ul><li>核间发送时，用户需要自行保证目的PID的真实存在，以免造成内存泄漏。</li></ul>
 * <ul><li>通过MSGM发送的消息只支持发送到目的端线程的0号消息队列。</li></ul>
 * <ul><li>对于长短消息，发送者SRE_MsgAlloc申请消息节点及SRE_MsgQSend/SRE_MsgSend发送消息后，</li></ul>
 * <ul><li>发送者不能SRE_MsgFree消息节点(除发送消息返回错误外)。</li></ul>
 *
 * @param msgHandle [IN]  类型#MsgHandle，消息体句柄，来源于消息体创建成功或接收成功的输出值。
 * @param msgId     [IN]  类型#U32，消息ID，用以标识该消息的唯一性。
 * @param dstPid    [IN]  类型#MsgPid，消息目的线程PID。可以是核内或核间的任务或软中断的PID。
 * @param dstQid    [IN]  类型#U8，消息队列号(发送给软中断，此参数无效，但参数值应小于8，与任务保持一致)，
 * 任务的最大消息队列数是8，所以最大的队列号是7。
 *
 * @retval #SRE_OK                       0x00000000，操作成功。
 * @retval #OS_ERRNO_MSG_HANDLE_INVALID  0x02000d00，消息体句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_MSG_PID_INVALID     0x02000d01，线程PID为非法值，或线程已被删除。
 * @retval #OS_ERRNO_MSG_PID_ILLEGAL     0x02000d16，发送核间消息时线程PID非法。
 * @retval #OS_ERRNO_MSG_NO_MSGNODE      0x02000d04，无空闲消息节点。
 * @retval #OS_ERRNO_MSG_QID_INVALID     0x02000d06，向任务发消息时指定的接收消息的任务的Qid非法。
 * @retval #OS_ERRNO_IPC_SHQ_FULL        0x02001486，SD6181和SD6108平台下，发送核间中断，共享队列已满。
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MsgQRecv
 */
extern U32 SRE_MsgQSend(MsgHandle msgHandle, U32 msgId, MsgPid dstPid, U8 dstQid);

/*
 * @ingroup SRE_msg
 * @brief 从任务或软中断的消息队列中获取消息。
 *
 * @par 描述
 * 从任务自身的指定的消息队列中获取第一条消息，消息的接收采取先到先收的顺序进行。
 * 从软中断的0号消息队列中获取第一条消息，消息的接收采取先到先收的顺序进行。
 * 除SD6155外其他平台都支持。
 * @attention
 * <ul><li>软中断不支持多消息队列，故在软中断中调用此接口时，recvQID参数无效，默认从0号消息队列接收消息。</li></ul>
 * <ul><li>软中断不支持超时等待，故在软中断中调用此接口时，timeout参数无效，若接收失败则直接返回不做等待。</li></ul>
 * <ul><li>有消息体消息，接收成功后可使用*msgHandle句柄获取消息体首地址。</li></ul>
 * <ul><li>无消息体的消息的发送与接收依赖于消息节点作为中介，发送过程会获取一个消息节点，</li></ul>
 * <ul><li>接收过程释放这个消息节点。系统可以使用的消息节点最大数量受配置项中核内、核间消息节点数限制。</li></ul>
 *
 * @param msgHandle [OUT] 类型#MsgHandle *，对于有消息体消息，msgHandle为返回的消息体首地址，若不需要此项输出，
 * 填写NULL即可。
 * @param msgId     [OUT] 类型#U32 *，接收到的消息ID。若不需要此项输出，填写NULL即可。
 * @param senderPid [OUT] 类型#MsgPid *，消息发送者的PID，为#OS_MSG_HWI_SENDER时表示发送者为硬中断。若不需要此项输出，
 * 填写NULL即可。
 * @param recvQid   [IN]  类型#U8，任务的指定接收消息队列号；软中断线程接收时，此参数无效，若接收核间消息，
 * 接收消息队列号只能为0。
 * @param timeout   [IN]  类型#U32，任务接收消息的等待时间限制，单位为tick，
 * 取值范围为[0, 0xfffffffe]或者OS_WAIT_FOREVER；软中断接收消息时，此参数无效。
 *
 * @retval #SRE_OK                       0x00000000，表示接收消息成功。
 * @retval #OS_ERRNO_MSG_QID_INVALID     0x02000d06，任务的消息队列ID非法。
 * @retval #OS_ERRNO_MSG_THREAD_INVALID  0x02000d07，接收线程非法，不支持在硬中断中接收消息。
 * @retval #OS_ERRNO_MSG_RECEIVE_TIMEOUT 0x02000d08，消息接收超时，在用户设置的超时时间内没有接收到消息。
 * @retval #OS_ERRNO_MSG_QUEUE_EMPTY     0x02000d0a，该消息队列中无消息。
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li><ul>
 * @see SRE_MsgQSend
 */
extern U32 SRE_MsgQRecv(MsgHandle *msgHandle, U32 *msgId, MsgPid *senderPid, U8 recvQid, U32 timeout);

/*
 * @ingroup SRE_msg
 * @brief 获取任务的指定消息队列的消息数。
 *
 * @par 描述
 * 查询的任务的指定队列，返回该队列的消息数。
 * @attention
 * <ul><li>当任务的PID非法(不能为IDLE任务的PID、软中断、硬中断)、指定任务的消息队列ID非法，</li></ul>
 * <ul><li>或者任务控制块指针为空时，将返回消息个数0。</li></ul>
 *
 * @param taskId [IN]  类型#U32，非Cortex RX平台只能是本核内的任务的ID，Cortex RX平台不分核。
 * @param qId    [IN]  类型#U8，指定任务的消息队列号。
 *
 * @retval               指定任务的指定消息队列的消息数目。
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MsgQRecv|SRE_MsgQSend
 */
extern U32 SRE_MsgQNumGet(U32 taskId, U8 qId);

/*
 * @ingroup SRE_msg
 * @brief 消息发送函数。
 *
 * @par 描述
 * 默认发送到目的端线程的0号消息队列，其它功能及注意事项描述见SRE_MsgQSend
 * @attention
 * <ul><li>无</li></ul>
 * @param msgHandle [IN]  类型#MsgHandle，消息体句柄，来源于消息体创建成功或接收成功的输出值。
 * @param msgId     [IN]  类型#U32，消息ID，用以标识该消息的唯一性。
 * @param dstPid    [IN]  类型#MsgPid，消息目的线程PID。可以是核内或核间的任务或软中断的PID。
 * @retval #SRE_OK                       0x00000000，操作成功。
 * @retval #OS_ERRNO_MSG_HANDLE_INVALID  0x02000d00，消息体句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_MSG_PID_INVALID     0x02000d01，线程PID为非法值，或线程已被删除。
 * @retval #OS_ERRNO_MSG_PID_ILLEGAL     0x02000d16，发送核间消息时线程PID非法。
 * @retval #OS_ERRNO_MSG_NO_MSGNODE      0x02000d04，无空闲消息节点。
 * @retval #OS_ERRNO_MSG_QID_INVALID     0x02000d06，向任务发消息时指定的接收消息的任务的Qid非法。
 * @retval #OS_ERRNO_IPC_SHQ_FULL        0x02001486，SD6181和SD6108平台下，发送核间中断，共享队列已满。
 * @par 依赖
 * @li SRE_msg.h：该接口声明所在的头文件。
 * @see SRE_MsgQSend
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_MsgSend(MsgHandle msgHandle, U32 msgId, MsgPid dstPid)
{
    return SRE_MsgQSend(msgHandle, msgId, dstPid, 0);
}

/*
 * @ingroup SRE_msg
 * @brief 消息获取函数。
 *
 * @par 描述
 * 固定从线程的0号消息队列接收消息，若带消息体消息接收成功，则返回消息体句柄msgHandle。其它功能描述见SRE_MsgQRecv。
 * @attention
 * <ul><li>当接收的是无消息体消息，则返回的句柄值msgHandle为0，</li></ul>
 * <ul><li>若接收的是带消息体消息，返回的句柄值msgHandle是消息体首地址。</li></ul>
 *
 * @param timeout   [IN]  类型#U32，任务接收消息的等待时间限制，单位为tick，
 * 取值范围为[0, 0xfffffffe]或者OS_WAIT_FOREVER；软中断接收消息时，此参数无效。
 * @param msgId     [OUT] 类型#U32 *，接收到的消息ID。若不需要此项输出，填写NULL即可。
 * @param senderPid [OUT] 类型#MsgPid *，消息发送者的PID，为#OS_MSG_HWI_SENDER时表示发送者为硬中断。若不需要此项输出，
 * 填写NULL即可。
 *
 * @retval  #OS_MSG_RECEIVE_ERR  0xFFFFFFFF，接收超时、消息队列ID非法、接收线程非法、消息队列中无消息。
 * @retval  其他                 返回消息体句柄。
 *
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MsgQRecv
 */
OS_SEC_ALW_INLINE INLINE MsgHandle SRE_MsgReceive(U32 timeout, U32 *msgId, MsgPid *senderPid)
{
    U32 ret;
    MsgHandle msgHandle;

    ret = SRE_MsgQRecv(&msgHandle, msgId, senderPid, 0, timeout);
    if (ret != SRE_OK) {
        return OS_MSG_RECEIVE_ERR;
    }

    return msgHandle;
}

/*
 * @ingroup SRE_msg
 * @brief 获取消息体长度。
 *
 * @par 描述
 * 获取MSG内容长度，单位:字节。
 * @attention
 * <ul><li>用户需保证传入消息句柄的有效性。</li></ul>
 *
 * @param msgHandle [IN]  类型#MsgHandle，消息体句柄，来源于消息体创建成功或接收成功的输出值。
 *
 * @retval  #OS_MSG_LENGTH_ERR  0x0000，消息体句柄非法、已被删除或消息无消息体。
 * @retval  非0                 消息体长度。
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MsgAlloc
 */
extern U16 SRE_MsgLength(MsgHandle msgHandle);

/*
 * @ingroup SRE_msg
 * @brief 获取消息结点使用情况。
 *
 * @par 描述
 * 获取本核消息结点使用情况。
 * @attention 无
 *
 * @param curUsedNum  [OUT]  类型#U32 *，当前使用的消息结点个数。
 * @param peakUsedNum [OUT]  类型#U32 *，最大使用的消息结点个数。
 *
 * @retval #SRE_OK 0x00000000，获取成功
 * @retval #其它值，获取失败
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see NA
 */
extern U32 SRE_MsgGetNodeUsedInfo(U32 *curUsedNum, U32 *peakUsedNum);

#if defined(OS_OPTION_MSG_ALLOW_LIST)
/*
 * @ingroup SRE_msg
 * @brief 获取本核可以给哪些核发送消息。
 *
 * @par 描述
 * 获取本核可以给哪些核发送消息，除SD6155平台除外。
 * @attention
 * <ul><li>获取本核允许发送消息的核名单。</li></ul>
 *
 * @param corelist [OUT] 类型#struct SysSreCoreMask *，每Bit对应一个核。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern void SRE_MsgDstCoreListGet(struct SysSreCoreMask *corelist);

/*
 * @ingroup SRE_msg
 * @brief 配置本核可以给哪些核发送消息，除SD6155平台除外。
 *
 * @par 描述
 * 配置本核可以给哪些核发送消息。
 * @attention
 * <ul><li>可重复配置，单核复位前，可以将故障核从白名单中去除，则可以避免RAT误发消息给故障核。</li></ul>
 *
 * @param corelist [IN]  类型#struct SysSreCoreMask *，每Bit对应一个核.
 *
 * @retval #OS_ERRNO_MSG_CORELIST_INVALID     0x02000d15，传入的消息白名单非法。
 * @retval #SRE_OK                            0x00000000，操作成功。
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_MsgDstCoreListSet(struct SysSreCoreMask *corelist);
#endif

#if defined(OS_OPTION_VOS_IPC)
/*
 * @ingroup SRE_msg
 * @brief VOS消息发送。
 *
 * @par 描述
 * 发送指定的VOS消息。
 * @attention
 * <ul>
 * <li>如果目标任务阻塞在VOS事件或消息上，会唤醒该任务。</li>
 * </ul>
 * @param taskPid    [IN]  类型#MsgPid，目标任务的ID。
 * @param queId      [IN]  类型#U32，消息队列ID
 * @param msgHandle  [IN]  类型#MsgHandle，消息体句柄。
 * @param urgentFlag [IN]  类型#bool，消息是否为紧急消息。
 *
 * @retval #SRE_OK                            0x00000000，操作成功。
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_SendVMsg(MsgPid taskPid, U32 queId, MsgHandle msgHandle, bool urgentFlag);

/*
 * @ingroup SRE_msg
 * @brief VOS消息接收。
 *
 * @par 描述
 * 接收VOS消息。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param queId     [IN]  类型#U32，消息队列ID，目前支持-1，表示接收任意队列的消息
 * @param msgHandle [OUT] 类型#MsgHandle *，消息体句柄。
 * @param timeout   [IN]  类型#U32，消息接收超时时间，单位tick。
 *
 * @retval #SRE_OK                            0x00000000，操作成功。
 * @par 依赖
 * <ul><li>SRE_msg.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_RecvVMsg(U32 queId, MsgHandle *msgHandle, U32 timeout);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_MSG_H */
