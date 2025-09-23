#include <stdlib.h>
#ifndef  __QUIC_BAREMETAL
#include <signal.h>
#include "syscall.h"
#endif

_Noreturn void abort(void)
{
#ifndef __QUIC_BAREMETAL
	raise(SIGABRT);
	raise(SIGKILL);
#endif
	for (;;);
}
