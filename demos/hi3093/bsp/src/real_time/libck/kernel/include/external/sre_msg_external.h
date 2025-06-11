/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 消息模块的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01, LibCK项目组,  华为规范整改
 */
#ifndef SRE_MSG_EXTERNAL_H
#define SRE_MSG_EXTERNAL_H

#include "sre_base.h"
#include "sre_msg.h"
#include "sre_list_external.h"
#include "sre_sys.h"
#include "sre_sys_external.h"
#include "sre_task_external.h"
#include "sre_mem_external.h"
#include "sre_err_external.h"
#include "sre_hunt_external.h"
#include "sre_log_external.h"

/* 模块间宏定义 */
#if defined(OS_OPTION_SMP)
/* 消息申请/释放时使用的锁 */
extern volatile uintptr_t g_msgInitLock;
#define MSG_INIT_IRQ_LOCK(intSave) OS_MCMUTEX_IRQ_LOCK(0, &g_msgInitLock, (intSave))
#define MSG_INIT_IRQ_UNLOCK(intSave) OS_MCMUTEX_IRQ_UNLOCK(0, &g_msgInitLock, (intSave))
#define MSG_INIT_LOCK() OS_MCMUTEX_LOCK(0, &g_msgInitLock)
#define MSG_INIT_UNLOCK() OS_MCMUTEX_UNLOCK(0, &g_msgInitLock)
#else
#define MSG_INIT_IRQ_LOCK(intSave) ((intSave) = OsIntLock())
#define MSG_INIT_IRQ_UNLOCK(intSave) (OsIntRestore(intSave))
#define MSG_INIT_LOCK() (void)NULL
#define MSG_INIT_UNLOCK() (void)NULL
#endif

#if defined(OS_OPTION_VOS_IPC)
/* OSA消息 */
#define OS_VOS_MSG 3
#endif

/* IIU_BEGIN+22: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
#define OS_MSG_LMSG 0 /* 附加消息载荷的消息 (长消息) */
#define OS_MSG_SMSG 1 /* 短载荷的消息 */
#define OS_MSG_NPLD 2 /* 无载荷的消息 */

#define OS_MSG_MAGIC_ALLOC 0xabc1 /* 长消息体中消息头的魔术字状态，表示已申请可使用状态、可删除 */
#define OS_MSG_MAGIC_SEND 0xabc2 /* 长消息体中消息头的魔术字状态，正在发送，不可删除 */
#define OS_MSG_MAGIC_IPCIRQ 0xabc4 /* 长消息体中消息头的魔术字状态，正在核间处理状态，不可删除 */
#define OS_MSG_MAGIC_INLIST 0xabc8 /* 长消息体中消息头的魔术字状态，正在接收，不可删除 */
#define OS_MSG_MAGIC_PACKAGED 0xabd1 /* 长消息体中消息头的魔术字状态，消息已组装状态，可删除 */

#define OS_MSG_MAGIC_DELETED 0xffff /* 长消息体已经删除 */
#define OS_MSG_MAGIC_LONGMSG 0xabcddcba /* 长消息魔术字 */

#if defined(OS_OPTION_MSG_MULTICAST)
/* OS_OPTION_MSG_MULTICAST 功能宏on消息结构体按默认16字节对齐即可 */
#define OS_MSG_HEAD_LEN sizeof(struct MsgHead)
#else
/* OS_OPTION_MSG_MULTICAST 功能宏off消息结构体按默认32字节对齐 */
#define OS_MSG_HEAD_LEN ALIGN(sizeof(struct MsgHead), (1UL << (U32)MEM_ADDR_ALIGN_032))
#endif

#define OS_MSG_IN_USE 0x74U /* 短消息处于发送，核间处理或者正在接收状态，不可删除 */
#define OS_MSG_RELEASABLE 0x78U /* 短消息处于申请可使用或者接收完成状态，可删除 */
#define OS_MSG_DELETED 0x7cU /* 短消息处于空闲状态，不可删除 */

/* 32/64位情况下，消息头的size不一样，消息内存的对齐要求也就不一样 */
#if defined(OS_OPTION_CPU64)
#define MSG_MEM_ADDR_ALIGN MEM_ADDR_ALIGN_064
#else
#define MSG_MEM_ADDR_ALIGN MEM_ADDR_ALIGN_032
#endif

#define QMSG_ID_WPOS     2
#define QMSG_HEAD_WPOS   3

/*
 * 消息头未使用过或使用过已经释放，用于计算消息头使用过程最大占用量
 * 消息空闲时存储在list.prev字段。
 */
#define OS_MSG_NODE_UNUSED 0xFFFFFFFF

#define OS_MSG_MAX_QID 7 /* 任务的最大的消息队列ID */

/* 短消息Payload长度，不同平台有差异，该宏定义不对用户感知 */
#define OS_SMSG_MAX_LEN 8 /* SD6182：MSGM的第4个WORD为控制头，第3个WORD为MsgID，剩下2个WORD为短消息Payload */
#if defined(OS_OPTION_MSG_ALLOW_LIST)
/* 消息白名单检查 */
#define OS_CORELIST_CHECK(coreId) \
    (OS_COREID_CHK_INVALID(coreId, g_maxNumOfCores) || (!OS_SYS_CORE_MASK_BIT_VALID(&g_coreList, (coreId))))

extern struct SysSreCoreMask g_coreList;
extern void OsMsgAllowListInit(void);

/*
 * 描述: 核间软中断收到消息后的相关处理
 */
OS_SEC_ALW_INLINE INLINE void OsMsgAllowListSet(U32 excCore)
{
    uintptr_t intSave;

    intSave = OsIntLock();

    OS_SYS_CORE_MASK_BIT_CLEAR(&g_coreList, excCore);

    OsIntRestore(intSave);
}

#else
#define OS_CORELIST_CHECK(coreId) (OS_COREID_CHK_INVALID(coreId, g_maxNumOfCores))
#endif

#define OS_SMSG_MAGIC 0x5A5A5A5A /* 短消息魔术字 */

#define OS_IS_SMSG(msgHdl) (OS_SMSG_MAGIC == *(U32 *)((uintptr_t)(msgHdl) - sizeof(U32)))
#define OS_IS_NPLD(msgHdl) ((msgHdl) == 0)

#define OS_SMSG_HDL2NODE(msgHdl) (LIST_COMPONENT((msgHdl), struct TagMsgNode, data[1]))
#define OS_SMSG_NODE2HDL(msgNode) ((MsgHandle)(&(msgNode)->data[1]))

#define FUSION_PUB_HEAD_OFF sizeof(struct FusionMsgHead)

/* 融合之后，VOS格式也一样 */
#define VOS_PUB_HEAD_OFF FUSION_PUB_HEAD_OFF

/* IIU_BEGIN+30: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
#define MSG_PUBLIC_HEAD                                             \
    struct TagListObject list; /* 链接消息接收队列，必须放在头位 */ \
    MsgPid dstPid; /* 消息接收者的pid */                            \
    MsgPid srcPid; /* 消息发送者的pid */                            \
    U32 msgID; /* 4字节的消息数据 */                                \
    U8 qID; /* 接收任务的QID */                                     \
    U8 msgType /* 消息类型：无消息体, 长消息, 短消息 */

/* 模块间数据结构定义 */
struct MsgHead {
#if defined(OS_OPTION_MSG_MULTICAST)
    U32 reserved;
    /* 引用计数 */
    U16 referCount;
#else
    MSG_PUBLIC_HEAD;
#endif
    /* 消息内存是否共享 */
    U8 shared;
    /* 消息内存是否即共享又Cache */
    U8 cachable;
    /* 发送的消息体内存长度 */
    U16 length;
    /* 魔术字 */
    U16 magicWord;
    /* 消息头相对内存块起始地址的偏移 */
    void *memBlock;
};

struct TagMsgNode {
#if defined(OS_OPTION_MSG_MULTICAST)
    struct TagListObject list; /* 用于挂在进程消息接收队列，必须放在头位 */
    MsgPid dstPid; /* 消息接收者的pid */
    MsgPid srcPid; /* 消息发送者的pid */
    uintptr_t msgID;   /* 4字节的消息数据 */
    U32 msgHdl;    /* 消息句柄 */
#else
    MSG_PUBLIC_HEAD;
    /* 短消息长度 */
    U8 smsgLen;
    /* 可释放标志 */
    U8 releasable;
    /* 数组的第0个WORD存放短消息魔术字，后面的WORD才用于存储短消息的净荷 */
    U32 data[1 + (OS_SMSG_MAX_LEN / sizeof(U32))];
#endif
};

#if (defined(OS_OPTION_VOS_IPC) || defined(OS_OPTION_FUSION_INTEGRATE_API))
/* 融合接口时，OS不感知具体的消息体结构，只需要挂接链表 */
struct FusionMsgHead {
    struct TagListObject list;
};
#endif

/* 模块间typedef声明 */
#if defined(OS_OPTION_CORE_RESET)

/* 消息内存申请函数指针 */
typedef void *(*MSGMEM_ALLOC_ALIGN_F)(U32 mid, U8 ptNo, U32 size, enum MemAlign alignPow);

/* 消息内存释放函数指针 */
typedef U32(*MSGMEM_FREE_F)(uintptr_t addr, U32 share, U32 cache);

/* 消息内存发送函数指针 */
typedef void(*MSGMEM_SENT_F)(struct MsgHead *addr);

/* 消息内存接收函数指针 */
typedef void(*MSGMEM_RECEIVED_F)(struct MsgHead *addr);

/* 消息内存函数库 */
struct TagFuncsLibMsgMem {
    /* 内存申请 */
    MSGMEM_ALLOC_ALIGN_F memAllocAlign;
    /* 内存已发送 */
    MSGMEM_SENT_F memSent;
    /* 内存已接收 */
    MSGMEM_RECEIVED_F memReceived;
    /* 内存释放 */
    MSGMEM_FREE_F memFree;
};

/* 消息内存控制块Cache Inv函数指针 */
typedef void (*MsgMemLinkHeadProcFunc)(struct MsgHead *addr);
#endif

enum MsgUsedAndPeak {
    MSG_USED_NUM = 0,
    MSG_PEAK_NUM,
    MSG_USEDANDPEAK
};
/* 模块间全局变量声明 */
/* 指向第一个核内消息空闲节点 */
extern struct TagMsgNode *g_freeLcMsgHead;
/* 指向第一个核内消息空闲节点 */
extern struct TagMsgNode *g_freeLcMsgEnd;
/* 消息使用峰值 */
extern U32 g_msgUsedAndPeak[MSG_USEDANDPEAK];
/* 消息节点内存首地址 */
extern struct TagMsgNode *g_freeLcMsgArray;

#if defined(OS_OPTION_MSG_MULTICAST)
extern uintptr_t g_mcMsgGuard;

/* 指向第一个核间消息空闲节点 */
extern struct TagMsgNode *g_freeMcMsgHead;

/* 所有核间消息节点的起始地址 */
extern struct TagMsgNode *g_mcMsgArray;
#endif

#if defined(OS_OPTION_CORE_RESET)
/* 消息内存函数钩子 */
extern struct TagFuncsLibMsgMem g_msgMemHook;
extern MsgMemLinkHeadProcFunc g_msgMemLinkHeadHook;
#endif

extern void OsMsgAllowLiseUpdate(void);

/* 模块间函数声明 */
/* 核间消息发送钩子函数 */
extern void OsMsgSubmit(struct TagListObject *listWare, U32 threadID, U8 queId, U8 urgent);
extern U32 OsMsgTotalNumGet(void);
extern U32 OsMsgCurNodeNumGet(void);
extern U32 OsMsgPeakNodeNumGet(void);

/* 模块间内联函数定义 */
OS_SEC_ALW_INLINE INLINE MsgPid OsMsgPidGet(void)
{
    MsgPid srcPid;

#if defined(OS_OPTION_SMP)
    uintptr_t intSave = OsIntLock();
#endif

    if (OS_HWI_ACTIVE) {
        /* 硬中断创建消息不具体区别中断号 */
        srcPid = COMPOSE_PID(OsGetHwThreadId(), OS_MSG_HWI_SENDER);
#if defined(OS_OPTION_SWI)
    } else if ((UNI_FLAG & OS_FLG_SWI_ACTIVE) != 0) {
        srcPid = g_swiActive->swiPid;
#endif
    } else {
        srcPid = RUNNING_TASK->taskPID;
    }

#if defined(OS_OPTION_SMP)
    OsIntRestore(intSave);
#endif

    return srcPid;
}

/* 关中断保护由调用者保证!!! */
OS_SEC_ALW_INLINE INLINE struct TagMsgNode *OsMsgNodeAlloc(U8 msgLen, U8 releasable)
{
    struct TagMsgNode *msgNode = NULL;

#if defined(OS_OPTION_MSG_MULTICAST)
    (void)releasable;
    (void)msgLen;
#endif

    MSG_INIT_LOCK();

    if (g_freeLcMsgHead != g_freeLcMsgEnd) {
        g_msgUsedAndPeak[MSG_USED_NUM]++;
        if (g_msgUsedAndPeak[MSG_USED_NUM] > g_msgUsedAndPeak[MSG_PEAK_NUM]) {
            g_msgUsedAndPeak[MSG_PEAK_NUM] = g_msgUsedAndPeak[MSG_USED_NUM];
        }
        msgNode = g_freeLcMsgHead;
        /* 空闲链表头指向下一个节点 */
        g_freeLcMsgHead = (struct TagMsgNode *)msgNode->list.next;
#if !defined(OS_OPTION_MSG_MULTICAST)
        msgNode->releasable = releasable;
        msgNode->smsgLen = msgLen;
#endif
    }

    MSG_INIT_UNLOCK();

    return msgNode;
}

/* 关中断保护和指针的有效性由调用者保证!!! */
OS_SEC_ALW_INLINE INLINE void OsMsgNodeFree(struct TagMsgNode *msgNode)
{
    MSG_INIT_LOCK();

    g_msgUsedAndPeak[MSG_USED_NUM]--;
    /* 不允许重复释放 */
#if defined(OS_OPTION_MSG_MULTICAST)
    msgNode->msgHdl = (uintptr_t)NULL;
#else
    msgNode->releasable = (U8)OS_MSG_DELETED;
#endif
    g_freeLcMsgEnd->list.next = (struct TagListObject *)msgNode;
    /* 空闲链表移至表尾 */
    g_freeLcMsgEnd = msgNode;

    MSG_INIT_UNLOCK();
}

/*
 * 描述: 判断消息目的线程(软中断、任务)和队列号是否合法
 *       1)线程ID合法，且线程已创建
 *       2)发到任务的消息，队列号合法
 */
OS_SEC_ALW_INLINE INLINE U32 OsInnerMsgDstCheck(U32 threadId, U32 queId)
{
    U32 innerThreadId = threadId;

    // OS_THREAD_NUM包括软中断和任务的总数
    if (innerThreadId >= OS_THREAD_NUM) {
        return OS_ERRNO_MSG_PID_INVALID;
    }

#if defined(OS_OPTION_SWI)
    if (innerThreadId < g_swiMaxNum) {
        if (((U32)g_swiData.list[innerThreadId].status & (U32)OS_SWI_CREATED) == 0) {
            return OS_ERRNO_MSG_PID_INVALID;
        }

        return SRE_OK;
    }

    innerThreadId -= g_swiMaxNum;
#endif

#if defined(OS_OPTION_SMP)
    if (innerThreadId >= g_tskMaxNum) {
        return OS_ERRNO_MSG_PID_INVALID;
    }
#endif

    if (g_tskCBArray[innerThreadId].taskStatus == OS_TSK_UNUSED) {
        return OS_ERRNO_MSG_PID_INVALID;
    }

    /* 不用判断目的线程是否为IDLE任务，IDLE任务的消息队列数一定为0 */
    if (g_tskCBArray[innerThreadId].queNum <= queId) {
        return OS_ERRNO_MSG_QID_INVALID;
    }

    return SRE_OK;
}

/* 核间通信 */
#if defined(OS_OPTION_AMP_MULTICORE)
#if defined(OS_OPTION_MSG_MULTICAST)
extern void OsMcMsgDrop(struct TagMsgNode *msgNode);

OS_SEC_ALW_INLINE INLINE struct TagMsgNode *OsMcMsgNodeAlloc(void)
{
    struct TagMsgNode *msgNode = g_freeMcMsgHead;

    /* 这里为多核获取msgnode方式，后续错误分支注意释放消息节点 */
    if (msgNode != NULL) {
        g_freeMcMsgHead = (struct TagMsgNode *)msgNode->msgID;
    }

    return msgNode;
}

OS_SEC_ALW_INLINE INLINE void OsMcMsgNodeFree(struct TagMsgNode *msgNode)
{
    msgNode->dstPid = 0;
    msgNode->srcPid = 0;
    msgNode->msgID = (uintptr_t)g_freeMcMsgHead;

    g_freeMcMsgHead = msgNode;
}

OS_SEC_ALW_INLINE INLINE void OsMsgIrptHandler(uintptr_t para)
{
    uintptr_t intSave;
    U32 threadId;
    struct TagMsgNode *msgNode = (struct TagMsgNode *)para;

    threadId = GET_HANDLE(msgNode->dstPid);

    intSave = OsIntLock();
    if (OsInnerMsgDstCheck(threadId, 0) != SRE_OK) {
        OsMcMsgDrop(msgNode);

        OsIntRestore(intSave);

        OS_REPORT_ERROR(OS_ERRNO_MSG_PID_INVALID);
        return;
    }

    OsMsgSubmit(&msgNode->list, threadId, 0, 0);

    OsIntRestore(intSave);
}

#else
/* IIU_BEGIN+58: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
struct MsgToken {
    uintptr_t tag : 3; /* tag == 0 */
    uintptr_t type : 2;  /* 0: long msg, 1: short msg, 2: no payload msg */
    uintptr_t slen : 3; /* short msg length when type == 1 */
    uintptr_t dstTid : 8;  /* destination thread pid */
    uintptr_t srcTid : 16; /* source thread pid */
};

struct ShortMsgQData {
    uintptr_t context1;
    uintptr_t context2;
    uintptr_t msgID;
    uintptr_t token;
};

struct LongMsgQData {
    uintptr_t uncacheMsgHead;
    uintptr_t rsv1;
    uintptr_t msgMagicWord;
    uintptr_t token;
};

#define OS_MAKE_MSG_TOKEN(type, slen, dstPid, srcPid)                               \
    ({                                                                              \
        struct MsgToken tk_ = {0, (type), (slen), OS_GTID_GET((dstPid)), (srcPid)}; \
        TO_INTEGER(tk_);                                                            \
    })

OS_SEC_ALW_INLINE INLINE void OsMakeNpldMsg(struct ShortMsgQData *npld, U32 msgId, U32 dstPid,
                                            U32 srcPid)
{
    npld->msgID = (msgId);
    npld->token = OS_MAKE_MSG_TOKEN(OS_MSG_NPLD, 0, dstPid, srcPid);
}

OS_SEC_ALW_INLINE INLINE void OsMakeShortMsg(struct ShortMsgQData *smsg, U32 msgId, U32 dstPid,
                                             U32 srcPid, U32 slen, MsgHandle msgHandle)
{
    smsg->context1 = *(U32 *)msgHandle;
    smsg->context2 = *(U32 *)(msgHandle + sizeof(U32));
    smsg->msgID = msgId;
    smsg->token = OS_MAKE_MSG_TOKEN(OS_MSG_SMSG, slen - 1, dstPid, srcPid);
}

OS_SEC_ALW_INLINE INLINE void OsMakeLongMsg(struct LongMsgQData *lmsg, struct MsgHead *msgHead,
                                            U32 msgId, U32 dstPid, U32 srcPid)
{
    msgHead->dstPid = dstPid;
    msgHead->srcPid = srcPid;
    msgHead->qID = 0;
    msgHead->msgID = msgId;
    msgHead->magicWord = OS_MSG_MAGIC_SEND;
    lmsg->uncacheMsgHead = OsMemAddrToUncache((uintptr_t)msgHead);
    lmsg->msgMagicWord = OS_MSG_MAGIC_LONGMSG;
    lmsg->token = OS_MAKE_MSG_TOKEN(OS_MSG_LMSG, 0, dstPid, srcPid);
}

#define OS_SMSG_DATA0_INDEX 1
#define OS_SMSG_DATA1_INDEX 2
extern void OsMsgIrqDstTidInvalid(struct LongMsgQData *longMsg);

OS_SEC_ALW_INLINE INLINE U32 OsMsgIrqSubmitMsg(struct MsgToken tk, struct DssMsqData *qData)
{
    struct TagMsgNode *msgNode = NULL;
    struct MsgHead *msgHead = NULL;

    if (tk.type != OS_MSG_LMSG) {
        msgNode = OsMsgNodeAlloc((U8)(tk.slen + 1), (U8)OS_MSG_IN_USE);
        if (msgNode == NULL) {
            return OS_ERRNO_MSG_NO_MSGNODE;
        }

        struct ShortMsgQData *shortMsg = (struct ShortMsgQData *)qData;

        msgNode->list.prev = NULL;

        // 非长消息PID: COREID+LTID,前面高16位为0
        msgNode->dstPid = COMPOSE_PID(OsGetHwThreadId(), tk.dstTid);
        // 非长消息PID: COREID+LTID,前面高16位为0
        msgNode->srcPid = tk.srcTid;
        msgNode->qID = 0;
        msgNode->msgType = (U8)tk.type;
        msgNode->msgID = (U32)shortMsg->msgID;
        // 出于代码归一的考虑，无消息体消息，同样拷贝qData[0]和qData[1]
        msgNode->data[OS_SMSG_DATA0_INDEX] = (U32)shortMsg->context1;
        msgNode->data[OS_SMSG_DATA1_INDEX] = (U32)shortMsg->context2;

        OsMsgSubmit(&msgNode->list, tk.dstTid, 0, 0);
    } else {
        struct LongMsgQData *longMsg = (struct LongMsgQData *)qData;
        if (longMsg->msgMagicWord != OS_MSG_MAGIC_LONGMSG) {
            OS_LOG_DBG("msg data rev not suit any type: %8x %8x %8x %8x", longMsg->uncacheMsgHead,
                longMsg->rsv1, longMsg->msgMagicWord, longMsg->token);
            return OS_ERRNO_MSG_LONGMSG_INVALID;
        }

        msgHead = (struct MsgHead *)(uintptr_t)longMsg->uncacheMsgHead;

        msgHead->msgType = (U8)tk.type;
        msgHead->magicWord = OS_MSG_MAGIC_INLIST;

#if defined(OS_OPTION_CORE_RESET)
        g_msgMemHook.memReceived((void *)msgHead);
#endif
        OsMsgSubmit(&msgHead->list, tk.dstTid, 0, 0);
    }
    return SRE_OK;
}

/*
 * 描述: 消息中断，读取消息队列任务，并转发至对应进程
 * 备注: NA
 */
OS_SEC_ALW_INLINE INLINE void OsMsgIrptHandler(struct DssMsqData *qData)
{
    uintptr_t intSave;
    U32 ret;
    struct LongMsgQData *longMsg = (struct LongMsgQData *)qData;
    struct MsgToken tk = FROM_INTEGER(longMsg->token, struct MsgToken);

    intSave = OsIntLock();
    if (OsInnerMsgDstCheck(tk.dstTid, 0) != SRE_OK) {
        OsIntRestore(intSave);
        OsMsgIrqDstTidInvalid(longMsg);
        return;
    }
    /* 向指定任务发送消息 */
    ret = OsMsgIrqSubmitMsg(tk, qData);
    if (ret != SRE_OK) {
        OsIntRestore(intSave);
        OS_REPORT_ERROR(ret);
        return;
    }

    OsIntRestore(intSave);
}
#endif
#endif

#if defined(OS_OPTION_FUSION_INTEGRATE_API)
extern U32 OsMsgQueRecv(U32 qId, MsgHandle *msgHandle, U32 timeout);

extern U32 OsMsgQueSend(U32 pid, U32 qId, MsgHandle msgHandle, U32 flag);

extern U32 OsMsgQueNumGet(U32 taskID, U32 *qNum);
#endif

#endif /* SRE_MSG_EXTERNAL_H */
