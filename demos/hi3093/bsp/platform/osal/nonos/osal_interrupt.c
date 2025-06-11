/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: OS Abstract Layer.
 */
#include "soc_osal.h"

#ifdef __aarch64__
#define DAIF_IRQ_BIT      (1U << 1)
#define INT_MASK          (1U << 7)
#endif

unsigned int osal_irq_lock(void)
{
#ifdef __aarch64__
    unsigned long state = 0;

    __asm__ __volatile__("mrs %0, DAIF      \n"
                         "msr DAIFSet, %1   \n"
                         : "=r"(state)
                         : "i"(DAIF_IRQ_BIT)
                         : "memory", "cc");
    return state & INT_MASK;
#else
    return 0;
#endif
}

unsigned int osal_irq_unlock(void)
{
#ifdef __aarch64__
    unsigned long state = 0;

    __asm__ __volatile__("mrs %0, DAIF      \n"
                         "msr DAIFClr, %1   \n"
                         : "=r"(state)
                         : "i"(DAIF_IRQ_BIT)
                         : "memory", "cc");

    return state & INT_MASK;
#else
    return 0;
#endif
}

void osal_irq_restore(unsigned int irq)
{
#ifdef __aarch64__
    if ((irq & INT_MASK) == 0) {
        __asm__ __volatile__("msr DAIFClr, %0\n" : : "i"(DAIF_IRQ_BIT) : "memory", "cc");
    } else {
        __asm__ __volatile__("msr DAIFSet, %0\n" : : "i"(DAIF_IRQ_BIT) : "memory", "cc");
    }
#endif
}

int osal_in_interrupt(void)
{
    return OSAL_NONEOS_DEFAULT_RET;
}
