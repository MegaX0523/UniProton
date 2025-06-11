/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: The interrupt vector table processes four EL1 exception vectors (Sync/IRQ/FIQ/SERROR).
 * The exception of each entry must 128-byte alignment. The entire table must be 2 KB aligned.
 * 32 instructions (128 bytes) is not enough to process the push-pop registers, so the push-pop instructions
 * are scattered in different entries.
 */

.macro EXC_HANDLE vecId
	stp	x29, x30, [sp, #-16]!
	bl	store_regs
	mov x0, #\vecId
	bl	exception_proc
	b	exception_exit
.endm

.section .vectortbl.text, "ax"
	.align	11
	.globl	vector_entry
vector_entry:
	.align	7
	EXC_HANDLE 0	/* Synchronous el0*/

store_regs:
	stp	x27, x28, [sp, #-16]!
	stp	x25, x26, [sp, #-16]!
	stp	x23, x24, [sp, #-16]!
	stp	x21, x22, [sp, #-16]!
	stp	x19, x20, [sp, #-16]!
	stp	x17, x18, [sp, #-16]!
	stp	x15, x16, [sp, #-16]!
	stp	x13, x14, [sp, #-16]!
	stp	x11, x12, [sp, #-16]!
	stp	x9, x10, [sp, #-16]!
	stp	x7, x8, [sp, #-16]!
	stp	x5, x6, [sp, #-16]!
	stp	x3, x4, [sp, #-16]!
	stp	x1, x2, [sp, #-16]!
	b	store_el_regs

	.align	7
	EXC_HANDLE 	1     /* IRQ/vIRQ */

store_el_regs:
    mrs	x1, esr_el1
	mrs	x2, elr_el1
	mrs	x3, spsr_el1
	stp	x1, x0, [sp, #-16]!
	stp	x3, x2, [sp, #-16]!
	mov	x1, sp
	ret

	.align	7
	EXC_HANDLE 	2    /* FIQ/vFIQ */

exception_exit:
	ldp	xzr, x2, [sp],#16
	msr	elr_el1, x2
	b	restore_regs		/* jump to the second part */

	.align	7
	EXC_HANDLE 3     /* SERROR */


restore_regs:
	ldp	xzr, x0, [sp],#16
	ldp	x1, x2, [sp],#16
	ldp	x3, x4, [sp],#16
	ldp	x5, x6, [sp],#16
	ldp	x7, x8, [sp],#16
	ldp	x9, x10, [sp],#16
	ldp	x11, x12, [sp],#16
	ldp	x13, x14, [sp],#16
	ldp	x15, x16, [sp],#16
	ldp	x17, x18, [sp],#16
	ldp	x19, x20, [sp],#16
	ldp	x21, x22, [sp],#16
	ldp	x23, x24, [sp],#16
	ldp	x25, x26, [sp],#16
	ldp	x27, x28, [sp],#16
	ldp	x29, x30, [sp],#16
	eret

	.align	7
	EXC_HANDLE 4	/* Synchronous el1*/

	.align	7
	stp	x29, x30, [sp, #-16]!
	bl	store_regs
	bl	do_irq
	b	exception_exit

	.align	7
	EXC_HANDLE 6	/* FIQ/vFIQ el1*/

	.align	7
	EXC_HANDLE 7     /* SERROR el1*/
