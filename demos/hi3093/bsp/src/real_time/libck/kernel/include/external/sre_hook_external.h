/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: HOOK模块的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01 LibCK项目组     华为规范整改
 */
#ifndef SRE_HOOK_EXTERNAL_H
#define SRE_HOOK_EXTERNAL_H

#include "sre_hook.h"
#include "sre_sys.h"
#include "sre_base.h"
#include "sre_raw_spinlock_external.h"
#include "sre_mem.h"

/*
 * 内核钩子管理原则
 * 1）服务于内核模块的钩子才可纳入钩子模块统一管理，例如补丁、shell模块的钩子就不适合。
 * 内核模块指mem,kernel,ipc,服务于内核的arch。
 * 2）没有返回值，没有输出参数的钩子才可纳入钩子模块统一管理。
 */
#define OS_HOOK_EMPTY NULL
#define OS_MHOOK_BOUNDARY 1UL
#define OS_MHOOK_NODE_DEAD ((OsVoidFunc)2)

/* 限制：参数类型不能为U64大于uintptr_t */
typedef void (*OsFunPara0)(void);
typedef void (*OsFunPara1)(uintptr_t);
typedef void (*OsFunPara2)(uintptr_t, uintptr_t);
typedef void (*OsFunPara3)(uintptr_t, uintptr_t, uintptr_t);
typedef void (*OsFunPara4)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);
typedef void (*OsFunPara5)(uintptr_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);

// 除了OS_HOOK_EMPTY和OS_MHOOK_BOUNDARY外，都是有效钩子
#define OS_MHOOK_IS_VALID(hook) ((uintptr_t)(hook) > OS_MHOOK_BOUNDARY)

#define OS_MHOOK_NOT_BOUNDARY(hook) ((uintptr_t)(hook) != OS_MHOOK_BOUNDARY)

#if defined(OS_OPTION_HOOK_ORDER)
OS_SEC_ALW_INLINE INLINE bool OsMhookBoundaryCheck(OsVoidFunc hook)
{
    return ((uintptr_t)hook == OS_MHOOK_BOUNDARY);
}

OS_SEC_ALW_INLINE INLINE bool OsMhookValidCheck(OsVoidFunc hook)
{
    return (hook != OS_HOOK_EMPTY && hook != OS_MHOOK_NODE_DEAD);
}
#else
OS_SEC_ALW_INLINE INLINE bool OsMhookBoundaryCheck(OsVoidFunc hook)
{
    return ((uintptr_t)hook <= OS_MHOOK_BOUNDARY);
}

OS_SEC_ALW_INLINE INLINE bool OsMhookValidCheck(OsVoidFunc hook)
{
    return (hook != OS_MHOOK_NODE_DEAD);
}
#endif

#define OS_MHOOK_ACTIVATE_PROC(hook, funcType, pfn, list)                        \
    do {                                                                         \
        OsVoidFunc *tmp_ = hook;                                                 \
        while (!OsMhookBoundaryCheck((OsVoidFunc)((pfn) = (funcType)(*tmp_)))) { \
            if (OsMhookValidCheck((OsVoidFunc)(pfn))) {                          \
                (list);                                                          \
            }                                                                    \
            tmp_++;                                                              \
        }                                                                        \
    } while (0)

#define OS_MHOOK_ACTIVATE(hookType, funcType, list)              \
    do {                                                         \
        OsVoidFunc *hook = g_hookCB[(hookType)].mulHook;         \
        funcType pfn;                                            \
        if (hook != NULL) {                                      \
            OS_MHOOK_ACTIVATE_PROC(hook, funcType, pfn, (list)); \
        }                                                        \
    } while (0)

#define OS_SHOOK_ACTIVATE(hookType, funcType, list)            \
    do {                                                       \
        funcType pfn = (funcType)g_hookCB[(hookType)].sigHook; \
        if (pfn != NULL)                                       \
            (list);                                            \
    } while (0)

#define OS_MHOOK_ACTIVATE_PARA0(hookType) OS_MHOOK_ACTIVATE((hookType), OsFunPara0, pfn())
#define OS_MHOOK_ACTIVATE_PARA1(hookType, arg0) OS_MHOOK_ACTIVATE((hookType), OsFunPara1, pfn((uintptr_t)(arg0)))
#define OS_MHOOK_ACTIVATE_PARA2(hookType, arg0, arg1) OS_MHOOK_ACTIVATE((hookType), \
    OsFunPara2, pfn((uintptr_t)(arg0), (uintptr_t)(arg1)))
#define OS_MHOOK_ACTIVATE_PARA3(hookType, arg0, arg1, arg2) OS_MHOOK_ACTIVATE((hookType), \
    OsFunPara3, pfn((uintptr_t)(arg0), (uintptr_t)(arg1), (uintptr_t)(arg2)))
#define OS_MHOOK_ACTIVATE_PARA4(hookType, arg0, arg1, arg2, arg3) \
    OS_MHOOK_ACTIVATE((hookType),                                 \
    OsFunPara4, pfn((uintptr_t)(arg0), (uintptr_t)(arg1), (uintptr_t)(arg2), (uintptr_t)(arg3)))
#define OS_MHOOK_ACTIVATE_PARA5(hookType, arg0, arg1, arg2, arg3, arg4) \
    OS_MHOOK_ACTIVATE((hookType), OsFunPara5,                           \
    pfn((uintptr_t)(arg0), (uintptr_t)(arg1), (uintptr_t)(arg2), (uintptr_t)(arg3), (uintptr_t)(arg4)))

#define OS_SHOOK_ACTIVATE_PARA0(hookType) OS_SHOOK_ACTIVATE((hookType), OsFunPara0, pfn())
#define OS_SHOOK_ACTIVATE_PARA1(hookType, arg0) OS_SHOOK_ACTIVATE((hookType), OsFunPara1, pfn((uintptr_t)(arg0)))
#define OS_SHOOK_ACTIVATE_PARA2(hookType, arg0, arg1) OS_SHOOK_ACTIVATE((hookType), \
    OsFunPara2, pfn((uintptr_t)(arg0), (uintptr_t)(arg1)))
#define OS_SHOOK_ACTIVATE_PARA3(hookType, arg0, arg1, arg2) OS_SHOOK_ACTIVATE((hookType), \
    OsFunPara3, pfn((uintptr_t)(arg0), (uintptr_t)(arg1), (uintptr_t)(arg2)))
#define OS_SHOOK_ACTIVATE_PARA4(hookType, arg0, arg1, arg2, arg3) \
    OS_SHOOK_ACTIVATE((hookType), OsFunPara4,                     \
    pfn((uintptr_t)(arg0), (uintptr_t)(arg1), (uintptr_t)(arg2), (uintptr_t)(arg3)))
#define OS_SHOOK_ACTIVATE_PARA5(hookType, arg0, arg1, arg2, arg3, arg4) \
    OS_SHOOK_ACTIVATE((hookType), OsFunPara5,                           \
    pfn((uintptr_t)(arg0), (uintptr_t)(arg1), (uintptr_t)(arg2), (uintptr_t)(arg3), (uintptr_t)(arg4)))

#if defined(OS_OPTION_SMP)
#define HOOK_ADD_IRQ_LOCK(intSave) OS_MCMUTEX_IRQ_LOCK(0, &g_hookRegLock, (intSave))
#define HOOK_ADD_IRQ_UNLOCK(intSave) OS_MCMUTEX_IRQ_UNLOCK(0, &g_hookRegLock, (intSave))
#define HOOK_DEL_IRQ_LOCK(intSave) OS_MCMUTEX_IRQ_LOCK(0, &g_hookRegLock, (intSave))
#define HOOK_DEL_IRQ_UNLOCK(intSave) OS_MCMUTEX_IRQ_UNLOCK(0, &g_hookRegLock, (intSave))
#else
#define HOOK_ADD_IRQ_LOCK(intSave) \
    do {                           \
        (intSave) = SRE_IntLock(); \
    } while (0)
#define HOOK_ADD_IRQ_UNLOCK(intSave) SRE_IntRestore((intSave))

#define HOOK_DEL_IRQ_LOCK(intSave) \
    do {                           \
        (intSave) = SRE_IntLock(); \
    } while (0)
#define HOOK_DEL_IRQ_UNLOCK(intSave) SRE_IntRestore((intSave))
#endif

enum {
    OS_HOOK_TICK_ENTRY = OS_HOOK_TYPE_NUM,
    OS_HOOK_TICK_EXIT,
    OS_HOOK_FIRST_TIME_SWH,

    OS_HOOK_TSK_MON, /* TSKMON钩子 */
    OS_HOOK_CPUP_WARN, /* CPUP告警钩子 */
    OS_HOOK_ERR_REG, /* 错误处理钩子 */
    OS_HOOK_IDLE_PREFIX, /* IDLE前置钩子 */
    OS_HOOK_MEM_DAMAGE, /* 踩内存处理钩子 */

    OS_HOOK_TYPE_TOTAL
};

#define OS_SHOOK_TYPE_START ((U32)OS_HOOK_TSK_MON)

enum OsHookChgType {
    HOOK_ADD_FIRST,
    HOOK_DEL_LAST
};

union TagMhookCB {
    // 在初始化阶段复用为钩子注册数目
    uintptr_t num;
    // 单钩子
    OsVoidFunc sigHook;
    OsVoidFunc *mulHook;
};
typedef U32(*OsHookChgFunc)(U32 hookType, enum OsHookChgType chgType);

extern union TagMhookCB g_hookCB[OS_HOOK_TYPE_TOTAL];

/*
 * 多钩子添加内部接口
 */
extern U32 OsMhookAdd(U32 hookType, OsVoidFunc hook);

/*
 * 多钩子删除内部接口
 */
extern U32 OsMhookDel(U32 hookType, OsVoidFunc hook);

/*
 * 多钩子注册内部接口， 为了与原有规格兼容，传入NULL表示删除
 */
extern U32 OsShookReg(U32 hookType, OsVoidFunc hook);

/*
 * 多钩子挂接数查询
 */
extern U32 OsMhookCount(U32 hookType);

/*
 * 钩子状态变更通知
 */
extern void OsHookChgHandlerReg(U32 hookType, OsHookChgFunc proc);

/*
 * 在注册阶段，不同模块通过osMhookReserve接口预留钩子,不影响用户注册钩子数
 * 仅在register阶段使用
 */
extern void OsMhookReserve(U32 hookType, U32 incCnt);

/*
 * 测试性接口，慎用
 */
#define OS_SHOOK_GET(hookType) g_hookCB[(hookType)].sigHook
#define OS_SHOOK_SET(hookType, hook) (g_hookCB[(hookType)].sigHook = (hook))

#endif /* SRE_HOOK_EXTERNAL_H */
