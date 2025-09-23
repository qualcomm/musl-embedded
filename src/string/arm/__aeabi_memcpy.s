.syntax unified

.global __aeabi_memcpy8
.global __aeabi_memcpy4
.global __aeabi_memcpy
.global __aeabi_memmove8
.global __aeabi_memmove4
.global __aeabi_memmove

.type __aeabi_memcpy8,%function
.type __aeabi_memcpy4,%function
.type __aeabi_memcpy,%function
.type __aeabi_memmove8,%function
.type __aeabi_memmove4,%function
.type __aeabi_memmove,%function

__aeabi_memmove8:
__aeabi_memmove4:
__aeabi_memmove:
	cmp   r0, r1
	bls   3f
	cmp   r2, #0
	beq   2f
	adds  r0, r0, r2
	adds  r2, r1, r2
1:	subs  r2, r2, #1
	ldrb  r3, [r2]
	subs  r0, r0, #1
	strb  r3, [r0]
	cmp   r1, r2
	bne   1b
2:	bx    lr
__aeabi_memcpy:
3:	cmp   r2, #0
	beq   2f
	adds  r2, r1, r2
1:	ldrb  r3, [r1]
	adds  r1, r1, #1
	strb  r3, [r0]
	adds  r0, r0, #1
	cmp   r1, r2
	bne   1b
2:	bx    lr
__aeabi_memcpy8:
__aeabi_memcpy4:
	push {r4, r5, r6, r7, lr}
	mov r3, r0
	b 3f
2:	ldmia r1!, {r4, r5, r6, r7}
	stmia r3!, {r4, r5, r6, r7}
	subs r2, #16
3:	cmp r2, #16
	bhs 2b
	cmp r2, #8
	blo 4f
	ldmia r1!, {r4, r5}
	stmia r3!, {r4, r5}
	subs r2, #8
4:	cmp r2, #4
	blo 6f
	ldmia r1!, {r4}
	stmia r3!, {r4}
	subs r2, #4
	b 6f
5:	ldrb r4, [r1, #0]
	strb r4, [r3, #0]
	adds r1, #1
	adds r3, #1
6:	subs r2, #1
	bhs 5b
1:	pop {r4, r5, r6, r7, pc}
