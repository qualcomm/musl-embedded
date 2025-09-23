#include "stdio_impl.h"
#include "libc.h"

static FILE *ofl_head = 0;
static volatile int ofl_lock[2];

FILE **__ofl_lock()
{
#ifndef __QUIC_ARM_SEMIHOST
	LOCK(ofl_lock);
#endif
	return &ofl_head;
}

void __ofl_unlock()
{
#ifndef __QUIC_ARM_SEMIHOST
	UNLOCK(ofl_lock);
#endif
}
