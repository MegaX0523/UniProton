/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
#include "bm_config.h"
#include "bm_uart.h"
#include "bm_hwi.h"
#include "hwi_init.h"

#define REASON_SIZE 8

struct irq_action {
    interrupt_handler_t *handler;
    void *arg;
    unsigned int count;
};

struct exc_regs {
    uintptr_t spsr;
    uintptr_t elr;
    uintptr_t esr;
    uintptr_t xregs[31]; // xregs[0]=x0, xregs[30]=x30
};

char *g_exc_reason[] = {
    "(SP_EL0) Synchronous",
    "(SP_EL0) IRQ/vIRQ",
    "(SP_EL0) FIQ/vFIQ",
    "(SP_EL0) Error",
    "(SP_ELx) Synchronous",
    "(SP_ELx) IRQ/vIRQ", // do_irq
    "(SP_ELx) FIQ/vFIQ",
    "(SP_ELx) Error"
};
void exception_proc(unsigned int exc_type, struct exc_regs *regs);
void exception_proc(unsigned int exc_type, struct exc_regs *regs)
{
    if (exc_type < REASON_SIZE) {
        bm_printf("exception: %s [core%d]\n", g_exc_reason[exc_type], bm_get_coreid());
    }
    bm_printf("esr   : %016lx   elr   : %016lx\n", regs->esr, regs->elr);
    bm_printf("spsr  : %016lx   sp    : %016lx\n", regs->spsr, (uintptr_t)regs + sizeof(struct exc_regs));
    for (int i = 0; i < 29; i += 2) { // 29 reg num
        bm_printf("x%-6d: %016lx   x%-6d: %016lx\n", i, regs->xregs[i], i + 1, regs->xregs[i + 1]);
    }
    bm_printf("\n[core%d] enter infinite loop!!!\n", bm_get_coreid());
    while (1) {
    };
}

#define SYS_NUM_IRQS 1024
static struct irq_action g_irq_handlers[SYS_NUM_IRQS] = { {0} };
#if defined(BM_INT_PERF) && (BM_INT_PERF == 1)
static int_perf_hook g_interrput_perf_hook[SYS_NUM_IRQS] = { 0 };
#endif

void do_irq(void);
void do_irq(void)
{
    unsigned int irq = 0;
    asm volatile("MRS %0," reg_alias(ICC_IAR1_EL1) " \n" : "=&r"(irq) : : "memory");
    irq = irq & 0x3ff;
#if defined(BM_INT_PERF) && (BM_INT_PERF == 1)
    if (g_interrput_perf_hook[irq]) {
        g_interrput_perf_hook[irq]();
    }
#endif
    if (g_irq_handlers[irq].handler) {
        g_irq_handlers[irq].handler(g_irq_handlers[irq].arg);
        g_irq_handlers[irq].count++;
    }
    asm volatile("MSR " reg_alias(ICC_EOIR1_EL1) ", %0   \n" : : "r"(irq)); // 对寄存器的写操作表示中断的结束
    asm volatile("MSR " reg_alias(ICC_DIR_EL1) ", %0   \n" : : "r"(irq)); // 对该寄存器的写操作将deactive指定的中断。
}

int irq_install_handler(unsigned int irq, interrupt_handler_t *handler, void *arg)
{
    if (irq > MAX_INT_NUM) {
        bm_printf("irq_install_handler: bad irq number %d\n", irq);
        return BM_FAIL;
    }

    if (g_irq_handlers[irq].handler != NULL) {
        bm_printf("info: irq_install_handler: replaced! \n");
    }
    g_irq_handlers[irq].handler = handler;
    g_irq_handlers[irq].arg = arg;
    g_irq_handlers[irq].count = 0;

    return BM_OK;
}

int irq_free_handler(unsigned int irq)
{
    if (irq > MAX_INT_NUM) {
        bm_printf("irq_uninstall_handler: bad irq number %d\n", irq);
        return BM_FAIL;
    }

    g_irq_handlers[irq].handler = NULL;
    g_irq_handlers[irq].arg = NULL;
    g_irq_handlers[irq].count = 0;

    return BM_OK;
}

#if defined(BM_INT_PERF) && (BM_INT_PERF == 1)
int irq_perf_register(unsigned int int_id, int_perf_hook hook)
{
    if (int_id > MAX_INT_NUM) {
        bm_printf("irq_perf_register: bad irq number %d\n", int_id);
        return BM_FAIL;
    }

    if (hook == NULL) {
        bm_printf("irq_perf_register: int_perf_hookLL %d\n");
        return BM_FAIL;
    }

    g_interrput_perf_hook[int_id] = hook;

    return BM_OK;
}
#endif
