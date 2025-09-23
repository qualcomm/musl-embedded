#ifndef _SEMIHOSTING_CALLS_H
#define _SEMIHOSTING_CALLS_H

#ifdef __QUIC_AARCH64_SEMIHOST
#include "stdio_impl.h"

long __attribute__((weak))
_sys_close(int fd);

long __attribute__((weak))
_sys_flen(int fd);

long __attribute__((weak))
_sys_open(const char *restrict filename, int mode);

long __attribute__((weak))
_sys_read(int fd, unsigned char *buf, size_t len, int mode);

long __attribute__((weak))
_sys_seek(int fd, off_t off);

long __attribute__((weak))
_sys_write(int fd, const unsigned char *buf, size_t len, int mode);

#endif

#endif
