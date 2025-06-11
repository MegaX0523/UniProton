/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2022-2023. All rights reserved.
 */
#include "bm_types.h"
#ifndef __BM_HWI_H__
#define __BM_HWI_H__
/**
 * @defgroup bm_irq bm_irq
 */

#define BM_INT_PERF 0

typedef void(int_handler_t)(void *arg);
#if defined(BM_INT_PERF) && (BM_INT_PERF == 1)
typedef void(*int_perf_hook)(void);
#endif

typedef enum {
    HWI_CORE0_MAP = 1 << 0,
    HWI_CORE1_MAP = 1 << 1,
    HWI_CORE2_MAP = 1 << 2,
    HWI_CORE3_MAP = 1 << 3,
} bm_hwi_core_map;

/**
 *
 * @ingroup bm_irq
 * @brief Interrupt controller initialization.
 *
 * @par description:
 * Interrupt controller initialization.
 *
 * @param void
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_hwi_init(void);

/**
 *
 * @ingroup bm_irq
 * @brief Registering the Interrupt Callback Function.
 *
 * @par description:
 * Registering the Interrupt Callback Function.
 *
 * @param int_id [in]: Interrupt ID, range (0<=int_id<=288).
 * @param handler [in]: Interrupt callback function.
 * @param arg [in]: User Parameters.
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_request_irq(unsigned int int_id, int_handler_t *handler, void *arg);

/**
 *
 * @ingroup bm_irq
 * @brief free the Interrupt Callback Function.
 *
 * @par description:
 * free the Interrupt Callback Function.
 *
 * @param int_id [in]: Interrupt ID, range (0<=int_id<=288).
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_free_irq(unsigned int int_id);

/**
 *
 * @ingroup bm_irq
 * @brief Enable global CPU interrupts.
 *
 * @par description:
 * This function is used to clear the global CPU interrupt mask.
 *
 * @param void
 *
 * @return void
 *
 */
void bm_local_irq_unmask(void);

/**
 *
 * @ingroup bm_irq
 * @brief Disables global CPU interrupts.
 *
 * @par description:
 * Sets the global CPU interrupt mask bit.
 *
 * @param void
 *
 * @return void
 *
 */
void bm_local_irq_mask(void);

/**
 *
 * @ingroup bm_irq
 * @brief Enable Interrupt Controller Interrupt.
 *
 * @par description:
 * Enables the int_id interrupt of the interrupt controller.
 *
 * @param int_id [in]: Interrupt ID, range (0<=int_id<=288).
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_enable_irq(unsigned int int_id);

/**
 *
 * @ingroup bm_irq
 * @brief Disabling interrupt controller interrupts.
 *
 * @par description:
 * Disables the int_id interrupt of the interrupt controller.
 *
 * @param int_id [in]: Interrupt ID, range (0<=int_id<=288).
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_disable_irq(unsigned int int_id);

/**
 *
 * @ingroup bm_irq
 * @brief Sets the interrupt priority of the interrupt controller.
 *
 * @par description:
 * Sets the interrupt priority of the interrupt controller.
 *
 * @param int_id [in]: Interrupt ID, range (0<=int_id<=288).
 * @param priority [in]: Interrupt Priority, (0<=priority<=13), note: all interrupts default priority is the highest 0.
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_irq_set_priority(unsigned int int_id, unsigned int priority);

/**
 *
 * @ingroup bm_irq
 * @brief Setting Interrupt Routes for Shared Peripherals of the Interrupt Controller.
 *
 * @par description:
 * Setting Interrupt Routes for Shared Peripherals of the Interrupt Controller.[intid>31].
 *
 * @param int_id [in]: Shared Peripherals, range (32<=int_id<=288).
 * @param core_id [in]: Interrupt routing target core, range (0<=core_id<=3).
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_irq_set_affinity(unsigned int int_id, unsigned int core_id);

/**
 *
 * @ingroup bm_irq
 * @brief Triggers the SGI interrupt of the target core.
 *
 * @par description:
 * Triggers the SGI interrupt of the target core.
 *
 * @param int_id [in]: Interrupt ID, range (0<=int_id<=15)
 * @param core_map [in]: Bitmap of the target core to be triggered, for the low 4 bits, each bit represents a core.
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_hwi_trigger(unsigned int int_id, bm_hwi_core_map core_map);

#if defined(BM_INT_PERF) && (BM_INT_PERF == 1)
/**
 *
 * @ingroup bm_irq
 * @brief register perf hook.
 *
 * @par description:
 * register perf hook.
 *
 * @param int_id [in]: Interrupt ID, range (0<=int_id<=15)
 * @param hook [in]: perf hook.
 *
 * @return BM_OK/BM_FAIL
 *
 */
int bm_irq_perf_register(unsigned int int_id, int_perf_hook hook);
#endif

#endif /* __BM_HWI_H__ */