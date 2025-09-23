#ifndef __QUIC_AARCH64_SEMIHOST
#include "../system.c"
#else
#include "syscall.h"
#include "string.h"

long __attribute__((weak)) system (const char *cmd)
{
  if (!cmd)
    return 1;

  long params[2];
  params[0] = (long)cmd;
  params[1] = strlen(cmd);

  return do_semihost_call(AARCH64_SEMIHOST_System, params);
}
#endif
