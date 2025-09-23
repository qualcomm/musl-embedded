#ifndef __QUIC_AARCH64_SEMIHOST
#include "../rename.c"
#else
#include "syscall.h"
#include "string.h"

int __attribute__((weak)) rename (const char *old, const char *new)
{
  long params[4] = { (long)old, strlen(old),
                     (long)new, strlen(new) };
  return do_semihost_call(AARCH64_SEMIHOST_Rename, params);
}
#endif
