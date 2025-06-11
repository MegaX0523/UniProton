/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: hunt模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_HUNT_H
#define SRE_HUNT_H

#include "sre_base.h"
#include "sre_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup sre_hunt
 * HUNT错误码：HUNT模块初始化失败。
 *
 * 值: 0x02001f00
 *
 * 解决方案：确保内存空间足够大，SD6183:从实例间共享UNCACHE空间申请，其他平台:从共享静态fsc内存或者私有fsc内存分区申请。
 */
#define OS_ERRNO_HUNT_INIT_ERROR SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x00)

/*
 * @ingroup sre_hunt
 * HUNT错误码：创建任务或软中断时，线程名重名。
 *
 * 值: 0x02001f01
 *
 * 解决方案：创建任务或软中断时，确保线程名本核内唯一。
 */
#define OS_ERRNO_HUNT_THREAD_NAME_REPEAT SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x01)

/*
 * @ingroup sre_hunt
 * HUNT错误码：通过进程ID及线程名获取线程TID时，该线程未创建或已删除。
 *
 * 值: 0x02001f02
 *
 * 解决方案：确保线程已创建且未被删除。
 */
#define OS_ERRNO_HUNT_THREAD_NOT_CREAT SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x02)

/*
 * @ingroup sre_hunt
 * HUNT错误码：通过线程ID获取线程名时入参指针为空； 或通过实例名获取实例ID时入参指针非法。
 *
 * 值: 0x02001f03
 *
 * 解决方案：确保入参指针非空、入参线程ID及实例ID指针等的合法性。
 */
#define OS_ERRNO_HUNT_ILLEGAL_INPUT SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x03)

/*
 * @ingroup sre_hunt
 * HUNT错误码：通过线程名获取ID时，Hunt还表没有初始化。
 *
 * 值: 0x02001f04
 *
 * 解决方案：确保进程所在核的hunt表已初始化。
 */
#define OS_ERRNO_HUNT_NOT_INIT SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x04)

/*
 * @ingroup sre_hunt
 * HUNT错误码：创建用户进程，进程名重名。
 *
 * 值: 0x02001f05
 *
 * 解决方案：创建用户进程时，确保进程名本核内唯一。
 */
#define OS_ERRNO_HUNT_PROCESS_NAME_REPEAT SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x05)

/*
 * @ingroup sre_hunt
 * HUNT错误码：hunt 模块名字申请失败。
 *
 * 值: 0x02001f07
 *
 * 解决方案：扩大共享名字空间大小。
 */
#define OS_ERRNO_HUNT_NAME_ALLOC_FAILED SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x07)

/*
 * @ingroup sre_hunt
 * HUNT错误码：HUNT实例ID时，对应的实例没有创建。
 *
 * 值: 0x02001f08
 *
 * 解决方案：确保主核已经创建了该实例。
 */
#define OS_ERRNO_HUNT_INST_NOT_CREAT SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x08)

/*
 * @ingroup sre_hunt
 * HUNT错误码：通过实例号没有找到本核的进程PID 或者进程信息添加失败。
 *
 * 值: 0x02001f09
 *
 * 解决方案：确保进程所在实例信息添加的正确性
 */
#define OS_ERRNO_HUNT_PROCESS_INFO_ERR SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x09)

/*
 * @ingroup sre_hunt
 * HUNT错误码：通过线程inst info指针为空； 或通过实例名获取实例ID时入参指针非法。
 *
 * 值: 0x02001f0a
 *
 * 解决方案：确保入参指针非空、入参线程ID及实例ID指针等的合法性。
 */
#define OS_ERRNO_HUNT_INST_INFO_NULL SRE_ERRNO_OS_ERROR(OS_MID_HUNT, 0x0a)

/*
 * @ingroup sre_hunt
 * @brief 通过线程名获取线程ID。
 *
 * @par 描述
 * 通过线程名(软中断或任务)获取线程ID。
 * @attention
 * <ul>
 * <li>本核调用#SRE_HuntMature后调用该接口，会检查目的核是否调用#SRE_HuntMature，</li>
 * <li>如果调用了，则继续进行查询操作，否则一直等待。</li>
 * <li>本核调用#SRE_HuntMature之前调用该接口，不关注目的核是否调用#SRE_HuntMature，直接进行查询操作。</li>
 * <li>SD6185以上芯片支持本实例内的核进行HUNT线程TID。</li>
 * <li>注意线程名长度最大支持15字节,hunt的时候线程名不要超过15字节。</li>
 * </ul>
 *
 * @param vcpuId     [IN]  类型#U32，对于裸核来说为核号(硬线程号)。
 * @param threadName [IN]  类型#const char *，线程名长度范围[1,15]。
 * @param tid        [OUT] 类型#U32 *，线程TID。
 *
 * @retval #OS_ERRNO_HUNT_THREAD_NOT_CREAT 0x02001f02，该线程未创建或已删除。
 * @retval #OS_ERRNO_HUNT_ILLEGAL_INPUT    0x02001f03，入参非法。
 * @retval #OS_ERRNO_HUNT_NOT_INIT         0x02001f04，hunt还表没有初始化。
 * @retval #SRE_OK                         0x00000000，获取PID成功。
 * @par 依赖
 * <ul><li>sre_hunt.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HuntTID
 */
extern U32 SRE_HuntByName(U32 vcpuId, const char *threadName, U32 *tid);

/*
 * @ingroup sre_hunt
 * @brief 新增实例间HUNT接口，获取其他实例的所在核的线程ID。
 *
 * @par 描述
 * 通过进程名获取进程PID。
 * @attention
 * <ul>
 * <li>本核调用#SRE_HuntMature后调用该接口，会检查目的核是否调用#SRE_HuntMature，</li>
 * <li>如果调用了，则继续进行查询操作，否则一直等待。</li>
 * <li>本核调用#SRE_HuntMature之前调用该接口，不关注目的核是否调用#SRE_HuntMature，直接进行查询操作。</li>
 * <li>调用该接口需要保证查询的进程已经创建。</li>
 * <li>注意线程名长度最大支持15字节, hunt的时候线程名不要超过15字节。</li>
 * </ul>
 *
 * @param instId     [IN]  类型#U32，实例号。
 * @param vcpuId     [IN]  类型#U32，对于裸核来说为核号(SD6183/SD6185为硬线程号)。
 * @param threadName [IN]  类型#const char *，线程名长度范围[1,15]。
 * @param tid        [OUT] 类型#U32 *，线程TID。
 *
 * @retval #OS_ERRNO_HUNT_ILLEGAL_INPUT     0x02001f03，入参非法。
 * @retval #OS_ERRNO_HUNT_NOT_INIT          0x02001f04，hunt还表没有初始化。
 * @retval #OS_ERRNO_HUNT_THREAD_NOT_CREAT  0x02001f02，该线程未创建或已删除。
 * @retval #SRE_OK                          0x00000000，获取PID成功。
 * @par 依赖
 * <ul><li>sre_hunt.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HuntByName
 */
extern U32 SRE_HuntTID(U32 instId, U32 vcpuId, const char *threadName, U32 *tid);

/*
 * @ingroup sre_hunt
 * @brief 标记本核所有的任务和软中断已创建完成（OS的IDLE除外），其它核可以成功查询本核的PID。
 *
 * @par 描述
 * 置上本核mature标志。同时置标志：使得本核在调用#SRE_HuntByName时，
 * 如果目的核没有启动，则一直等待，直到目的核置上了mature标志才返回查询结果。
 * @attention
 * <ul>
 * <li>必须在本核所有的任务和软中断（OS的IDLE除外）创建完成之后调用。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_hunt.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_HuntByName
 */
extern void SRE_HuntMature(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_HUNT_H */
