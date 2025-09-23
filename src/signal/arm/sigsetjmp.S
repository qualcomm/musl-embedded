.syntax unified
.global sigsetjmp
.global __sigsetjmp
#ifdef VISIBILITY_HIDDEN
.hidden sigsetjmp
.hidden __sigsetjmp
#endif
.type sigsetjmp,%function
.type __sigsetjmp,%function
sigsetjmp:
__sigsetjmp:
	tst r1,r1
	beq setjmp

	//str lr,[r0,#256]
	//str r4,[r0,#260+8]
	movs r3, #1
	lsls r3, #8
	mov r2, lr
	str r2,[r0,r3]
	adds r3, r3, #12
	str r4,[r0,r3]
	mov r4,r0

	bl setjmp

	mov r1,r0
	mov r0,r4
	//ldr lr,[r0,#256]
	//ldr r4,[r0,#260+8]
	movs r3, #1
	lsls r3, #8
	ldr r4,[r0,r3]
	mov lr, r4
	adds r3, r3, #12
	ldr r4,[r0,r3]

.hidden __sigsetjmp_tail
	b __sigsetjmp_tail
