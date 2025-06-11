/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */

.global armv8_switch_to_el1

/*
 * CPTR_EL2 bits definitions
 */
#define CPTR_EL2_RES1		(3 << 12 | 0x3ff)           

#define SPSR_EL_END_LE		(0 << 9)  
#define SPSR_EL_DEBUG_MASK	(1 << 9)  
#define SPSR_EL_ASYN_MASK	(1 << 8)  
#define SPSR_EL_SERR_MASK	(1 << 8)  
#define SPSR_EL_IRQ_MASK	(1 << 7)  
#define SPSR_EL_FIQ_MASK	(1 << 6)  
#define SPSR_EL_T_A32		(0 << 5)  
#define SPSR_EL_M_AARCH64	(0 << 4)  
#define SPSR_EL_M_AARCH32	(1 << 4)  
#define SPSR_EL_M_SVC		(0x3)     
#define SPSR_EL_M_HYP		(0xa)     
#define SPSR_EL_M_EL1H		(5)       
#define SPSR_EL_M_EL2H		(9)       

/*
 * HCR_EL2 bits definitions
 */
#define HCR_EL2_API		(1 << 41) 
#define HCR_EL2_APK		(1 << 40) 
#define HCR_EL2_RW_AARCH64	(1 << 31) 
#define HCR_EL2_RW_AARCH32	(0 << 31) 
#define HCR_EL2_HCD_DIS		(1 << 29) 
#define HCR_EL2_AMO_EL2		(1 <<  5) 

#define ES_TO_AARCH64		1
#define ES_TO_AARCH32		0

/*
 * CNTHCTL_EL2 bits definitions
 */
#define CNTHCTL_EL2_EL1PCEN_EN	(1 << 1)  
#define CNTHCTL_EL2_EL1PCTEN_EN	(1 << 0)  

/*
 * ID_AA64ISAR1_EL1 bits definitions
 */
#define ID_AA64ISAR1_EL1_GPI	(0xF << 28) 
#define ID_AA64ISAR1_EL1_GPA	(0xF << 24) 
#define ID_AA64ISAR1_EL1_API	(0xF << 8)  
#define ID_AA64ISAR1_EL1_APA	(0xF << 4)  

/*
 * CPACR_EL1 bits definitions
 */
#define CPACR_EL1_FPEN_EN	(3 << 20) 

/*
 * SCTLR_EL1 bits definitions
 */
#define SCTLR_EL1_RES1		(3 << 28 | 3 << 22 | 1 << 20 | 1 << 11) 
#define SCTLR_EL1_UCI_DIS	(0 << 26) 
#define SCTLR_EL1_EE_LE		(0 << 25) 
#define SCTLR_EL1_WXN_DIS	(0 << 19) 
#define SCTLR_EL1_NTWE_DIS	(0 << 18) 
#define SCTLR_EL1_NTWI_DIS	(0 << 16) 
#define SCTLR_EL1_UCT_DIS	(0 << 15) 
#define SCTLR_EL1_DZE_DIS	(0 << 14) 
#define SCTLR_EL1_ICACHE_DIS	(0 << 12) 
#define SCTLR_EL1_UMA_DIS	(0 << 9)  
#define SCTLR_EL1_SED_EN	(0 << 8)  
#define SCTLR_EL1_ITD_EN	(0 << 7)  
#define SCTLR_EL1_CP15BEN_DIS	(0 << 5)  
#define SCTLR_EL1_SA0_DIS	(0 << 4)  
#define SCTLR_EL1_SA_DIS	(0 << 3)  
#define SCTLR_EL1_DCACHE_DIS	(0 << 2)  
#define SCTLR_EL1_ALIGN_DIS	(0 << 1)  
#define SCTLR_EL1_MMU_DIS	(0)       

/*
 * Branch according to exception level
 */
.macro	switch_el, xreg, el3_label, el2_label, el1_label
	mrs	\xreg, CurrentEL
	cmp	\xreg, #0x8
	b.gt	\el3_label
	b.eq	\el2_label
	b.lt	\el1_label
.endm

.macro armv8_switch_to_el1_m, ep, flag, tmp, tmp2
	
	mrs	\tmp, cnthctl_el2
	
	orr	\tmp, \tmp, #(CNTHCTL_EL2_EL1PCEN_EN |\
		CNTHCTL_EL2_EL1PCTEN_EN)
	msr	cnthctl_el2, \tmp
	msr	cntvoff_el2, xzr

	
	mrs	\tmp, midr_el1
	msr	vpidr_el2, \tmp
	mrs	\tmp, mpidr_el1
	msr	vmpidr_el2, \tmp

	
	mov	\tmp, #CPTR_EL2_RES1
	msr	cptr_el2, \tmp		
	msr	hstr_el2, xzr		
	mov	\tmp, #CPACR_EL1_FPEN_EN
	msr	cpacr_el1, \tmp		

	ldr	\tmp, =(SCTLR_EL1_RES1 | SCTLR_EL1_UCI_DIS |\
			SCTLR_EL1_NTWE_DIS | SCTLR_EL1_NTWI_DIS |\
			SCTLR_EL1_UCT_DIS | SCTLR_EL1_DZE_DIS |\
			SCTLR_EL1_ICACHE_DIS | SCTLR_EL1_UMA_DIS |\
			SCTLR_EL1_EE_LE | SCTLR_EL1_WXN_DIS |\
			SCTLR_EL1_CP15BEN_DIS | SCTLR_EL1_SA0_DIS |\
			SCTLR_EL1_SA_DIS | SCTLR_EL1_DCACHE_DIS |\
			SCTLR_EL1_SED_EN | SCTLR_EL1_ITD_EN |\
			SCTLR_EL1_ALIGN_DIS | SCTLR_EL1_MMU_DIS)
	msr	sctlr_el1, \tmp

	mov	\tmp, sp
	msr	sp_el1, \tmp		
	mrs	\tmp, vbar_el2
	msr	vbar_el1, \tmp		

	cmp	\flag, #ES_TO_AARCH32
	b.eq	1f

	mrs	\tmp, id_aa64isar1_el1
	ldr	\tmp2, =(ID_AA64ISAR1_EL1_API | ID_AA64ISAR1_EL1_APA | \
		      ID_AA64ISAR1_EL1_GPI | ID_AA64ISAR1_EL1_GPA)
	tst	\tmp, \tmp2
	mov	\tmp2, #(HCR_EL2_HCD_DIS | HCR_EL2_RW_AARCH64)
	orr	\tmp, \tmp2, #(HCR_EL2_API | HCR_EL2_APK)
	csel	\tmp, \tmp2, \tmp, eq
	msr	hcr_el2, \tmp

	ldr	\tmp, =(SPSR_EL_IRQ_MASK | SPSR_EL_FIQ_MASK |\
			SPSR_EL_DEBUG_MASK | SPSR_EL_SERR_MASK |\
			SPSR_EL_M_AARCH64 | SPSR_EL_M_EL1H)
	msr	spsr_el2, \tmp
	msr     elr_el2, \ep
	eret

1:
	ldr	\tmp, =(HCR_EL2_RW_AARCH32 | HCR_EL2_HCD_DIS)
	msr	hcr_el2, \tmp

	ldr	\tmp, =(SPSR_EL_END_LE | SPSR_EL_ASYN_MASK |\
			SPSR_EL_T_A32 | SPSR_EL_M_AARCH32 |\
			SPSR_EL_IRQ_MASK | SPSR_EL_FIQ_MASK |\
			SPSR_EL_M_SVC)
	msr     spsr_el2, \tmp
	msr     elr_el2, \ep
	eret
.endm

armv8_switch_to_el1:
	switch_el x6, 0f, 1f, 0f
0:
	br x4
	ret
1:	armv8_switch_to_el1_m x4, x5, x6, x7
	ret
