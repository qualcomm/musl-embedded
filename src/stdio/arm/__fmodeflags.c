#include <fcntl.h>
#include <string.h>

#ifdef __QUIC_ARM_SEMIHOST
int __fmodeflags(const char *mode) {
  // Value of mode: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0376d/Bgbjhiea.html
  // mode	             0	1	  2	  3	  4	5	  6	  7	  8	9	  10	11
  // ISO C fopen mode  r	rb	r+	r+b	w	wb	w+	w+b	a	ab	a+	a+b
  int flags = 0;
  if (strchr(mode, 'b')) flags |= 0x1;
  if (strchr(mode, '+')) flags |= 0x2;
  if (*mode == 'w') flags |= 0x4;
  if (*mode == 'a') flags |= 0x8;
  return flags;
}
#else
int __fmodeflags(const char *mode)
{
	int flags;
	if (strchr(mode, '+')) flags = O_RDWR;
	else if (*mode == 'r') flags = O_RDONLY;
	else flags = O_WRONLY;
	if (strchr(mode, 'x')) flags |= O_EXCL;
	if (strchr(mode, 'e')) flags |= O_CLOEXEC;
	if (*mode != 'r') flags |= O_CREAT;
	if (*mode == 'w') flags |= O_TRUNC;
	if (*mode == 'a') flags |= O_APPEND;
	return flags;
}
#endif
