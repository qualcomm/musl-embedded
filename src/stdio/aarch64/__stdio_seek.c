#ifndef __QUIC_BAREMETAL
#include "../__stdio_seek.c"
#else
#include "semihosting_calls.h"

off_t __stdio_seek(FILE *f, off_t off, int whence) {
#ifndef __QUIC_AARCH64_SEMIHOST
  __builtin_trap();
  return 0;
#else
  off_t ret;
  // Normalize offset to SEEK_SET based.
  if (whence == SEEK_END) {
    off = _sys_flen(f->fd);
    if (off < 0)
      return -1;
  } else if (whence == SEEK_CUR) {
    if (off == 0)
      return f->off;
    off += f->off;
  }
  ret = _sys_seek(f->fd, off);
  if (ret == 0) {
    f->off = off;
    return f->off;
  }
  return ret;
#endif
}

#endif
