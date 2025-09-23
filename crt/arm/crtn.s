.syntax unified

.section .init, "ax", %progbits
	pop {r0,lr}
	bx lr

.section .fini, "ax", %progbits
	pop {r0,lr}
	bx lr
