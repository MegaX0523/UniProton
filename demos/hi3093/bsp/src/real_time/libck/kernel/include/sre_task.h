/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 任务模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * @defgroup SRE_task 任务基本功能
 * @ingroup SRE_kernel
 */

#ifndef SRE_TASK_H
#define SRE_TASK_H

#include "sre_base.h"
#include "sre_hook.h"
#include "sre_typedef.h"
#include "./common/os_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup OS_task
 * 任务的消息队列最大的个数。
 *
 */
#define OS_TSK_MSGQ_MAX 15

/*
 * @ingroup OS_task
 * 空任务ID。
 *
 * 调用SRE_TaskYield时，使用OS_TSK_NULL_ID，由OS选择就绪队列中的第一个任务。
 */
#define OS_TSK_NULL_ID 0xFFFFFFFF

/*
 * @ingroup SRE_task
 * 任务栈信息的结构体定义。
 *
 * 保存任务栈的信息。
 */
struct StackInfo {
    /* 栈顶 */
    uintptr_t top;
    /* 栈底 */
    uintptr_t bottom;
    /* 栈当前SP指针值 */
    uintptr_t sp;
    /* 栈当前使用的大小 */
    U32 currUsed;
    /* 栈使用的历史峰值 */
    U32 peakUsed;
    /* 栈是否溢出 */
    bool ovf;
};

/*
 * @ingroup SRE_task
 * 任务创建参数的结构体定义。
 *
 * 传递任务创建时指定的参数信息。
 */
struct TskInitParam {
    /* 任务入口函数 */
    TskEntryFunc taskEntry;
    /* 任务优先级 */
    TskPrior taskPrio;
    /* 消息队列数 */
    U16 queNum;
    /* 任务参数，最多4个 */
    uintptr_t args[4];
    /* 任务栈的大小 */
    U32 stackSize;
    /* 任务名 */
    const char *name;
    /*
     * 本任务的任务栈独立配置起始地址，用户必须对该成员进行初始化，
     * 若配置为0表示从系统内部空间分配，否则用户指定栈起始地址
     */
    uintptr_t stackAddr;
    /* 专属于本任务的私有数据 */
    uintptr_t privateData;
};

/*
 * *@ingroup SRE_task
 * 任务TCB首地址信息结构体。
 */
struct TskTcbAddr {
    /* 任务ID */
    U32 tskId;
    /* 任务TCB首地址 */
    void *tcbAddr;
};

/*
 * @ingroup  SRE_task
 * @brief 创建任务。
 *
 * @par 描述
 * 创建一个任务。在系统OS初始化前创建的任务只是简单地加入就绪队列。
 * 系统初始化后创建的任务，如果优先级高于当前任务且未锁任务，则立即发生任务调度并被运行，否则加入就绪队列。
 *
 * @attention
 * <ul>
 * <li>若指定的任务栈独立配置起始地址不为0，则采用用户配置的独立任务栈进行栈空间分配，</li>
 * <li>并且系统会占用(消息队列个数乘以12字节)的空间用于消息队列头。</li>
 * <li>任务创建时，会对之前自删除任务的任务控制块和任务栈进行回收，用户独立配置的任务栈除外。</li>
 * <li>任务名的最大长度为16字节，含结束符。</li>
 * <li>创建任务时需要配置消息队列数。</li>
 * <li>同一核内任务名不允许重复，且不允许和软中断重名。</li>
 * <li>若指定的任务栈大小为0，则使用配置项#OS_TSK_DEFAULT_STACK_SIZE指定的默认的任务栈大小。</li>
 * <li>任务栈的大小必须按16字节大小对齐。确定任务栈大小的原则是，够用就行：多了浪费，少了任务栈溢出。</li>
 * <li>具体多少取决于需要消耗多少的栈内存，视情况而定：函数调用层次越深，栈内存开销越大，</li>
 * <li>局部数组越大，局部变量越多，栈内存开销也越大。</li>
 * <li>用户配置的任务栈首地址需16字节对齐，且配置的任务栈空间中，os会使用消息队列数乘以消息队列控制块的内存大小。</li>
 * <li>用户配置的任务栈空间需由用户保证其合法性，即对可cache空间的地址用户需要保证其任务栈首地址及栈大小cache</li>
 * <li>line对齐，系统不做对齐处理，并在使用后需由用户进行释放。</li>
 * <li>任务创建时，任务创建参数中的任务栈大小配置建议不要使用最小任务栈大小OS_TSK_MIN_STACK_SIZE，</li>
 * <li>该项只是包含任务上下文预留的栈空间，任务调度时额外的任务开销需要由用户自行保证足够的任务栈空间配置。</li>
 * </ul>
 *
 * @param taskPid   [OUT] 类型#TskHandle *，保存任务PID。
 * @param initParam [IN]  类型#struct TskInitParam *，任务创建参数，
 * 其结构体中的成员参数stackAddr传入时必须进行初始化，若不采用用户配置的独立任务栈进行栈空间分配，
 * 该成员必须初始化为0。
 *
 * @retval #OS_ERRNO_TSK_NO_MEMORY              0x02000800，申请内存失败。
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_STKSZ_NOT_ALIGN        0x02000802，任务栈大小未按16字节大小对齐，HIAVP未按64字节对齐。
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ENTRY_NULL             0x02000804，任务入口函数为空。
 * @retval #OS_ERRNO_TSK_NAME_EMPTY             0x02000805，任务名的指针为空或任务名为空字符串。
 * @retval #OS_ERRNO_TSK_STKSZ_TOO_SMALL        0x02000806，指定的任务栈空间太小。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_SUSPENDED          0x02000809，任务未被挂起。
 * @retval #OS_ERRNO_TSK_FREE_STACK_FAILED      0x02000817，释放任务栈失败。
 * @retval #OS_ERRNO_TSK_TCB_UNAVAILABLE        0x02000811，没有可用的任务控制块资源。
 * @retval #OS_ERRNO_TSK_MSG_Q_TOO_MANY         0x0200081d，任务的MSG队列个数超过15。
 * @retval #OS_ERRNO_HUNT_THREAD_NAME_REPEAT    0x02001f01，创建任务时，线程名重名。
 * @retval #OS_ERRNO_TSK_STACKADDR_NOT_ALIGN    0x02000822，创建任务时，用户配置任务栈地址未16字节对齐。
 * @retval #SRE_OK                              0x00000000，任务创建成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskDelete | SRE_TaskCreateHookAdd | SRE_TaskCreateOnly
 */
extern U32 SRE_TaskCreate(TskHandle *taskPid, struct TskInitParam *initParam);

/*
 * @ingroup  SRE_task
 * @brief 创建任务，但不激活任务。
 *
 * @par 描述
 * 创建一个任务。该任务不加入就绪队列，只处于挂起状态，用户需要激活该任务需要通过调用SRE_TaskResume函数将其激活。
 *
 * @attention
 * <ul>
 * <li>若指定的任务栈独立配置起始地址不为0，则采用用户配置的独立任务栈进行栈空间分配，并且系统会占用(queNum</li>
 * <li>12字节)的空间用于消息队列头。</li>
 * <li>任务创建时，会对之前自删除任务的任务控制块和任务栈进行回收，用户独立配置的任务栈除外。</li>
 * <li>任务名的最大长度为16字节，含结束符。</li>
 * <li>创建任务时需要配置消息队列数。</li>
 * <li>同一核内任务名不允许重复，且不允许和软中断重名。</li>
 * <li>若指定的任务栈大小为0，则使用配置项#OS_TSK_DEFAULT_STACK_SIZE指定的默认的任务栈大小。</li>
 * <li>任务栈的大小必须按16字节大小对齐。确定任务栈大小的原则是，够用就行：多了浪费，少了任务栈溢出。</li>
 * <li>具体多少取决于需要消耗多少的栈内存，视情况而定：函数调用层次越深，栈内存开销越大，</li>
 * <li>局部数组越大，局部变量越多，栈内存开销也越大。</li>
 * <li>用户配置的任务栈首地址需16字节对齐，且配置的任务栈空间中，os会使用消息队列数*消息队列控制块的内存大小。</li>
 * <li>用户配置的任务栈空间需由用户保证其合法性，即对可cache空间的地址用户需要保证其任务栈首地址及栈大小cache</li>
 * <li>line对齐，系统不做对齐处理，并在使用后需由用户进行释放。</li>
 * <li>任务创建时，任务创建参数中的任务栈大小配置建议不要使用最小任务栈大小OS_TSK_MIN_STACK_SIZE，</li>
 * <li>该项只是包含任务上下文预留的栈空间，任务调度时额外的任务开销需要由用户自行保证足够的任务栈空间配置。</li>
 * </ul>
 *
 * @param taskPid   [OUT] 类型#TskHandle *，保存任务PID。
 * @param initParam [IN]  类型#struct TskInitParam *，任务创建参数，
 * 其结构体中的成员参数stackAddr传入时必须进行初始化，若不采用用户配置的独立任务栈进行栈空间分配，
 * 该成员必须初始化为0。
 *
 * @retval #OS_ERRNO_TSK_NO_MEMORY              0x02000800，申请内存失败。
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_STKSZ_NOT_ALIGN        0x02000802，任务栈大小未按16字节大小对齐。
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ENTRY_NULL             0x02000804，任务入口函数为空。
 * @retval #OS_ERRNO_TSK_NAME_EMPTY             0x02000805，任务名的指针为空或任务名为空字符串。
 * @retval #OS_ERRNO_TSK_STKSZ_TOO_SMALL        0x02000806，指定的任务栈空间太小。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_TCB_UNAVAILABLE        0x02000811，没有可用的任务控制块资源。
 * @retval #OS_ERRNO_TSK_MSG_Q_TOO_MANY         0x0200081d，任务的MSG队列个数超过15。
 * @retval #OS_ERRNO_HUNT_THREAD_NAME_REPEAT    0x02001f01，创建任务时，线程名重名.
 * @retval #OS_ERRNO_TSK_STACKADDR_NOT_ALIGN    0x02000822，创建任务时，用户配置任务栈地址未16字节对齐.
 * @retval #SRE_OK                              0x00000000，任务创建成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskDelete | SRE_TaskCreateHookAdd | SRE_TaskCreate
 */
extern U32 SRE_TaskCreateOnly(TskHandle *taskPid, struct TskInitParam *initParam);

/*
 * @ingroup  SRE_task
 * @brief 恢复任务。
 *
 * @par 描述
 * 恢复挂起的任务。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>若任务仍处于延时、阻塞态，则只是取消挂起态，并不加入就绪队列。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TskHandle，任务PID。
 *
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，操作IDLE任务。
 * @retval #OS_ERRNO_TSK_NOT_SUSPENDED          0x02000809，任务未被挂起。
 * @retval #SRE_OK                              0x00000000，恢复任务成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskSuspend
 */
extern U32 SRE_TaskResume(TskHandle taskPid);

/*
 * @ingroup  SRE_task
 * @brief 挂起任务。
 *
 * @par 描述
 * 挂起指定的任务，任务将从就绪队列中被删除。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>若为当前任务且已锁任务，则不能被挂起。</li>
 * <li>IDLE任务不能被挂起。</li>
 * <li>SMP多核并发操作SRE_TaskCoreBind,SRE_TaskDelete,Sre_TaskSuspend时避免关中断调用，</li>
 * <li>否则核间中断无法响应造成死循环。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TskHandle，任务PID。
 *
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_ALREADY_SUSPENDED      0x02000808，任务已被挂起。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，操作IDLE任务。
 * @retval #OS_ERRNO_TSK_SUSPEND_LOCKED         0x03000815，在锁任务的状态下挂起当前任务。
 * @retval #SRE_OK                              0x00000000，挂起任务成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskResume | SRE_TaskLock
 */
extern U32 SRE_TaskSuspend(TskHandle taskPid);

/*
 * @ingroup  SRE_task
 * @brief 删除任务。
 *
 * @par 描述
 * 删除指定的任务，释放任务栈和任务控制块资源。
 *
 * @attention
 * <ul>
 * <li>若为自删除，则任务控制块和任务栈在下一次创建任务时才回收。</li>
 * <li>SMP多核并发操作SRE_TaskCoreBind,SRE_TaskDelete,Sre_TaskSuspend时避免关中断调用，</li>
 * <li>否则核间中断无法响应造成死循环。</li>
 * <li>对于任务自删除，LibCK处理该任务相关的信号量和接收到的消息会强制删除。</li>
 * <li>任务自删除时，删除钩子不允许进行pend信号量、挂起等操作。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TskHandle，任务PID。
 *
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_DELETE_LOCKED          0x0300080b，在锁任务的状态下删除当前任务。
 * @retval #OS_ERRNO_TSK_MSG_NONZERO            0x0200080c，任务待处理的消息数非零。
 * @retval #OS_ERRNO_TSK_HAVE_MUTEX_SEM         0x02000826，任务持有互斥型信号量时删除该任务。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，操作IDLE任务。
 * @retval #OS_ERRNO_TSK_QUEUE_DOING            0x02000823，任务正在操作队列。
 * @retval #SRE_OK                              0x00000000，删除任务成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskCreate | SRE_TaskDeleteHookAdd
 */
extern U32 SRE_TaskDelete(TskHandle taskPid);

/*
 * @ingroup  SRE_task
 * @brief 延迟正在运行的任务。
 *
 * @par 描述
 * 延迟当前运行任务的执行。任务延时等待指定的Tick数后，重新参与调度。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>硬中断或软中断处理中，或已锁任务，则延时操作失败。</li>
 * <li>若传入参数0，且未锁任务调度，则顺取同优先级队列中的下一个任务执行。如没有同级的就绪任务，</li>
 * <li>则不发生任务调度，继续执行原任务。</li>
 * </ul>
 *
 * @param tick [IN]  类型#U32，延迟的Tick数。
 *
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_YIELD_INVALID_TASK     0x0200080f，任务ID不在Yield操作指定的优先级队列中。
 * @retval #OS_ERRNO_TSK_DELAY_IN_INT           0x0300080d，在硬中断或软中断的处理中进行延时操作。
 * @retval #OS_ERRNO_TSK_DELAY_IN_LOCK          0x0200080e，在锁任务的状态下进行延时操作。
 * @retval #OS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK  0x02000810，Yield操作指定的优先级队列中，就绪任务数小于2。
 * @retval #SRE_OK                              0x00000000，任务延时成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskYield
 */
extern U32 SRE_TaskDelay(U32 tick);

/*
 * @ingroup  SRE_task
 * @brief 锁任务调度。
 *
 * @par 描述
 * 锁任务调度。若任务调度被锁，则不发生任务切换。
 *
 * @attention
 * <ul>
 * <li>只是锁任务调度，并不关中断，因此任务仍可被中断打断。</li>
 * <li>执行此函数则锁计数值加1，解锁则锁计数值减1。因此，必须与#SRE_TaskUnlock配对使用。</li>
 * <li>Cortex-RX SMP下，只会锁当前核的任务调度，其他核的调度不受影响。</li>
 * <li>Cortex-RX SMP下，锁任务期间当前核不保证运行的是Top-N优先级任务，直至解锁后OS重新发起任务调度。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskUnlock
 */
extern void SRE_TaskLock(void);

#if defined(OS_OPTION_SMP)
/*
 * @ingroup  SRE_task
 * @brief 内部实现无关中断的锁任务调度。
 *
 * @par 描述
 * 锁任务调度。若任务调度被锁，则不发生任务切换。
 *
 * @attention
 * <ul>
 * <li>调用该接口时请确保外部已关中断，该接口为提升锁任务性能，内部实现无关中断。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskLock
 */
extern void SRE_TaskLockNoIntLock(void);
#endif

/*
 * @ingroup  SRE_task
 * @brief 解锁任务调度。
 *
 * @par 描述
 * 任务锁计数值减1。若嵌套加锁，则只有锁计数值减为0才真正的解锁了任务调度。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>执行此函数则锁计数值加1，解锁则锁计数值减1。因此，必须与#SRE_TaskLock配对使用。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskLock
 */
extern void SRE_TaskUnlock(void);

/*
 * @ingroup  SRE_task
 * @brief 获取当前任务PID。
 *
 * @par 描述
 * 获取处于运行态的任务PID。
 *
 * @attention
 * <ul>
 * <li>硬中断或软中断处理中，也可获取当前任务PID，即被中断打断的任务。</li>
 * <li>在任务切换钩子处理中调用时，获取的是切入任务的ID。</li>
 * </ul>
 *
 * @param taskPid [OUT] 类型#TskHandle *，保存任务PID。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务ID非法。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskStatusGet | SRE_TaskInfoGet
 */
extern U32 SRE_TaskSelf(TskHandle *taskPid);

/*
 * @ingroup  SRE_task
 * @brief 检查任务PID。
 *
 * @par 描述
 * 检查任务PID是否合法。
 *
 * @attention
 * <ul>
 * <li>任务ID并不是从0开始编号的。</li>
 * <li>该接口只支持本核任务PID的检测。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TskHandle，任务PID。
 *
 * @retval #TRUE        1， 任务PID输入合法。
 * @retval #FALSE       0， 任务PID输入不合法。
 *
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskSelf
 */
extern bool SRE_TaskIDCheck(TskHandle taskPid);

/*
 * @ingroup  SRE_task
 * @brief 获取任务状态。
 *
 * @par 描述
 * 获取指定任务的状态。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param taskPid [IN]  类型#TskHandle，任务PID。
 *
 * @retval #(TskStatus)OS_INVALID    返回失败。
 * @retval #任务状态                 返回成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskInfoGet | SRE_TaskContextGet | SRE_TaskPrivateDataGet
 */
extern TskStatus SRE_TaskStatusGet(TskHandle taskPid);

/*
 * @ingroup  SRE_task
 * @brief 获取任务信息。
 *
 * @par 描述
 * 获取指定任务的信息。
 *
 * @attention
 * <ul>
 * <li>若获取当前任务的信息，则只在硬中断、软中断、异常的处理中才有意义，</li>
 * <li>否则获取的SP值和PC值是不准的。(在Xtensa平台下不允许在自身任务中获取上下文信息)</li>
 * <li>由于任务切换时，上下文信息也保存在任务栈中，因此任务信息中的SP是保存上下文之后的实际的SP值。</li>
 * </ul>
 *
 * @param taskPid  [IN]  类型#TskHandle，任务PID。
 * @param taskInfo [OUT] 类型#struct TskInfo *，保存任务信息。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskStatusGet | SRE_TaskContextGet | SRE_TaskPrivateDataGet
 */
extern U32 SRE_TaskInfoGet(TskHandle taskPid, struct TskInfo *taskInfo);

/*
 * @ingroup  SRE_task
 * @brief 获取任务上下文。
 *
 * @par 描述
 * 获取指定任务的上下文。
 *
 * @attention
 * <ul>
 * <li>若获取当前任务的上下文信息，则只在硬中断、软中断、异常的处理中才有意义，否则获取的PC值是不准的。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TskHandle，任务PID。
 * @param context [OUT] 类型#struct TskContext *，保存任务上下文信息。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_CONTEXT_NOT_GETED      0x02000829, 获取当前正在运行任务的任务上下文信息失败。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskStatusGet | SRE_TaskInfoGet | SRE_TaskPrivateDataGet
 */
extern U32 SRE_TaskContextGet(TskHandle taskPid, struct TskContext *context);

/*
 * @ingroup  SRE_task
 * @brief 获取私有数据。
 *
 * @par 描述
 * 获取当前任务的私有数据。
 *
 * @attention
 * <ul>
 * <li>当当前任务正在运行时，调用该接口能获取当前任务的私有数据。</li>
 * <li>当任务被中断打断时，中断里调用该接口能获取被中断打断的任务的私有数据。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval #当前任务的私有数据 任意值，如未设置，则返回的值不确定。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskPrivateDataSet
 */
extern uintptr_t SRE_TaskPrivateDataGet(void);

/*
 * @ingroup  SRE_task
 * @brief 设置私有数据值。
 *
 * @par 描述
 * 设置当前任务的私有数据值。
 *
 * @attention
 * <ul>
 * <li>只能在任务处理中调用。若在中断中设置，则操作的是刚被打断的任务。</li>
 * </ul>
 *
 * @param privateData [IN]  类型#uintptr_t，数据值。
 *
 * @retval 无
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskPrivateDataGet
 */
extern void SRE_TaskPrivateDataSet(uintptr_t privateData);

/*
 * @ingroup  SRE_task
 * @brief 获取优先级。
 *
 * @par 描述
 * 获取指定任务的优先级。
 *
 * @attention 无
 *
 * @param taskPid  [IN]  类型#TskHandle，任务PID。
 * @param taskPrio [OUT] 类型#TskPrior *，保存任务优先级指针。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskPrioritySet
 */
extern U32 SRE_TaskPriorityGet(TskHandle taskPid, TskPrior *taskPrio);

/*
 * @ingroup  SRE_task
 * @brief 设置优先级。
 *
 * @par 描述
 * 设置指定任务的优先级。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>若设置的优先级高于当前运行的任务，则可能引发任务调度。</li>
 * <li>若调整当前运行任务的优先级，同样可能引发任务调度。</li>
 * <li>若任务发生优先级继承，或者任务阻塞在互斥信号量或优先级唤醒模式的信号量上，不可以设置任务的优先级。</li>
 * </ul>
 *
 * @param taskPid  [IN]  类型#TskHandle，任务PID。
 * @param taskPrio [IN]  类型#TskPrior，任务优先级。
 *
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，操作IDLE任务。
 * @retval #OS_ERRNO_TSK_PRIORITY_INHERIT       0x02000824，任务发生优先级继承。
 * @retval #OS_ERRNO_TSK_PEND_ON_MUTEX          0x02000825，任务阻塞在互斥信号量上。
 * @retval #OS_ERRNO_TSK_PRIOR_LOW_THAN_PENDTSK 0x02000828，设置优先级低于阻塞于它持有的互斥信号量的
 * @retval                                                  最高优先级任务的优先级
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskPriorityGet
 */
extern U32 SRE_TaskPrioritySet(TskHandle taskPid, TskPrior taskPrio);

/*
 * @ingroup  SRE_task
 * @brief 调整指定优先级的任务调度顺序。
 *
 * @par 描述
 * 若nextTask为#OS_TSK_NULL_ID，则优先级队列中的第一个就绪任务调整至队尾,
 * 否则，将nextTask指定的任务调整至优先级队列的队首。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>前提是指定优先级的就绪队列至少要有两个就绪任务，否则报错返回。</li>
 * <li>Cortex-RX SMP下，只会对本核(本Run Queue)指定优先级的就绪队列进行调整。</li>
 * </ul>
 *
 * @param taskPrio [IN]  类型#TskPrior，任务taskPrio，指定任务优先级队列。
 * @param nextTask [IN]  类型#TskHandle，任务ID或OS_TSK_NULL_ID。
 * @param yieldTo  [OUT] 类型#TskHandle *，保存被调整到队首的任务PID，可为NULL(不需要保存队首任务PID)。
 *
 * @retval #OS_ERRNO_TSK_PRIOR_ERROR            0x02000803，任务优先级非法。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_YIELD_INVALID_TASK     0x0200080f，任务PID不在Yield操作指定的优先级队列中。
 * @retval #OS_ERRNO_TSK_YIELD_NOT_ENOUGH_TASK  0x02000810，Yield操作指定的优先级队列中，就绪任务数小于2。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskSuspend
 */
extern U32 SRE_TaskYield(TskPrior taskPrio, TskHandle nextTask, TskHandle *yieldTo);

/*
 * @ingroup  SRE_task
 * @brief 查询本核指定任务正在PEND的信号量。
 *
 * @par 描述
 * 根据任务状态和任务控制块，判断任务是否在PEND信号量，以及PEND的信号量ID。
 *
 * @attention
 * <ul>
 * <li>用户应先判断PEND状态，状态为0表明任务没有被信号量阻塞。</li>
 * <li>快速信号量没有信号量ID，若任务阻塞于快速信号量，则返回的信号量ID为#OS_INVALID。</li>
 * </ul>
 *
 * @param taskId    [IN]  类型#TskHandle，任务PID。
 * @param semId     [OUT] 类型#U16 *，任务PEND的信号量ID或者#OS_INVALID。
 * @param pendState [OUT] 类型#U16 *，任务的PEND状态：0，#OS_TSK_FSEM_PEND，#OS_TSK_PEND, #OS_TSK_MCSEM_PEND。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL       0x02000801，指针参数为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID     0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED    0x0200080a，任务未创建。
 * @retval #SRE_OK                      0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemPendListGet | SRE_McSemPendListGet
 */
extern U32 SRE_TaskPendSemGet(TskHandle taskId, U16 *semId, U16 *pendState);

/*
 * @ingroup  SRE_task
 * @brief 查询当前运行任务指定队列的消息待接收个数。
 *
 * @par 描述
 * 查询当前运行任务消息队列queId上待接收消息个数。
 *
 * @attention
 * <ul>
 * <li>6181/6108/6182平台上表示任务上的指定的消息队列的消息数，其他平台不支持多消息队列，</li>
 * <li>该接口表示消息队列上的消息数，入参queId无效。</li>
 * <li>入参queId有效性需由用户保证。</li>
 * </ul>
 *
 * @param queId [IN]  类型#U32，6181/6108/6182平台上表示任务上的消息队列，其他平台无效入参。
 *
 * @retval  当前运行任务指定队列的消息待接收个数。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_TaskMsgNumGet(U32 queId);

/*
 * @ingroup  SRE_task
 * @brief 获取指定任务的堆栈信息。
 *
 * @par 描述
 * 获取指定任务的堆栈信息。
 *
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param taskPid   [IN]  类型#TskHandle，任务PID。
 * @param stackInfo [OUT] 类型#struct StackInfo *，任务栈信息。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL      0x02000801，入参指针为空。
 * @retval #OS_ERRNO_TSK_ID_INVALID    0x02000807，入参任务ID不合法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED   0x0200080a，任务未创建。
 * @retval #SRE_OK                     0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_TaskStackInfoGet(TskHandle taskPid, struct StackInfo *stackInfo);

#if defined(OS_OPTION_VEC_LAZY_SAVE)

/*
 * @ingroup  SRE_task
 * @brief 给任务分配矢量操作上下文数据保存区
 *
 * @par 描述
 * 给指定任务分配矢量操作上下文数据保存区
 *
 * @attention
 * <ul>
 * <li>该操作适用于SD6182/SD6157平台。</li>
 * <li>保存矢量操作上下文用到核的双LoadStore指令，建议配置的SaveAreaA/SaveAreaB在不同的Memory Bank中。</li>
 * <li>矢量上下文数据保存区大小需由用户根据芯片实际存在的矢量寄存器总大小确定。</li>
 * </ul>
 *
 * @param taskId [IN]  类型#TskHandle，任务ID号
 * @param param  [IN]  类型#struct CpSaveCfgParam *，参数结构指针
 *
 * @retval #OS_ERRNO_TSK_PARA_NULL              0x0200082a，入参为NULL。
 * @retval #OS_ERRNO_TSK_CP_SAVE_AREA_NULL      0x0200081e，任务的coprocessor上下文保存区域的地址为空指针。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，输入任务ID非法。
 * @retval #OS_ERRNO_TSK_CP_SAVE_AREA_NOT_ALIGN 0x0200801b，保存区域未按16字节对齐。
 * @retval #SRE_OK                              0x00000000，成功。
 *
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskCpEntry | SRE_TaskCpExit
 */
extern U32 SRE_TaskCpSaveCfg(TskHandle taskId, struct CpSaveCfgParam *param);

/*
 * @ingroup  SRE_task
 * @brief 任务中执行矢量操作的入口
 *
 * @par 描述
 * 任务中执行矢量操作的入口, 矢量操作开始之前需调用此接口。
 *
 * @attention
 * <ul>
 * <li>该操作适用于SD6182/SD6157平台。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 *
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskCpExit
 */
extern void SRE_TaskCpEntry(void);

/*
 * @ingroup  SRE_task
 * @brief 任务中执行矢量操作的出口
 *
 * @par 描述
 * 任务中执行矢量操作的出口，矢量操作完成之后需调用此接口。
 *
 * @attention
 * <ul>
 * <li>该操作适用于SD6182/SD6157平台。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 *
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskCpEntry
 */
extern void SRE_TaskCpExit(void);

#else
OS_SEC_ALW_INLINE INLINE U32 SRE_TaskCpSaveCfg(TskHandle taskId, struct CpSaveCfgParam *param)
{
    (void)taskId;
    (void)param;
    return SRE_OK;
}

OS_SEC_ALW_INLINE INLINE void SRE_TaskCpEntry(void)
{
    return;
}

OS_SEC_ALW_INLINE INLINE void SRE_TaskCpExit(void)
{
    return;
}
#endif

/*
 * @ingroup  SRE_task
 * @brief 获取指定PID的任务TCB首地址。
 *
 * @par 描述
 * 获取指定PID的任务TCB首地址。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>不能获取ID不合法的任务TCB地址。</li>
 * <li>若获取ID的任务没有创建，返回失败。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TskHandle，任务PID。
 * @param tcbAddr [OUT] 类型#uintptr_t *，保存任务TCB地址。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，tskTcbAddr为NULL。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #SRE_OK                              0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskIDList
 */
extern U32 SRE_TaskTcbAddrGet(TskHandle taskPid, uintptr_t *tcbAddr);

/*
 * @ingroup  SRE_task
 * @brief 获取所有已创建的任务PID信息。
 *
 * @par 描述
 * 获取所有已创建的任务PID信息，按照任务ID从小到大的排序。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * </ul>
 *
 * @param maxNum  [I/O] 类型#U16 *，元素个数，输入/输出参数，输入：缓冲区taskPid可容纳的元素个数；
 * 输出：填写在缓冲区taskPid中有效的元素个数。
 * @param taskPid [OUT] 类型#U32 *，缓冲区，输出参数，用于保存所有任务的PID。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL           0x02000801，获取所有任务的PID信息时，输入入参为NULL。
 * @retval #OS_ERRNO_TSK_INPUT_NUM_ERROR    0x02000821，获取所有任务的PID信息时，任务数为0个。
 * @retval #SRE_OK                          0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see 无
 */
extern U32 SRE_TaskIDList(U16 *maxNum, U32 *taskPid);

/*
 * @ingroup  SRE_task
 * @brief 查询任务名。
 *
 * @par 描述
 * 根据任务PID，查询任务名。
 *
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>不能查询ID不合法的任务名。</li>
 * <li>若查询没有创建的任务名，查询失败。</li>
 * </ul>
 *
 * @param taskId  [IN]  类型#TskHandle，任务ID。
 * @param name    [OUT] 类型#char **，保存任务名字符串的首地址。
 *
 * @retval #OS_ERRNO_TSK_PTR_NULL               0x02000801，  保存任务名指针为NULL。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，  任务未创建。
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，  任务ID非法。
 * @retval #SRE_OK                              0x00000000，  查询成功。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see  SRE_TaskIDList
 */
extern U32 SRE_TaskNameGet(TskHandle taskId, char **name);

/*
 * @ingroup  SRE_task
 * @brief 注册任务切换钩子。
 *
 * @par 描述
 * 注册任务切换钩子函数。钩子函数在切入新任务前被调用。
 *
 * @attention
 * <ul>
 * <li>不同钩子函数间执行的先后顺序，不应当存在依赖关系。</li>
 * <li>不应在任务切换钩子里进行可能引起任务调度的处理，如：任务延时、P信号量等。</li>
 * </ul>
 *
 * @param hook [IN]  类型#TskSwitchHook，任务切换钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，添加钩子成功。
 * @retval 其他                          添加钩子失败。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskSwitchHookDelete | SRE_TaskDeleteHookAdd | SRE_TaskCreateHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_TaskSwitchHookAdd(TskSwitchHook hook)
{
    return SRE_HookAdd(OS_HOOK_TSK_SWITCH, (OsVoidFunc)(uintptr_t)hook);
}

/*
 * @ingroup  SRE_task
 * @brief 取消任务切换钩子。
 *
 * @par 描述
 * 取消指定的任务切换钩子。钩子函数在切入新任务前被调用。
 *
 * @attention  无
 *
 * @param hook [IN]  类型#TskSwitchHook，任务切换钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，删除钩子成功。
 * @retval 其他                          删除钩子失败。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskSwitchHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_TaskSwitchHookDelete(TskSwitchHook hook)
{
    return SRE_HookDel(OS_HOOK_TSK_SWITCH, (OsVoidFunc)(uintptr_t)hook);
}

/*
 * @ingroup  SRE_task
 * @brief 注册任务创建钩子。
 *
 * @par 描述
 * 注册任务创建钩子函数,钩子函数在任务创建成功后被调用。
 *
 * @attention
 * <ul>
 * <li>不应在任务创建钩子里创建任务。</li>
 * </ul>
 *
 * @param hook [IN]  类型#TskCreateHook，任务创建钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，添加钩子成功。
 * @retval 其他                          添加钩子失败。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskCreateHookDelete | SRE_TaskDeleteHookAdd | SRE_TaskSwitchHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_TaskCreateHookAdd(TskCreateHook hook)
{
    return SRE_HookAdd(OS_HOOK_TSK_CREATE, (OsVoidFunc)(uintptr_t)hook);
}

/*
 * @ingroup  SRE_task
 * @brief 取消任务创建钩子。
 *
 * @par 描述
 * 取消指定的任务创建钩子函数,钩子函数在任务创建成功后被调用。
 *
 * @attention 无
 *
 * @param hook [IN]  类型#TskCreateHook，任务创建钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，删除钩子成功。
 * @retval 其他                          删除钩子失败。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskCreateHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_TaskCreateHookDelete(TskCreateHook hook)
{
    return SRE_HookDel(OS_HOOK_TSK_CREATE, (OsVoidFunc)(uintptr_t)hook);
}

/*
 * @ingroup  SRE_task
 * @brief 注册任务删除钩子。
 *
 * @par 描述
 * 注册任务删除钩子函数,钩子函数在资源回收前被调用。
 *
 * @attention
 * <ul>
 * <li>任务删除钩子中不允许进行pend信号量操作。</li>
 * </ul>
 *
 * @param hook [IN]  类型#TskDeleteHook，任务删除钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，添加钩子成功。
 * @retval 其他                          添加钩子失败。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskDeleteHookDelete | SRE_TaskCreateHookAdd | SRE_TaskSwitchHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_TaskDeleteHookAdd(TskDeleteHook hook)
{
    return SRE_HookAdd(OS_HOOK_TSK_DELETE, (OsVoidFunc)(uintptr_t)hook);
}

/*
 * @ingroup  SRE_task
 * @brief 取消任务删除钩子。
 *
 * @par 描述
 * 取消指定的任务删除钩子,钩子函数在资源回收前被调用。
 *
 * @attention
 * <ul>
 * <li>不应在任务删除钩子里删除任务。</li>
 * </ul>
 *
 * @param hook [IN]  类型#TskDeleteHook，任务删除钩子函数。
 *
 * @retval #SRE_OK                       0x00000000，删除钩子成功。
 * @retval 其他                          删除钩子失败。
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskDeleteHookAdd
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_TaskDeleteHookDelete(TskDeleteHook hook)
{
    return SRE_HookDel(OS_HOOK_TSK_DELETE, (OsVoidFunc)(uintptr_t)hook);
}

// 支持SMP
#if defined(OS_OPTION_SMP)
/*
 * @ingroup  SRE_task
 * @brief SMP调度时任务核绑定。
 *
 * @par 描述
 * 将任务绑定到部分核上，使得任务在OS调度时只会在指定的核上发生迁移。
 *
 * @attention
 * <ul>
 * <li>如果任务正在运行，会使得运行任务也会立即发生迁移，迁移至目的核后可能使得该任务阻塞。</li>
 * <li>因为任务绑定的关系，会使得系统里正在运行的任务可能不是就绪的最高优先级任务。</li>
 * <li>多核并发操作SRE_TaskCoreBind,SRE_TaskDelete,Sre_TaskSuspend时避免关中断调用，</li>
 * <li>否则核间中断无法响应造成死循环。如果任务是长期绑定的，为避免任务运行时的迁移，</li>
 * <li>建议任务的创建顺序为SRE_TaskCreateOnly,SRE_TaskCoreBind,SRE_TaskResume.</li>
 * </ul>
 *
 * @param taskPid  [IN]  类型#TskHandle，待绑定的任务句柄。
 * @param coreMask [IN]  类型#U32，待绑定的目标核掩码。注意每bit位表示一个对应核号，
 * 例如绑定0核和1核的核掩码为二进制0b'11，即0x3.
 *
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，不能对Idle任务进行绑定。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #OS_ERRNO_TSK_BIND_CORE_INVALID      0x0200082b，核掩码非法。
 * @retval #SRE_OK                              0x00000000，操作成功
 *
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskCoreUnBind
 */
extern U32 SRE_TaskCoreBind(TskHandle taskPid, U32 coreMask);

/*
 * @ingroup  SRE_task
 * @brief SMP调度时任务核解绑定。
 *
 * @par 描述
 * 将任务绑定去解绑，恢复任务的可运行核为所有核。
 *
 * @attention
 * <ul>
 * <li>多核并发操作SRE_TaskCoreBind,SRE_TaskDelete,Sre_TaskSuspend时避免关中断调用，</li>
 * <li>否则核间中断无法响应造成死循环。</li>
 * <li>任务解绑会尝试发起一次调度，保证系统正在运行的是未绑定的任务里最高优先级的任务。</li>
 * </ul>
 *
 * @param taskPid [IN]  类型#TskHandle，待解绑的任务句柄。
 *
 * @retval #OS_ERRNO_TSK_ID_INVALID             0x02000807，任务PID非法。
 * @retval #OS_ERRNO_TSK_OPERATE_IDLE           0x02000814，不能对Idle任务进行解绑。
 * @retval #OS_ERRNO_TSK_NOT_CREATED            0x0200080a，任务未创建。
 * @retval #SRE_OK                              0x00000000，操作成功
 *
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskCoreBind
 */
extern U32 SRE_TaskCoreUnBind(TskHandle taskPid);

/*
 * @ingroup  SRE_task
 * @brief 获取指定核正在运行的任务ID。
 *
 * @par 描述
 * 获取指定核正在运行的任务ID。
 *
 * @attention
 * <ul>
 * <li>只有Cortex-RX的SMP平台支持该接口。</li>
 * </ul>
 *
 * @param coreId  [IN]  类型#U32，查询的目标核ID。
 * @param taskPid [OUT] 类型#TskHandle *，保存任务PID。
 *
 * @retval #OS_ERRNO_TSK_GET_CURRENT_COREID_INVALID  0x0200082d，核号非法。
 * @retval #OS_ERRNO_TSK_ID_INVALID                  0x02000807，任务ID非法。
 * @retval #OS_ERRNO_TSK_DESTCORE_NOT_RUNNING        0x0200082e，任务未运行。
 * @retval #SRE_OK                                   0x00000000，操作成功
 *
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskSelf
 */
extern U32 SRE_TaskGetCurrentOnCore(U32 coreId, TskHandle *taskPid);

/*
 * @ingroup  SRE_task
 * @brief 获取指定核运行队列的就绪任务个数。
 *
 * @par 描述
 * 获取指定核运行队列的就绪任务个数。
 *
 * @attention
 * <ul>
 * <li>只有Cortex-RX的SMP平台支持该接口。</li>
 * </ul>
 *
 * @param coreId    [IN]  类型#U32，查询的目标核ID。
 * @param nrRunning [OUT] 类型#U32 *，保存就绪任务个数。
 *
 * @retval #OS_ERRNO_TSK_GET_CURRENT_COREID_INVALID  0x0200082d，核号非法。
 * @retval #SRE_OK                                   0x00000000，操作成功
 *
 * @par 依赖
 * <ul><li>sre_task.h：该接口声明所在的头文件。</li></ul>
 * @see NA
 */
extern U32 SRE_TaskGetNrRunningOnCore(U32 coreId, U32 *nrRunning);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_TASK_H */
