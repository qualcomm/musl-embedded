#include "stdio_impl.h"

off_t __stdio_seek(FILE *f, off_t off, int whence)
{
	off_t ret;
#ifdef __QUIC_ARM_SEMIHOST
  // Normalize offset to SEEK_SET based.
  if (whence == SEEK_END) {
    off = do_semihost_call(ARM_SEMIHOST_FLen, &f->fd);
    if (off < 0)
      return -1;
  } else if (whence == SEEK_CUR) {
    if (off == 0)
      return f->off;
    off += f->off;
  }
  int params[2] = { f->fd, off};
  ret = do_semihost_call(ARM_SEMIHOST_Seek, params);
  if (ret == 0) {
    f->off = off;
    return f->off;
  }
  return ret;
#else
#ifdef SYS__llseek
	if (syscall(SYS__llseek, f->fd, off>>32, off, &ret, whence)<0)
		ret = -1;
#else
	ret = syscall(SYS_lseek, f->fd, off, whence);
#endif
	return ret;
#endif
}
