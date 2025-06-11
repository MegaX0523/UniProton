/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: PID获取的对外头文件
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-01-01, LibCK项目组,  华为规范整改
 */
#ifndef SRE_HUNT_EXTERNAL_H
#define SRE_HUNT_EXTERNAL_H

#include "sre_base.h"
#include "sre_hunt.h"
#include "sre_task.h"
#include "sre_sys_external.h"

/* 模块间宏定义 */
#define OS_THREAD_NUM g_threadNum  // 当前进程中的线程数

#define OS_PROCESS_NAME_LEN OS_TSK_NAME_LEN  // 进程名长度
#define OS_PROCESS_INSTID_INVALID 0xffffffff  // 进程实例ID非法

#if defined(OS_OPTION_MODULARIZATION_LOAD)
// 一个核中进程可支持的线程总数,裁剪后加载任务模块的场景
#define OS_MAX_THREAD_NUM (g_swiMaxNum + g_sysCfgItem.tskModCfgItem.maxNum + 1)
#else
#define OS_MAX_THREAD_NUM OS_THREAD_NUM
#endif

#define OS_PROCESS_NUM 1 /* BARE OS 进程数为1 */

#if (!defined OS_OPTION_LP_PRELINK)
/* 线程结构 */
struct TagThreadRegS {
    /* 线程所在的进程信息指针, Reserve */
    char *processInfo;
    U32 threadPID;
    /* 线程名 */
    char name[OS_TSK_NAME_LEN];
};

/* 进程名结构 */
struct TagProcessInfoS {
    /* 进程对应的实例ID */
    U32 instID;
    /* 进程名,Reserve */
    char name[OS_PROCESS_NAME_LEN];
};
#else
/* 线程结构 */
/* IIU_BEGIN+5: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
struct TagThreadRegS {
    U32 threadPID;
    char *name;
};
#endif

/* IIU_BEGIN+19: Itype Independent Upgrade, 数字表示行数，当前行+数字表示范围(额外增加一行非空行) */
/* HUNT模块的控制头 */
struct TagThreadInfoS {
    /* 支持多进程时，该变量无效, Reserve */
    U8 swiNum;
    /* 支持多进程时，该变量无效, Reserve */
    U8 tskNum;
    /* 总共的线程数 */
    U8 totNum;
    /* 本核支持的进程数 */
    U8 processNum;
    /* 本核存放线程名的起始地址 + 进程名空间 */
#if (!defined OS_OPTION_LP_PRELINK)
    struct TagThreadRegS *threadLst;
#else
    struct TagThreadRegS **threadLst;
#endif
};

/* 保存在本核的线程信息 */
extern struct TagThreadRegS *g_localThreadInfo;
extern struct TagThreadInfoS *g_localThreadInfoHead;

/* 模块间函数声明 */
extern U32 OsHuntTID(U32 instID, U32 vcpuID, const char *threadName, U32 *tid);

#if defined(OS_OPTION_INST_RESET)
extern void OsHuntClear(U32 coreIndex);
#endif

/* 模块间内联函数定义 */
#endif /* SRE_HUNT_EXTERNAL_H */
