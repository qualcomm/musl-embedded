#include "stdio_impl.h"
#include <fcntl.h>
#include <string.h>
#include <errno.h>

FILE *fopen(const char *restrict filename, const char *restrict mode)
{
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
#ifdef __QUIC_ARM_SEMIHOST
  int params[3];
  params[0] = (int)filename;
  params[1] = flags;
  params[2] = strlen(filename);
  fd = do_semihost_call(ARM_SEMIHOST_Open, params);
#else
	fd = sys_open(filename, flags, 0666);
#endif
	if (fd < 0) return 0;
#ifndef __QUIC_ARM_SEMIHOST
	if (flags & O_CLOEXEC)
		__syscall(SYS_fcntl, fd, F_SETFD, FD_CLOEXEC);
#endif

	f = __fdopen(fd, mode);
	if (f) return f;
#ifndef __QUIC_ARM_SEMIHOST
	__syscall(SYS_close, fd);
#endif
	return 0;
}

LFS64(fopen);
