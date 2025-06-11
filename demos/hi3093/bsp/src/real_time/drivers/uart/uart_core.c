/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 * UniProton is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 * 	http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * See the Mulan PSL v2 for more details.
 */
#include "uart_core.h"
#include "bm_uart.h"

#define uart_reg_read(addr) (*(volatile unsigned int *)(((uintptr_t)(addr))))
#define uart_reg_write(value, addr) (*(volatile unsigned int *)((uintptr_t)(addr)) = (unsigned int)(value))

/* get UART register base addr */
int bm_uart_core_base_addr(int uart_no, unsigned int *reg_base)
{
    if (uart_no > UART_MAX_NUM || uart_no < UART_MIN_NUM) {
        return BM_FAIL;
    }

    unsigned long reg_bases[] = {
        UART0_REG_BASE,
        UART1_REG_BASE,
        UART2_REG_BASE,
        UART3_REG_BASE,
        UART4_REG_BASE,
        UART5_REG_BASE,
        UART6_REG_BASE,
        UART7_REG_BASE,
    };

    *reg_base = reg_bases[uart_no];

    return BM_OK;
}

int bm_uart_reg_read(int uart_no, unsigned int offset, unsigned int *val)
{
    int ret;
    unsigned int reg_base = 0x0;

    ret = bm_uart_core_base_addr(uart_no, &reg_base);
    if (ret) {
        return ret;
    }

    *val = uart_reg_read((unsigned long)(reg_base + offset));
    return BM_OK;
}

void bm_uart_reg_write(int uart_no, unsigned int offset, unsigned int val)
{
    int ret;
    unsigned int reg_base = 0x0;

    ret = bm_uart_core_base_addr(uart_no, &reg_base);
    if (ret) {
        return;
    }

    uart_reg_write(val, (unsigned long)(reg_base + offset));
    return;
}

static void bm_uart_set_lcr_dlab(int uart_no, int dlab_sel)
{
    int ret;
    unsigned int lcr = 0;

    ret = bm_uart_reg_read(uart_no, DW_UART_LCR, &lcr);
    if (ret) {
        return;
    }

    if (dlab_sel) {
        lcr |= DW_UART_DLAB;
    } else {
        lcr &= ~DW_UART_DLAB;
    }

    bm_uart_reg_write(uart_no, DW_UART_LCR, lcr);
    return;
}

void bm_uart_set_dll_dlh(int uart_no, unsigned int dll, unsigned int dlh)
{
    /* enable DLL/DLH/FCR access */
    bm_uart_set_lcr_dlab(uart_no, 1);
    bm_uart_reg_write(uart_no, DW_UART_DLL, dll);
    bm_uart_reg_write(uart_no, DW_UART_DLH, dlh);
    bm_uart_set_lcr_dlab(uart_no, 0);
    return;
}

void bm_uart_get_dll_dlh(int uart_no, unsigned int *dll, unsigned int *dlh)
{
    /* enable DLL/DLH/FCR access */
    bm_uart_set_lcr_dlab(uart_no, 1);
    bm_uart_reg_read(uart_no, DW_UART_DLL, dll);
    bm_uart_reg_read(uart_no, DW_UART_DLH, dlh);
    bm_uart_set_lcr_dlab(uart_no, 0);
    return;
}

void bm_uart_set_fifo_ctrl(int uart_no, unsigned int fifo_ctrl)
{
    /* enable DLL/DLH/FCR access */
    bm_uart_set_lcr_dlab(uart_no, 1);
    bm_uart_reg_write(uart_no, DW_UART_FCR, fifo_ctrl);
    bm_uart_set_lcr_dlab(uart_no, 0);
    return;
}

void bm_uart_set_lcr(int uart_no, unsigned int lcr)
{
    bm_uart_reg_write(uart_no, DW_UART_LCR, lcr);
    return;
}

int bm_uart_txfifo_full(int uart_no)
{
    int ret;
    unsigned int usr = 0;

    ret = bm_uart_reg_read(uart_no, DW_UART_USR, &usr);
    if (ret) {
        return BM_OK;
    }

    return (usr & DW_XFIFO_NOT_FULL) ? 0 : 1;
}

int bm_uart_rx_ready(int uart_no)
{
    unsigned int lsr = 0;

    (void)bm_uart_reg_read(uart_no, DW_UART_LSR, &lsr);
    return (lsr & DW_UART_LSR_DDR) ? 1 : 0;
}

void bm_uart_tx_char(int uart_no, const char c)
{
    bm_uart_reg_write(uart_no, DW_UART_THR, (unsigned int)c);
    return;
}

void bm_uart_rx_char(int uart_no, char *c)
{
    unsigned int rbr = 0;

    (void)bm_uart_reg_read(uart_no, DW_UART_RBR, &rbr);
    *c = (char)(rbr & 0xFF);

    return;
}

int bm_uart_wait_idle(int uart_no, unsigned int timeout)
{
    unsigned int usr = 0;
    unsigned int time = timeout;
    while (time) {
        (void)bm_uart_reg_read(uart_no, DW_UART_USR, &usr);
        if ((usr & (DW_UART_BUSY)) == 0) {
            return BM_OK;
        }
        time--;
    }

    return -1;
}
