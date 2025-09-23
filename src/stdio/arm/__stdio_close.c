#include "stdio_impl.h"

static int dummy(int fd)
{
	return fd;
}

weak_alias(dummy, __aio_close);

int __stdio_close(FILE *f)
{
#ifdef __QUIC_ARM_SEMIHOST
  return do_semihost_call(ARM_SEMIHOST_Close, &(f->fd));
#else
	return syscall(SYS_close, __aio_close(f->fd));
#endif
}
