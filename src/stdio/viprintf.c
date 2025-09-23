//===----------------------------------------------------------------------===//
// (c) 2018 Qualcomm Innovation Center, Inc. All rights reserved.
//===----------------------------------------------------------------------===//

#if defined(__arm__)
#include "stdio_impl.h"
#include <stdarg.h>

int viprintf(const char *restrict fmt, va_list ap) {
  return vfiprintf(stdout, fmt, ap);
}
#endif
