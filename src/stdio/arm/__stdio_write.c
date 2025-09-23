#include "stdio_impl.h"
#include <sys/uio.h>

size_t __stdio_write(FILE *f, const unsigned char *buf, size_t len)
{
	struct iovec iovs[2] = {
		{ .iov_base = f->wbase, .iov_len = f->wpos-f->wbase },
		{ .iov_base = (void *)buf, .iov_len = len }
	};
	struct iovec *iov = iovs;
	size_t rem = iov[0].iov_len + iov[1].iov_len;
	int iovcnt = 2;
	ssize_t cnt;
	for (;;) {
#ifdef __QUIC_ARM_SEMIHOST
    int params[3];
    params[0] = f->fd;
    params[1] = iovs[0].iov_base;
    params[2] = iovs[0].iov_len;
    cnt = iovs[0].iov_len - do_semihost_call(ARM_SEMIHOST_Write, params);
    f->off += cnt;
    if (iovcnt > 1) {
      params[1] = iovs[1].iov_base;
      params[2] = iovs[1].iov_len;
      int nr = iovs[1].iov_len - do_semihost_call(ARM_SEMIHOST_Write, params);
      f->off += nr;
      cnt += nr;
    }
#else
		cnt = syscall(SYS_writev, f->fd, iov, iovcnt);
#endif

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
}
