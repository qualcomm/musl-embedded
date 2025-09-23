#include "stdio_impl.h"
#include <sys/uio.h>

size_t __stdio_read(FILE *f, unsigned char *buf, size_t len)
{
	struct iovec iov[2] = {
		{ .iov_base = buf, .iov_len = len - !!f->buf_size },
		{ .iov_base = f->buf, .iov_len = f->buf_size }
	};
	ssize_t cnt;

#ifdef __QUIC_ARM_SEMIHOST
  int params[3], ret = 0;
  cnt = -1;
  if (!iov[0].iov_len)
    cnt = 0;
  else {
    params[0] = f->fd;
    params[1] = iov[0].iov_base;
    params[2] = iov[0].iov_len;
    int ret = do_semihost_call(ARM_SEMIHOST_Read, params);
  }
  if (ret >= 0) {
    cnt = iov[0].iov_len - ret;
    ret = 0;
    if (iov[1].iov_len) {
      params[0] = f->fd;
      params[1] = iov[1].iov_base;
      params[2] = iov[1].iov_len;
      ret = do_semihost_call(ARM_SEMIHOST_Read, params);
    }
    if (ret >= 0)
      cnt += iov[1].iov_len - ret;
  }
#else
	cnt = syscall(SYS_readv, f->fd, iov, 2);
#endif
	if (cnt <= 0) {
		f->flags |= F_EOF ^ ((F_ERR^F_EOF) & cnt);
		return cnt;
	}

#ifdef __QUIC_ARM_SEMIHOST
  f->off += cnt;
#endif
	if (cnt <= iov[0].iov_len) return cnt;
	cnt -= iov[0].iov_len;
	f->rpos = f->buf;
	f->rend = f->buf + cnt;
	if (f->buf_size) buf[len-1] = *f->rpos++;
	return len;
}
