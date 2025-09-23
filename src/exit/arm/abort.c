#include <stdlib.h>
#ifndef  __QUIC_BAREMETAL
#include <signal.h>
#include "syscall.h"
#include "pthread_impl.h"
#include "atomic.h"
#endif

_Noreturn void abort(void)
{
#ifdef __QUIC_BAREMETAL
	__builtin_trap();
#else
	raise(SIGABRT);
	__block_all_sigs(0);
	a_crash();
	raise(SIGKILL);
	_Exit(127);
#endif
}