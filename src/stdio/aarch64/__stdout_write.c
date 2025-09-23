#ifndef __QUIC_BAREMETAL
#include "../__stdout_write.c"
#else
#include "semihosting_calls.h"
#include <sys/ioctl.h>

#ifdef __QUIC_AARCH64_SEMIHOST
static void init_stdout_for_semihost() {
  static char inited = 0;
  if (inited)
    return;
  stdout->fd = _sys_open(":tt", /* mode write */ 4);
  inited = 1;
}
#endif

size_t __stdout_write(FILE *f, const unsigned char *buf, size_t len) {
#ifndef __QUIC_AARCH64_SEMIHOST
  __builtin_trap();
  return 0;
#else
  init_stdout_for_semihost();
  struct winsize wsz;
  f->write = __stdio_write;
  return __stdio_write(f, buf, len);
#endif
}

#endif
