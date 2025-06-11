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
#include "uart_regs.h"
#include "bm_common.h"
#ifndef __UART_CORE_H__
#define __UART_CORE_H__

#define MAX_UART_NUM 8
#define UART_BUSY_TIMEOUT 1000000

#ifndef IS_ODD
#define IS_ODD 2
#endif

int bm_uart_reg_read(int uart_no, unsigned int offset, unsigned int *val);
void bm_uart_reg_write(int uart_no, unsigned int offset, unsigned int val);
void bm_uart_set_dll_dlh(int uart_no, unsigned int dll, unsigned int dlh);
void bm_uart_get_dll_dlh(int uart_no, unsigned int *dll, unsigned int *dlh);
void bm_uart_set_fifo_ctrl(int uart_no, unsigned int fifo_ctrl);
void bm_uart_set_lcr(int uart_no, unsigned int lcr);
int bm_uart_txfifo_full(int uart_no);
int bm_uart_rx_ready(int uart_no);
void bm_uart_tx_char(int uart_no, const char c);
void bm_uart_rx_char(int uart_no, char *c);
int bm_uart_wait_idle(int uart_no, unsigned int timeout);
int bm_uart_core_base_addr(int uart_no, unsigned int *reg_base);
#endif /* __UART_CORE_H__ */