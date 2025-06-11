/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 优先级管理链表公共头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_PLIST_EXTERNAL_H
#define SRE_PLIST_EXTERNAL_H

#include "sre_list_external.h"
#include "sre_base.h"

/*
 * 模块间宏定义
 */
#define OS_GET_WORD_NUM_BY_PRIONUM(prio) (((prio) + 0x1f) >> 5)  // 通过支持的优先级个数计算需要多少个word表示

/*
 * prioList:优先级链表 (每遇到不同的优先级链接一次)
 * nodeList:节点链表   (将所有可push的节点链接一次)
 *           |
 *           |    prio0 <--> prio2 <------------------------> prio5 <--> prio...
 *           |      |          |          |          |          |          |
 *           |      |          |          |          |          |          |
 *           |<-->node0 <--> node1 <--> node2 <--> node3 <--> node4 <--> node...
 *
 *           按优先级插入摘除时效率高，同优先级按FIFO顺序
 *           插入的时间复杂度O(n),n表示优先级个数,与任务个数无关.摘除的时间复杂度为O(1)
 */
struct PushablTskList {
    U32 prio;
    struct TagListObject nodeList;
};

/*
 * 可push的队列链表头
 */
struct PushablTskListHead {
    struct TagListObject nodeList;
};

/*
 * 模块间全局变量声明
 */
/*
 * 模块间函数声明
 */
extern void OsPshAblListAdd(struct PushablTskList *node, struct PushablTskListHead *head);
extern void OsPshAblListDel(struct PushablTskList *node);
extern void OsPshAblListReQue(struct PushablTskList *node, struct PushablTskList *head);

/* 可push队列为空，表示没有可push的任务 */
OS_SEC_ALW_INLINE INLINE bool OsPlistHeadIsEmpty(struct PushablTskListHead *head)
{
    return ListEmpty(&head->nodeList);
}

/* 节点不在链表上，指向自己 */
OS_SEC_ALW_INLINE INLINE bool OsPlistNodeIsEmpty(struct PushablTskList *node)
{
    return ListEmpty(&node->nodeList);
}

/* 获取链表里的第一个节点，即最高优先级的节点 */
OS_SEC_ALW_INLINE INLINE struct PushablTskList *OsPlistGetFirst(struct PushablTskListHead *head)
{
    return LIST_COMPONENT(head->nodeList.next, struct PushablTskList, nodeList);
}

/* 可push链表头的初始化 */
OS_SEC_ALW_INLINE INLINE void OsPlistHeadInit(struct PushablTskList *node)
{
    OS_LIST_INIT(&node->nodeList);
}

/* 可push链表节点的初始化 */
OS_SEC_ALW_INLINE INLINE void OsPlistNodeInit(struct PushablTskList *node, U32 prio)
{
    node->prio = prio;
    OsPlistHeadInit(node);
}

/* 获取链表里的第一个节点，即最高优先级的节点 */
#define PLIST_GET_FIRST_TSK(head, type, field) LIST_COMPONENT(OsPlistGetFirst(head), type, field)

#endif /* SRE_PLIST_EXTERNAL_H */
