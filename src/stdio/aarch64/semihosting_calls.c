#ifdef __QUIC_AARCH64_SEMIHOST
#include "libc.h"
#include "semihosting_calls.h"
#include "string.h"
#include "syscall.h"

long __attribute__((weak))
_sys_close(int fd)
{
  long param = fd;
  return do_semihost_call(AARCH64_SEMIHOST_Close, &param);
}

long __attribute__((weak))
_sys_flen(int fd)
{
  long param = fd;
  return do_semihost_call(AARCH64_SEMIHOST_FLen, &param);
}
weak_alias(_sys_flen, _sys_ftell);

long __attribute__((weak))
_sys_open(const char *restrict filename, int mode)
{
  // stdout
  if (filename[0] == ':' && filename[1] == 't' && filename[2] == 't')
    return 1;

  long params[3];
  params[0] = (long)filename;
  params[1] = mode;
  params[2] = strlen(filename);
  return do_semihost_call(AARCH64_SEMIHOST_Open, params);
}

long __attribute__((weak))
_sys_read(int fd, unsigned char *buf, size_t len, int mode)
{
  long params[3] = {fd, (long)buf, len};
  return do_semihost_call(AARCH64_SEMIHOST_Read, params);
}

long __attribute__((weak))
_sys_seek(int fd, off_t off)
{
  long params[2] = {fd, off};
  return do_semihost_call(AARCH64_SEMIHOST_Seek, params);
}

long __attribute__((weak))
_sys_write(int fd, const unsigned char *buf, size_t len, int mode)
{
  long params[3] = { fd, (long)buf, len };
  return do_semihost_call(AARCH64_SEMIHOST_Write, params);
}

#endif
