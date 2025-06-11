/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */
#include "bm_mem.h"

/*
 * 描述: 关闭全局可屏蔽中断。
 */
static uintptr_t bm_close_interrupt(void)
{
    uintptr_t state = 0;
    OS_EMBED_ASM(
        "mrs %0, DAIF      \n"
        "msr DAIFSet, %1   \n"
        : "=r"(state)
        : "i"(DAIF_IRQ_BIT)
        : "memory", "cc");

    return state & INT_MASK;
}

/*
 * 描述: 恢复原中断状态寄存器。
 */
static void bm_restore_interrupt(uintptr_t int_save)
{
    if ((int_save & INT_MASK) == 0) {
       OS_EMBED_ASM(
            "msr DAIFClr, %0\n"
            :
            : "i"(DAIF_IRQ_BIT)
            : "memory", "cc");
    } else {
        OS_EMBED_ASM(
            "msr DAIFSet, %0\n"
            :
            : "i"(DAIF_IRQ_BIT)
            : "memory", "cc");
    }
    return;
}

unsigned int bm_get_bits(unsigned int value)
{
    int i;
    unsigned int max = 0;
    unsigned int flag_bits = 31;
    for (i = flag_bits; i >= 0; i--) {
        if (((1U << (unsigned int)i) & value) != 0) {
            max = (unsigned int)i;
            break;
        }
    }

    return (flag_bits - max);
}

void *bm_memory_alloc(unsigned int size)
{
    void *addr;
    uintptr_t int_save;

    int_save = bm_close_interrupt();
    addr = g_memory_arith_api.alloc(size);
    bm_restore_interrupt(int_save);

    return addr;
}

void *bm_memory_alloc_align(unsigned int size, enum MemAlign align_pow)
{
    void *addr;
    uintptr_t int_save;

    int_save = bm_close_interrupt();
    addr = g_memory_arith_api.alloc_align(size, align_pow);
    bm_restore_interrupt(int_save);

    return addr;
}

unsigned int bm_memory_free(void *addr)
{
    unsigned int ret;
    uintptr_t int_save;

    int_save = bm_close_interrupt();
    ret = g_memory_arith_api.free(addr);
    bm_restore_interrupt(int_save);

    return ret;
}