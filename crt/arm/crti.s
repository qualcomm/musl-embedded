.syntax unified

.section .init, "ax", %progbits
.global _init
.type _init,%function
# Qualcomm-specific begin.
.align 4
# Qualcomm-specific end.
_init:
	push {r0,lr}

.section .fini, "ax", %progbits
.global _fini
.type _fini,%function
# Qualcomm-specific begin.
.align 4
# Qualcomm-specific end.
_fini:
	push {r0,lr}
