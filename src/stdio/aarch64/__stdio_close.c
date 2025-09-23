#ifndef __QUIC_BAREMETAL
#include "../__stdio_close.c"
#else
#include "semihosting_calls.h"

int __stdio_close(FILE *f) {
#ifdef __QUIC_AARCH64_SEMIHOST
  return _sys_close(f->fd);
#else
  __builtin_trap();
  return 0;
#endif
}

#endif
