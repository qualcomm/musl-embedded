#include "stdio_impl.h"
#include <sys/ioctl.h>

#ifdef __QUIC_ARM_SEMIHOST
static void init_stdout_for_semihost() {
  static char inited = 0;
  if (inited)
    return;
  int params[3];
  params[0] = (int) ":tt";
  params[2] = 3; // len of filename
  params[1] = 4; // mode write
  stdout->fd = do_semihost_call(ARM_SEMIHOST_Open, params);
  inited = 1;
}
#endif

size_t __stdout_write(FILE *f, const unsigned char *buf, size_t len)
{
#ifdef __QUIC_ARM_SEMIHOST
  init_stdout_for_semihost();
#endif
	struct winsize wsz;
	f->write = __stdio_write;
#ifndef __QUIC_ARM_SEMIHOST
	if (!(f->flags & F_SVB) && __syscall(SYS_ioctl, f->fd, TIOCGWINSZ, &wsz))
		f->lbf = -1;
#endif
	return __stdio_write(f, buf, len);
}
