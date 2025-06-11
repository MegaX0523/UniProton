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
#include <stdarg.h>
#include "uart_core.h"
#include "uart_regs.h"
#include "bm_uart.h"
#include "bm_common.h"
#include "bm_types.h"
#include "securec.h"
#include "bm_dmac.h"
#if defined(BM_OPEN_UART_DEVICE) && (BM_OPEN_UART_DEVICE == 1)
unsigned char g_print_init_done = 0;
unsigned char g_print_uart_num = 0;

static bm_uart_callback g_uart_dma_tx_callback_array[UART_NUM_BUTT] = { 0 };
static bm_uart_callback g_uart_dma_rx_callback_array[UART_NUM_BUTT] = { 0 };
static const int uart_exist_array[UART_NUM_BUTT] = {
    dt_device_exists(UART, UART0),
    dt_device_exists(UART, UART1),
    dt_device_exists(UART, UART2),
    dt_device_exists(UART, UART3),
    dt_device_exists(UART, UART4),
    dt_device_exists(UART, UART5),
    dt_device_exists(UART, UART6),
    dt_device_exists(UART, UART7),
};

#ifndef DIV_ROUND_CLOSEST
#define DIV_ROUND_CLOSEST(x, divisor, result)                                                             \
    ({                                                                                                    \
        typeof(x)__x = x;                                                                                 \
        typeof(divisor)__d = divisor;                                                                     \
        result = (((typeof(x))-1) > 0 || ((typeof(divisor))-1) > 0 || (((__x) > 0) == ((__d) > 0))) ? \
            (((__x) + ((__d) / 2)) / (__d)) :                                                             \
            (((__x) - ((__d) / 2)) / (__d));                                                              \
    })
#endif

static int bm_uart_busy_timeout(void)
{
    return UART_BUSY_TIMEOUT;
}

static int uart_check_valid(unsigned int uart_num)
{
    if (uart_num > UART_MAX_NUM || uart_num < UART_MIN_NUM) {
        return BM_FAIL;
    }

    if (!uart_exist_array[uart_num]) {
        return BM_FAIL;
    }

    return BM_OK;
}

/* caculate UART divisor factor */
static void bm_calc_uart_dll_dlh(unsigned int uart_clk, unsigned int baud_rate, unsigned int *dll, unsigned int *dlh)
{
    unsigned int divisor;

    /*
     * baud_rate = bus_clock / (16 * divisor)
     * ==> divisor = bus_clock/(baud_rate * 16) */
    DIV_ROUND_CLOSEST(uart_clk, (16 * baud_rate), divisor); /* 16 just for codestyle */
    *dll = divisor & 0xFF;
    *dlh = (divisor >> 8) & 0xFF; /* take high 8 bits */
}

static unsigned int bm_calc_lcr_reg_val(serial_cfg *cfg)
{
    unsigned int lcr = 0;

    lcr |= cfg->data_bits;

    /* 0 - 1 stop bit 1 - 2 stop bit */
    if (cfg->stop != UART_STOP_1BIT) {
        lcr |= DW_UART_STOP;
    }

    if (cfg->pen) {
        lcr |= DW_UART_PEN;
        if (cfg->eps) {
            lcr |= DW_UART_EPS;
        }
    }

    return lcr;
}

static int bm_uart_printf(const char *format, va_list va_list)
{
    int len;
    char buff[BM_MAX_SHOW_LEN] = {0};
    char *str = buff;

    len = vsnprintf_s(buff, BM_MAX_SHOW_LEN, BM_MAX_SHOW_LEN - 1, format, va_list);
    if (len == -1) {
        return len;
    }

    while (*str != '\0') {
        bm_uart_tx(g_print_uart_num, *str);
        if ((*str) == '\n') {
            bm_uart_tx(g_print_uart_num, '\r');
        }
        str++;
    }
    return BM_OK;
}

static void bm_uart_io_mux_cfg(serial_cfg *cfg)
{
    unsigned int uart_txiomux_bases[] = {
        0,
        0,
        UART2_TX_IOMUX_REG_BASE,
        UART3_TX_IOMUX_REG_BASE,
        UART4_TX_IOMUX_REG_BASE,
        UART5_TX_IOMUX_REG_BASE,
        UART6_TX_IOMUX_REG_BASE,
        UART7_TX_IOMUX_REG_BASE,
    };

    unsigned int uart_rxiomux_bases[] = {
        0,
        0,
        UART2_RX_IOMUX_REG_BASE,
        UART3_RX_IOMUX_REG_BASE,
        UART4_RX_IOMUX_REG_BASE,
        UART5_RX_IOMUX_REG_BASE,
        UART6_RX_IOMUX_REG_BASE,
        UART7_RX_IOMUX_REG_BASE,
    };

    if (cfg->hw_uart_no <= UART_NUM4) {
        *(unsigned int *)(uintptr_t)(uart_txiomux_bases[cfg->hw_uart_no]) = 0;
        *(unsigned int *)(uintptr_t)(uart_rxiomux_bases[cfg->hw_uart_no]) = 0;
    } else if (cfg->hw_uart_no <= UART_NUM6) {
        *(unsigned int *)(uintptr_t)(uart_txiomux_bases[cfg->hw_uart_no]) = 0x3;
    } else {
        *(unsigned int *)(uintptr_t)(uart_txiomux_bases[cfg->hw_uart_no]) = 1;
        *(unsigned int *)(uintptr_t)(uart_rxiomux_bases[cfg->hw_uart_no]) = 1;
    }
}

static void bm_uart_set_baud_rate(serial_cfg *cfg)
{
    unsigned int dll = 0;
    unsigned int dlh = 0;

    /* wait till uart is idle */
    (void)bm_uart_wait_idle(cfg->hw_uart_no, bm_uart_busy_timeout());
    bm_calc_uart_dll_dlh(cfg->uart_src_clk, cfg->baud_rate, &dll, &dlh);
    bm_uart_set_dll_dlh(cfg->hw_uart_no, dll, dlh);
}

int bm_uart_init(serial_cfg *cfg)
{
    if ((cfg == NULL) || (cfg->hw_uart_no > UART_MAX_NUM) || (cfg->hw_uart_no < UART_MIN_NUM) ||
        (cfg->data_bits > UART_DATA_8BIT) || (cfg->data_bits < UART_DATA_5BIT) || (cfg->stop > UART_STOP_2BIT) ||
        (cfg->stop < UART_STOP_1BIT) || (cfg->pen > UART_VERIFY_ENABLE) || (cfg->pen < UART_VERIFY_DISABLE) ||
        (cfg->eps > UART_VERIFY_EVEN) || (cfg->eps < UART_VERIFY_ODD)) {
        return BM_FAIL;
    }

    /* Stop bit judgment */
    if ((cfg->stop == UART_STOP_1P5BIT) && (cfg->data_bits != UART_DATA_5BIT)) {
        return BM_FAIL;
    }

    /* Clock judgment */
    if (cfg->uart_src_clk != UART_CLK) {
        return BM_FAIL;
    }

    if (!uart_exist_array[cfg->hw_uart_no]) {
        return BM_FAIL;
    }

    unsigned int fifo_ctrl;

    bm_uart_io_mux_cfg(cfg);
    /* config FIFO,DLL,DLH at first */
    bm_uart_set_baud_rate(cfg);
    fifo_ctrl = FIFOENA | UART_FCR_RXCLR | UART_FCR_TXCLR;
    bm_uart_set_fifo_ctrl(cfg->hw_uart_no, fifo_ctrl);
    /* set data bits, stop bit, parity check */
    bm_uart_set_lcr(cfg->hw_uart_no, bm_calc_lcr_reg_val(cfg));
    return BM_OK;
}

int bm_uart_deinit(bm_uart_num uart_no)
{
    if (uart_check_valid(uart_no) != BM_OK) {
        return BM_FAIL;
    }

    int ret = bm_uart_set_irq_enable(uart_no, 0);
    return ret;
}

int bm_uart_enable_print(bm_uart_num uart_no)
{
    if (uart_check_valid(uart_no) != BM_OK) {
        return BM_FAIL;
    }

    g_print_init_done = 1;
    g_print_uart_num = uart_no;
    return BM_OK;
}

int bm_uart_tx(bm_uart_num uart_no, const char c)
{
    if (uart_check_valid(uart_no) != BM_OK) {
        return BM_FAIL;
    }

    int timeout = 0;
    int max_timeout = bm_uart_busy_timeout();

    while (bm_uart_txfifo_full(uart_no)) {
        timeout++;
        if (timeout >= max_timeout) {
            return BM_FAIL;
        }
    }
    bm_uart_tx_char(uart_no, c);
    return BM_OK;
}

int bm_uart_rx(bm_uart_num uart_no, char *out)
{
    if ((uart_check_valid(uart_no) != BM_OK) || (out == NULL)) {
        return BM_FAIL;
    }

    int timeout = 0;
    int max_timeout = bm_uart_busy_timeout();

    while (!bm_uart_rx_ready(uart_no)) {
        timeout++;
        if (timeout >= max_timeout) {
            return BM_FAIL;
        }
    }

    bm_uart_rx_char(uart_no, out);
    return BM_OK;
}

int bm_uart_raw_puts(bm_uart_num uart_no, const char *str, unsigned int len)
{
    if ((uart_check_valid(uart_no) != BM_OK) || (str == NULL) ||
        is_invalid_addr_len((unsigned int)(uintptr_t)str, len)) {
        return BM_FAIL;
    }

    unsigned int length = len;
    while (*str != '\0' && length > 0) {
        if (bm_uart_tx(uart_no, *str++) != BM_OK) {
            return BM_FAIL;
        }
        length--;
    }
    return BM_OK;
}

int bm_printf(const char *format, ...)
{
    if (g_print_init_done == 0) {
        return BM_FAIL;
    }
    va_list va_list;
    int count;

    va_start(va_list, format);
    count = bm_uart_printf(format, va_list);
    va_end(va_list);

    return count;
}

/* Value: 0: disabled; 1: enabled */
int bm_uart_set_irq_enable(bm_uart_num uart_no, unsigned int status)
{
    if (uart_check_valid(uart_no) != BM_OK) {
        return BM_FAIL;
    }

    unsigned int is_enable = 0;
    if (status != 0) {
        is_enable = 1;
    }
    bm_uart_reg_write(uart_no, ELSI, is_enable);
    return BM_OK;
}

int bm_uart_get_status(bm_uart_num uart_no, bm_uart_status *status)
{
    if ((uart_check_valid(uart_no) != BM_OK) || (status == NULL)) {
        return BM_FAIL;
    }
    int ret = bm_uart_reg_read(uart_no, DW_UART_LSR, (unsigned int *)status);
    return ret;
}

static bm_dmac_req uart_dma_get_dmac_req(int id, int is_read)
{
    if (id <= UART_NUM4) {
        if (is_read == 1) {
            return (DMAC_REQ_SEL_UART2_RX + (id - UART_NUM2) * IS_ODD);
        } else {
            return (DMAC_REQ_SEL_UART2_TX + (id - UART_NUM2) * IS_ODD);
        }
    } else {
        if (is_read == 1) {
            return (DMAC_REQ_SEL_UART5_RX + (id - UART_NUM5) * IS_ODD);
        } else {
            return (DMAC_REQ_SEL_UART5_TX + (id - UART_NUM5) * IS_ODD);
        }
    }
}

static int uart_dma_get_id_from_req(bm_dmac_req req)
{
    if (req <= DMAC_REQ_SEL_UART4_RX) {
        return (req - DMAC_REQ_SEL_UART2_TX) / IS_ODD + UART_NUM2;
    } else {
        return (req - DMAC_REQ_SEL_UART5_TX) / IS_ODD + UART_NUM5;
    }
}

static void uart_dma_finish_func(bm_dmac_req req, bm_dmac_transmit_async_t type)
{
    bm_uart_callback uart_callback = NULL;
    bm_uart_transmit_async_t state = UART_TRANSMIT_BUTT;
    int uart_id = uart_dma_get_id_from_req(req);

    if (req % IS_ODD == 0) {
        if (type != DMAC_TRANSMIT_OK) {
            state = UART_WRITE_FAIL;
        } else {
            state = UART_WRITE_OK;
        }
        uart_callback = g_uart_dma_tx_callback_array[uart_id];
    } else {
        if (type != DMAC_TRANSMIT_OK) {
            state = UART_READ_FAIL;
        } else {
            state = UART_READ_OK;
        }
        uart_callback = g_uart_dma_rx_callback_array[uart_id];
    }
    if (uart_callback) {
        uart_callback(uart_id, state);
    }
}

int bm_uart_rx_dma(bm_uart_num uart_no, char *out, unsigned int len, bm_uart_callback callback)
{
    if ((uart_check_valid(uart_no) != BM_OK) || (out == NULL) ||
        is_invalid_addr_len((unsigned int)(uintptr_t)out, len)) {
        return BM_FAIL;
    }
    int ret;
    int dma_channel = uart_dma_get_dmac_req(uart_no, 1);
    bm_dmac_channel_cfg channel_cfg = { 0 };

    unsigned int base_addr;
    ret = bm_uart_core_base_addr(uart_no, &base_addr);
    if (ret) {
        return BM_FAIL;
    }

    /* Configure the DMA to start transmission. */
    channel_cfg.req = dma_channel;
    channel_cfg.src_addr = (uintptr_t)(base_addr + DW_UART_THR);
    channel_cfg.dest_addr = (uintptr_t)(out);
    channel_cfg.data_len = len;
    channel_cfg.trans_type = TRASFER_TYPE_P2M;
    g_uart_dma_rx_callback_array[uart_no] = callback;
    ret = bm_dmac_transmit_async(&channel_cfg, uart_dma_finish_func);
    return ret;
}

int bm_uart_tx_dma(bm_uart_num uart_no, const char *str, unsigned int len, bm_uart_callback callback)
{
    if ((uart_check_valid(uart_no) != BM_OK) || (str == NULL) ||
        is_invalid_addr_len((unsigned int)(uintptr_t)str, len)) {
        return BM_FAIL;
    }

    int ret;
    int dma_channel = uart_dma_get_dmac_req(uart_no, 0);
    bm_dmac_channel_cfg channel_cfg = { 0 };

    unsigned int base_addr;
    ret = bm_uart_core_base_addr(uart_no, &base_addr);
    if (ret) {
        return BM_FAIL;
    }

    /* Configure the DMA to start transmission. */
    channel_cfg.req = dma_channel;
    channel_cfg.src_addr = (uintptr_t)(str);
    channel_cfg.dest_addr = (uintptr_t)(base_addr + DW_UART_THR);
    channel_cfg.data_len = len;
    channel_cfg.trans_type = TRASFER_TYPE_M2P;
    g_uart_dma_tx_callback_array[uart_no] = callback;
    ret = bm_dmac_transmit_async(&channel_cfg, uart_dma_finish_func);
    return ret;
}

#else
int bm_uart_init(serial_cfg *cfg)
{
    (void)cfg;
    return BM_OK;
}

int bm_uart_deinit(bm_uart_num uart_no)
{
    (void)uart_no;
    return BM_OK;
}

int bm_uart_enable_print(bm_uart_num uart_no)
{
    (void)uart_no;
    return BM_OK;
}

int bm_printf(const char *format, ...)
{
    (void)format;
    return BM_OK;
}

int bm_uart_set_irq_enable(bm_uart_num uart_no, unsigned int status)
{
    (void)uart_no;
    (void)status;
    return BM_OK;
}

int bm_uart_tx(bm_uart_num uart_no, const char c)
{
    (void)uart_no;
    (void)c;
    return BM_OK;
}

int bm_uart_rx(bm_uart_num uart_no, char *out)
{
    (void)uart_no;
    (void)out;
    return BM_OK;
}

int bm_uart_raw_puts(bm_uart_num uart_no, const char *str, unsigned int len)
{
    (void)uart_no;
    (void)str;
    (void)len;
    return BM_OK;
}

int bm_uart_get_status(bm_uart_num uart_no, bm_uart_status *status)
{
    (void)uart_no;
    (void)status;
    return BM_OK;
}

int bm_uart_rx_dma(bm_uart_num uart_no, char *out, unsigned int len, bm_uart_callback callback)
{
    (void)uart_no;
    (void)out;
    (void)len;
    (void)callback;
    return BM_OK;
}

int bm_uart_tx_dma(bm_uart_num uart_no, const char *str, unsigned int len, bm_uart_callback callback)
{
    (void)uart_no;
    (void)str;
    (void)len;
    (void)callback;
    return BM_OK;
}
#endif