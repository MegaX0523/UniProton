/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: Shell模块的基础头文件，用户不能直接包含
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_SHELL_H
#define OS_SHELL_H

#include "os_errno.h"
#include "os_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * @ingroup OS_shell
 * HShell错误码:Target shell内存操作时，指定修改或查询的内存宽度不合法。
 *
 * 值: 0x02002d01
 *
 * 解决方案: Target shell对内存值做修改操作时需指定宽度为1,2或4字节；内存查询时最大宽度不能超过256字节。
 *
 */
#define OS_ERRNO_SHELL_MEMWR_WIDTH_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x01)

/*
 * @ingroup OS_shell
 * HShell错误码:  内存操作时，临时内存申请失败。
 *
 * 值: 0x02002d02
 *
 * 解决方案: 排查代码有无内存泄露。
 *
 */
#define OS_ERRNO_SHELL_MEMWR_ALLOC OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x02)

/*
 * @ingroup OS_shell
 * HShell错误码: 内存操作时，目的地址无效。
 *
 * 值: 0x02002d03
 *
 * 解决方案: 排查目的地址是否有效。
 *
 */
#define OS_ERRNO_SHELL_MEMWR_ADDR_INVAIL OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x03)

/*
 * @ingroup OS_shell
 * HShell错误码: Target shell查询内存操作时，输入的内存单元个数非法或者被忽略。
 *
 * 值: 0x02002d04
 *
 * 解决方案: 使用查询内存操作命令时，必须输入内存单元个数，若已经输入内存单元个数，
 * 请排查输入的内存单元个数大小是否在合法范围[1,512]内。
 *
 */
#define OS_ERRNO_SHELL_MEMWR_UNITS_NUM_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x04)

/*
 * @ingroup OS_shell
 * HShell错误码: Target shell查询或者修改内存操作时，输入的内存地址未按照要求对齐。
 *
 * 值: 0x02002d05
 *
 * 解决方案: 排查输入的内存地址是否按照要求对齐，当输入的数据宽度为2或者4时，要求输入的地址须为2字节或者4字节对齐。
 *
 */
#define OS_ERRNO_SHELL_MEMWR_ADDR_NOT_ALIGNED OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x05)

/*
 * @ingroup OS_shell
 * LIBC错误码：注册打印前缀钩子时，传入的钩子函数为NULL。
 *
 * 值: 0x02002d06
 *
 * 解决方案: 保证注册打印前缀钩子时传入的钩子函数不为NULL。
 *
 */
#define OS_ERRNO_SHELL_PRINT_PRIFIX_HOOK_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x06)

/*
 * @ingroup OS_shell
 * IO接口错误码：SRE_SendChrToChlHookReg函数入参为空。
 *
 * 值: 0x02002d07
 *
 * 解决方案: SRE_SendChrToChlHookReg函数的入参否为空指针。
 *
 */
#define OS_ERRNO_SHELL_SENDCHR2SHELL_PARA_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x07)

/*
 * @ingroup OS_shell
 * 符号表管理错误码:入参指针为空。
 *
 * 值: 0x02002d08
 *
 * 解决方案: 排查函数入参是否为空
 *
 */
#define OS_ERRNO_SHELL_PARA_POINT_NULL OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x08)

/*
 * @ingroup OS_shell
 * shell错误码:shell未初始化完成
 *
 * 值: 0x02002d10
 *
 * 解决方案: 检查shell模块是否打开以及配置正确
 *
 */
#define OS_ERRNO_SHELL_NOT_INITIALIZED  OS_ERRNO_BUILD_ERROR(OS_MID_SHELL, 0x09)

/*
 * @ingroup  OS_shell
 *
 * 定义用户注册的通道字符输出钩子函数类型。
 */
typedef U32(*OsSendChr2ChlHookFunc)(U8 chr);

/*
 * @ingroup  OS_shell
 *
 * 设置HSHELL模块的配置信息。
 */
struct HshellModInfo {
    /* Shell任务的优先级 */
    U32 shTskPriority;
    /* Shell任务的栈大小 */
    U32 shTskStackSize;
};

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* OS_SHELL_H */
