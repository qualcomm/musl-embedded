#if defined(__arm__)
#include "stdio_impl.h"
#include <inttypes.h>

static char *fmt_i32(int x, char *s) {
  int is_neg = x < 0;
  if (is_neg)
    x = -1*(unsigned)x;
  unsigned ux = x;
  do {
    *--s = '0' + (ux % 10);
    ux /= 10;
  } while(ux);

  if (is_neg)
    *--s = '-';
  return s;
}

int puts_decimal(int arg) {
  char buf[sizeof(int)*3+1];
  char *bufEnd = buf + sizeof(buf) - 1;
  *bufEnd = 0;
  char *strInt  = fmt_i32(arg, bufEnd);
  return fputs(strInt, stdout);
}
#endif
