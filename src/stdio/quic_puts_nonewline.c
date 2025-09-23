#if defined(__arm__)
#include "stdio_impl.h"

int puts_nonewline(const char *s)
{
  int r;
  FLOCK(stdout);
  r = -(fputs(s, stdout) < 0);
  FUNLOCK(stdout);
  return r;
}
#endif
