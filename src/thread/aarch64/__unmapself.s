.global __unmapself
#ifdef VISIBILITY_HIDDEN
.hidden __unmapself
#endif
.type   __unmapself,%function
__unmapself:
	mov x8,#215 // SYS_munmap
	svc 0
	mov x8,#93 // SYS_exit
	svc 0
