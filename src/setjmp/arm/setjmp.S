.syntax unified
.global __setjmp
.global _setjmp
.global setjmp
#ifdef VISIBILITY_HIDDEN
.hidden __setjmp
.hidden _setjmp
.hidden setjmp
#endif
.type __setjmp,%function
.type _setjmp,%function
.type setjmp,%function
__setjmp:
_setjmp:
setjmp:
	stmia r0!,{v1,v2,v3,v4}
	mov r1, v5
	mov r2, v6
	mov r3, sl
	stmia r0!, {r1-r3}
	mov r1, fp
	mov r2, sp
	mov r3, lr
	stmia r0!, {r1-r3}
	mov ip, r0
	movs r0,#0

#if defined(__QUIC_BAREMETAL)
  // Ignore IWMMXT arch and FPA.
#if defined(__ARM_PCS_VFP)
	.fpu vfp
	vstmia ip!, {d8-d15}
	.fpu softvfp
	.eabi_attribute 10, 0
	.eabi_attribute 27, 0
#endif
  bx lr
#else // For Linux, using HWCAP
	adr r1,1f
	ldr r2,1f
	ldr r1,[r1,r2]

	tst r1,#0x260
	beq 3f
	tst r1,#0x20 // HWCAP_FPA
	beq 2f
	stc p2, cr4, [ip], #48
2:	tst r1,#0x40 // HWCAP_VFP
	beq 2f
	.fpu vfp
	vstmia ip!, {d8-d15}
	.fpu softvfp
	.eabi_attribute 10, 0
	.eabi_attribute 27, 0
2:	tst r1,#0x200 // HWCAP_IWMMXT
	beq 3f
	stcl p1, cr10, [ip], #8
	stcl p1, cr11, [ip], #8
	stcl p1, cr12, [ip], #8
	stcl p1, cr13, [ip], #8
	stcl p1, cr14, [ip], #8
	stcl p1, cr15, [ip], #8
3:	bx lr

.hidden __hwcap
1:	.word __hwcap-1b

#endif
