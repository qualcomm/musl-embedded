#include <stdint.h>
#include "syscall.h"
#ifdef __QUIC_BAREMETAL
uintptr_t __heap_base __attribute__((weak));
uintptr_t __heap_limit __attribute__((weak));
extern uintptr_t g_tzt_heap_base __attribute__((weak, visibility("default")));
extern uintptr_t g_tzt_heap_end __attribute__((weak, visibility("default")));
#endif

uintptr_t __brk(uintptr_t newbrk)
{
#ifdef __QUIC_BAREMETAL
  if (newbrk == 0) {
    // For backward compatibility, if user defines g_tzt_heap_{base,end}, they
    // will be used.
    if (&g_tzt_heap_base && __heap_base == 0)
      __heap_base = g_tzt_heap_base;
    if (&g_tzt_heap_end && __heap_limit == 0)
      __heap_limit = g_tzt_heap_end;
    return __heap_base;
  }
  return newbrk < __heap_limit ? newbrk : 0;
#else
  return __syscall(SYS_brk, newbrk);
#endif
}
