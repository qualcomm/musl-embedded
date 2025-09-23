#ifndef __QUIC_AARCH64_SEMIHOST
#include "../time.c"
#else
#include <time.h>
#include "syscall.h"

time_t __attribute__((weak)) time (time_t *t)
{
  time_t r = do_semihost_call(AARCH64_SEMIHOST_Time, NULL);
  if (t)
    *t = r;
  return r;
}
#endif
