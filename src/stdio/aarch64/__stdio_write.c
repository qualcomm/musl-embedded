#ifndef __QUIC_BAREMETAL
#include "../__stdio_write.c"
#else
#include "semihosting_calls.h"
#include <sys/uio.h>

size_t __stdio_write(FILE *f, const unsigned char *buf, size_t len) {
#ifndef __QUIC_AARCH64_SEMIHOST
  __builtin_trap();
  return 0;
#else
  struct iovec iovs[2] = {
    { .iov_base = f->wbase, .iov_len = f->wpos-f->wbase },
    { .iov_base = (void *)buf, .iov_len = len }
  };

  struct iovec *iov = iovs;
  size_t rem = iov[0].iov_len + iov[1].iov_len;
  int iovcnt = 2;
  ssize_t cnt;
  for (;;) {
    cnt = iovs[0].iov_len - _sys_write(f->fd, iovs[0].iov_base, iovs[0].iov_len,
                                       /* mode(unused) */ 0);
    f->off += cnt;
    if (iovcnt > 1) {
      int nr = iovs[1].iov_len - _sys_write(f->fd, iovs[1].iov_base,
                                            iovs[1].iov_len,
                                            /* mode(unused) */ 0);
      f->off += nr;
      cnt += nr;
    }

    if (cnt == rem) {
      f->wend = f->buf + f->buf_size;
      f->wpos = f->wbase = f->buf;
      return len;
    }
    if (cnt < 0) {
      f->wpos = f->wbase = f->wend = 0;
      f->flags |= F_ERR;
      return iovcnt == 2 ? 0 : len-iov[0].iov_len;
    }
    rem -= cnt;
    if (cnt > iov[0].iov_len) {
      cnt -= iov[0].iov_len;
      iov++; iovcnt--;
    }
    iov[0].iov_base = (char *)iov[0].iov_base + cnt;
    iov[0].iov_len -= cnt;
  }
#endif
}

#endif
