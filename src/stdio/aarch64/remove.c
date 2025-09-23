#ifndef __QUIC_AARCH64_SEMIHOST
#include "../remove.c"
#else
#include "syscall.h"
#include "string.h"

int __attribute__((weak)) remove (const char *path)
{
  long params[2] = { (long)path, strlen(path) };
  return do_semihost_call(AARCH64_SEMIHOST_Remove, params);
}
#endif
