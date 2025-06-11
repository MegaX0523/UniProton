/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
#include "bm_config.h"
#include "bm_hwi.h"
#include "hwi_init.h"

enum gic_int_state {
    GIC_DISABLE = 0,
    GIC_ENABLE = 1,
    GIC_BUTT
};

enum sic_group_type {
    SIC_GROUP_G0S = 0,
    SIC_GROUP_G1NS = 1,
    SIC_GROUP_G1S = 2,
    SIC_GROUP_BUTT,
};

/* distributor control register */
union gicd_ctrl {
    struct {
        unsigned int en_g0s : 1; // bit[0] enable secure group0 interrupt
        unsigned int en_g1ns : 1; // bit[1] enable non-Secure group1 interrupt
        unsigned int en_g1s : 1; // bit[2] enable secure group1 interrupt
        unsigned int res1 : 1; // bit[3]
        unsigned int are_s : 1; // bit[4] affinity routing eanble（for secure）
        unsigned int are_ns : 1; // bit[5]
        unsigned int ds : 1; // bit[6] disable security
        unsigned int res0 : 24; // bit[30:7]
        unsigned int rwp : 1; // bit[31]
    } bits;
    unsigned int value;
};

union gicr_ctrl {
    struct {
        unsigned int en_lpis : 1; // bit[0]
        unsigned int res2 : 2; // bit[2:1] enable non-Secure group1 interrupt
        unsigned int rwp : 1; // bit[3] register write pending
        unsigned int res1 : 20; // bit[23:4]
        unsigned int dpg0 : 1; // bit[24]
        unsigned int dpg1ns : 1; // bit[25]
        unsigned int dpg1s : 1; // bit[26]
        unsigned int res0 : 4; // bit[30:27]
        unsigned int uwp : 1; // bit[31]
    } bits;
    unsigned int value;
};

union sicr_waker {
    struct {
        unsigned int res0 : 1;
        unsigned int sleep_req : 1;
        unsigned int is_sleep : 1;
        unsigned int res1 : 29;
    } bits;
    unsigned int value;
};

union hwi_icc_sre_elx {
    struct {
        unsigned long long prt : 1;
        unsigned long long dfb : 1;
        unsigned long long dib : 1;
        unsigned long long enable : 1;
        unsigned long long res : 60;
    } bits;
    unsigned long long value;
};

/* GICD_ROUTER */
union gicd_router {
    struct {
        unsigned long long af0 : 2; // bit[1:0]
        unsigned long long res1 : 6; // bit[7:2]
        unsigned long long af1 : 2; // bit[9:8]
        unsigned long long res0 : 21; // bit[30:10]
        unsigned long long mode : 1; // bit[31]
        unsigned long long res2 : 32; // bit[32~63]
    } bits;
    unsigned long long value;
};

union gic_core_map {
    struct {
        unsigned long long aff0 : 8; // bit[7:0]
        unsigned long long aff1 : 8; // bit[15:8]
        unsigned long long aff2 : 8; // bit[23:16]
        unsigned long long mt : 1; // bit[24]
        unsigned long long rsvd0 : 5; // bit[29:25]
        unsigned long long uni : 1; // bit[30]
        unsigned long long rsvd1 : 1; // bit[31]
        unsigned long long aff3 : 8; // bit[39:32]
        unsigned long long rsvd2 : 24; // bit[63~40]
    } bits;
    unsigned long long value;
};

/* ICC_SGIR_EL1 */
union icc_sgir_el1 {
    struct {
        unsigned long long targetlist : 16; // bit[0..15]
        unsigned long long aff1 : 8; // bit[16..23]
        unsigned long long int_id : 4; // bit[24..27] SGI.
        unsigned long long rsvd0 : 4; // bit[28..31]
        unsigned long long aff2 : 8; // bit[32..39]
        unsigned long long irm : 1; // bit[40]
        unsigned long long rsvd1 : 3; // bit[41..43]
        unsigned long long rs : 4; // bit[44..47]
        unsigned long long aff3 : 8; // bit[48..55]
        unsigned long long rsvd2 : 8; // bit[63..56]
    } bits;
    unsigned long long value;
};

static union gic_core_map g_gic_core_map = { 0 };

static void bm_gicd_wait_cfg_work(void)
{
    union gicd_ctrl gicd_ctrl;

    do {
        gicd_ctrl.value = gic_reg_read(GICD_CTLR_S_ADDR);
    } while (gicd_ctrl.bits.rwp == 1);
}

static unsigned int bm_gic_get_reg(uintptr_t base, unsigned int id)
{
    unsigned long long reg_addr;
    unsigned int reg_val;
    unsigned int offset;
    unsigned int bit_width;
    unsigned int bit_mask;

    bit_width = BM_WORD_BIT_NUM / GIC_IENABLE_INT_NUM;
    bit_mask = (1u << bit_width) - 1u;
    reg_addr = base + ((id / GIC_IENABLE_INT_NUM) * sizeof(unsigned int));
    offset = ((id % GIC_IENABLE_INT_NUM) * bit_width);

    reg_val = gic_reg_read(reg_addr);

    return (reg_val & (bit_mask << offset)) >> offset;
}

static void bm_gic_set_reg(uintptr_t base, unsigned int int_id, unsigned int val)
{
    uintptr_t reg_addr;
    unsigned int reg_val;
    unsigned int offset;
    unsigned int bit_width;
    unsigned int bit_mask;

    bit_width = BM_WORD_BIT_NUM / GIC_IENABLE_INT_NUM;
    bit_mask = (1u << bit_width) - 1u;
    reg_addr = base + ((int_id / GIC_IENABLE_INT_NUM) * sizeof(unsigned int));
    offset = ((int_id % GIC_IENABLE_INT_NUM) * bit_width);

    reg_val = ((val & bit_mask) << offset);
    gic_reg_write(reg_addr, reg_val);
}

static enum gic_int_state bm_gicr_get_int_state(unsigned int core_id, unsigned int int_id)
{
    return bm_gic_get_reg(GICR_ISENABLER0_ADDR + core_id * GICR_ADDR_OFFSET_PER_CORE, int_id);
}

static enum gic_int_state bm_gicd_get_int_state(unsigned int int_id)
{
    return bm_gic_get_reg(GICD_ISENABLER0_ADDR, int_id);
}

static void bm_gicr_wait_cfg_work(unsigned int core_id)
{
    union gicr_ctrl gicr_ctrl;
    uintptr_t reg_addr;

    reg_addr = GICR_CTRL_ADDR + (core_id * GICR_ADDR_OFFSET_PER_CORE);
    do {
        gicr_ctrl.value = gic_reg_read(reg_addr);
    } while (gicr_ctrl.bits.rwp == 1);
}

static void bm_gicr_disable_int(unsigned int core_id, unsigned int int_id)
{
    bm_gic_set_reg(GICR_ICENABLER0_ADDR + core_id * GICR_ADDR_OFFSET_PER_CORE, int_id, 1);

    bm_gicr_wait_cfg_work(core_id);
}

static void bm_gicr_enable_int(unsigned int core_id, unsigned int int_id)
{
    bm_gic_set_reg(GICR_ISENABLER0_ADDR + core_id * GICR_ADDR_OFFSET_PER_CORE, int_id, 1);

    bm_gicr_wait_cfg_work(core_id);
}

static void bm_gicd_disable_int(unsigned int int_id)
{
    bm_gic_set_reg(GICD_ICENABLER0_ADDR, int_id, 1);

    bm_gicd_wait_cfg_work();
}

static void bm_gicd_enable_int(unsigned int int_id)
{
    bm_gic_set_reg(GICD_ISENABLER0_ADDR, int_id, 1);

    bm_gicd_wait_cfg_work();
}

static void bm_sicr_init(void)
{
    union sicr_waker sicr_waker;
    uintptr_t reg_addr;

    reg_addr = GICR_WAKER_ADDR + bm_get_coreid() * SICR_ADDR_OFFSET_PER_CORE;
    sicr_waker.value = gic_reg_read(reg_addr);
    sicr_waker.bits.sleep_req = 0;
    gic_reg_write(reg_addr, sicr_waker.value);
    sicr_waker.value = gic_reg_read(reg_addr);
    while (sicr_waker.bits.is_sleep == 1) {
        sicr_waker.value = gic_reg_read(reg_addr);
    }
}

static void bm_sicr_set_int_group(unsigned int core_id, unsigned long long int_id, enum sic_group_type group_id)
{
    uintptr_t group0_reg_addr;
    uintptr_t mod_reg_addr;
    unsigned int group0_reg_tmp;
    unsigned int mod_reg_tmp;
    (void)core_id;
    group0_reg_addr = GICR_IGROUPR0_ADDR + bm_get_coreid() * SICR_ADDR_OFFSET_PER_CORE;
    group0_reg_tmp = gic_reg_read(group0_reg_addr);

    if ((group_id == SIC_GROUP_G0S) || (group_id == SIC_GROUP_G1S)) {
        group0_reg_tmp &= ~(0x1U << int_id);
    } else {
        group0_reg_tmp |= (0x1U << int_id);
    }
    gic_reg_write(group0_reg_addr, group0_reg_tmp);

    mod_reg_addr = GICR_IGRPMODR0_ADDR + bm_get_coreid() * SICR_ADDR_OFFSET_PER_CORE;
    mod_reg_tmp = gic_reg_read(mod_reg_addr);

    if (group_id == SIC_GROUP_G1S) {
        mod_reg_tmp |= (0x1U << int_id);
    } else {
        mod_reg_tmp &= ~(0x1U << int_id);
    }
    gic_reg_write(mod_reg_addr, mod_reg_tmp);
}

static int bm_sicc_enable_sre(void)
{
    volatile union hwi_icc_sre_elx icc_sre;

    BM_EMBED_ASM("MRS %0, " reg_alias(ICC_SRE_EL1) " \n" : "=&r"(icc_sre));
    icc_sre.bits.prt = 1;
    icc_sre.bits.dfb = 1;
    icc_sre.bits.dib = 1;
    BM_EMBED_ASM("MSR " reg_alias(ICC_SRE_EL1) ", %0 \n" : : "r"(icc_sre));
    BM_EMBED_ASM("DSB SY");
    BM_EMBED_ASM("ISB");

    BM_EMBED_ASM("MRS %0, " reg_alias(ICC_SRE_EL1) " \n" : "=&r"(icc_sre.value));

    if (icc_sre.bits.prt != 1) {
        return BM_FAIL;
    }

    return BM_OK;
}

static void bm_sicc_cfg_int_preempt(void)
{
    unsigned long long tmp = 0;

    BM_EMBED_ASM("MRS %0, " reg_alias(ICC_BPR1_EL1) " \n" : "=&r"(tmp) : : "memory");
    tmp &= ~(GROUP_MAX_BPR);
    tmp |= GROUP1_BP;
    BM_EMBED_ASM("MSR " reg_alias(ICC_BPR1_EL1) ", %0 \n" : : "r"(tmp) : "memory");
}

static void bm_sicc_enable_group1(void)
{
    unsigned long long tmp = 0;

    BM_EMBED_ASM("MRS %0, " reg_alias(ICC_IGRPEN1_EL1) " \n" : "=&r"(tmp) : : "memory");
    tmp |= 0x01U;
    BM_EMBED_ASM("MSR " reg_alias(ICC_IGRPEN1_EL1) ", %0 \n" : : "r"(tmp) : "memory");
}

static void bm_sicc_cfg_priority_mask(void)
{
    unsigned long long tmp = 0;

    BM_EMBED_ASM("MRS %0, " reg_alias(ICC_PMR_EL1) " \n" : "=&r"(tmp) : : "memory");
    tmp |= PRIO_MASK_LEVEL;
    BM_EMBED_ASM("MSR " reg_alias(ICC_PMR_EL1) ", %0 \n" : : "r"(tmp) : "memory");
}

static int bm_sicc_init(void)
{
    int ret;

    ret = bm_sicc_enable_sre();
    if (ret != BM_OK) {
        return ret;
    }

    bm_sicc_cfg_int_preempt();

    bm_sicc_enable_group1();

    bm_sicc_cfg_priority_mask();

    return BM_OK;
}

static void bm_sicd_set_int_group(unsigned int int_id, enum sic_group_type group_id)
{
    unsigned long long group0_reg_addr;
    unsigned long long mod_reg_addr;
    unsigned int group0_reg_tmp;
    unsigned int mod_reg_tmp;
    unsigned int sicd_m;
    unsigned int bit_offset;

    sicd_m = (int_id - MIN_GIC_SPI_NUM) / SICD_IGROUP_INT_NUM;
    group0_reg_addr = GICD_IGROUPN_ADDR + (SICD_REG_SIZE * sicd_m);
    mod_reg_addr = GICD_IGRPMODRN_ADDR + (SICD_REG_SIZE * sicd_m);
    bit_offset = (int_id - MIN_GIC_SPI_NUM) % SICD_IGROUP_INT_NUM;

    group0_reg_tmp = gic_reg_read(group0_reg_addr);
    if ((group_id == SIC_GROUP_G0S) || (group_id == SIC_GROUP_G1S)) {
        group0_reg_tmp &= ~(0x1U << bit_offset);
    } else {
        group0_reg_tmp |= (0x1U << bit_offset);
    }
    gic_reg_write(group0_reg_addr, group0_reg_tmp);

    mod_reg_tmp = gic_reg_read(mod_reg_addr);
    if (group_id == SIC_GROUP_G1S) {
        mod_reg_tmp |= (0x1U << bit_offset);
    } else {
        mod_reg_tmp &= ~(0x1U << bit_offset);
    }
    gic_reg_write(mod_reg_addr, mod_reg_tmp);
}

static void bm_sic_set_group(unsigned int int_id, enum sic_group_type group_id)
{
    unsigned int core_id = bm_get_coreid();
    enum gic_int_state state;

    if (int_id < MIN_GIC_SPI_NUM) {
        state = bm_gicr_get_int_state(core_id, int_id);
        bm_gicr_disable_int(core_id, int_id);
        bm_sicr_set_int_group(core_id, int_id, group_id);
        if (state == GIC_ENABLE) {
            bm_gicr_enable_int(core_id, int_id);
        }
    } else {
        state = bm_gicd_get_int_state(int_id);
        bm_gicd_disable_int(int_id);
        bm_sicd_set_int_group(int_id, group_id);
        if (state == GIC_ENABLE) {
            bm_gicd_enable_int(int_id);
        }
    }
}

static void bm_gic_rmw_reg(uintptr_t base, unsigned int int_num, unsigned int int_id, unsigned int val)
{
    uintptr_t reg_addr;
    unsigned int reg_val;
    unsigned int offset;
    unsigned int bit_width;
    unsigned int bit_mask;

    bit_width = BM_WORD_BIT_NUM / int_num;
    bit_mask = (1u << bit_width) - 1u;
    reg_addr = base + ((int_id / int_num) * sizeof(unsigned int));
    offset = ((int_id % int_num) * bit_width);

    reg_val = gic_reg_read(reg_addr);
    reg_val &= ~(bit_mask << offset);
    reg_val |= ((val & bit_mask) << offset);
    gic_reg_write(reg_addr, reg_val);
}

static void bm_gicr_set_priority(unsigned int core_id, unsigned int int_id, unsigned int priority)
{
    bm_gic_rmw_reg(GICR_IPRIORITY_SGI_S_ADDR + core_id * GICR_ADDR_OFFSET_PER_CORE, GIC_IPRIORITY_INT_NUM, int_id,
        priority << GIC_IPRIORITY_HIGH_BIT);
}

static void bm_gicd_set_priority(unsigned int int_id, unsigned int priority)
{
    bm_gic_rmw_reg(GICD_IPRIORITYRN_ADDR, GIC_IPRIORITY_INT_NUM, int_id, priority << GIC_IPRIORITY_HIGH_BIT);
}

static int bm_sic_init_local(void)
{
    int ret;
    unsigned int int_id;

    bm_sicr_init();

    ret = bm_sicc_init();
    if (ret != BM_OK) {
        return ret;
    }

    for (int_id = 0; int_id < MIN_GIC_SPI_NUM; ++int_id) {
        bm_sic_set_group(int_id, SIC_GROUP_G1NS);
        bm_irq_set_priority(int_id, 0);
    }

    return BM_OK;
}

static void bm_sicd_init(void)
{
    union gicd_ctrl gicd_ctrl;

    gicd_ctrl.value = gic_reg_read(GICD_CTLR_S_ADDR);

    if ((gicd_ctrl.bits.en_g0s == 1) || (gicd_ctrl.bits.en_g1ns == 1) || (gicd_ctrl.bits.en_g1s == 1)) {
        return;
    }

    gicd_ctrl.bits.ds = 0;
    gicd_ctrl.bits.are_ns = 1;
    gicd_ctrl.bits.are_s = 1;
    gicd_ctrl.bits.en_g1ns = 1;
    gic_reg_write(GICD_CTLR_S_ADDR, gicd_ctrl.value);

    bm_gicd_wait_cfg_work();
}

static void bm_sic_init_global(void)
{
    unsigned int int_id;

    bm_sicd_init();

    for (int_id = MIN_GIC_SPI_NUM; int_id < MAX_INT_NUM; ++int_id) {
        bm_sic_set_group(int_id, SIC_GROUP_G1NS);
        bm_irq_set_priority(int_id, 0);
    }
}

static uintptr_t bm_gic_get_router_addr(unsigned int int_id)
{
    return (GICD_SPI_IROUTERN_L_ADDR + GIC_PRIORITY_PER_REG * int_id);
}

int bm_enable_irq(unsigned int int_id)
{
    if (int_id > MAX_INT_NUM) {
        return BM_FAIL;
    }

    if (int_id < MIN_GIC_SPI_NUM) {
        unsigned int core_id = bm_get_coreid();
        bm_gicr_enable_int(core_id, int_id);
    } else {
        bm_gicd_enable_int(int_id);
    }
    return BM_OK;
}

int bm_disable_irq(unsigned int int_id)
{
    if (int_id > MAX_INT_NUM) {
        return BM_FAIL;
    }

    if (int_id < MIN_GIC_SPI_NUM) {
        unsigned int core_id = bm_get_coreid();
        bm_gicr_disable_int(core_id, int_id);
    } else {
        bm_gicd_disable_int(int_id);
    }
    return BM_OK;
}

int bm_irq_set_priority(unsigned int int_id, unsigned int priority)
{
    if (int_id > MAX_INT_NUM || priority > MAX_INT_PRIORITY) {
        return BM_FAIL;
    }

    unsigned int core_id;
    enum gic_int_state state;

    if (int_id <= MAX_NNSPI_ID) {
        for (core_id = 0; core_id < BM_MAX_CORE_NUM; core_id++) {
            state = bm_gicr_get_int_state(core_id, int_id);
            bm_gicr_disable_int(core_id, int_id);
            bm_gicr_set_priority(core_id, int_id, priority);
            if (state == GIC_ENABLE) {
                bm_gicr_enable_int(core_id, int_id);
            }
        }
    } else {
        state = bm_gicd_get_int_state(int_id);
        bm_gicd_disable_int(int_id);
        bm_gicd_set_priority(int_id, priority);
        if (state == GIC_ENABLE) {
            bm_gicd_enable_int(int_id);
        }
    }
    return BM_OK;
}

int bm_irq_set_affinity(unsigned int int_id, unsigned int core_id)
{
    if (int_id < MIN_SPI_ID || int_id > MAX_INT_NUM) {
        return BM_FAIL;
    }

    if (core_id >= BM_MAX_CORE_NUM) {
        return BM_FAIL;
    }

    union gicd_router gicd_router;
    uintptr_t addr;

    addr = bm_gic_get_router_addr(int_id);
    gicd_router.value = gic_reg_read(addr);

    gicd_router.bits.mode = 0x0;
    gicd_router.bits.af1 = (core_id & 0x3); // core_id <= 3
    gicd_router.bits.af0 = 0;

    gic_reg_write(addr, gicd_router.value);
    return BM_OK;
}

int bm_hwi_trigger(unsigned int int_id, bm_hwi_core_map core_map)
{
    if (int_id >= MIN_PPI_ID) {
        return BM_FAIL;
    }

    if (core_map > BM_CORE_MAP || core_map < 0) {
        return BM_FAIL;
    }

    /* 判断中断是否使能 */
    unsigned int core_id = bm_get_coreid();
    enum gic_int_state state;

    state = (int_id < MIN_GIC_SPI_NUM) ? bm_gicr_get_int_state(core_id, int_id) : bm_gicd_get_int_state(int_id);
    if (state != GIC_ENABLE) {
        return BM_FAIL;
    }

    /* 中断使能后才能发送软中断 */
    union icc_sgir_el1 icc_sgir_el1;
    unsigned int core;
    unsigned int target_mask = 0x1;

    dsb();
    for (core = 0; (core < BM_MAX_CORE_NUM) && (core_map != 0); ++core) {
        if ((core_map & (1U << core)) != 0) {
            icc_sgir_el1.value = 0;
            icc_sgir_el1.bits.int_id = int_id;
            icc_sgir_el1.bits.targetlist = target_mask;
            icc_sgir_el1.bits.aff1 = core;
            icc_sgir_el1.bits.aff2 = g_gic_core_map.bits.aff2;
            icc_sgir_el1.bits.aff3 = g_gic_core_map.bits.aff3;
            BM_EMBED_ASM("MSR " reg_alias(ICC_SGI1R_EL1) ", %0 \n" : : "r"(icc_sgir_el1.value) : "memory");
        }
    }
    isb();
    return BM_OK;
}

int bm_request_irq(unsigned int int_id, int_handler_t *handler, void *arg)
{
    return irq_install_handler(int_id, handler, arg);
}

int bm_free_irq(unsigned int int_id)
{
    return irq_free_handler(int_id);
}

void bm_local_irq_unmask(void)
{
    asm volatile("msr	daifclr, #2		// bm_local_irq_unmask" : : : "memory");
}

void bm_local_irq_mask(void)
{
    asm volatile("msr	daifset, #2		// bm_local_irq_mask" : : : "memory");
}

#ifdef MCS_2WIHT2_HI309X
#define HOST_HWI_INIT_CORE 2
#else
#define HOST_HWI_INIT_CORE 0
#endif

int bm_hwi_init(void)
{
    asm("msr vbar_el1, %0" : : "r"(vector_entry) : "memory");
    unsigned int ret;
    ret = bm_sic_init_local();
    if (ret != BM_OK) {
        return ret;
    }

    if (bm_get_coreid() == HOST_HWI_INIT_CORE) {
        bm_sic_init_global();
    }

    return BM_OK;
}

#if defined(BM_INT_PERF) && (BM_INT_PERF == 1)
int bm_irq_perf_register(unsigned int int_id, int_perf_hook hook)
{
    return irq_perf_register(int_id, hook);
}
#endif
