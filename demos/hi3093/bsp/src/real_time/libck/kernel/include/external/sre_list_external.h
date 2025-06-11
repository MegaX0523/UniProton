/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 双向链表操作的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
#ifndef SRE_LIST_EXTERNAL_H
#define SRE_LIST_EXTERNAL_H

#include "sre_base.h"
#include "bits/list_types.h"

/*
 * @ingroup SREList
 * 初始化链表(链表重用时的初始化)
 *
 * @param head [IN] 链表头结点的地址(The address of the head of a list )
 */
#define OS_LIST_INIT(head)     \
    do {                       \
        (head)->prev = (head); \
        (head)->next = (head); \
    } while (0)

#define LIST_OBJECT_INIT(object) { \
        &(object), &(object)       \
    }

#define LIST_OBJECT(object) (struct TagListObject (object) = LIST_OBJECT_INIT(object))

#define OS_LIST_FIRST(object) ((object)->next)

/* list action low level add */
OS_SEC_ALW_INLINE INLINE void ListLowLevelAdd(struct TagListObject *newNode, struct TagListObject *prev,
                                              struct TagListObject *next)
{
    newNode->next = next;
    newNode->prev = prev;
    next->prev = newNode;
    prev->next = newNode;
}

/* list action add */
OS_SEC_ALW_INLINE INLINE void ListAdd(struct TagListObject *newNode, struct TagListObject *listObject)
{
    ListLowLevelAdd(newNode, listObject, listObject->next);
}

/* list action tail add */
OS_SEC_ALW_INLINE INLINE void ListTailAdd(struct TagListObject *newNode, struct TagListObject *listObject)
{
    ListLowLevelAdd(newNode, listObject->prev, listObject);
}

/* list action lowel delete */
OS_SEC_ALW_INLINE INLINE void ListLowLevelDelete(struct TagListObject *prevNode, struct TagListObject *nextNode)
{
    nextNode->prev = prevNode;
    prevNode->next = nextNode;
}

/* list action delete */
OS_SEC_ALW_INLINE INLINE void ListDelAndInit(struct TagListObject *node)
{
    ListLowLevelDelete(node->prev, node->next);
    OS_LIST_INIT(node);
}

/* list action delete */
#if defined(OS_OPTION_SMP)
OS_SEC_ALW_INLINE INLINE void ListDelete(struct TagListObject *node)
{
    /* 有些复杂场景需要支持重复删除，因此结点删除后需要重新初始化 */
    ListDelAndInit(node);
}
#else
OS_SEC_ALW_INLINE INLINE void ListDelete(struct TagListObject *node)
{
    ListLowLevelDelete(node->prev, node->next);

    node->next = NULL;
    node->prev = NULL;
}
#endif

/* list action move */
OS_SEC_ALW_INLINE INLINE void ListMove(struct TagListObject *list, struct TagListObject *listObject)
{
    ListLowLevelDelete(list->prev, list->next);
    ListAdd(list, listObject);
}

/* list action tail move */
OS_SEC_ALW_INLINE INLINE void ListMoveTail(struct TagListObject *list, struct TagListObject *listObject)
{
    ListLowLevelDelete(list->prev, list->next);
    ListTailAdd(list, listObject);
}

/* list action empty */
OS_SEC_ALW_INLINE INLINE bool ListEmpty(const struct TagListObject *listObject)
{
    return (bool)(listObject->next == listObject);
}

#define OFFSET_OF_FIELD(type, field) ((uintptr_t)((uintptr_t)(&((type *)0x10)->field) - (uintptr_t)0x10))

#define COMPLEX_OF(ptr, type, field) ((type *)((uintptr_t)(ptr) - OFFSET_OF_FIELD(type, field)))

/* 根据成员地址得到控制块首地址, ptr成员地址, type控制块结构, field成员名 */
#define LIST_COMPONENT(ptrOfList, typeOfList, fieldOfList) COMPLEX_OF(ptrOfList, typeOfList, fieldOfList)

#define LIST_FOR_EACH(posOfList, listObject, typeOfList, field)                                                    \
    for ((posOfList) = LIST_COMPONENT((listObject)->next, typeOfList, field); &(posOfList)->field != (listObject); \
         (posOfList) = LIST_COMPONENT((posOfList)->field.next, typeOfList, field))

#define LIST_FIRST_ENTITY(listForFirstEntity, typeOfList, field) \
    LIST_COMPONENT((listForFirstEntity)->next, typeOfList, field)

/* 倒序循环 2018-4-21 add */
#define LIST_FOR_EACH_REV_ORDER(pos, listObject, type, field)                                    \
    for ((pos) = LIST_COMPONENT((listObject)->prev, type, field); &(pos)->field != (listObject); \
         (pos) = LIST_COMPONENT((pos)->field.prev, type, field))

#define LIST_FOR_EACH_SAFE(posOfList, listObject, typeOfList, field)                \
    for ((posOfList) = LIST_COMPONENT((listObject)->next, typeOfList, field);       \
         (&(posOfList)->field != (listObject))&&((posOfList)->field.next != NULL);  \
         (posOfList) = LIST_COMPONENT((posOfList)->field.next, typeOfList, field))

#endif /* SRE_LIST_EXTERNAL_H */
