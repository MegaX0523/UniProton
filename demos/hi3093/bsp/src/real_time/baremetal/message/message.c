/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 */

#include "bm_message.h"
#include "bm_common.h"
#include "bm_config.h"
#include "bm_atomic.h"
#include "bm_hwi.h"
#include "securec.h"

typedef struct {
    unsigned int msg_id;
    unsigned int msg_base;
    unsigned int msg_len;
} msg_mng_inst;

#if defined(MSG_PERF_STAT_ENABLE) && (MSG_PERF_STAT_ENABLE == 1)
unsigned long long g_recv_perf[MSG_ID_BOTTOM] = {0};
#endif

static const unsigned int g_ipi_trigger[] = {
    HI_SHARED_CORE0_IPI_ARG,
    HI_SHARED_CORE1_IPI_ARG,
    HI_SHARED_CORE2_IPI_ARG,
    HI_SHARED_CORE3_IPI_ARG,
};

static const msg_mng_inst g_msg_inst[MSG_ID_BOTTOM] = {
    {MSG_ID_0, MSG0_BASE, MSG0_BODY_LEN},
    {MSG_ID_1, MSG1_BASE, MSG1_BODY_LEN},
    {MSG_ID_2, MSG2_BASE, MSG2_BODY_LEN},
    {MSG_ID_3, MSG3_BASE, MSG3_BODY_LEN},
    {MSG_ID_4, MSG4_BASE, MSG4_BODY_LEN},
};

#define CHECK_MAGIC_WORD_ENABLE 0

#define get_flag_status(addr) (*(unsigned int *)(uintptr_t)(addr)&0xff)
#define set_flag_status(addr, value) (*(unsigned int *)(uintptr_t)(addr) = msg_head_flag(value))
#define get_flag_magic(addr) (*(unsigned int *)(uintptr_t)(addr) >> 8) // 1Byte=8bit

static inline void set_trigger(unsigned int recv_cpu, unsigned int msg_id)
{
    *(unsigned int *)(uintptr_t)g_ipi_trigger[recv_cpu & 3] = msg_id; // cpuid <= 3
}

INIT_TEXT void bm_msg_init(void);
INIT_TEXT void bm_msg_init(void)
{
    int size = sizeof(g_msg_inst) / sizeof(g_msg_inst[0]);
    for (int i = 0; i < size; i++) {
        set_flag_status(g_msg_inst[i].msg_base, MSG_STATUS_FREE);
    }
    for (int i = 0; i < BM_MAX_CORE_NUM; i++) {
        set_trigger(i, 0);
    }
}

static inline unsigned int bm_check_magic(unsigned int msg_id)
{
#if defined(CHECK_MAGIC_WORD_ENABLE) && (CHECK_MAGIC_WORD_ENABLE == 1)
    if (get_flag_magic(g_msg_inst[msg_id].msg_base) != MAGIC_NUM) {
        bm_log("Fatal error! msg[%d] buf[0x%x] dameged!!!\n", msg_id, g_msg_inst[msg_id].msg_base);
        return BM_FAIL;
    }
#else
    (void)msg_id;
#endif
    return BM_OK;
}

int bm_cmp_xchg_status(unsigned int msg_id, msg_status_enum val, msg_status_enum old)
{
    if (msg_id >= MSG_ID_BOTTOM || val >= MSG_STATUS_BOTTOM) {
        bm_log("msg_id:%d val:%d error!\n", msg_id, val);
        return BM_FAIL;
    }
    unsigned int *addr = (unsigned int *)(uintptr_t)g_msg_inst[msg_id].msg_base;
    return bm_cmp_xchg32bits(addr, msg_head_flag(val), msg_head_flag(old));
}

void bm_get_msg_status(unsigned int msg_id, msg_status_enum *status)
{
    if (msg_id >= MSG_ID_BOTTOM || status == NULL) {
        bm_log("msg_id:%d or status:%d error!\n", msg_id, status);
        return;
    }
    (void)bm_check_magic(msg_id);
    *status = get_flag_status(g_msg_inst[msg_id].msg_base);
}

void bm_set_msg_status(unsigned int msg_id, msg_status_enum status)
{
    if (msg_id >= MSG_ID_BOTTOM || status >= MSG_STATUS_BOTTOM) {
        bm_log("msg_id:%d or status:%d error!\n", msg_id, status);
        return;
    }
    (void)bm_check_magic(msg_id);
    set_flag_status(g_msg_inst[msg_id].msg_base, status);
}

unsigned char *bm_get_ipi_msg(void)
{
    unsigned int cpu = bm_get_coreid();
    unsigned int msg_id = *(unsigned int *)(uintptr_t)g_ipi_trigger[cpu & 3];
    if (msg_id >= MSG_ID_BOTTOM) {
        return NULL;
    }
    return (unsigned char *)(uintptr_t)g_msg_inst[msg_id].msg_base + MSG_HEAD_LEN;
}

unsigned int bm_send_msg(unsigned int msg_id, unsigned char *buf, unsigned int buf_len)
{
    if (msg_id >= MSG_ID_BOTTOM || buf == NULL || is_invalid_addr_len((unsigned int)(uintptr_t)buf, buf_len)) {
        return BM_FAIL;
    }
    (void)bm_check_magic(msg_id);
    unsigned int *addr = (unsigned int *)(uintptr_t)g_msg_inst[msg_id].msg_base;
    int ret = bm_cmp_xchg32bits(addr, msg_head_flag(MSG_STATUS_USING), msg_head_flag(MSG_STATUS_FREE));
    if (ret) {
        bm_log("cmp_xchg32 error! old:0x%x\n", get_flag_status(addr));
        return BM_FAIL;
    }
    unsigned char *msg_buf = (unsigned char *)(uintptr_t)g_msg_inst[msg_id].msg_base + MSG_HEAD_LEN;
    unsigned int dest_max = g_msg_inst[msg_id].msg_len;
    if (buf_len > dest_max) {
        bm_atomic_set(addr, msg_head_flag(MSG_STATUS_FREE));
        dsb();
        return BM_FAIL;
    }
    ret = memcpy_s(msg_buf, dest_max, buf, buf_len);
    if (ret != 0) {
        bm_log("memcpy_s error! buf_len:%d\n", buf_len);
        bm_atomic_set(addr, msg_head_flag(MSG_STATUS_FREE));
        dsb();
        return BM_FAIL;
    }
    bm_atomic_set(addr, msg_head_flag(MSG_STATUS_FREE));
    dsb();
    return BM_OK;
}

unsigned int bm_send_msg_with_ipi(unsigned int recv_cpu, unsigned int msg_id, unsigned char *buf, unsigned int buf_len)
{
#if defined(MSG_PERF_STAT_ENABLE) && (MSG_PERF_STAT_ENABLE == 1)
    if (msg_id < MSG_ID_BOTTOM) {
        *(unsigned long long *)(HI_SHARED_IPI_PERF_BUF) = bm_get_tick();
    }
#endif
    unsigned int target_list = 0;
    int ret = bm_send_msg(msg_id, buf, buf_len);
    if (ret != BM_OK) {
        return BM_FAIL;
    }
    if (recv_cpu >= BM_MAX_CORE_NUM) {
        return BM_FAIL;
    }
    set_trigger(recv_cpu, msg_id);
    target_list |= (1U << recv_cpu);
    bm_hwi_trigger(IPI_INT_NO, target_list);
    return BM_OK;
}

unsigned char *bm_get_msg(unsigned int msg_id)
{
    if (msg_id >= MSG_ID_BOTTOM) {
        return NULL;
    }
    return (unsigned char *)(uintptr_t)g_msg_inst[msg_id].msg_base + MSG_HEAD_LEN;
}

unsigned int bm_recv_msg(unsigned int msg_id, unsigned char *buf, unsigned int buf_len)
{
    if (msg_id >= MSG_ID_BOTTOM || buf == NULL || is_invalid_addr_len((unsigned int)(uintptr_t)buf, buf_len)) {
        bm_log("msg_id:%d error!\n", msg_id);
        return BM_FAIL;
    }
    (void)bm_check_magic(msg_id);
    unsigned int *addr = (unsigned int *)(uintptr_t)g_msg_inst[msg_id].msg_base;
    int ret = bm_cmp_xchg32bits(addr, msg_head_flag(MSG_STATUS_USING), msg_head_flag(MSG_STATUS_FREE));
    if (ret) {
        bm_log("cmp_xchg32 error! old:0x%x\n", get_flag_status(addr));
        return BM_FAIL;
    }
    unsigned char *msg_buf = (unsigned char *)(uintptr_t)g_msg_inst[msg_id].msg_base + MSG_HEAD_LEN;
    unsigned int dest_max = g_msg_inst[msg_id].msg_len;
    int real_len = (buf_len > dest_max) ? dest_max : buf_len;
    ret = memcpy_s(buf, dest_max, msg_buf, real_len);
    if (ret != 0) {
        bm_log("memcpy_s error! buf_len:%d\n", real_len);
        bm_atomic_set(addr, msg_head_flag(MSG_STATUS_FREE));
        dsb();
        return BM_FAIL;
    }
    bm_atomic_set(addr, msg_head_flag(MSG_STATUS_FREE));
    dsb();
    return BM_OK;
}

unsigned int bm_recv_msg_with_ipi(unsigned int *msg_id, unsigned char *buf, unsigned int buf_len)
{
    if (msg_id == NULL || buf == NULL) {
        bm_log("input null ptr!\n");
        return BM_FAIL;
    }
    unsigned int cpu = bm_get_coreid();
    *msg_id = *(unsigned int *)(uintptr_t)g_ipi_trigger[cpu & 3]; // core_id <= 3
    int ret = bm_recv_msg(*msg_id, buf, buf_len);
    if (ret != BM_OK) {
        return BM_FAIL;
    }
#if defined(MSG_PERF_STAT_ENABLE) && (MSG_PERF_STAT_ENABLE == 1)
    unsigned long long end_cycle = bm_get_tick();
    unsigned long long beg_cycle = *(unsigned long long *)(HI_SHARED_IPI_PERF_BUF);
    if (end_cycle > beg_cycle && beg_cycle != 0 && end_cycle != 0) {
        g_recv_perf[*msg_id] = end_cycle - beg_cycle;
    }
#endif
    return BM_OK;
}

void bm_msg_show_perf(void)
{
#if defined(MSG_PERF_STAT_ENABLE) && (MSG_PERF_STAT_ENABLE == 1)
    int size = sizeof(g_msg_inst) / sizeof(g_msg_inst[0]);
    for (int i = 0; i < size; i++) {
        if (g_recv_perf[i] == 0) {
            continue;
        }
        bm_log("msgid[%d] ipi msg cost: %llu \n", i, g_recv_perf[i]);
    }
#else
	bm_log("should define MSG_PERF_STAT_ENABLE first \n");
#endif
}