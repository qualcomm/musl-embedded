#include <stdio.h>
#include <stdarg.h>

int printf(const char *restrict fmt, ...) {
  int ret;
  va_list ap;
  va_start(ap, fmt);
#if defined(__QUIC_BAREMETAL) && !defined(__QUIC_AARCH64_SEMIHOST)
  va_end(ap);
  return 0;
#else
  ret = vfprintf(stdout, fmt, ap);
  va_end(ap);
  return ret;
#endif
}
