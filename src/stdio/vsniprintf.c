//===----------------------------------------------------------------------===//
// (c) 2018 Qualcomm Innovation Center, Inc. All rights reserved.
//===----------------------------------------------------------------------===//

#if defined(__arm__)
#include "stdio_impl.h"
#include <errno.h>
#include <stdarg.h>

struct cookie {
    char *s;
    size_t n;
};

extern size_t sn_write(FILE *f, const unsigned char *s, size_t l);

int vsniprintf_full(char *restrict s, size_t n,
                    const char *restrict fmt, va_list ap) {
  unsigned char buf[1];
  char dummy[1];
  struct cookie c = { .s = n ? s : dummy, .n = n ? n-1 : 0 };
  FILE f = {
          .lbf = EOF,
          .write = sn_write,
          .lock = -1,
          .buf = buf,
          .cookie = &c,
  };

  if (n > INT_MAX) {
    errno = EOVERFLOW;
    return -1;
  }

  *c.s = 0;
  return vfiprintf(&f, fmt, ap);
}
#endif
