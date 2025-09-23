#include "pthread_impl.h"

int *__errno_location(void)
{
#if defined(__QUIC_BAREMETAL) && defined(STANDALONE_ERRNO)
	static int errno_val;
	return &errno_val;
#else
	return &__pthread_self()->errno_val;
#endif
}
