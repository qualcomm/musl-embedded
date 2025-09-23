
#ifdef __QUIC_BAREMETAL
unsigned long __initial_sp = 0;
__asm__("\
.global __main \n\
.type __main,%function \n\
__main: \n\
	mov x29, #0 \n\
	mov x30, #0 \n\
  bl __user_setup_stackheap \n\
	and sp, x0, #-16 \n\
	bl _start_c \n\
  \
.global __user_setup_stackheap \n\
.weak __user_setup_stackheap \n\
.type __user_setup_stackheap, %function \n\
__user_setup_stackheap: \n\
  adrp x0, __initial_sp \n\
  ldr x0, [x0, #:lo12:__initial_sp] \n\
  cbz x0, 1f \n\
  and sp, x0, #-16 \n\
  1: ret \n\
");

#else
__asm__(
".text \n"
".global " START "\n"
".type " START ",%function\n"
START ":\n"
"	mov x29, #0\n"
"	mov x30, #0\n"
"	mov x0, sp\n"
".weak _DYNAMIC\n"
".hidden _DYNAMIC\n"
"	adrp x1, _DYNAMIC\n"
"	add x1, x1, #:lo12:_DYNAMIC\n"
"	and sp, x0, #-16\n"
"	b " START "_c\n"
);
#endif
