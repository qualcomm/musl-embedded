#ifndef __QUIC_BAREMETAL
#include "../__stdio_read.c"
#else
#include "semihosting_calls.h"
#include <sys/uio.h>

size_t __stdio_read(FILE *f, unsigned char *buf, size_t len) {
#ifndef __QUIC_AARCH64_SEMIHOST
  __builtin_trap();
  return 0;
#else
  struct iovec iov[2] = {
    { .iov_base = buf, .iov_len = len - !!f->buf_size },
    { .iov_base = f->buf, .iov_len = f->buf_size }
  };
  ssize_t cnt;

  long params[3], ret = 0;
  cnt = -1;
  if (!iov[0].iov_len)
    cnt = 0;
  else
    ret = _sys_read(f->fd, iov[0].iov_base, iov[0].iov_len,
                    /* mode(unused) */ 0);
  if (ret >= 0) {
    cnt = iov[0].iov_len - ret;
    ret = 0;
    if (iov[1].iov_len)
      ret = _sys_read(f->fd, iov[1].iov_base, iov[1].iov_len,
                      /* mode(unused) */ 0);
    if (ret >= 0)
      cnt += iov[1].iov_len - ret;
  }

  if (cnt <= 0) {
    f->flags |= F_EOF ^ ((F_ERR^F_EOF) & cnt);
    return cnt;
  }

  f->off += cnt;

  if (cnt <= iov[0].iov_len) return cnt;
  cnt -= iov[0].iov_len;
  f->rpos = f->buf;
  f->rend = f->buf + cnt;
  if (f->buf_size) buf[len-1] = *f->rpos++;
  return len;
#endif
}

#endif
