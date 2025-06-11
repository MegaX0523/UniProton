#include "hi309x_memmap.h"

.macro  branch_corex, xreg, cpu0_label, cpu1_label, cpu2_label, cpu3_label
        mrs     \xreg, mpidr_el1
        lsl     \xreg, \xreg, #48
        lsr     \xreg, \xreg, #48
        lsr     \xreg, \xreg, #8
        cmp \xreg, #0x00
        b.eq \cpu0_label
        cmp \xreg, #0x01
        b.eq \cpu1_label
        cmp \xreg, #0x02
        b.eq \cpu2_label
        cmp \xreg, #0x03
        b.eq \cpu3_label
.endm
 
.global  vector_entry
.global  armv8_switch_to_el1
.section .mcsboot.text, "ax"

#define ES_TO_AARCH64		1

_start:
	adr	x4, normal_start
	ldr	x5, =ES_TO_AARCH64
	bl	armv8_switch_to_el1
normal_start:
    ldr x0, =vector_entry
    msr vbar_el1, x0
    mov x0  ,#0 /* clear x0*/
    mov x1  ,#0 /* clear x1 */
    mov x2  ,#0 /* clear x2 */
    mov x3  ,#0 /* clear x3 */
    mov x4  ,#0 /* clear x4 */
    mov x5  ,#0 /* clear x5 */
    mov x6  ,#0 /* clear x6 */
    mov x7  ,#0 /* clear x7 */
    mov x8  ,#0 /* clear x8 */
    mov x9  ,#0 /* clear x9  */
    mov x10 ,#0 /* clear x10 */
    mov x11 ,#0 /* clear x11 */
    mov x12 ,#0 /* clear x12 */
    mov x13 ,#0 /* clear x13 */
    mov x14 ,#0 /* clear x14 */
    mov x15 ,#0 /* clear x15 */
    mov x16 ,#0 /* clear x16 */
    mov x17 ,#0 /* clear x17 */
    mov x18 ,#0 /* clear x18 */
    mov x19 ,#0 /* clear x19 */
    mov x20 ,#0 /* clear x20 */
    mov x21 ,#0 /* clear x21 */
    mov x22 ,#0 /* clear x22 */
    mov x23 ,#0 /* clear x23 */
    mov x24 ,#0 /* clear x24 */
    mov x25 ,#0 /* clear x25 */
    mov x26 ,#0 /* clear x26 */
    mov x27 ,#0 /* clear x27 */
    mov x28 ,#0 /* clear x28 */
    mov x29 ,#0 /* clear x29 */
branch_corex x1, label_cpu0 label_cpu1, label_cpu2, label_cpu3
label_cpu0:
    b label_comm
label_cpu1:
    LDR x0, =(HI_CORE1_STACK_BASE)
    BIC sp, x0, #0xf
    b label_comm
label_cpu2:
    LDR x0, =(HI_CORE2_STACK_BASE)
    BIC sp, x0, #0xf
    b label_comm
label_cpu3:
    LDR x0, =(HI_CORE3_STACK_BASE)
    BIC sp, x0, #0xf
    b label_comm
label_comm:
    bl app_startup
    b  label_comm