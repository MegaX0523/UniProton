/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: spinlock内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_RAW_SPINLOCK_EXTERNAL_H
#define SRE_RAW_SPINLOCK_EXTERNAL_H

#include "sre_base.h"
#if defined(OS_OPTION_EMT)
#include "sre_atomic.h"
#endif
#if defined(OS_OPTION_EMT)
#include "sre_emt_external.h"
#elif defined(OS_OPTION_HWSEM)
#include "sre_hwsem_external.h"
#endif
#include "sre_cpu_external.h"

#if defined(OS_OPTION_EMT)
#define OS_MCMUTEX_00 OS_EMT_00  // 用于nsm、外设(Hwsem、Msgm、mchwtmr)模块的互斥,包含这几个模块的初始化部分
#define OS_MCMUTEX_01 OS_EMT_01  // 共享内存申请、释放
#define OS_MCMUTEX_02 OS_EMT_02  // 核间
#define OS_MCMUTEX_03 OS_EMT_03  // 初始化
#define OS_MCMUTEX_04 OS_EMT_04  // 实例内互斥LP功能使用
#define OS_MCMUTEX_05 OS_EMT_05  // 实例内互斥符号表功能使用
// 6~9号MCMUTEX由JSF使用
#define OS_MCMUTEX_10 OS_EMT_10  // DSW内存算法专用锁，不能复用
#define OS_MCMUTEX_11 OS_EMT_11  // 11 ~ 26号共16个MCMUTEX由实例间长消息共享内存使用(实例内锁)
#define OS_FSC_MCMUTEX_MAX 16
#elif defined(OS_OPTION_HWSEM)
#define OS_MCMUTEX_00 OS_HWSEM_00  // 用于nsm、外设(Hwsem、Msgm、mchwtmr)模块的互斥,包含这几个模块的初始化部分
#define OS_MCMUTEX_01 OS_HWSEM_01  // 共享内存申请、释放
#define OS_MCMUTEX_02 OS_HWSEM_02
#define OS_MCMUTEX_03 OS_HWSEM_03  // 初始化
#define OS_MCMUTEX_04 g_hwSemHandle  // 实例内互斥LP功能使用
#define OS_MCMUTEX_05 g_symbolHwSemId0  // 实例内互斥符号表功能使用
#else
#define OS_MCMUTEX_00 0
#define OS_MCMUTEX_01 1
#define OS_MCMUTEX_02 2
#define OS_MCMUTEX_03 3
#define OS_MCMUTEX_04 4
#define OS_MCMUTEX_05 5
#endif

#if defined(OS_OPTION_EMT)
#define OS_MCMUTEX_LOCK(emtID, addr)                                         \
    do {                                                                     \
        U32 emtIndex = (emtID);                                              \
        (void)(addr);                                                        \
        SRE_SplLock((struct SreSpinLock *)OS_EMT_GLOBAL_ADDR_GET(emtIndex)); \
    } while (0)

#define OS_MCMUTEX_UNLOCK(emtID, addr)                                         \
    do {                                                                       \
        U32 emtIndex = (emtID);                                                \
        (void)(addr);                                                          \
        SRE_SplUnlock((struct SreSpinLock *)OS_EMT_GLOBAL_ADDR_GET(emtIndex)); \
    } while (0)

#define OS_MCMUTEX_IRQ_LOCK(emtID, addr, intSave)                                           \
    do {                                                                                    \
        U32 emtIndex = (emtID);                                                             \
        (void)(addr);                                                                       \
        (intSave) = SRE_SplIrqLock((struct SreSpinLock *)OS_EMT_GLOBAL_ADDR_GET(emtIndex)); \
    } while (0)

#define OS_MCMUTEX_IRQ_UNLOCK(emtID, addr, intSave)                                          \
    do {                                                                                     \
        U32 emtIndex = (emtID);                                                              \
        (void)(addr);                                                                        \
        SRE_SplIrqUnlock((struct SreSpinLock *)OS_EMT_GLOBAL_ADDR_GET(emtIndex), (intSave)); \
    } while (0)

#define OS_MCMUTEX_DYNC_LOCK(mutexID, intSave) \
    do {                                       \
        (void)(intSave);                       \
        OS_MCMUTEX_LOCK((mutexID), NULL);      \
    } while (0)
#define OS_MCMUTEX_DYNC_UNLOCK(mutexID, intSave) \
    do {                                         \
        (void)(intSave);                         \
        OS_MCMUTEX_UNLOCK((mutexID), NULL);      \
    } while (0)

#define OS_MCMUTEX_DYNC_IRQ_LOCK(mutexID, intSave) OS_MCMUTEX_IRQ_LOCK((mutexID), NULL, (intSave))
#define OS_MCMUTEX_DYNC_IRQ_UNLOCK(mutexID, intSave) OS_MCMUTEX_IRQ_UNLOCK((mutexID), NULL, (intSave))

#elif defined(OS_OPTION_HWSEM)
/* explorer运行需要添加此编译宏 */
#if defined(OS_OPTION_HWSEM_DIRECT)
#define OS_MCMUTEX_LOCK(hwSemID, addr) OsGHwSemPendDirectInner(g_mcGHwsemStartID + (hwSemID))
#define OS_MCMUTEX_UNLOCK(hwSemID, addr) OsGHwSemPostDirectInner(g_mcGHwsemStartID + (hwSemID))
#define OS_MCMUTEX_LOCK_DIRECT(hwSemId, addr) OsGHwSemPendDirectInner(hwSemId)
#define OS_MCMUTEX_UNLOCK_DIRECT(hwSemId, addr) OsGHwSemPostDirectInner(hwSemId)
#else
#ifdef OS_LIB_SIM_FLAG
#define OS_MCMUTEX_LOCK(hwSemID, addr) (void)(addr)
#define OS_MCMUTEX_UNLOCK(hwSemID, addr) (void)(addr)
#define OS_MCMUTEX_IRQ_LOCK(hwSemID, addr, intSave) \
    do {                                            \
        (void)(addr);                               \
        (void)(intSave);                            \
    } while (0)
#define OS_MCMUTEX_IRQ_UNLOCK(hwSemID, addr, intSave) \
    do {                                              \
        (void)(addr);                                 \
        (void)(intSave);                              \
    } while (0)
#else
#define OS_MCMUTEX_LOCK(hwSemID, addr) \
    do {                               \
        (void)(addr);                  \
        OsHwSemPend(hwSemID);          \
    } while (0)
#define OS_MCMUTEX_UNLOCK(hwSemID, addr) \
    do {                                 \
        (void)(addr);                    \
        OsHwSemPost(hwSemID);            \
    } while (0)
#define OS_MCMUTEX_IRQ_LOCK(hwSemID, addr, intSave) \
    do {                                            \
        (void)(addr);                               \
        (intSave) = SRE_IntLock();                  \
        OsHwSemPend(hwSemID);                       \
    } while (0)

#define OS_MCMUTEX_IRQ_UNLOCK(hwSemID, addr, intSave) \
    do {                                              \
        (void)(addr);                                 \
        OsHwSemPost(hwSemID);                         \
        SRE_IntRestore(intSave);                      \
    } while (0)

#endif
#endif

/* symbol 和LP的互斥锁使用的硬件信号量，而且需要动态申请 */
OS_SEC_ALW_INLINE INLINE U32 OsMcmutexDyncCreate(struct HwsemCreatePara *createPara, U32 *hwSemId)
{
    return SRE_HwSemCreate(createPara, hwSemId);
}
#define OS_MCMUTEX_DYNC_LOCK(hwSemId, intSave) (void)SRE_HwSemIrqPend((hwSemId), &(intSave))
#define OS_MCMUTEX_DYNC_UNLOCK(hwSemId, intSave) (void)SRE_HwSemIrqPost((hwSemId), (intSave))
#define OS_MCMUTEX_DYNC_IRQ_LOCK(mutexID, intSave) (void)SRE_HwSemIrqPend((mutexID), &(intSave))
#define OS_MCMUTEX_DYNC_IRQ_UNLOCK(mutexID, intSave) (void)SRE_HwSemIrqPost((mutexID), (intSave))

#else
#if (OS_MAX_CORE_NUM > 1)
#define OS_MCMUTEX_LOCK(hwSemId, addr) \
    do {                               \
        (void)(hwSemId);               \
        OsSplLock(addr);               \
    } while (0)

#define OS_MCMUTEX_UNLOCK(hwSemId, addr) \
    do {                                 \
        (void)(hwSemId);                 \
        OsSplUnlock(addr);               \
    } while (0)

#define OS_MCMUTEX_IRQ_LOCK(hwSemId, addr, intSave) \
    do {                                            \
        (void)(hwSemId);                            \
        (intSave) = SRE_IntLock();                  \
        OsSplLock(addr);                            \
    } while (0)

#define OS_MCMUTEX_IRQ_UNLOCK(hwSemId, addr, intSave) \
    do {                                              \
        (void)(hwSemId);                              \
        OsSplUnlock(addr);                            \
        SRE_IntRestore(intSave);                      \
    } while (0)

#define OS_MCMUTEX_DYNC_LOCK(mutexID, intSave) (void)(intSave)
#define OS_MCMUTEX_DYNC_UNLOCK(mutexID, intSave) (void)(intSave)
#define OS_MCMUTEX_DYNC_IRQ_LOCK(mutexID, intSave) (void)(intSave)
#define OS_MCMUTEX_DYNC_IRQ_UNLOCK(mutexID, intSave) (void)(intSave)
#else
#define OS_MCMUTEX_LOCK(...)
#define OS_MCMUTEX_UNLOCK(...)

#define OS_MCMUTEX_IRQ_LOCK(hwSemId, addr, intSave) \
    do {                                            \
        (intSave) = SRE_IntLock();                  \
        OS_MCMUTEX_LOCK(hwSemId, addr);             \
    } while (0)

#define OS_MCMUTEX_IRQ_UNLOCK(hwSemId, addr, intSave) \
    do {                                              \
        OS_MCMUTEX_UNLOCK(hwSemId, addr);             \
        SRE_IntRestore(intSave);                      \
    } while (0)

#define OS_MCMUTEX_DYNC_LOCK(mutexID, intSave) (void)(intSave)
#define OS_MCMUTEX_DYNC_UNLOCK(mutexID, intSave) (void)(intSave)
#define OS_MCMUTEX_DYNC_IRQ_LOCK(mutexID, intSave) (void)(intSave)
#define OS_MCMUTEX_DYNC_IRQ_UNLOCK(mutexID, intSave) (void)(intSave)
#endif
#endif

#if defined(OS_OPTION_INT_LOCK_TC)
extern uintptr_t OsSplIrqLockTc(volatile U32 *addr);
extern void OsSplIrqUnlockTc(volatile U32 *addr, uintptr_t intSave);
#endif

extern volatile uintptr_t g_mcInitGuard;

#define OS_SPIN_LOCK(lockVar)    OsSplLock((uintptr_t *)(lockVar))
#define OS_SPIN_UNLOCK(lockVar)    OsSplUnlock((uintptr_t *)(lockVar))
#define OS_SPIN_TRY_LOCK(lockVar)    OsSplTryLock((uintptr_t *)(lockVar))

#endif /* SRE_RAW_SPINLOCK_EXTERNAL_H */
