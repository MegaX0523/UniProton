/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 软定时器模块的内部头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-28 LibCK项目组  华为规范整改
 */
/* @defgroup SRE_swtmr 软定时器接口 */
#ifndef SRE_SWTMR_EXTERNAL_H
#define SRE_SWTMR_EXTERNAL_H

#include "sre_timer_external.h"
#include "sre_list_external.h"

/*
 * @ingroup  SRE_swtmr
 * @brief 获取低位的值。
 *
 * @par 描述
 * 宏定义，定义完成从一个32位的数获取低26位的值
 *
 * @attention 无
 * @param  num [IN] 源数。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_swtmr_external.h: 该宏定义所在的头文件。</li></ul>
 * @see UWROLLNUMSUB | UWROLLNUMADD | EVALUATE_L | EVALUATE_H | UWROLLNUM | UWSORTINDEX。
 */
#define UWROLLNUM(num) ((num) & 0x03ffffffU)
/*
 * @ingroup  SRE_swtmr
 * @brief 获取高位的值。
 *
 * @par 描述
 * 宏定义，定义完成从一个32位的数获取高位的值
 *
 * @attention 无
 * @param  num [IN] 源数。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_swtmr_external.h: 该宏定义所在的头文件。</li></ul>
 * @see UWROLLNUMSUB | UWROLLNUMADD | EVALUATE_L | EVALUATE_H | UWROLLNUM | UWSORTINDEX。
 */
#define UWSORTINDEX(num) ((num) >> 26)
/*
 * @ingroup  SRE_swtmr
 * @brief 定义高位赋值。
 *
 * @par 描述
 * 宏定义，屏蔽高6位，把值与到高6位
 *
 * @attention 无
 * @param  num [IN] 被赋值的变量。
 * @param  value [IN] 待赋的值或变量。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_swtmr_external.h: 该宏定义所在的头文件。</li></ul>
 * @see UWROLLNUMSUB | UWROLLNUMADD | EVALUATE_L | EVALUATE_H | UWROLLNUM | UWSORTINDEX。
 */
#define EVALUATE_H(num, value) ((num) = (((num) & 0x03ffffffU) | ((value) << 26)))

/*
 * @ingroup  SRE_swtmr
 * @brief 定义低位赋值。
 *
 * @par 描述
 * 宏定义，定义完成一个32位的数低位赋值的功能，高位不变。
 *
 * @attention 无
 * @param  num [IN] 被赋值的变量。
 * @param  value [IN] 待赋的值或变量。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_swtmr_external.h: 该宏定义所在的头文件。</li></ul>
 * @see UWROLLNUMSUB | UWROLLNUMADD | EVALUATE_L | EVALUATE_H | UWROLLNUM | UWSORTINDEX。
 */
#define EVALUATE_L(num, value) ((num) = (((num) & 0xfc000000U) | (value)))

/*
 * @ingroup  SRE_swtmr
 * @brief 定义两个数低位相减的功能。
 *
 * @par 描述
 * 宏定义，定义完成一个32位的数低位相加，结果存放到第一个参数，H(num1) = H(num1) - H(num2)。
 * 如 两个定时器控制块中sortData低位的rollNum进行相加。
 *
 * @attention 无
 * @param  num1 [IN] 被加数。
 * @param  num2 [IN] 加数。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_swtmr_external.h: 该宏定义所在的头文件。</li></ul>
 * @see UWROLLNUMSUB | UWROLLNUMADD | EVALUATE_L | EVALUATE_H | UWROLLNUM | UWSORTINDEX。
 */
#define UWROLLNUMADD(num1, num2) ((num1) = (((num1) & 0xfc000000U) | (UWROLLNUM(num1) + UWROLLNUM(num2))))

/*
 * @ingroup  SRE_swtmr
 * @brief 定义两个数低位相减的功能。
 *
 * @par 描述
 * 宏定义，定义完成一个32位的数低位相减，结果存放到第一个参数，H(num1) = H(num1) - H(num2)。
 * 如 两个定时器控制块中sortData低位的rollNum进行相减。
 *
 * @attention 无
 * @param  num1 [IN] 被减数。
 * @param  num2 [IN] 减数。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_swtmr_external.h: 该宏定义所在的头文件。</li></ul>
 * @see UWROLLNUMSUB | UWROLLNUMADD | EVALUATE_L | EVALUATE_H | UWROLLNUM | UWSORTINDEX。
 */
#define UWROLLNUMSUB(num1, num2) ((num1) = (((num1) & 0xfc000000U) | (UWROLLNUM(num1) - UWROLLNUM(num2))))

/*
 * @ingroup  SRE_swtmr
 * @brief 定义低位减1的功能。
 *
 * @par 描述
 * 宏定义，定义完成一个32位的数低位自减1。
 * 如 定时器控制块中sortData低位的rollNum进行自减1操作
 *
 * @attention 无
 * @param  num [IN] 低为待自减1的数。
 *
 * @retval 无
 * @par 依赖
 * <li>sre_swtmr_external.h: 该宏定义所在的头文件。</li></ul>
 * @see UWROLLNUMSUB | UWROLLNUMADD | EVALUATE_L | EVALUATE_H | UWROLLNUM | UWSORTINDEX。
 */
#define UWROLLNUMDEC(num) ((num) = ((num) - 1))

/* 最大支持软件定时器个数 */
extern U32 g_swTmrMaxNum;
OS_SEC_ALW_INLINE INLINE U32 OsSwTmrMaxNum(void)
{
    return g_swTmrMaxNum;
}
#define OS_TIMER_MAX_NUM ((TIMER_TYPE_SWTMR << 28) + OsSwTmrMaxNum())
#define OS_SWTMR_MIN_NUM (TIMER_TYPE_SWTMR << 28)

/*
 * 模块内宏定义
 */
#if !defined(OS_OPTION_SMP)
#define OS_SWTMR_SORTLINK_LEN   64U /* SortLink数组长度 */
#define OS_SWTMR_SORTLINK_MASK  0x3FUL
#endif
#define OS_TICK_SWTMR_GROUP_ID 0 /* 软件定时器组ID号 */

#define OS_SWTMR_STATUS_DEFAULT 0
#define OS_SWTMR_STATUS_MASK 0xfU
#define OS_SWTMR_PRE_STATUS_MASK 0xf0U

/*
 * 模块间宏定义
 */
#define OS_SWTMR_INDEX_2_ID(index) (((U32)(index)) + OS_SWTMR_MIN_NUM)
#define OS_SWTMR_ID_2_INDEX(timerId) ((timerId) - OS_SWTMR_MIN_NUM)

#if defined(OS_OPTION_SMP)
#define SWTMR_CREATE_DEL_LOCK()    OsSplLock(&g_tmrFreeList.spinLock)
#define SWTMR_CREATE_DEL_UNLOCK()  OsSplUnlock(&g_tmrFreeList.spinLock)
#else
#define SWTMR_CREATE_DEL_LOCK()
#define SWTMR_CREATE_DEL_UNLOCK()
#endif

/* 软件定时器结构定义 */
struct TagSwTmrCtrl {
    /* 指向前一个定时器 */
    struct TagSwTmrCtrl *prev;
    /* 指向下一个定时器 */
    struct TagSwTmrCtrl *next;
    /* 定时器状态 */
    U8 state;
    /* 定时器类型 */
    U8 mode;
    /* 软件定时器序号 */
    U16 swtmrIndex;
    /* 定时器SortLink属性，低位为rollNum，高位为sortIndex */
    U32 idxRollNum;
    /* 定时器超时时间 */
    U32 interval;
    /* 定时器用户参数1 */
    U32 arg1;
    /* 定时器用户参数2 */
    U32 arg2;
    /* 定时器用户参数3 */
    U32 arg3;
    /* 定时器用户参数4 */
    U32 arg4;
    /* 定时器超时处理函数 */
    SreTmrProcFunc handler;
#if defined(OS_OPTION_SMP)
    U32 coreID;
    U32 reserved;
#endif
#if (defined(OS_OPTION_TICKLESS) || defined(OS_OPTION_SMP))
    U64 expries;
#endif
#if defined(OS_OPTION_SWTMR_STAGGER)
    U32 toleranceCfg;
#endif
}; /* 定时器数据类型 */

struct TagSwTmrFreeList {
#if defined(OS_OPTION_SMP)
    volatile uintptr_t spinLock;
#endif
    struct TagSwTmrCtrl *freeList;
};

#if defined(OS_OPTION_SMP)
struct TagSwTmrSortLinkAttr {
    /* 自旋锁 */
    volatile uintptr_t spinLock;
    U32 reserved;
    /* 软件定时器头节点 */
    struct TagListObject sortLink;
    /* 最近超时的Tick刻度 */
    U64 nearestTicks;
#if defined(OS_OPTION_HOTPLUG_CPU)
    /* 链入软定时器链的托管链 */
    struct TagListObject agentList;
    /* 指向本软定时器链的托管链，未托管时=NULL */
    struct TagListAgent *swtmrAgent;
    /* 软定时器链托管迁移时，可能漏掉一次处理，新增字段避免该可能，详见插拔核的设计方案 */
    U32 nrNeedProc;
#endif
};
#else
struct TagSwTmrSortLinkAttr {
    /* Tick游标位置 */
    U16 cursor;
    /* 保留数据 */
    U16 unused;
    /* 软件定时器指针数组 */
    struct TagListObject *sortLink;
};
#endif

/*
 * 模块间typedef声明
 */
/*
 * 模块间全局变量声明
 */
/* 定时器内存空间首地址 */
extern struct TagSwTmrCtrl *g_swtmrCBArray;

/*
 * 模块间函数声明
 */
/*
 * 查询软件定时器剩余超时时间
 */
extern U32 OsSwTmrQuery(TimerHandle tmrHandle, U32 *expireTime);

/*
 * 模块间内联函数定义
 */
#endif /* SRE_SWTMR_EXTERNAL_H */
