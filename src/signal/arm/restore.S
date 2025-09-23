.syntax unified

.global __restore
#ifdef VISIBILITY_HIDDEN
.hidden __restore
#endif
.type __restore,%function
__restore:
	movs r7,#119
	swi 0x0

.global __restore_rt
#ifdef VISIBILITY_HIDDEN
.hidden __restore_rt
#endif

.type __restore_rt,%function
__restore_rt:
	movs r7,#173
	swi 0x0
