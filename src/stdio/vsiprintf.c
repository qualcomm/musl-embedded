//===----------------------------------------------------------------------===//
// (c) 2018 Qualcomm Innovation Center, Inc. All rights reserved.
//===----------------------------------------------------------------------===//

#if defined(__arm__)
#include "stdio_impl.h"
#include <stdarg.h>

int vsiprintf(char *restrict s, const char *restrict fmt, va_list ap) {
  return vsniprintf(s, INT_MAX, fmt, ap);
}
#endif
