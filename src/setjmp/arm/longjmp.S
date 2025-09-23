.syntax unified
.global _longjmp
.global longjmp
#ifdef VISIBILITY_HIDDEN
.hidden _longjmp
.hidden longjmp
#endif
.type _longjmp,%function
.type longjmp,%function
_longjmp:
longjmp:
	ldmia r0!, {v1,v2,v3,v4}
	ldmia r0!, {r2, r3}
	mov v5, r2
	mov v6, r3
	ldmia r0!, {r2,r3}
	mov sl, r2
	mov fp, r3
	ldmia r0!, {r2, r3}
	mov sp, r2
	mov lr, r3
	mov ip,r0
  movs r0, r1
  bne 1f
  movs r0, #1
1:

#if defined(__QUIC_BAREMETAL)
  // Ignore IWMMXT arch and FPA.
 #if defined(__ARM_PCS_VFP)
	.fpu vfp
	vldmia ip!, {d8-d15}
	.fpu softvfp
	.eabi_attribute 10, 0
	.eabi_attribute 27, 0
 #endif
  bx lr
#else //For Linux, use HWCAP
	adr r1,1f
	ldr r2,1f
	ldr r1,[r1,r2]

	tst r1,#0x260
	beq 3f
	tst r1,#0x20
	beq 2f
	ldc p2, cr4, [ip], #48
2:	tst r1,#0x40
	beq 2f
	.fpu vfp
	vldmia ip!, {d8-d15}
	.fpu softvfp
	.eabi_attribute 10, 0
	.eabi_attribute 27, 0
2:	tst r1,#0x200
	beq 3f
	ldcl p1, cr10, [ip], #8
	ldcl p1, cr11, [ip], #8
	ldcl p1, cr12, [ip], #8
	ldcl p1, cr13, [ip], #8
	ldcl p1, cr14, [ip], #8
	ldcl p1, cr15, [ip], #8
3:	bx lr

.hidden __hwcap
1:	.word __hwcap-1b
#endif
