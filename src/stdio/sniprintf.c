//===----------------------------------------------------------------------===//
// (c) 2018 Qualcomm Innovation Center, Inc. All rights reserved.
//===----------------------------------------------------------------------===//

#if defined(__arm__)
#include "stdio_impl.h"
#include <stdarg.h>

int sniprintf(char *restrict s, size_t n, const char *restrict fmt, ...) {
  int ret;
  va_list ap;
  va_start(ap, fmt);
  ret = vsniprintf(s, n, fmt, ap);
  va_end(ap);
  return ret;
}
#endif
