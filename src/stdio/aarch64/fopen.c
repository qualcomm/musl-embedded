#ifndef __QUIC_LIBC_STANDALONE
#include "../fopen.c"
#else
#include "semihosting_calls.h"
#include <fcntl.h>
#include <string.h>
#include <errno.h>

FILE *fopen(const char *restrict filename, const char *restrict mode) {
#ifndef __QUIC_AARCH64_SEMIHOST
  __builtin_trap();
  return 0;
#else
  FILE *f;
  int fd;
  int flags;

  /* Check for valid initial mode character */
  if (!strchr("rwa", *mode)) {
    errno = EINVAL;
    return 0;
  }

  /* Compute the flags to pass to open() */
  flags = __fmodeflags(mode);

  // AArch64 semihosting has 3 modes: SWI, DCC and EBuffer.
  // For SWI we use hlt 0xff00 to invoke a semihosting call, this is done
  // in do_semihost_call(). For DCC and EBuffer, user need to implement their
  // own _sys_open() which overrides the weak symbol define in this file.
  fd = _sys_open(filename, flags);

  if (fd < 0) return 0;

  f = __fdopen(fd, mode);
  if (f) return f;

  return 0;
#endif
}

LFS64(fopen);
#endif
