/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <stdarg.h>
#include "sre_typedef.h"
#include "securec.h"
#include "sre_base.h"
#include "sre_hwi.h"
#include "sre_shell.h"
#include "sre_log.h"
#include "bm_uart.h"
#include "sre_common.h"
#include "securec.h"
#include "sre_mcs.h"
#include "openamp/open_amp.h"
#include "os_cpu_armv8_external.h"

typedef U32 (*PrintFunc)(const char *format, va_list vaList);
#define OS_MAX_SHOW_LEN 0x200
#define SHELL_MAX_PARSE_LEN 20

static void uart_recv_hwi(void)
{
#if defined(BM_OPEN_UART_DEVICE) && (BM_OPEN_UART_DEVICE == 1)
    char data;
    bm_uart_rx(UART_NUM4, &data);
    SRE_SendChrToShell(data);
#endif
}

static unsigned int SRE_SendChr2ChlHookDefault(char c)
{
    bm_uart_tx(UART_NUM4, c);

    return 0;
}

static U32 SRE_UartInit(void)
{
    U32 ret;
    (void)SRE_HwiDelete(CORE_SYS_UART4_INTID);

    int uart_num = UART_NUM4;
    serial_cfg uart_cfg = {
        .hw_uart_no = uart_num,
        .uart_src_clk = UART_CLK,
        .data_bits = UART_DATA_8BIT, /* default data_bits is 8 */
        .stop = UART_STOP_1BIT,
        .pen = UART_VERIFY_DISABLE,
        .baud_rate = 115200 /* default baud_rate is 115200 */
    };

    ret = bm_uart_init(&uart_cfg);
    if (ret) {
        return OS_FAIL;
    }

#if defined(CONFIG_VIRTUAL_SERIAL) && (CONFIG_VIRTUAL_SERIAL == 0)
    ret = bm_uart_set_irq_enable(uart_num, 1);
    if (ret) {
        return OS_FAIL;
    }

    ret = SRE_HwiSetAttr(CORE_SYS_UART4_INTID, 0, OS_HWI_MODE_ENGROSS);
    if (ret != OS_OK) {
        return ret;
    }

    ret = SRE_HwiCreate(CORE_SYS_UART4_INTID, (HwiProcFunc)uart_recv_hwi, CORE_SYS_UART4_INTID);
    if (ret != OS_OK) {
        return ret;
    }

    ret = SRE_HwiEnable(CORE_SYS_UART4_INTID);
    if (ret != OS_OK) {
        return ret;
    }

    OsHwiAffinitySet(CORE_SYS_UART4_INTID, 1 << LIBCK_CORE_ID);

    SRE_SendChrToChlHookReg((OsSendChr2ChlHookFunc)SRE_SendChr2ChlHookDefault);
#endif

    return OS_OK;
}

static void TestPutc(unsigned char ch)
{
    bm_uart_tx(UART_NUM4, ch);
    if (ch == '\n') {
        bm_uart_tx(UART_NUM4, '\r');
    }
}

static int TestPrintf(const char *format, va_list vaList)
{
    int len;
    char buff[OS_MAX_SHOW_LEN] = {0};
    char *str = buff;

    len = vsnprintf_s(buff, OS_MAX_SHOW_LEN, OS_MAX_SHOW_LEN - 1, format, vaList);
    if (len == -1) {
        return len;
    }
#if defined(CONFIG_VIRTUAL_SERIAL) && (CONFIG_VIRTUAL_SERIAL == 1)
    SRE_DelayUs(100); /* delay 100us */
    int buffer_len;
    void *buffer_addr = sre_mcs_get_zero_cpy_data_addr(ENDPOINT_VIRT_SERIAL, &buffer_len);
    if (buffer_len < len) {
        return OS_ERROR;
    }

    if (memcpy_s(buffer_addr, buffer_len, str, len) != EOK) {
        return OS_ERROR;
    }

    sre_mcs_send(ENDPOINT_VIRT_SERIAL, buffer_addr, len);
#else
    while (*str != '\0') {
        TestPutc(*str);
        str++;
    }
#endif

    return OS_OK;
}

int virtual_serial_callback(struct rpmsg_endpoint *ept, void *data, size_t len, unsigned int src, void *priv)
{
    if (len >= SHELL_MAX_PARSE_LEN) {
        SRE_Printf("Shell Error: invalid commond len = %d\r\n", len);
        return OS_ERROR;
    }
    for (int i = 0; i < len; i++) {
        SRE_SendChrToShell(((char*)data)[i]);
    }
    return OS_OK;
}

U32 SRE_Printf(const char *format, ...);
U32 SRE_Printf(const char *format, ...)
{
    S32 count = 0;
#if defined(BM_OPEN_UART_DEVICE) && (BM_OPEN_UART_DEVICE == 1)
    va_list vaList;

    va_start(vaList, format);
    count = TestPrintf(format, vaList);
    va_end(vaList);

#endif
    return count;
}

U32 SRE_PrintfInit(void);
U32 SRE_PrintfInit(void)
{
#if defined(BM_OPEN_UART_DEVICE) && (BM_OPEN_UART_DEVICE == 1)
    U32 ret;

    ret = SRE_UartInit();
    if (ret != OS_OK) {
        return ret;
    }

    ret = SRE_PrintHookReg((PrintFunc)TestPrintf);
    if (ret != OS_OK) {
        return ret;
    }
#endif
    return OS_OK;
}
