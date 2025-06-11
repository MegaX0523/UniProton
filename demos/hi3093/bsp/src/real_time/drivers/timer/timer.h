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
#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER0_REG_BASE 0x08700000
#define TIMER1_REG_BASE 0x08700040
#define TIMER2_REG_BASE 0x08702000
#define TIMER3_REG_BASE 0x08702040
#define TIMER4_REG_BASE 0x08704000
#define TIMER5_REG_BASE 0x08704040
/**
 * @brief timer control register structure
 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int oneshot : 1;
        unsigned int timersize : 1;
        unsigned int timerpre : 2;
        unsigned int reserve1 : 1;
        unsigned int intenable : 1;
        unsigned int timermode : 1;
        unsigned int timeren : 1;
        unsigned int reserve2 : 24;
    } bit;
} timer_control_reg;

/**
 * @brief timer original interrupt register
 */
typedef struct {
    unsigned int timerris : 1;
    unsigned int reserve : 31;
} timer_ris_reg;

/**
 * @brief timer interrupt register of shield
 */
typedef struct {
    unsigned int timermis : 1;
    unsigned int reserve : 31;
} timer_mis_reg;

/**
 * @brief timer register structure
 */
typedef struct {
    unsigned int timer_load_l;
    unsigned int timer_load_h;
    unsigned int timer_value_l;
    unsigned int timer_value_h;
    timer_control_reg timer_control;
    unsigned int timer_intclr;
    timer_ris_reg timer_ris;
    timer_mis_reg timer_mis;
    unsigned int timer_bgload_l;
    unsigned int timer_bgload_h;
} volatile timer_reg_struct;

#endif /* __TIMER_H__ */