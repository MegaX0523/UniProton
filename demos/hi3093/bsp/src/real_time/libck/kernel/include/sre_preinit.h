/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: 模块ID编号。
 * Author: LibCK项目组
 * Create: 2022-01-17
 */
#ifndef SRE_PREINIT_H
#define SRE_PREINIT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * constructor priorities must be integers from 0 to 65535 inclusive
 * 1-100:  gcc保留
 * 101-199:预留给产品
 * 200-299:预留给OS使用，222-236预留给OS内存算法扩展
 * 500-599:产品可裁剪的模块，520-539:给内存算法预留
 */
#define OS_LEVEL_SYS_INIT     220
#define OS_LEVEL_MEM_INIT     221
#define OS_LEVEL_HWI_INIT     237
#define OS_LEVEL_NSM_INIT     238
#define OS_LEVEL_HARDDRV_INIT 239
#define OS_LEVEL_HOOK_INIT    240
#define OS_LEVEL_EXC_INIT     241
#define OS_LEVEL_CACHE_INIT   242
#define OS_LEVEL_MSGM_INIT    243
#define OS_LEVEL_HWSEM_INIT   244
#define OS_LEVEL_TRING_INIT   245
#define OS_LEVEL_MBX_INIT     246
#define OS_LEVEL_CPUHP_INIT   247
#define OS_LEVEL_DL_INIT      248
#define OS_LEVEL_SCHED_INIT   249
#define OS_LEVEL_CRE_INIT     250
#define OS_LEVEL_MCCOM_INIT   251
#define OS_LEVEL_HUNT_INIT    252
#define OS_LEVEL_MMU_INIT     253
#define OS_LEVEL_SREMNG_INIT  254
#define OS_LEVEL_OSAL_INIT    255

#define OS_LEVEL_SYNC_INIT       540
#define OS_LEVEL_MCHW_TIMER_INIT 541
#define OS_LEVEL_HW_TIMER_INIT   542
#define OS_LEVEL_SWI_INIT        543
#define OS_LEVEL_TSK_INIT        544
#define OS_LEVEL_STKMON_INIT     545
#define OS_LEVEL_TSKMON_INIT     546
#define OS_LEVEL_TICK_INIT       547
#define OS_LEVEL_SWTMR_INIT      548
#define OS_LEVEL_MSG_INIT        549
#define OS_LEVEL_EVENT_INIT      550
#define OS_LEVEL_CPUP_INIT       551
#define OS_LEVEL_CORESLEEP_INIT  552
#define OS_LEVEL_SEM_INIT        553
#define OS_LEVEL_QUEUE_INIT      554
#define OS_LEVEL_TRACE_INIT      555
#define OS_LEVEL_DEVICE_INIT     556
#define OS_LEVEL_SYMBOL_INIT     557
#define OS_LEVEL_SHELL_INIT      558
#define OS_LEVEL_LP_INIT         559
#define OS_LEVEL_PAT_INIT        560

#define OS_INIT_SECTIONS()               \
    .init_array : {                      \
        __init_os_array_start = .;       \
        KEEP (*(SORT(.init_array.002*))) \
        __init_os_array_end = .;         \
    }

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_PREINIT_H */
