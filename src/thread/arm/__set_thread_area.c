#ifdef __QUIC_BAREMETAL
__attribute__((__visibility__("hidden")))
void __a_barrier() {
#if __ARM_ARCH >= 6
  __builtin_arm_dmb(0xF);
#endif
}

__attribute__((__visibility__("hidden")))
int __a_cas(int t, int s, volatile int *p) {
#if __ARM_ARCH_6M__ == 1
  // ARMv6m has no ldrex/strex, swith to dummy version.
  // Below C code is similar to __a_cas_dummy in atomics.s.
  int tmp = *p;
  if (tmp == t) {
    *p = s;
    return 0;
  }
  return 1;
#else
  return __sync_val_compare_and_swap(p, t, s);
#endif
}

__attribute__((__visibility__("hidden")))
void __a_gettp() {
  __builtin_trap();
}

int aeabi_read_tp() __attribute__((alias("__a_gettp")));
// Pthread is unsupported in baremetal. Returning -1 forces __init_tp
// to fail.
int __set_thread_area(void *p) { return -1;};

#else

#include <stdint.h>
#include <elf.h>
#include "pthread_impl.h"
#include "libc.h"

#define HWCAP_TLS (1 << 15)

extern const unsigned char __attribute__((__visibility__("hidden")))
	__a_barrier_oldkuser[], __a_barrier_v6[], __a_barrier_v7[],
	__a_cas_v6[], __a_cas_v7[],
	__a_gettp_cp15[];

#define __a_barrier_kuser 0xffff0fa0
#define __a_barrier_oldkuser (uintptr_t)__a_barrier_oldkuser
#define __a_barrier_v6 (uintptr_t)__a_barrier_v6
#define __a_barrier_v7 (uintptr_t)__a_barrier_v7

#define __a_cas_kuser 0xffff0fc0
#define __a_cas_v6 (uintptr_t)__a_cas_v6
#define __a_cas_v7 (uintptr_t)__a_cas_v7

#define __a_gettp_kuser 0xffff0fe0
#define __a_gettp_cp15 (uintptr_t)__a_gettp_cp15

extern uintptr_t __attribute__((__visibility__("hidden")))
	__a_barrier_ptr, __a_cas_ptr, __a_gettp_ptr;

int __set_thread_area(void *p)
{
#if !__ARM_ARCH_7A__ && !__ARM_ARCH_7R__ && __ARM_ARCH < 7
	if (__hwcap & HWCAP_TLS) {
		size_t *aux;
		__a_cas_ptr = __a_cas_v7;
		__a_barrier_ptr = __a_barrier_v7;
		for (aux=libc.auxv; *aux; aux+=2) {
			if (*aux != AT_PLATFORM) continue;
			const char *s = (void *)aux[1];
			if (s[0]!='v' || s[1]!='6' || s[2]-'0'<10u) break;
			__a_cas_ptr = __a_cas_v6;
			__a_barrier_ptr = __a_barrier_v6;
			break;
		}
	} else {
		int ver = *(int *)0xffff0ffc;
		__a_gettp_ptr = __a_gettp_kuser;
		__a_cas_ptr = __a_cas_kuser;
		__a_barrier_ptr = __a_barrier_kuser;
		if (ver < 2) a_crash();
		if (ver < 3) __a_barrier_ptr = __a_barrier_oldkuser;
	}
#endif
	return __syscall(0xf0005, p);
}
#endif
