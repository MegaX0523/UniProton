/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 信号量模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * @defgroup OS_sem 信号量
 * @ingroup SRE_comm
 */

#ifndef SRE_SEM_H
#define SRE_SEM_H

#include "sre_base.h"
#include "./common/os_sem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*
 * @ingroup OS_sem
 * 信号量不被任何任务持有，处于空闲状态。
 */
#define OS_INVALID_OWNER_ID 0xFFFFFFFE

/*
 * @ingroup OS_sem
 * 信号量句柄类型定义。
 */
typedef U16 SemHandle;

/*
 * @ingroup OS_sem
 * 信号量类型。
 */
enum SemType {
    SEM_TYPE_COUNT, /* 计数型信号量 */
    SEM_TYPE_BIN /* 二进制信号量 */
};

/*
 * @ingroup SRE_sem
 * 信号量模块获取信号量详细信息时的信息结构体。
 */
struct SemInfo {
    /* 信号量计数 */
    U32 count;
    /* 信号量占用者，对于计数型信号量，记录的是最后一次信号量获得者；如果没有被任务获得，则为OS_THREAD_ID_INVALID */
    U32 owner;
    /* 信号量唤醒方式，为SEM_MODE_FIFO或SEM_MODE_PRIOR */
    enum SemMode mode;
    /* 信号量类型，为SEM_TYPE_COUNT（计数型）或SEM_TYPE_BIN（二进制） */
    enum SemType type;
};

/*
 * @ingroup SRE_sem
 * @brief 创建一个计数型信号量。
 *
 * @par 描述
 * 根据用户指定的计数值，创建一个计数型信号量，设定初始计数器数值，唤醒方式为FIFO。
 * @attention
 * <ul><li>创建是否成功会受到"核内信号量裁剪开关"和"最大支持信号量"配置项的限制。</li></ul>
 *
 * @param count     [IN]  类型#U32，计数器初始值，取值范围为[0, 0xFFFFFFFE]。
 * @param semHandle [OUT] 类型#SemHandle *，输出信号量句柄。
 *
 * @retval #OS_ERRNO_SEM_OVERFLOW 0x02000f08，计数器初始值超出范围。
 * @retval #OS_ERRNO_SEM_ALL_BUSY 0x02000f03，没有空闲信号量，建议增加"最大支持信号量"配置。
 * @retval #OS_ERRNO_SEM_PTR_NULL 0x02000f02，入参指针为空。
 * @retval #SRE_OK                0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemDelete | SRE_SemCCreate | SRE_SemBCreate
 */
extern U32 SRE_SemCreate(U32 count, SemHandle *semHandle);

/*
 * @ingroup SRE_sem
 * @brief 创建一个计数型信号量并指定其唤醒方式。
 *
 * @par 描述
 * 创建一个计数信号量，设定初始计数器数值，并指定其唤醒方式。
 * @attention
 * <ul><li>创建是否成功会受到"最大支持信号量"配置项的限制。</li></ul>
 *
 * @param count     [IN]  类型#U32，计数器初始值，取值范围为[0, 0xFFFFFFFE]。
 * @param semHandle [OUT] 类型#SemHandle *，输出信号量句柄。
 * @param mode      [IN]  类型#enum SemMode，信号量模式，取值范围为SEM_MODE_FIFO或者SEM_MODE_PRIOR。
 *
 * @retval #OS_ERRNO_SEM_OVERFLOW     0x02000f08，计数器初始值超出范围。
 * @retval #OS_ERRNO_SEM_ALL_BUSY     0x02000f03，没有空闲信号量，建议增加"最大支持信号量"配置。
 * @retval #OS_ERRNO_SEM_PTR_NULL     0x02000f02，入参指针为空。
 * @retval #OS_ERRNO_SEM_MODE_INVALID 0x02000f13，指定的唤醒模式非法。
 * @retval #SRE_OK                    0x00000000，操作成功。
 * @par 依赖
 * <ul><li>SRE_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemDelete | SRE_SemCreate | SRE_SemBCreate
 */
extern U32 SRE_SemCCreate(U32 count, SemHandle *semHandle, enum SemMode mode);

/*
 * @ingroup SRE_sem
 * @brief 创建一个二进制信号量。
 *
 * @par 描述
 * 创建一个二进制信号量，设定初始计数器数值。
 * @attention
 * <ul><li>创建是否成功会受到"最大支持信号量"配置项的限制。</li></ul>
 *
 * @param count     [IN]  类型#U32，计数器初始值，取值范围为OS_SEM_FULL或OS_SEM_EMPTY。
 * @param semHandle [OUT] 类型#SemHandle *，输出信号量句柄。
 * @param mode      [IN]  类型#enum SemMode，信号量模式，取值范围为SEM_MODE_FIFO或者SEM_MODE_PRIOR。
 *
 * @retval #OS_ERRNO_SEM_BIN_VALUE_ERR     0x02000f14，计数器初始值错误。
 * @retval #OS_ERRNO_SEM_ALL_BUSY          0x02000f03，没有空闲信号量，建议增加"最大支持信号量"配置。
 * @retval #OS_ERRNO_SEM_PTR_NULL          0x02000f02，入参指针为空。
 * @retval #OS_ERRNO_SEM_MODE_INVALID      0x02000f13，指定的唤醒模式非法。
 * @retval #OS_ERRNO_SEM_MUTEX_SHOULD_PRIO 0x02000f1c，互斥型信号量的唤醒方式只能为优先级方式。
 * @retval #SRE_OK                         0x00000000，操作成功。
 * @par 依赖
 * <ul><li>SRE_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemDelete
 */
extern U32 SRE_SemBCreate(U32 count, SemHandle *semHandle, enum SemMode mode);

/*
 * @ingroup SRE_sem
 * @brief 删除一个信号量。
 *
 * @par 描述
 * 删除用户传入的信号量句柄指定的信号量，如果有任务阻塞于该信号量，则删除失败。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param semHandle [IN]  类型#SemHandle，信号量句柄，来源于信号量创建成功的输出值。
 *
 * @retval #OS_ERRNO_SEM_INVALID 0x02000f01，信号量句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_SEM_PENDED  0x02000f09，有任务阻塞于该信号量，禁止删除。
 * @retval #SRE_OK               0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemCreate
 */
extern U32 SRE_SemDelete(SemHandle semHandle);

/*
 * @ingroup SRE_sem
 * @brief 设置一个信号量的最大值。
 *
 * @par 描述
 * 设置一个信号量计数的最大值。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param semHandle [IN]  类型#SemHandle，信号量句柄，来源于信号量创建成功的输出值。
 * @param maxCount  [IN]  类型#U32，待设置的最大计数值，不能小于当前计数值，且不能大于0xFFFFFFFE。
 *
 * @retval #OS_ERRNO_SEM_INVALID                       0x02000f01，信号量句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_SEM_MAXCOUNT_INVALID              0x02000f15，设置的最大值非法。
 * @retval #OS_ERRNO_SEM_BIN_NO_MAXCOUNT               0x02000f17，对二进制信号量设置最大值。
 * @retval #OS_ERRNO_SEM_MAXCOUNT_GREATER_THAN_CURRENT 0x02000f16，信号量设置计数最大值时指定的最大值小于当前计数值。
 * @retval #SRE_OK                                     0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemCreate | SRE_SemReset
 */
extern U32 SRE_SemCMaxCountSet(SemHandle semHandle, U32 maxCount);

/*
 * @ingroup SRE_sem
 * @brief 重设信号量计数器数值函数。
 *
 * @par 描述
 * 根据用户输入信号量句柄和计数值，重设信号量计数器数值。
 * @attention
 * <ul><li>如果有任务阻塞于该信号量，则重设失败。</li></ul>
 *
 * @param semHandle [IN]  类型#SemHandle，信号量句柄，来源于信号量创建成功的输出值。
 * @param count     [IN]  类型#U32，计数器设定值，取值范围为[0, 0xFFFFFFFE]。
 *
 * @retval #OS_ERRNO_SEM_INVALID             0x02000f01，信号量句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_SEM_OVERFLOW            0x02000f08，计数器设定值超过OS_SEM_COUNT_MAX。
 * @retval #OS_ERRNO_SEM_BIN_CANNOT_RESET    0x02000f1a，对二进制型信号量重设当前计数值。
 * @retval #OS_ERRNO_SEM_PENDED              0x02000f09，有任务阻塞于该信号量，禁止重设。
 * @retval #OS_ERRNO_SEM_OVER_MAXCOUNT       0x02000f18，计数器设定值超过设置的最大值。
 * @retval #SRE_OK                           0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemCreate | SRE_SemCountGet | SRE_SemCMaxCountSet
 */
extern U32 SRE_SemReset(SemHandle semHandle, U32 count);

/*
 * @ingroup SRE_sem
 * @brief 获取信号量计数器数值。
 *
 * @par 描述
 * 根据用户输入信号量句柄和计数值，获取信号量计数器数值。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param semHandle [IN]  类型#SemHandle，信号量句柄，来源于信号量创建成功的输出值。
 * @param semCnt    [OUT] 类型#U32 *，保存信号量计数值指针。
 *
 * @retval #OS_ERRNO_SEM_INVALID                 0x02000f01，信号量句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_SEM_COUNT_GET_PTR_NULL      0x02000f1d，获取当前信号量计数时传入的出参为NULL。
 * @retval #SRE_OK                               0x00000000，获取信号量计数器值成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemCreate | SRE_SemReset | SRE_SemInfoGet
 */
extern U32 SRE_SemCountGet(SemHandle semHandle, U32 *semCnt);

/*
 * @ingroup SRE_sem
 * @brief 等待一个信号量。
 *
 * @par 描述
 * 等待用户传入信号量句柄指定的信号量，若其计数器值大于0，则直接减1返回成功。否则任务阻塞，
 * 等待其他线程发布该信号量，等待超时时间可设定。
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>等待时间可以选择零等待、等待特定时间、永久等待。</li>
 * <li>该接口只能被任务调用。</li>
 * <li>在锁任务情况下，用户要PEND信号量，要保证当前有可用信号量资源。</li>
 * </ul>
 *
 * @param semHandle [IN]  类型#SemHandle，信号量句柄，来源于信号量创建成功的输出值。
 * @param timeout   [IN]  类型#U32，等待时间限制，单位为tick，取值范围为[0, 0xffffffff]。#OS_NO_WAIT或0表示不等待，
 * #OS_WAIT_FOREVER或0xffffffff表示永久等待。
 *
 * @retval #OS_ERRNO_SEM_INVALID      0x02000f01，信号量句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_SEM_UNAVAILABLE  0x02000f04，信号量零等待时获取失败。
 * @retval #OS_ERRNO_SEM_PEND_INTERR  0x02000f05，中断处理函数禁止发生阻塞。
 * @retval #OS_ERRNO_SEM_PEND_IN_LOCK 0x02000f06，系统可用资源为0且任务切换锁定时，禁止任务发生阻塞。
 * @retval #OS_ERRNO_SEM_TIMEOUT      0x02000f07，信号量等待超时。
 * @retval #SRE_OK                    0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemPost
 */
extern U32 SRE_SemPend(SemHandle semHandle, U32 timeout);

/*
 * @ingroup SRE_sem
 * @brief 发布指定的信号量。
 *
 * @par 描述
 * 发布指定的信号量，若没有任务等待该信号量，则直接将计数器加1返回。
 * 否则根据唤醒方式唤醒相应的阻塞任务，FIFO方式唤醒最早阻塞的任务，优先级方式唤醒阻塞在此信号量的最高优先级任务。
 * @attention
 * <ul>
 * <li>在osStart之前不能调用该接口。</li>
 * <li>在未锁任务的情况下，如果唤醒的任务优先级高于当前任务，则会立刻发生任务切换。</li>
 * <li>发生任务切换时，如果支持优先级唤醒方式，且创建信号量时指定唤醒方式为优先级，</li>
 * <li>则唤醒阻塞在该信号量上的最高优先级任务。</li>
 * </ul>
 * @param semHandle [IN]  类型#SemHandle，信号量句柄，来源于信号量创建成功的输出值。
 *
 * @retval #OS_ERRNO_SEM_INVALID              0x02000f01，信号量句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_SEM_OVERFLOW             0x02000f08，信号量加操作后溢出。
 * @retval #OS_ERRNO_SEM_MUTEX_POST_INTERR    0x02000f0a，中断中释放互斥型信号量。
 * @retval #OS_ERRNO_SEM_MUTEX_NOT_OWNER_POST 0x02000f0b，非互斥信号量的持有者在释放此互斥信号量。
 * @retval #SRE_OK                            0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemPend
 */
extern U32 SRE_SemPost(SemHandle semHandle);

/*
 * @ingroup SRE_sem
 * @brief 获取阻塞在指定核内信号量上的任务PID清单。
 *
 * @par 描述
 * 根据用户指定的核内信号量句柄，获取阻塞在指定核内信号量上的任务PID清单。
 * 若有任务阻塞于该核内信号量，则返回阻塞于该核内信号量的任务数目，以及相应任务的PID。
 * 若没有任务阻塞于该核内信号量，则返回阻塞于该核内信号量的任务数目为0。
 * @attention
 * <ul><li>用户应保证存储任务PID清单的内存空间足够大，建议将bufLen配置为(#OS_TSK_MAX_SUPPORT_NUM + 1) 4bytes。</li></ul>
 *
 * @param semHandle [IN]  类型#SemHandle，信号量句柄，来源于信号量创建成功的输出值。
 * @param tskCnt    [OUT] 类型#U32 *，阻塞于该核内信号量的任务数。
 * @param pidBuf    [OUT] 类型#U32 *，由用户指定的内存区域首地址，用于存储阻塞于指定核内信号量的任务PID。
 * @param bufLen    [IN]  类型#U32，用户指定的内存区域的长度（单位：字节）。
 *
 * @retval #OS_ERRNO_SEM_INVALID              0x02000f01，信号量句柄为非法值，或已被删除。
 * @retval #OS_ERRNO_SEM_INPUT_ERROR          0x02000f12，指针为空或者bufLen小于4。
 * @retval #OS_ERRNO_SEM_INPUT_BUF_NOT_ENOUGH 0x02000f11，指定的内存空间不足，但内存中的任务PID有效。
 * @retval #SRE_OK                            0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TaskPendSemGet | SRE_McSemPendListGet | SRE_SemInfoGet
 */
extern U32 SRE_SemPendListGet(SemHandle semHandle, U32 *tskCnt, U32 *pidBuf, U32 bufLen);

/*
 * @ingroup SRE_sem
 * @brief 获取信号量详细信息:信号量当前计数值，信号量持有者(最后一次pend成功的线程ID)，信号量唤醒方式，信号量类型。
 *
 * @par 描述
 * 根据用户输入信号量句柄获取信号量详细信息。
 * @attention
 * <ul><li>无</li></ul>
 *
 * @param semHandle [IN]  类型#SemHandle，信号量句柄，来源于信号量创建成功的输出值。
 * @param semInfo   [OUT] 类型#struct SemInfo *，信号量详细信息:count--信号量计数，owner--信号量占用者，
 * mode--信号量唤醒方式，type--信号量类型。
 *
 * @retval #OS_ERRNO_SEM_INFO_NULL       0x02000f19，传入的出参结构体指针为NULL。
 * @retval #OS_ERRNO_SEM_INVALID         0x02000f01，信号量句柄非法或已被删除。
 * @retval #SRE_OK                       0x00000000，获取信号量计数器值成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemCountGet | SRE_SemPendListGet
 */
extern U32 SRE_SemInfoGet(SemHandle semHandle, struct SemInfo *semInfo);

#if defined(OS_OPTION_SEM_DBG)

/*
 * @ingroup SRE_sem
 * @brief 创建一个二进制信号量并记录用户传入cookie。
 *
 * @par 描述
 * 创建一个二进制信号量，设定初始计数器数值，记录用户传入cookie作信号量分析。
 * @attention
 * <ul><li>创建是否成功会受到"最大支持信号量"配置项的限制。</li></ul>
 *
 * @param count     [IN]   类型#U32，计数器初始值，取值范围为OS_SEM_FULL或OS_SEM_EMPTY。
 * @param semHandle [OUT]  类型#SemHandle *，输出信号量句柄。
 * @param mode      [IN]   类型#enum SemMode，信号量模式，取值范围为SEM_MODE_FIFO或者SEM_MODE_PRIOR。
 * @param cookie    [IN]   类型#U32，传入的数值，OS做记录。
 *
 * @retval #OS_ERRNO_SEM_BIN_VALUE_ERR     0x02000f14，计数器初始值错误。
 * @retval #OS_ERRNO_SEM_ALL_BUSY          0x02000f03，没有空闲信号量，建议增加"最大支持信号量"配置。
 * @retval #OS_ERRNO_SEM_PTR_NULL          0x02000f02，入参指针为空。
 * @retval #OS_ERRNO_SEM_MODE_INVALID      0x02000f13，指定的唤醒模式非法。
 * @retval #OS_ERRNO_SEM_MUTEX_SHOULD_PRIO 0x02000f1c，互斥型信号量的唤醒方式只能为优先级方式。
 * @retval #SRE_OK                         0x00000000，操作成功。
 * @par 依赖
 * <ul><li>SRE_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemBCreate
 */
extern U32 SRE_SemBCreateDbg(U32 count, SemHandle *semHandle, enum SemMode mode, U32 cookie);

/*
 * @ingroup SRE_sem
 * @brief 创建一个计数型信号量并记录用户cookie，指定其唤醒方式。
 *
 * @par 描述
 * 创建一个计数信号量，设定初始计数器数值。
 * @attention
 * <ul><li>创建是否成功会受到"最大支持信号量"配置项的限制。</li></ul>
 *
 * @param count     [IN]  类型#U32，计数器初始值，取值范围为[0, 0xFFFFFFFE]。
 * @param semHandle [OUT]  类型#SemHandle *，输出信号量句柄。
 * @param mode      [IN] 类型#enum SemMode，信号量模式，取值范围为SEM_MODE_FIFO或者SEM_MODE_PRIOR。
 * @param cookie    [IN]  类型#U32，传入的数值，OS做记录。
 *
 * @retval #OS_ERRNO_SEM_OVERFLOW     0x02000f08，计数器初始值超出范围。
 * @retval #OS_ERRNO_SEM_ALL_BUSY     0x02000f03，没有空闲信号量，建议增加"最大支持信号量"配置。
 * @retval #OS_ERRNO_SEM_PTR_NULL     0x02000f02，入参指针为空。
 * @retval #OS_ERRNO_SEM_MODE_INVALID 0x02000f13，指定的唤醒模式非法。
 * @retval #SRE_OK                    0x00000000，操作成功。
 * @par 依赖
 * <ul><li>SRE_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemDelete | SRE_SemCreate | SRE_SemBCreate
 */
extern U32 SRE_SemCCreateDbg(U32 count, SemHandle *semHandle, enum SemMode mode, U32 cookie);

/*
 * @ingroup SRE_sem
 * @brief 创建一个计数型信号量并记录用户cookie。
 *
 * @par 描述
 * 根据用户指定的计数值，创建一个计数型信号量，设定初始计数器数值，唤醒方式为FIFO。
 * @attention
 * <ul><li>创建是否成功会受到"核内信号量裁剪开关"和"最大支持信号量"配置项的限制。</li></ul>
 *
 * @param count     [IN]  类型#U32，计数器初始值，取值范围为[0, 0xFFFFFFFE]。
 * @param semHandle [OUT] 类型#SemHandle *，输出信号量句柄。
 * @param cookie    [IN]  类型#U32，传入的数值，OS做记录。
 *
 * @retval #OS_ERRNO_SEM_OVERFLOW 0x02000f08，计数器初始值超出范围。
 * @retval #OS_ERRNO_SEM_ALL_BUSY 0x02000f03，没有空闲信号量，建议增加"最大支持信号量"配置。
 * @retval #OS_ERRNO_SEM_PTR_NULL 0x02000f02，入参指针为空。
 * @retval #SRE_OK                0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_sem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SemDelete | SRE_SemCCreate | SRE_SemBCreate
 */
extern U32 SRE_SemCreateDbg(U32 count, SemHandle *semHandle, U32 cookie);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* SRE_SEM_H */
