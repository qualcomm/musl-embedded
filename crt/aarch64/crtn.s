.section .init, "ax", %progbits
	ldp x29,x30,[sp],#16
	ret

.section .fini, "ax", %progbits
	ldp x29,x30,[sp],#16
	ret
