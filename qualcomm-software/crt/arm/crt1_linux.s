// Normally, baremetal users should not use this __main.
// We keep this for backward compability just in case.

.weak _init
.weak _fini
.weak __user_setup_stackheap
.weak __Heap_Initialize
.global __main
.type __main,%function
.section ".text.__main", "ax", %progbits
__main:
  nop
  bl __user_setup_stackheap
  bl __Heap_Initialize //Hook for legacy RVDS tools. argument of __Heap_Descriptor ignored.
  movs a2,#0
  mov fp,a2
  mov lr,a2
  add a2, sp, #4
  ldr a2,[a2]
  mov a3,sp
  ldr a4,=_fini
  mov a2, fp
  sub sp, #4
  str a2,[sp]
  sub sp, #4
  str a1,[sp]
  sub sp, #4
  str a4,[sp]
  ldr a4,=_init
  ldr a1,=main
  bl __libc_start_main
1:  b 1b


.type __user_setup_stackheap, %function
.section ".text.__user_setup_stackheap", "ax", %progbits
__user_setup_stackheap:
  ldr r0, [r0]
  cmp r0, #0
  beq 1f
  mov sp, r0
1: bx lr

.global __dso_handle
.section ".data.__dso_handler", "aw", %progbits
__dso_handle:
.long 0

.global __initial_sp
.section ".data.__initial_sp", "aw", %progbits
.align 2
.type __initial_sp, %object
.size __initial_sp, 4
__initial_sp:
.word 0
