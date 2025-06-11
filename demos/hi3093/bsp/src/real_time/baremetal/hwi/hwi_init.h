/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
#include <bm_common.h>
#include "bm_types.h"
#include "bm_hwi.h"
#ifndef __HWI_INIT_H__
#define __HWI_INIT_H__

#define SICD_IGROUP_INT_NUM 32
#define BM_MAX_CORE_NUM 4
#define BM_CORE_MAP 0xf

#define MAX_NNSPI_ID 31
#define MIN_SPI_ID 32

#define MAX_SGI_ID 15
#define MIN_PPI_ID 16
#define MAX_PPI_ID 31
#define MAX_SPI_ID 1019
#define MAX_INT_PRIORITY 13
#define GIC_INT_ID_MASK 0x3FFU

#define GIC_IPRIORITY_INT_NUM 4

#define GIC_IPRIORITY_HIGH_BIT 4

#define GIC_REG_BASE_ADDR 0x24000000

/* the following are defines for the SICR register addr */
#define GICR_BASE0 (GIC_REG_BASE_ADDR + 0x40000)
#define GICR_BASE1 (GIC_REG_BASE_ADDR + 0x50000)
#define GICR_CTRL_ADDR (GICR_BASE0 + 0x0000)
#define GICR_TYPER_ADDR (GICR_BASE0 + 0x0008)
#define GICR_WAKER_ADDR (GICR_BASE0 + 0x0014)
#define GICR_PWRR_ADDR (GICR_BASE0 + 0x0024)

#define GICR_IGROUPR0_ADDR (GICR_BASE1 + 0x0080)
#define GICR_ISENABLER0_ADDR (GICR_BASE1 + 0x0100)
#define GICR_ICENABLER0_ADDR (GICR_BASE1 + 0x0180)
#define GICR_IGRPMODR0_ADDR (GICR_BASE1 + 0x0D00)
#define GICR_IPRORITYN_ADDR (GICR_BASE1 + 0x0400)

#define GICR_IPRIORITY_SGI_S_ADDR (GICR_BASE1 + 0x0400)

#define SICR_ADDR_OFFSET_PER_CORE 0x20000U

#define GICR_ADDR_OFFSET_PER_CORE (0x20000U)

#define GICD_CTLR_S_ADDR (GIC_REG_BASE_ADDR + 0x0000)
#define GICD_IGROUPN_ADDR (GIC_REG_BASE_ADDR + 0x0080)
#define GICD_IGRPMODRN_ADDR (GIC_REG_BASE_ADDR + 0x0D00)
#define GICD_ISENABLER0_ADDR (GIC_REG_BASE_ADDR + 0x0100)
#define GICD_ICENABLER0_ADDR (GIC_REG_BASE_ADDR + 0x0180)
#define GICD_IPRIORITYRN_ADDR (GIC_REG_BASE_ADDR + 0x0400)
#define GICD_TYPER_ADDR (GIC_REG_BASE_ADDR + 0x0004)

#define GICD_ICPENDRN_ADDR (GIC_REG_BASE_ADDR + 0x0280)

#define GICD_SPENDSGIRN (GIC_REG_BASE_ADDR + 0x0f20)
#define GICD_ISPENDRN (GIC_REG_BASE_ADDR + 0x0200)
#define GICD_SPI_IROUTERN_L_ADDR (GIC_REG_BASE_ADDR + 0x6000U)

#define GICD_TYPE_LINES 0x01F
#define GIC_PRI_IRQ 0x0a
#define GIC_PRIORITY_PER_REG 8

#define MIN_GIC_SPI_NUM 32

#define SICD_IGORUP_INT_NUM 32U
#define SICD_REG_SIZE 4
#define SICD_IPRORITY 32U

#define GROUP_MAX_BPR 0x7
#define GROUP0_BP 0x0  // no preemption
#define GROUP1_BP 0x0  // no preemption

#define PRIO_MASK_LEVEL 0xF0U

#define ICC_SRE_EL1 S3_0_C12_C12_5
#define ICC_SRE_EL3 S3_6_C12_C12_5
#define ICC_BPR1_EL1 S3_0_C12_C12_3
#define ICC_BPR0_EL1 S3_0_C12_C8_3
#define ICC_IGRPEN1_EL1 S3_0_C12_C12_7
#define ICC_IGRPEN0_EL1 S3_0_C12_C12_6
#define ICC_PMR_EL1 S3_0_C4_C6_0
#define ICC_EOIR1_EL1 S3_0_C12_C12_1
#define ICC_DIR_EL1 S3_0_C12_C11_1
#define SCR_EL3 S3_6_C1_C1_0
#define ICC_RPR_EL1 S3_0_C12_C11_3

#define ICC_HPPIR1_EL1 S3_0_C12_C12_2
#define ICC_IAR1_EL1 S3_0_C12_C12_0
#define ICC_EOIR1_EL1 S3_0_C12_C12_1
#define ICC_SGI1R_EL1 S3_0_C12_C11_5

#define paras_to_string(x...) #x

#define reg_alias(x...) paras_to_string(x)

#define BM_WORD_BIT_NUM (sizeof(unsigned int) * 8)
#define GIC_IENABLE_INT_NUM 32

#define BM_EMBED_ASM __asm__ __volatile__

#define gic_reg_read(addr) (*(volatile unsigned int *)((uintptr_t)(addr)))
#define gic_reg_write(addr, data) (*(volatile unsigned int *)((uintptr_t)(addr)) = (unsigned int)(data))

typedef void(interrupt_handler_t)(void *arg);

extern void vector_entry(void);
int irq_install_handler(unsigned int irq, interrupt_handler_t *handler, void *arg);
int irq_free_handler(unsigned int irq);
#if defined(BM_INT_PERF) && (BM_INT_PERF == 1)
int irq_perf_register(unsigned int int_id, int_perf_hook hook);
#endif
#endif /* __HWI_INIT_H__ */