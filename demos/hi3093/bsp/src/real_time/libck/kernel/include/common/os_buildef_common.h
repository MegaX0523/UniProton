/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: the common part buildef.h
 * Author: LibCK项目组
 * Create: 2009-12-22
 * Notes: OS内部头文件，由sre_xxx.h文件包含，用户不能包含。
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef OS_BUILDEF_COMMON_H
#define OS_BUILDEF_COMMON_H

/* the endian definition */
#define OS_LITTLE_ENDIAN 0x1234
#define OS_BIG_ENDIAN 0x4321

/* To define OS_HARDWARE_PLATFORM */
/* 编译器有个bug, 未定义的宏的数值默认是0，所以不用使用'0' */
#define OS_CORTEX_RX 0x01
#define OS_HIDSPV200 0x02
#define OS_HIAVPV200 0x03
#define OS_HIAVPV280 0x04
#define OS_X86_64 0x05
#define OS_RISC220 0x06
#define OS_DSP220 0x07
#define OS_HACCC 0x08
#define OS_RISC170 0x09
#define OS_DSP170 0x0a
#define OS_CORTEX_AX 0x0b
#define OS_CORTEX_M4 0x0c
#define OS_HI1210 0x0d
#define OS_HIDSP 0x0e
#define OS_DSP100 0x0f
#define OS_CORTEX_M3 0x10
#define OS_ARM7 0x11
#define OS_ARM9 0x12
#define OS_HITSP_X 0x13
#define OS_HITVP_X 0x14
#define OS_ARMV8 0x15
#define OS_HIDSPV300LITE 0x16
#define OS_HIAVPV300LITE 0x17
#define OS_CORTEX_M7 0x18
#define OS_RISCV 0x19
#define OS_HI3093 0x1a
#define OS_PLATFORM_INVALID 0x1b

/* To define OS_CPU_TYPE */
/* 编译器有个bug, 未定义的宏的数值默认是0，所以不用使用'0' */
#define OS_CORTEX_R8 0x01
#define OS_SD6185 0x02
#define OS_INTEL_X86_64 0x03
#define OS_SD6801 0x04
#define OS_SD6801E 0x05
#define OS_SD6186 0x06
#define OS_SD6186L 0x07
#define OS_SD6182 0x08
#define OS_SD6157 0x09
#define OS_SD6108 0x0a
#define OS_SD6181 0x0b
#define OS_CORTEX_A9 0x0c
#define OS_STM32F407 0x0d
#define OS_CORTEX_A7 0x0e
#define OS_SD6183 0x0f
#define OS_SD6155 0x10
#define OS_STM32F307 0x11
#define OS_SD5000 0x12
#define OS_SD5010 0x13
#define OS_HI6881 0x14
#define OS_HI1171 0x15
#define OS_HI3911V200 0x16
#define OS_HITSP_V100 0x17
#define OS_HITSP_V101 0x18
#define OS_HITSP_V150 0x19
#define OS_HITSP_V200 0x1a
#define OS_8191 0x1b
#define OS_HI1880 0x1c
#define OS_8171 0x1d
#define OS_HI1823 0x1e
#define OS_8021 0x1f
#define OS_8011 0x20
#define OS_SAMV71 0x21
#define OS_HIMIDEER 0x22
#define OS_CPU_TYPE_INVALID 0x23

/* 必须方案sre_buildef的头文件中，否则会出现宏开关不生效的问题 */
#ifdef YES
#undef YES
#endif
#define YES 1

#ifdef NO
#undef NO
#endif
#define NO 0

#if (OS_CPU_TYPE == OS_SD6185) || (OS_CPU_TYPE == OS_SD6186) || (OS_CPU_TYPE == OS_SD6186L) || \
    (OS_CPU_TYPE == OS_SD6183) || (OS_CPU_TYPE == OS_HI6881) || (OS_CPU_TYPE == OS_HI1171) ||  \
    (OS_CPU_TYPE == OS_8171) || (OS_CPU_TYPE == OS_8021) || (OS_CPU_TYPE == OS_8011)

#ifndef RESET_SEC_DATA
#define RESET_SEC_DATA __attribute__((section(".ResetVector.cwna.data")))
#endif

#ifndef INIT_SEC_L4_TEXT
#define INIT_SEC_L4_TEXT __attribute__((section(".os.init.DDR64.cwna.text")))
#endif

#ifndef SYMBOL_SEC_L4_SH_DATA
#define SYMBOL_SEC_L4_SH_DATA __attribute__((section(".os.symbol.DDR64.ncnb.share.data")))
#endif

#ifndef INIT_SEC_L4_SH_DATA
#define INIT_SEC_L4_SH_DATA __attribute__((section(".os.symbol.DDR64.ncnb.share.data")))
#endif

#elif (OS_CPU_TYPE == OS_SD6182) || (OS_CPU_TYPE == OS_SD6157) || (OS_CPU_TYPE == OS_SD6155)

#ifndef INIT_SEC_L4_TEXT
#define INIT_SEC_L4_TEXT __attribute__((section(".os.init.text")))
#endif

#if (OS_CPU_TYPE == OS_SD6182) || (OS_CPU_TYPE == OS_SD6157)
#ifndef INIT_SEC_L4_SH_DATA
#define INIT_SEC_L4_SH_DATA __attribute__((section(".os.symbol.data")))
#endif
#else
#ifndef INIT_SEC_L4_SH_DATA
#define INIT_SEC_L4_SH_DATA __attribute__((section(".os.shared.data")))
#endif
#endif

#ifndef SYMBOL_SEC_L4_SH_DATA
#define SYMBOL_SEC_L4_SH_DATA __attribute__((section(".os.symbol.data")))
#endif

#endif

#if ((OS_HARDWARE_PLATFORM == OS_CORTEX_RX) || (OS_HARDWARE_PLATFORM == OS_HITSP_X))

#ifndef RESET_SEC_DATA
#define RESET_SEC_DATA __attribute__((section(".os.init.data")))
#endif

#ifndef INIT_SEC_L4_TEXT
#define INIT_SEC_L4_TEXT __attribute__((section(".os.recyclable.text")))
#endif

#ifndef INIT_SEC_L4_DATA
#define INIT_SEC_L4_DATA __attribute__((section(".os.init.data")))
#endif

#ifndef INIT_SEC_L4_SH_DATA
#define INIT_SEC_L4_SH_DATA __attribute__((section(".os.init.data")))
#endif

#ifndef SYMBOL_SEC_L4_SH_DATA
#define SYMBOL_SEC_L4_SH_DATA __attribute__((section(".os.symbol.data")))
#endif

#endif

#ifndef RESET_SEC_DATA
#define RESET_SEC_DATA __attribute__((section(".os.init.data")))
#endif

#ifndef INIT_SEC_L4_TEXT
#define INIT_SEC_L4_TEXT
#endif

#ifndef INIT_SEC_L4_DATA
#define INIT_SEC_L4_DATA
#endif

#ifndef INIT_SEC_L4_SH_DATA
#define INIT_SEC_L4_SH_DATA
#endif

#ifndef SYMBOL_SEC_L4_SH_DATA
#define SYMBOL_SEC_L4_SH_DATA
#endif

#endif /* OS_BUILDEF_COMMON_H */
