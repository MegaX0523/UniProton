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
#ifndef __WDG_H__
#define __WDG_H__

#define WDG0_REG_BASE 0x08763000
#define WDG1_REG_BASE 0x08764000
#define WDG2_REG_BASE 0x08765000
#define WDG3_REG_BASE 0x08766000
#define WDG4_REG_BASE 0x08767000

#define WDG_UNLOCK_VALUE (0x1acce551)
#define WDG_LOCK_VALUE (0x5a5a5a5a)

#define WDG_RESET_VALUE 0x1
#define WDG_RESET_MODE 0x3    /* reset the entire chip */
#define WDG_CTRL_PER_BITS 0x4 /* Every 4 bits represent a control domain. */
/**
 * @brief wdg control register structure
 */
typedef union {
    unsigned int reg;
    struct {
        unsigned int inten : 1;
        unsigned int resen : 1;
        unsigned int reserve : 30;
    } bit;
} wdg_control_reg;

/**
 * @brief wdg original interrupt register
 */
typedef struct {
    unsigned int wdgris : 1;
    unsigned int reserve : 31;
} wdg_ris_reg;

/**
 * @brief wdg interrupt register of shield
 */
typedef struct {
    unsigned int wdgmis : 1;
    unsigned int reserve : 31;
} wdg_mis_reg;

/**
 * @brief wdg register structure
 */
typedef struct {
    unsigned int wdg_load;
    unsigned int wdg_value;
    wdg_control_reg wdg_control;
    unsigned int wdg_intclr;
    wdg_ris_reg wdg_ris;
    wdg_mis_reg wdg_mis;
    unsigned int reserved0[762];
    unsigned int wdg_lock;
} volatile wdg_reg_struct;

#endif /* __WDG_H__ */