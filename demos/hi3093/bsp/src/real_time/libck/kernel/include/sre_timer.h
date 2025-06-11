/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 定时器模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * @defgroup sre_timer 定时器
 * @ingroup SRE_kernel
 */

#ifndef SRE_TIMER_H
#define SRE_TIMER_H

#include "sre_base.h"
#include "sre_module.h"
#include "./common/os_timer.h"
#include "./hw/board/os_board.h"

#if defined(OS_OPTION_TRING)
#if (OS_CPU_TYPE == OS_SD6185)
#include "./hw/drv/os_tringv200.h"
#endif

#if (OS_CPU_TYPE == OS_SD6186 || OS_CPU_TYPE == OS_SD6186L)
#include "./hw/drv/os_tringv210x.h"
#endif
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * @ingroup os_timer
 * 定时器状态枚举值
 */
enum TimerFlag {
    OS_TIMER_FREE = 1, /* 定时器空闲状态 */
    OS_TIMER_CREATED = 2, /* 定时器没有运行 */
    OS_TIMER_RUNNING = 4, /* 定时器正在运行 */
    OS_TIMER_EXPIRED = 8 /* 定时器已经超时 */
};

/*
 * @ingroup SRE_exc
 * 全局硬件定时器记录的错误信息类型，1-3共3种，McHwtmrErrInfo中的mcHwtmrErrTpye记录。
 * 全局硬件定时器的错误结构体信息。
 */
enum McHwtmrErr {
    OS_MCHWTMR_ERR_VMID = 1, /* VMID检查出错 */
    OS_MCHWTMR_ERR_LOCK = 2, /* 违反LOCK保护 */
    OS_MCHWTMR_ERR_VMLOCK = 3, /* VMID检查出错且违反LOCK保护 */
};

struct McHwtmrErrInfo {
    /* 全局硬件定时器错误类型，1-3共3种， 参照枚举McHwtmrErr */
    U32 mcHwtmrErrTpye;
};

/*
 * @ingroup os_timer
 * 基于tick的软件定时器ID。
 *
 * 当软件定时器的开关打开后，系统会创建基于tick的软件定时器组，用户通过此宏，获得软件定时器组ID。
 */
#define OS_TICK_SWTMR_GRPID 0

/*
 * @ingroup os_timer
 * OS_timer错误码: 指针参数为空。
 *
 * 值: 0x02002e01
 *
 * 解决方案: 查看入参指针是否为空。
 */
#define OS_ERRNO_TIMER_INPUT_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x01)

/*
 * @ingroup os_timer
 * OS_timer错误码: 定时器回调函数为空。
 *
 * 值: 0x02002e02
 *
 * 解决方案: 查看定时器回调函数是否为空。
 */
#define OS_ERRNO_TIMER_PROC_FUNC_NULL OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x02)

/*
 * @ingroup os_timer
 * OS_timer错误码: 定时器句柄非法。
 *
 * 值: 0x02002e03
 *
 * 解决方案: 检查输入的定时器句柄是否正确。
 */
#define OS_ERRNO_TIMER_HANDLE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x03)

/*
 * @ingroup os_timer
 * OS_timer错误码: 定时器周期参数非法。
 *
 * 值: 0x02002e04
 *
 * 解决方案: 查看定时器周期参数是否正确。
 */
#define OS_ERRNO_TIMER_INTERVAL_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x04)

/*
 * @ingroup os_timer
 * OS_timer错误码: 定时器工作模式参数非法。
 *
 * 值: 0x02002e05
 *
 * 解决方案: 查看定时器工作模式参数是否正确，参照工作模式配置枚举定义#enum TmrMode。
 */
#define OS_ERRNO_TIMER_MODE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x05)

/*
 * @ingroup os_timer
 * OS_timer错误码: 定时器类型参数非法。
 *
 * 值: 0x02002e06
 *
 * 解决方案: 查看定时器类型参数是否正确，参照类型配置枚举定义#enum TimerType。
 */
#define OS_ERRNO_TIMER_TYPE_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x06)

/*
 * @ingroup os_timer
 * OS_timer错误码: 硬件定时器没有初始化。
 *
 * 值: 0x02002e08
 *
 * 解决方案: 配置项打开硬件定时器，初始化硬件定时器。
 */
#define OS_ERRNO_TIMER_HWTMR_NOT_INIT OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x08)

/*
 * @ingroup os_timer
 * OS_timer错误码: 共享硬件定时器没有初始化。
 *
 * 值: 0x02002e09
 *
 * 解决方案: 配置项打开共享硬件定时器，初始化共享硬件定时器。
 */
#define OS_ERRNO_TIMER_SHARED_HWTMR_NOT_INIT OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x09)

/* =======================================================HWTMR ERRNO============================================ */
/*
 * @ingroup os_timer
 * 硬件定时器错误码：没有可分配的硬件定时器。
 *
 * 值: 0x02002e21
 *
 * 解决方案: 在创建硬件定时器前，请确保有硬件定时器资源可用。
 */
#define OS_ERRNO_HWTMR_UNAVAILABLE OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x21)

/*
 * @ingroup os_timer
 * 硬件定时器错误码：硬件定时器未创建。
 *
 * 值: 0x02002e24
 *
 * 解决方案: 检查硬件定时器是否创建。
 */
#define OS_ERRNO_HWTMR_NOT_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x24)

/*
 * @ingroup os_timer
 * 硬件定时器错误码：硬件定时器定时周期us转换cycle溢出。
 *
 * 值: 0x02002e25
 *
 * 解决方案: 硬件定时器定时周期us转cycle溢出,减小输入的us值。
 */
#define OS_ERRNO_HWTMR_US2CYCLE_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x25)

/*
 * @ingroup os_timer
 * 硬件定时器错误码：硬件定时器定时未启动。
 *
 * 值: 0x02002e27
 *
 * 解决方案: 检查硬件定时器是否启动。
 */
#define OS_ERRNO_HWTMR_NOT_START OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x27)

/* ==========================================================SWTMR ERRNO============================================ */
/*
 * @ingroup os_timer
 * 软件定时器错误码：软件定时器组的时钟源类型不为TICK类型。
 *
 * 值: 0x02002e40
 *
 * 解决方案: 软件定时器组的时钟源类型必须为TICK类型。
 */
#define OS_ERRNO_TIMER_SOURCE_NOT_TICK OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x40)

/*
 * @ingroup os_timer
 * 软件定时器错误码：软件定时器组的最大定时器个数为零。
 *
 * 值: 0x02002e41
 *
 * 解决方案: 软件定时器组的最大定时器个数必须大于零。
 */
#define OS_ERRNO_TIMER_NUM_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x41)

/*
 * @ingroup os_timer
 * 软件定时器错误码：定时器未初始化或定时器组未创建。
 *
 * 值: 0x02002e42
 *
 * 解决方案: 只有在定时器初始化且定时器组已经创建的情况下才能使用定时器。
 */
#define OS_ERRNO_TIMER_NOT_INIT_OR_GROUP_NOT_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x42)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:定时器设置的定时周期转化为Tick数后不为整数。
 *
 * 值: 0x02002e44
 *
 * 解决方案: 请确保设置的定时周期是Tick的整数倍。
 */
#define OS_ERRNO_SWTMR_INTERVAL_NOT_SUITED OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x44)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:返回指针参数为空。
 *
 * 值: 0x02002e47
 *
 * 解决方案: 输入有效的指针参数。
 */
#define OS_ERRNO_SWTMR_RET_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x47)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:达到最大支持定时器数目。
 *
 * 值: 0x02002e48
 *
 * 解决方案: 达到最大支持定时器个数，不能再创建软件定时器。
 */
#define OS_ERRNO_SWTMR_MAXSIZE OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x48)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:定时器未创建。
 *
 * 值: 0x02002e4a
 *
 * 解决方案: 创建定时器后再使用。
 */
#define OS_ERRNO_SWTMR_NOT_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x4a)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:定时器已经处于未启动状态。
 *
 * 值: 0x02002e4d
 *
 * 解决方案: 定时器处于未启动状态，不能进行一些操作，请检查操作的合法性。
 */
#define OS_ERRNO_SWTMR_UNSTART OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x4d)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:初始化内存不足。
 *
 * 值: 0x02002e50
 *
 * 解决方案: 请适当增加系统默认FSC分区大小。
 */
#define OS_ERRNO_SWTMR_NO_MEMORY OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x50)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:TICK没有初始化。
 *
 * 值: 0x02002e53
 *
 * 解决方案: 初始化Tick。
 */
#define OS_ERRNO_TICK_NOT_INIT OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x53)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:软件定时器组入参指针为空。
 *
 * 值: 0x02002e54
 *
 * 解决方案: 传入非空入参指针。
 */
#define OS_ERRNO_TIMERGROUP_INPUT_PTR_NULL OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x54)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:软件定时器定时周期过大，转换为Tick数超过0xFFFFFFFF。
 *
 * 值: 0x02002e55
 *
 * 解决方案: 请确保传入的定时周期转化为Tick数后不大于0xFFFFFFFF。
 */
#define OS_ERRNO_SWTMR_INTERVAL_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x55)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:Tick定时器组已创建。
 *
 * 值: 0x02002e56
 *
 * 解决方案: 不能重复创建定时器组。
 */
#define OS_ERRNO_TIMER_TICKGROUP_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x56)

/*
 * *@ingroup os_timer
 * 软件定时器错误码:创建软件定时器时传入定时器组组号非法。
 *
 * 值: 0x02002e57
 *
 * 解决方案: 确保传入的定时器组号是通过使用定时器组创建接口得到。
 */
#define OS_ERRNO_TIMERGROUP_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x57)

/*
 * @ingroup os_timer
 * 软件定时器错误码：软件定时器组的最大定时器个数大于0xffff。
 *
 * 值: 0x02002e58
 *
 * 解决方案: 确保软件定时器组的最大定时器个数小于等于0xffff。
 */
#define OS_ERRNO_TIMER_NUM_TOO_LARGE OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x58)

/*
 * ======================================================SHARED HWTMR ERRNO============================================
 */
/*
 * *@ingroup os_timer
 * 共享硬件定时器错误码:对于SD6182平台，创建共享硬件定时器时传入的定时超时处理函数非空。
 *
 * 值: 0x02002e70
 *
 * 解决方案:在SD6182平台确保传入的共享硬件定时器超时处理函数为空。
 */
#define OS_ERRNO_SHARED_TIMER_FUNC_NO_NULL OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x70)

/*
 * *@ingroup os_timer
 * 共享硬件定时器错误码:传入的共享硬件定时器ID不可用。
 *
 * 值: 0x02002e72
 *
 * 解决方案:确保传入的共享硬件定时器ID范围[0,7]。
 */
#define OS_ERRNO_SHARED_TIMER_ID_INVALID OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x72)

/*
 * *@ingroup os_timer
 * 共享硬件定时器错误码:共享硬件定时器时未创建。
 *
 * 值: 0x02002e73
 *
 * 解决方案:需要先创建共享硬件定时器再使用。
 */
#define OS_ERRNO_SHARED_TIMER_NOT_CREATED OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x73)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：共享硬件定时器定时未启动。
 *
 * 值: 0x02002e74
 *
 * 解决方案: 检查共享硬件定时器是否启动。
 */
#define OS_ERRNO_SHARED_TIMER_NOT_START OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x74)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：共享硬件定时器定时不可用。
 *
 * 值: 0x02002e75
 *
 * 解决方案: 检查共享硬件定时器是否有损坏。
 */
#define OS_ERRNO_SHARED_TIMER_IS_BAD OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x75)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：共享硬件定时器定时周期us转换cycle后为0。
 *
 * 值: 0x02002e76
 *
 * 解决方案: 硬件定时器定时周期us转cycle为0,增大输入的us值。
 */
#define OS_ERRNO_SHARED_TIMER_US2CYCLE_IS_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x76)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：共享硬件定时器定时周期us转换cycle溢出。
 *
 * 值: 0x02002e77
 *
 * 解决方案: 硬件定时器定时周期us转cycle溢出,减小输入的us值。
 */
#define OS_ERRNO_SHARED_TIMER_US2CYCLE_OVERFLOW OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x77)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：共享硬件定时器配置的时钟频率为0。
 *
 * 值: 0x02002e79
 *
 * 解决方案: 在config.h中配置的时钟频率不能为0。
 */
#define OS_ERRNO_SHARED_TIMER_CLOCK_IS_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x79)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：共享硬件定时器正在使用，不能自检。
 *
 * 值: 0x02002e80
 *
 * 解决方案: 定时器使用完成后再做自检，或者在使用之前自检。
 */
#define OS_ERRNO_SHARED_TIMER_IN_USING OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x80)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：SD6183平台共享硬件定时器通道组VMID配置时传入的通道组ID非法。
 *
 * 值: 0x02002e81
 *
 * 解决方案: SD6183平台共享硬件定时器通道组ID必须是0~3。
 */
#define OS_ERRNO_SHARE_TIMER_CHG_IDX_ILLEGAL OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x81)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：SD6183平台共享硬件定时器通道组VMID配置时传入的VMID非法。
 *
 * 值: 0x02002e82
 *
 * 解决方案: SD6183平台共享硬件定时器通道组VMID必须是0~7。
 */
#define OS_ERRNO_SHARE_TIMER_VMID_VALUE_ILLEGAL OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x82)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：SD6183平台共享硬件定时器通道组VMID配置时传入的资源表太大(大于0xff)。
 *
 * 值: 0x02002e83
 *
 * 解决方案: SD6183平台共享硬件定时器通道组VMID配置时传入的资源表Bit0~Bit7有效，其它Bit必须设置为0。
 */
#define OS_ERRNO_SHARE_TIMER_MAP_VALUE_TOO_LARGE OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x83)

/*
 * @ingroup os_timer
 * 共享硬件定时器错误码：SD6183平台共享硬件定时器通道组VMID配置时传入的资源表值为0。
 *
 * 值: 0x02002e84
 *
 * 解决方案: SD6183平台共享硬件定时器通道组VMID配置时传入的资源表值不能为0。
 */
#define OS_ERRNO_SHARE_TIMER_MAP_VALUE_ZERO OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0x84)

/*
 * @ingroup os_timer
 * 支持错峰的软件定时器错误码：容忍度配置错误。
 *
 * 值: 0x02002ea3
 *
 * 解决方案: 目前容忍度的允许配置范围为[0,5]
 */
#define OS_ERRNO_STAGGER_TIMER_TOLERANCE_ERROR OS_ERRNO_BUILD_ERROR(OS_MID_TIMER, 0xa3)

/*
 * @ingroup sre_timer
 * 定时器句柄定义
 */
typedef U32 TimerHandle;

/*
 * @ingroup sre_timer
 * 定时器组句柄定义
 */
typedef U32 TimerGroupId;

/*
 * @ingroup sre_timer
 * 定时器组时间轮回调函数类型定义
 */
typedef void (*TmrWheelProcFunc)(void);

/*
 * @ingroup sre_timer
 * 定时器回调函数定义，带5个参数、返回值类型为void的函数指针类型，其中参数tmrHandle在核内硬件定时器、
 * 全局硬件定时器和软件定时器中分别表示逻辑ID、物理ID和逻辑ID。
 */
typedef void (*SreTmrProcFunc)(TimerHandle tmrHandle, U32 arg1, U32 arg2, U32 arg3, U32 arg4);

/*
 * @ingroup sre_timer
 * 定时器组时钟源类型枚举定义
 */
enum TimerGrpSrcType {
    SRE_TIMER_GRP_SRC_HARDWARE, /* 硬件私有时钟源 */
    SRE_TIMER_GRP_SRC_HARDWARE_SHARED, /* 硬件共享时钟源 */
    SRE_TIMER_GRP_SRC_EXTERN, /* 外部时钟源 */
    SRE_TIMER_GRP_SRC_TICK, /* TICK时钟源 */
    SRE_TIMER_GRP_SRC_INVALID
};

/*
 * @ingroup sre_timer
 * 定时器组用户配置结构体
 */
struct TmrGrpUserCfg {
    /* 时钟源类型 */
    enum TimerGrpSrcType tmrGrpSrcType;
    /* 时间轮的刻度，即每刻度多少个us（Tring定时器必须是50us的倍数） */
    U32 perStep;
    /* 最大定时器个数 */
    U32 maxTimerNum;
    /* 时间轮长度必须是2的N次方，此处配置N的大小 */
    U32 wheelLen2Power;
    /* 使用的软中断优先级，当前无效 */
    U32 swiPrio;
    /* 定时器组时钟源函数，每推一次轮调用一次 */
    TmrWheelProcFunc wheelFunc;
#if defined(OS_OPTION_TRING)
    /* FreeList空间使用的分区号，大小不能超过0xff */
    U32 timerPtNo;
    /* 每个Wheel通道上配置的MSGM共享队列ID，支持消息通知方式启动定任务 */
    U32 shQueID;
    /* 用户TRING描述符的软件扩展字段长度,按WORD长度计算 */
    U32 usrTcbLen;
#endif
};

/*
 * @ingroup sre_timer
 * 定时器工作模式枚举定义
 */
enum TmrMode {
    SRE_TIMER_LOOP, /* 定时器周期触发模式 */
    SRE_TIMER_ONCE, /* 定时器单次触发模式 */
    SRE_TIMER_INVALID_MODE
};

/*
 * @ingroup sre_timer
 * 定时器类型枚举定义
 */
enum TimerType {
    SRE_TIMER_HARDWARE,        /* 硬件定时器(核内私有硬件定时器) */
    SRE_TIMER_HARDWARE_SHARED, /* 共享硬件定时器,在SD680X系列中，8 9 两个新增核内私有定时器创建也采用这个类型 */
    SRE_TIMER_SOFTWARE,        /* 软件定时器(核内私有软件定时器) */
    SRE_TIMER_SOFTWARE_SHARED, /* 共享软件定时器，目前不支持 */
    SRE_TIMER_SOFTWARE_QUERY,  /* 查询软件定时器，目前不支持 */
    SRE_TIMER_HARDWARE_TRING,  /* TRING定时器,SD6185/SD6186/SD6186L支持 */
    SRE_TIMER_INVALID_TYPE
};

/*
 * @ingroup sre_timer
 * 定时器创建参数结构体定义
 */
struct TimerCreatePara {
    /* 定时器创建模块ID,当前未使用，忽略 */
    U32 moduleId;
    /* 定时器类型 */
    enum TimerType type;
    /* 定时器工作模式 */
    enum TmrMode mode;
    /* 定时器周期(单次指定时器响应时长)，软件定时器单位是ms，硬件定时器单位是us，高精度定时器单位是ns */
    U32 interval;
    /*
     * 定时器组号，硬件定时器不使用，若创建软件定时器，定时器组ID由OS创建，
     * 用户可以输入宏OS_TICK_SWTMR_GRPID
     */
    U32 timerGroupId;
    /* 定时器硬中断优先级，Tensinica平台无效，SD6183平台有效(表示硬件定时器对应硬中断的优先级)。 */
    U16 hwiPrio;
    /* 全局定时器索引，仅全局硬件定时器使用，除8021、8011范围为0-15，其余均为0-7 */
    U16 timerIdx;
    /*
     * 定时器回调函数，SD6182/SD6157平台全局硬件定时器钩子必须为NULL，SD6183平台全局硬件定时器钩子需要配置，不能为NULL
     */
    SreTmrProcFunc callBackFunc;
    /* 定时器用户参数1 */
    U32 arg1;
    /* 定时器用户参数2 */
    U32 arg2;
    /* 定时器用户参数3 */
    U32 arg3;
    /* 定时器用户参数4 */
    U32 arg4;
#if defined(OS_OPTION_TRING)
    TRING_STRUCT_FOR_TIMER
#endif
};

/*
 * *@ingroup sre_timer
 * 软件定时器统计信息的结构体类型定义
 */
struct SwTmrStatInfo {
    /* 定时器使用数,包括处于创建态，计时态和超时处理态的定时器个数 */
    U32 usedCnt;
    /* 定时器空闲数 */
    U32 freeCnt;
    /* 定时器启动数，包括处于计时态和超时处理态的定时器个数 */
    U32 runCnt;
};

/*
 * *@ingroup sre_timer
 * 软件定时器信息的结构体类型定义
 */
struct SwTmrInfo {
    /* 定时器状态，三种状态:Free,Created,Running,Expired */
    U8 state;
    /* 保留字段 */
    U8 resved[3];
    /* 定时器类型，两种类型:周期性、一次性 */
    enum TmrMode mode;
    /* 定时器超时间隔 */
    U32 interval;
    /* 定时器离超时剩余的ms数 */
    U32 remainMs;
    /* 定时器超时处理函数 */
    SreTmrProcFunc handler;
};

/*
 * @ingroup  sre_timer
 * @brief 创建定时器。
 *
 * @par 描述
 * 创建一个属性为createPara的定时器，返回定时器逻辑ID tmrHandle。
 *
 * @attention
 * <ul>
 * <li>SD6182x平台可以使用2个硬件定时器资源,Cortex_RX平台不支持硬件定时器功能。</li>
 * <li>如果用户打开Tick开关则可创建硬件定时器个数少一个。</li>
 * <li>中断处理函数handler的第一个参数是创建的定时器的逻辑编号。</li>
 * <li>定时器创建成功后并不立即开始计数，需显式调用#SRE_TimerStart或者#SRE_TimerRestart启动。</li>
 * <li>对于周期定时模式的定时器，建议用户不要把定时间隔设置的过低，避免一直触发定时器的处理函数。</li>
 * <li>struct TimerCreatePara参数里面的interval元素表示定时器周期，软件定时器单位是ms，</li>
 * <li>核内硬件定时器、全局硬件定时器单位是us，设置时间间隔的时候请注意适配，过大会出现溢出。</li>
 * </ul>
 *
 * @param createPara [IN]  类型#struct TimerCreatePara *，定时器创建参数
 * @param tmrHandle  [OUT] 类型#TimerHandle *，定时器句柄
 *
 * @retval #OS_ERRNO_TIMER_INPUT_PTR_NULL                      0x02002e01，指针参数为空。
 * @retval #OS_ERRNO_TIMER_PROC_FUNC_NULL                      0x02002e02，定时器回调函数为空。
 * @retval #OS_ERRNO_TIMER_INTERVAL_INVALID                    0x02002e04，定时器周期参数非法。
 * @retval #OS_ERRNO_TIMER_MODE_INVALID                        0x02002e05，定时器工作模式参数非法。
 * @retval #OS_ERRNO_TIMER_TYPE_INVALID                        0x02002e06，定时器类型参数非法。
 * @retval #OS_ERRNO_TIMER_HWTMR_NOT_INIT                      0x02002e08，硬件定时器没有初始化。
 * @retval #OS_ERRNO_TIMERGROUP_ID_INVALID                     0x02002e57，传入定时器组组号非法。
 * @retval #OS_ERRNO_SWTMR_INTERVAL_NOT_SUITED                 0x02002e44，定时器设置的定时周期转化为Tick数后不为整数。
 * @retval #OS_ERRNO_SWTMR_INTERVAL_OVERFLOW                   0x02002e55，软件定时器定时周期过大。
 * @retval #OS_ERRNO_SWTMR_MAXSIZE                             0x02002e48，创建的定时器个数已达到最大值。
 * @retval #OS_ERRNO_HWTMR_US2CYCLE_OVERFLOW                   0x02002e25，硬件定时器定时周期us转换cycle溢出，
 * @retval                                                                 SD6183平台除外。
 * @retval #OS_ERRNO_SHARED_TIMER_US2CYCLE_IS_ZERO             0x02002e76，全局硬件定时器定时周期us转换cycle后为0。
 * @retval #OS_ERRNO_SHARED_TIMER_US2CYCLE_OVERFLOW            0x02002e77，全局硬件定时器定时周期us转换cycle溢出。
 * @retval #OS_ERRNO_HWTMR_UNAVAILABLE                         0x02002e21，没有可分配的硬件定时器。
 * @retval #OS_ERRNO_SHARED_TIMER_ID_INVALID                   0x02002e72，传入的全局硬件定时器ID不可用。
 * @retval #OS_ERRNO_SHARED_TIMER_FUNC_NO_NULL                 0x02002e70，创建全局硬件定时器时传入的
 * @retval                                                                 定时超时处理函数非空。
 * @retval #OS_ERRNO_SHARE_TIMER_CORE_CREATE_PRIVATE_INVALID   0x02002e85，SD6801E平台创建硬线程12核及13核的
 * @retval                                                                 SOC私有定时器创建失败。
 * @retval #SRE_OK                                             0x00000000，定时器创建成功。
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TimerDelete
 */
extern U32 SRE_TimerCreate(struct TimerCreatePara *createPara, TimerHandle *tmrHandle);

/*
 * @ingroup  sre_timer
 * @brief 删除定时器。
 *
 * @par 描述
 * 释放一个定时器逻辑ID为tmrHandle的定时器资源。
 *
 * @attention
 * <ul>
 * <li>硬件定时器删除后将停止计数。</li>
 * <li>删除处于超时状态下的软件定时器时，OS采用延时删除的方式。详细说明请见手册注意事项。</li>
 * <li>对于Tring定时器，建议用户在调用定时器start或stop操作后等待一段时间，</li>
 * <li>用户需保证等硬件真正完成后，再进行删除操作。</li>
 * </ul>
 *
 * @param mid       [IN]  类型#U32，模块号，当前未使用，忽略
 * @param tmrHandle [IN]  类型#TimerHandle，定时器句柄，通过SRE_TimerCreate接口获取
 *
 * @retval #OS_ERRNO_TIMER_HWTMR_NOT_INIT                    0x02002e08，硬件定时器没有初始化。
 * @retval #OS_ERRNO_TIMER_HANDLE_INVALID                    0x02002e03，定时器句柄非法。
 * @retval #OS_ERRNO_SWTMR_NOT_CREATED                       0x02002e4a，软件定时器未创建。
 * @retval #OS_ERRNO_HWTMR_NOT_CREATED                       0x02002e24，硬件定时器未创建。
 * @retval #OS_ERRNO_SHARED_TIMER_NOT_CREATED                0x02002e73，全局共享硬件定时器未创建。
 * @retval #SRE_OK                                           0x00000000，定时器删除成功。
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TimerCreate
 */
extern U32 SRE_TimerDelete(U32 mid, TimerHandle tmrHandle);

/*
 * @ingroup  sre_timer
 * @brief 启动定时器。
 *
 * @par 描述
 * 将定时器逻辑ID为tmrHandle的定时器由创建状态变成启动状态。
 *
 * @attention
 * <ul>
 * <li>创建后初次启动定时器时，从设置的值开始计数；如果重复启动或者启动后停止然后再启动，</li>
 * <li>从重复启动点或者停止点的计数值开始计数。</li>
 * <li>对于SD6183x平台全局硬件定时器，无论是第一次启动还是重复启动，下一次启动都从初始值开始计时。</li>
 * <li>对于单次触发模式，触发一次后，可以调用启动接口再次启动该定时器，时间间隔为设置的时间间隔。</li>
 * <li>启动处于超时状态下的软件定时器时，OS采用延时启动的方式。详细说明请见手册注意事项。</li>
 * <li>对于共享硬件定时器，该接口功能跟#SRE_TimerRestart功能完全一致，都是重启定时器，重新计时。</li>
 * <li>调用该接口执行Tring定时器start操作后，</li>
 * <li>用户需等待一定时间（最长一个精度），保证硬件真正完成Tring定时器启动操作后，再进行删除或其他相关操作。</li>
 * </ul>
 *
 * @param mid       [IN]  类型#U32，模块号，当前未使用，忽略
 * @param tmrHandle [IN]  类型#TimerHandle，定时器句柄，通过#SRE_TimerCreate接口获取
 *
 * @retval #OS_ERRNO_TIMER_HWTMR_NOT_INIT                    0x02002e08，硬件定时器没有初始化。
 * @retval #OS_ERRNO_SWTMR_NOT_CREATED                       0x02002e4a，软件定时器未创建。
 * @retval #OS_ERRNO_TIMER_HANDLE_INVALID                    0x02002e03，定时器句柄非法。
 * @retval #OS_ERRNO_HWTMR_NOT_CREATED                       0x02002e24，硬件定时器未创建。
 * @retval #OS_ERRNO_SHARED_TIMER_NOT_CREATED                0x02002e73，全局共享硬件定时器未创建。
 * @retval #SRE_OK                                           0x00000000，定时器启动成功。
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TimerStop
 */
extern U32 SRE_TimerStart(U32 mid, TimerHandle tmrHandle);

/*
 * @ingroup  sre_timer
 * @brief 停止定时器。
 *
 * @par 描述
 * 停止定时器逻辑ID为tmrHandle的定时器计数，使定时器由计时状态变成创建状态。
 *
 * @attention
 * <ul>
 * <li>定时器停止后，下一次启动将重新从停止点的计数值开始计数。</li>
 * <li>但是对于SD6183x全局硬件定时器，下一次启动从初始值开始计时。</li>
 * <li>不能停止未启动的定时器。</li>
 * <li>停止处于超时状态下的软件定时器时，OS采用延时停止的方式。详细说明请见手册注意事项。</li>
 * <li>SD6183x的核内硬件定时器在停止过程中如果发生超时，则剩余时间为0，但不会响应定时器处理函数。</li>
 * <li>Tring定时器不支持停止操作。</li>
 * </ul>
 *
 * @param mid       [IN]  类型#U32，模块号，当前未使用，忽略
 * @param tmrHandle [IN]  类型#TimerHandle，定时器句柄，通过#SRE_TimerCreate接口获取
 *
 * @retval #OS_ERRNO_TIMER_HWTMR_NOT_INIT                    0x02002e08，硬件定时器没有初始化。
 * @retval #OS_ERRNO_SWTMR_NOT_CREATED                       0x02002e4a，软件定时器未创建。
 * @retval #OS_ERRNO_TIMER_HANDLE_INVALID                    0x02002e03，定时器句柄非法。
 * @retval #OS_ERRNO_HWTMR_NOT_CREATED                       0x02002e24，硬件定时器未创建。
 * @retval #OS_ERRNO_HWTMR_NOT_START                         0x02002e27，硬件定时器定时未启动。
 * @retval #OS_ERRNO_SWTMR_UNSTART                           0x02002e4d，定时器已经处于未启动状态。
 * @retval #OS_ERRNO_SHARED_TIMER_NOT_CREATED                0x02002e73，全局共享硬件定时器未创建。
 * @retval #OS_ERRNO_SHARED_TIMER_NOT_START                  0x02002e74，全局共享硬件定时器未启动。
 * @retval #SRE_OK                                           0x00000000，定时器停止成功。
 *
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TimerStart
 */
extern U32 SRE_TimerStop(U32 mid, TimerHandle tmrHandle);

/*
 * @ingroup  sre_timer
 * @brief 重启定时器
 *
 * @par 描述
 * 重启定时器逻辑ID为tmrHandle的定时器计数，对于停止过的定时器，相当于恢复到刚创建时的定时时长开始计时。
 *
 * @attention
 * <ul>
 * <li>重启处于超时状态下的软件定时器时，OS采用延时重启的方式。详细说明请见手册注意事项。</li>
 * <li>Tring定时器不支持重启操作。</li>
 * </ul>
 *
 * @param mid       [IN]  类型#U32，模块号，当前未使用，忽略
 * @param tmrHandle [IN]  类型#TimerHandle，定时器句柄，通过#SRE_TimerCreate接口获取
 *
 * @retval #OS_ERRNO_SWTMR_NOT_CREATED                       0x02002e4a，软件定时器未创建。
 * @retval #OS_ERRNO_TIMER_HWTMR_NOT_INIT                    0x02002e08，硬件定时器没有初始化。
 * @retval #OS_ERRNO_TIMER_HANDLE_INVALID                    0x02002e03，定时器句柄非法。
 * @retval #OS_ERRNO_HWTMR_NOT_CREATED                       0x02002e24，硬件定时器未创建。
 * @retval #OS_ERRNO_SHARED_TIMER_NOT_CREATED                0x02002e73，全局共享硬件定时器未创建。
 * @retval #SRE_OK                                           0x00000000，定时器重启成功。
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TimerStop
 */
extern U32 SRE_TimerRestart(U32 mid, TimerHandle tmrHandle);

/*
 * @ingroup  sre_timer
 * @brief 定时器剩余超时时间查询
 *
 * @par 描述
 * 输入定时器句柄tmrHandle，获得对应定时器超时剩余时间expireTime。
 *
 * @attention
 * <ul>
 * <li>软件定时器单位毫秒，核内和全局硬件定时器单位微秒。</li>
 * <li>由于OS内部软件定时器采用Tick作为计时单位，硬件定时器采用Cycle作为计时单位，</li>
 * <li>所以剩余时间转化成ms或us不一定是整数，当转化后的ms或us数不为整数时，返回的剩余时间是该ms或us数取整后+1;</li>
 * <li>例如转化后ms数为4.2，则最终用户得到的剩余时间是5ms。</li>
 * </ul>
 *
 * @param mid        [IN]  类型#U32，模块号，当前未使用，忽略
 * @param tmrHandle  [IN]  类型#TimerHandle，定时器句柄，通过#SRE_TimerCreate接口获取
 * @param expireTime [OUT] 类型#U32 *，定时器的剩余的超时时间，共享和私有硬件定时器单位us，软件定时器单位ms
 *
 * @retval #OS_ERRNO_TIMER_HANDLE_INVALID                    0x02002e03，硬件定时器句柄不可用。
 * @retval #OS_ERRNO_TIMER_HWTMR_NOT_INIT                    0x02002e08，硬件定时器没有初始化。
 * @retval #OS_ERRNO_SWTMR_NOT_CREATED                       0x02002e4a，软件定时器未创建。
 * @retval #OS_ERRNO_HWTMR_NOT_CREATED                       0x02002e24，硬件定时器未创建。
 * @retval #OS_ERRNO_SHARED_TIMER_ID_INVALID                 0x02002e72，全局共享硬件定时器句柄不可用。
 * @retval #OS_ERRNO_TIMER_SHARED_HWTMR_NOT_INIT             0x02002e09，全局共享硬件定时器没有初始化。
 * @retval #OS_ERRNO_SHARED_TIMER_NOT_CREATED                0x02002e73，全局共享硬件定时器未创建。
 * @retval #SRE_OK                                           0x00000000，定时器剩余超时时间查询成功。
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TimerCreate
 */
extern U32 SRE_TimerQuery(U32 mid, TimerHandle tmrHandle, U32 *expireTime);

/*
 * @ingroup  sre_timer
 * @brief 设置硬件定时器时间间隔。
 *
 * @par 描述
 * 设置定时器ID为tmrHandle的硬件定时器时间间隔interVal，硬件定时器按照新设置的时间间隔计时。
 *
 * @attention
 * <ul>
 * <li>软件定时器不能使用该接口。</li>
 * <li>共享和私有硬件定时器单位us。</li>
 * <li>该接口支持无线所有平台。</li>
 * </ul>
 *
 * @param mid       [IN]  类型#U32，模块号，当前未使用忽略。
 * @param tmrHandle [IN]  类型#TimerHandle，定时器句柄，通过#SRE_TimerCreate接口获取。
 * @param interVal  [IN]  类型#U32，核内和全局硬件定时器单位微秒。
 *
 * @retval #OS_ERRNO_TIMER_HANDLE_INVALID                    0x02002e03，硬件定时器句柄不可用。
 * @retval #OS_ERRNO_TIMER_HWTMR_NOT_INIT                    0x02002e08，硬件定时器没有初始化。
 * @retval #OS_ERRNO_TIMER_INTERVAL_INVALID                  0x02002e04，定时器周期参数非法。
 * @retval #OS_ERRNO_HWTMR_NOT_CREATED                       0x02002e24，硬件定时器未创建。
 * @retval #OS_ERRNO_HWTMR_US2CYCLE_OVERFLOW                 0x02002e25，硬件定时器定时周期us转换cycle溢出。
 * @retval #OS_ERRNO_SHARED_TIMER_ID_INVALID                 0x02002e03，全局共享硬件定时器句柄不可用。
 * @retval #OS_ERRNO_TIMER_SHARED_HWTMR_NOT_INIT             0x02002e09，全局共享硬件定时器没有初始化。
 * @retval #OS_ERRNO_SHARED_TIMER_NOT_CREATED                0x02002e73，全局共享硬件定时器未创建。
 * @retval #OS_ERRNO_SHARED_TIMER_US2CYCLE_IS_ZERO           0x02002e76，全局共享硬件定时器定时周期us转换cycle后为0。
 * @retval #OS_ERRNO_SHARED_TIMER_US2CYCLE_OVERFLOW          0x02002e77，全局共享硬件定时器定时周期us转换cycle溢出。
 * @retval #SRE_OK                                           0x00000000，定时器时间间隔设置成功。
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_TimerCreate
 */
extern U32 SRE_HwTmrSetInterval(U32 mid, TimerHandle tmrHandle, U32 interVal);

/*
 * @ingroup  sre_timer
 * @brief 获取软件定时器的统计信息。
 *
 * @par 描述
 * 获取系统内软件定时器的使用状态statInfo(使用数、启动数、空闲数)。
 *
 * @attention
 * <ul>
 * <li>查询出的定时器使用数包括处于创建态，计时态和超时处理态的定时器个数。</li>
 * <li>查询出的定时器启动数包括处于计时态和超时处理态的定时器个数。</li>
 * </ul>
 *
 * @param statInfo [OUT] 类型#struct SwTmrStatInfo *，统计信息。
 *
 * @retval #OS_ERRNO_SWTMR_RET_PTR_NULL         0x02002e47，信息返回地址参数为空。
 * @retval #SRE_OK                              0x00000000，获取定时器统计信息成功。
 * @par 依赖
 * <ul><li>SRE_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SwTmrInfoGet
 */
extern U32 SRE_SwTmrStatInfoGet(struct SwTmrStatInfo *statInfo);

/*
 * @ingroup  sre_timer
 * @brief 获取指定软件定时器的信息。
 *
 * @par 描述
 * 根据指定的定时器ID，获取定时器ID为tmrHandle的定时器的信息info。
 *
 * @attention
 * <ul>
 * <li>由于OS内部采用Tick作为软件定时器的时钟源，所以剩余时间转化成ms不一定是整数，</li>
 * <li>当转化后的毫秒数不为整数时，返回的剩余时间是该毫秒数取整后+1;</li>
 * <li>例如转化后毫秒数为4.2，则最终用户得到的剩余时间是5ms。</li>
 * </ul>
 *
 * @param tmrHandle [IN]  类型#TimerHandle，定时器句柄；
 * @param info      [OUT] 类型#struct SwTmrInfo *，存放定时器信息结构体指针。
 *
 * @retval #OS_ERRNO_TIMER_HANDLE_INVALID       0x02002e03，软件定时器句柄非法。
 * @retval #OS_ERRNO_SWTMR_NOT_CREATED          0x02002e4a，定时器未创建。
 * @retval #OS_ERRNO_SWTMR_RET_PTR_NULL         0x02002e47，信息返回地址参数为空。
 * @retval #SRE_OK                              0x00000000，获取指定定时器的信息成功。
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_SwTmrStatInfoGet
 */
extern U32 SRE_SwTmrInfoGet(TimerHandle tmrHandle, struct SwTmrInfo *info);

/*
 * @ingroup  sre_timer
 * @brief 创建软件定时器组。
 *
 * @par 描述
 * 创建一个属性为config的软件定时器组，返回软件定时器组号groupID。
 *
 * @attention
 * <ul>
 * <li>软件定时器组时钟源只能指定为SRE_TIMER_GRP_SRC_TICK。</li>
 * </ul>
 *
 * @param config  [IN]  类型#struct TmrGrpUserCfg *，定时器组创建结构体
 * @param groupId [OUT] 类型#TimerGroupId *，返回成功创建后的软件定时器组号
 *
 * @retval #OS_ERRNO_TIMERGROUP_INPUT_PTR_NULL     0x02002e54，软件定时器组入参指针为空。
 * @retval #OS_ERRNO_TIMER_SOURCE_NOT_TICK         0x02002e40，软件定时器组的时钟源类型不为TICK类型。
 * @retval #OS_ERRNO_TICK_NOT_INIT                 0x02002e53，TICK没有初始化。
 * @retval #OS_ERRNO_TIMER_NUM_ZERO                0x02002e41，软件定时器组的最大定时器个数为零。
 * @retval #OS_ERRNO_TIMER_NUM_TOO_LARGE           0x02002e58，软件定时器组的最大定时器个数大于0xffff。
 * @retval #OS_ERRNO_SWTMR_NO_MEMORY               0x02002e50，初始化内存不足。
 * @retval #OS_ERRNO_TIMER_TICKGROUP_CREATED       0x02002e56，Tick定时器组已创建。
 * @retval #SRE_OK                                 0x00000000，定时器组创建成功。
 * @par 依赖
 * <ul><li>sre_timer.h：该接口声明所在的头文件。</li></ul>
 * @see无
 */
extern U32 SRE_SwTimerGroupCreate(struct TmrGrpUserCfg *config, TimerGroupId *groupId);

/*
 * @ingroup  sre_timer
 * @brief 共享硬件定时器自检接口。
 *
 * @par 描述
 * 输入共享硬件定时器编号，检查共享硬件定时器是否可用。
 *
 * @attention
 * <ul>
 * <li>该接口支持SD6182x等无线平台。</li>
 * </ul>
 *
 * @param tmrIdx [IN]  类型#U32，全局硬件定时器编号[0,7]。
 *
 * @retval #OS_ERRNO_SHARED_TIMER_ID_INVALID    0x02002e72，共享硬件定时器ID不可用。
 * @retval #OS_ERRNO_SHARED_TIMER_IS_BAD        0x02002e75，共享硬件定时器不可用。
 * @retval #OS_ERRNO_SHARED_TIMER_IN_USING      0x02002e80，共享硬件定时器正在使用，不能自检。
 * @retval #SRE_OK                              0x00000000，共享硬件定时器自检成功。
 * @par 依赖
 * SRE_timer.h：该接口声明所在的头文件。
 * @see 无
 */
extern U32 SRE_McHwTmrSelfCheck(U32 tmrIdx);

/*
 * @ingroup  sre_timer
 * @brief 共享硬件定时器通道组VMID配置。
 *
 * @par 描述
 * 配置共享硬件定时器通道组VMID，以保护通道资源表中的全局定时器资源。
 *
 * @attention
 * <ul>
 * <li>该接口只有SD6183x共享硬件定时器使用，其他定时器不可使用。</li>
 * <li>仅MCPU调用，否则硬件会上报错误中断。</li>
 * </ul>
 *
 * @param chgId   [IN]  类型#U32，通道组ID，取值范围为[0,3]。
 * @param chgMap  [IN]  类型#U32，通道配置资源表，取值范围为[1,0xff]。
 * @param chgVmid [IN]  类型#U32，通道组VMID值，取值范围为[0,7]。
 *
 * @retval #OS_ERRNO_SHARE_TIMER_CHG_IDX_ILLEGAL      0x02002e81，共享硬件定时器ID不可用。
 * @retval #OS_ERRNO_SHARE_TIMER_VMID_VALUE_ILLEGAL   0x02002e82，共享硬件定时器不可用。
 * @retval #OS_ERRNO_SHARE_TIMER_MAP_VALUE_TOO_LARGE  0x02002e83，共享硬件定时器正在使用，不能自检。
 * @retval #OS_ERRNO_SHARE_TIMER_MAP_VALUE_ZERO       0x02002e84，共享硬件定时器正在使用，不能自检。
 * @retval #SRE_OK                                    0x00000000，共享硬件定时器通道组vmid配置成功。
 * @par 依赖
 * SRE_timer.h：该接口声明所在的头文件。
 * @see 无
 */
extern U32 SRE_McHwTmrChgCfg(U32 chgId, U32 chgMap, U32 chgVmid);

/*
 * @ingroup  sre_timer
 * @brief 强制删除共享硬件定时器资源。
 *
 * @par 描述
 * 强制删除物理ID为mcHwTmrId的全局定时器资源。
 *
 * @attention
 * <ul>
 * <li>该接口只有SD6183x共享硬件定时器使用，其他定时器不可使用。</li>
 * <li>VMID权限由用户保证(该接口会操作Timer类寄存器)，若违反VMID权限，会上报错误中断。</li>
 * </ul>
 *
 * @param mcHwTmrId [IN]  类型#U32，全局定时器物理ID，取值范围为[0,7]。
 *
 * @retval #OS_ERRNO_SHARED_TIMER_ID_INVALID       0x02002e72，共享硬件定时器物理ID非法。
 * @retval #SRE_OK                                 0x00000000，强制删除共享硬件定时器资源成功。
 * @par 依赖
 * SRE_timer.h：该接口声明所在的头文件。
 * @see 无
 */
extern U32 SRE_McHwTmrForceDelete(U32 mcHwTmrId);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SRE_TIMER_H */
