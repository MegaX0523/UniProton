add_definitions(-D__UNIPROTON__)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/net
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio
    ${CMAKE_CURRENT_SOURCE_DIR}/i2c
    ${CMAKE_CURRENT_SOURCE_DIR}/dma
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/cache
    ${CMAKE_CURRENT_SOURCE_DIR}/uart
    ${CMAKE_CURRENT_SOURCE_DIR}/timer
    ${CMAKE_CURRENT_SOURCE_DIR}/spi
    ${CMAKE_CURRENT_SOURCE_DIR}/watchdog
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/core/kernel/irq
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/core/kernel/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/core/ipc/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/arch/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/om/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/mem/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/include/posix
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/osal/posix/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../src/utility/lib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/osal/include
    ${CMAKE_CURRENT_SOURCE_DIR}/osal/include/module
)

set(SRCS net/gmac.c net/mdio.c net/motor_phy.c
    gpio/gpio.c gpio/gpio_pinmux_tbl.c
    i2c/i2c
    dma/dmac.c
    cache/cache.c cache/cache_asm.S
    uart/uart_core.c uart/uart.c
    timer/timer.c
    spi/spi.c
    watchdog/wdg.c
    common/lp_subsys.c
    common/bm_common.c
    common/io_config.c
    common/reg_sysctrl.c
    osal/uniproton/osal_debug.c
    osal/uniproton/osal_interrupt.c
    osal/uniproton/osal_semaphore.c
)

add_library(drivers OBJECT ${SRCS})
