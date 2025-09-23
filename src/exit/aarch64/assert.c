#include <stdio.h>
#include <stdlib.h>

_Noreturn void __assert_fail(const char *expr, const char *file, int line, const char *func)
{
	fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
	fflush(NULL);
	abort();
}

#ifdef __QUIC_BAREMETAL
void __aeabi_assert(const char *str, const char *file, unsigned int line) {
  __assert_fail(str, file, line, NULL);
}
#endif
