/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 断点管理模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_BREAKPOINT_H
#define SRE_BREAKPOINT_H

#include "sre_base.h"
#include "sre_module.h"
#include "./hw/board/os_board.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * @ingroup SRE_breakpoint
 * 断点句柄类型定义
 */
typedef U32 BpHandle;

/*
 * @ingroup SRE_breakpoint
 * 断点保护区间大小定义
 */
enum OsBpMonSize {
    BP_MON_SIZE_001 = 0, /* 保护区间大小为1字节 */
    BP_MON_SIZE_002 = 1, /* 保护区间大小为2字节 */
    BP_MON_SIZE_004 = 2, /* 保护区间大小为4字节 */
    BP_MON_SIZE_008 = 3, /* 保护区间大小为8字节 */
    BP_MON_SIZE_016 = 4, /* 保护区间大小为16字节 */
    BP_MON_SIZE_032 = 5, /* 保护区间大小为32字节，SD6183无效 */
    BP_MON_SIZE_064 = 6, /* 保护区间大小为64字节，SD6183无效 */
    BP_MON_SIZE_INVALID  /* 保护区间大小非法 */
};

/*
 * @ingroup SRE_breakpoint
 * 断点类型
 */
enum OsBpType {
    BP_TYPE_INSTRCTION = 0, /* 指令断点 */
    BP_TYPE_DATA_READ = 1, /* 数据读断点 */
    BP_TYPE_DATA_WRITE = 2, /* 数据写断点 */
    BP_TYPE_DATA_READ_WRITE = 3, /* 数据读写断点, 只有6157、6182上有效，其他平台代表INVALID */
    BP_TYPE_INVALID
};

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:添加断点时传入的句柄指针为空。
 *
 * 值: 0x02002201
 *
 * 解决方案:添加断点时，请确保传入的句柄指针合法。
 */
#define OS_ERRNO_BP_ADD_PTR_NULL SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x01)

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:添加断点时传入的断点类型非法。
 *
 * 值: 0x02002202
 *
 * 解决方案:请确保传入的断点类型为BP_TYPE_INSTRCTION，BP_TYPE_DATA_READ，BP_TYPE_DATA_WRITE其中一种。
 */
#define OS_ERRNO_BP_TYPE_INVALID SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x02)

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:添加断点时传入的保护区间大小非法。
 *
 * 值: 0x02002203
 *
 * 解决方案:请确保传入的保护区间大小合法:对于数据断点，合法范围见OS_BP_MON_SIZE_E定义，
 * 对于指令断点，合法值只能为BP_MON_SIZE_001。
 */
#define OS_ERRNO_BP_MON_SIZE_INVALID SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x03)

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:添加断点时传入的保护区间起始地址不满足按保护区间大小对齐的要求。
 *
 * 值: 0x02002204
 *
 * 解决方案:请确保传入的保护区间起始地址按保护区间大小对齐。
 */
#define OS_ERRNO_BP_MON_ADDR_NOT_ALIGN SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x04)

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:断点资源耗尽。
 *
 * 值: 0x02002205
 *
 * 解决方案:请先删除不再使用的断点。
 */
#define OS_ERRNO_BP_RES_NOT_AVALIABLE SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x05)

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:删除断点时传入的断点句柄非法。
 *
 * 值: 0x02002206
 *
 * 解决方案:请确保传入的断点句柄是通过SRE_BreakPointAdd得到的，且该断点未被删除过。
 */
#define OS_ERRNO_BP_REMOVE_INVALID_HANDLE SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x06)

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:删除栈越界检测模块使用的断点资源。
 *
 * 值: 0x02002207
 *
 * 解决方案:请确保传入的断点句柄是通过SRE_BreakPointAdd得到的，而非OS的栈越界模块使用的断点资源。
 */
#define OS_ERRNO_BP_REMOVE_STKMON_HANDLE SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x07)

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:不支持数据断点功能。
 *
 * 值: 0x02002208
 *
 * 解决方案:芯片硬件上不支持数据断点功能，所以数据断点功能无法正常使用。
 */
#define OS_ERRNO_BP_NO_SUPPORT_DATA SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x08)

/*
 * *@ingroup SRE_breakpoint
 * 断点管理错误码:不支持指令断点功能。
 *
 * 值: 0x02002209
 *
 * 解决方案:芯片硬件上不支持指令断点功能，所以指令断点功能无法正常使用。
 */
#define OS_ERRNO_BP_NO_SUPPORT_INSTRUCTION SRE_ERRNO_OS_ERROR(OS_MID_BP, 0x09)

/*
 * @ingroup SRE_breakpoint
 * @brief 添加断点。
 *
 * @par 描述
 * 对bpAddr和size指定的地址区间，设置bpType类型的断点，返回断点句柄bpHdl。
 *
 * @attention
 * <ul>
 * <li>仅支持SD6182及以后平台。</li>
 * <li>支持8个断点(不区分指令/数据断点)，保护数据时保护范围为1字节~16字节。</li>
 * <li>当栈越界模块打开时，可使用的数据断点减少1个。</li>
 * <li>断点是核内功能，保护的是本核的行为，其他核或外设的行为不会被保护。</li>
 * <li>对于指令断点，断点的触发时机是核即将执行被保护指令时，所以用户观察到的断点现场中被保护指令还未被执行。</li>
 * <li>对于数据断点，断点的触发时机是核即将读/写被被保护数据时，所以用户观察到的断点现场中被保护数据还未被读/写。</li>
 * </ul>
 *
 * @param bpType [IN]  类型#enum OsBpType，添加的断点类型。
 * @param bpAddr [IN]  类型#uintptr_t，断点保护区间的起始地址，必须按size对齐。
 * @param size   [IN]  类型#enum OsBpMonSize，断点保护区间的大小。
 * @param bpHdl  [OUT] 类型#BpHandle *，用于存放断点句柄的地址。
 *
 * @retval #OS_ERRNO_BP_ADD_PTR_NULL            0x02002201，添加断点时传入的句柄指针为空。
 * @retval #OS_ERRNO_BP_TYPE_INVALID            0x02002202，添加断点时传入的断点类型非法。
 * @retval #OS_ERRNO_BP_MON_SIZE_INVALID        0x02002203，添加断点时传入的保护区间大小非法。
 * @retval #OS_ERRNO_BP_MON_ADDR_NOT_ALIGN      0x02002204，添加断点时传入的保护区间起始地址不满足
 * @retval                                                  按保护区间大小对齐的要求。
 * @retval #OS_ERRNO_BP_RES_NOT_AVALIABLE       0x02002205，断点资源耗尽。
 * @retval #SRE_OK                              0x00000000，添加断点成功。
 * @par 依赖
 * <li>sre_breakpoint.h：该接口声明所在的头文件。</li>
 * @see SRE_BreakPointRemove
 */
extern U32 SRE_BreakPointAdd(enum OsBpType bpType, uintptr_t bpAddr, enum OsBpMonSize size, BpHandle *bpHdl);

/*
 * @ingroup SRE_breakpoint
 * @brief 删除断点。
 *
 * @par 描述
 * 删除bpHdl指定的断点。
 *
 * @attention
 * <ul>
 * <li>仅支持SD6182及以后平台。</li>
 * </ul>
 *
 * @param bpHdl [IN]  类型#BpHandle，断点句柄。
 *
 * @retval #OS_ERRNO_BP_REMOVE_INVALID_HANDLE   0x02002206，删除断点时传入的断点句柄非法。
 * @retval #OS_ERRNO_BP_REMOVE_STKMON_HANDLE    0x02002207，删除栈越界检测模块使用的断点资源。
 * @retval #SRE_OK                              0x00000000，删除断点成功。
 * @par 依赖
 * <li>sre_breakpoint.h：该接口声明所在的头文件。</li>
 * @see SRE_BreakPointAdd
 */
extern U32 SRE_BreakPointRemove(BpHandle bpHdl);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SRE_BREAKPOINT_H */
