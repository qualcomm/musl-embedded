#include <string.h>
#include <stdint.h>
#include <endian.h>

#if (__ARM_ARCH == 6) && __thumb__
void __aeabi_memcpy4(void *restrict dest, const void *restrict src, size_t n);

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
	unsigned char *d = dest;
	const unsigned char *s = src;

	if (n >= 8) {
		// Align dest
		// If d end with
		// 001 -> 010, 2-byte aligned
		// 010 -> 010, no action
		// 011 -> 100, 4-byte aligned
		if ((unsigned)d & 0x01) {
			*d++ = *s++;
			n -= 1;
		}
		// d ends with 010 or 100 now
		if ((unsigned)d & 0x02) {
			*d++ = *s++;
			*d++ = *s++;
			n -= 2;
		}
		// Check src alignment after dest aligned
		unsigned t = (unsigned)s & 0x03;
		if (t) {
			// src not aligned
			// Move forward 4 bytes so we don't underflow src later
			// n is at least 5 at this point
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			*d++ = *s++;
			n -= 4;

			if (n > 4) {
				s -= t;
				unsigned *sw = (unsigned *)s;
				unsigned *dw = (unsigned *)d;
				unsigned r = t << 3;
				unsigned l = (4-t) << 3;

				unsigned w = *sw++;
				for (; n>=4; n-=4) {
					unsigned w2 = *sw++;
					w = w >> r | w2 << l;
					*dw++ = w;
					w = w2;
				}
				// For leftover bytes
				s = (const unsigned char *)sw;
				s -= l >> 3; //(4-t);
				d = (unsigned char *)dw;
			}
		} else {
			// src aligned
			__aeabi_memcpy4(d, s, n);
			return dest;
		}
	}
	// Copy leftover bytes
	if (n & 0x04) {
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
	}
	if (n & 0x02) {
		*d++ = *s++;
		*d++ = *s++;
	}
	if (n & 0x01)
		*d++ = *s++;

	return dest;
}
#elif (__ARM_ARCH == 7) && OPT_FOR_SIZE
#define SS (sizeof(size_t))
#define ALIGN (sizeof(size_t)-1)
void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
        unsigned char *d = dest;
        const unsigned char *s = src;

        if (((uintptr_t)d & ALIGN) != ((uintptr_t)s & ALIGN))
                goto misaligned;

        for (; ((uintptr_t)d & ALIGN) && n; n--) *d++ = *s++;
        if (n) {
                size_t *wd = (void *)d;
                const size_t *ws = (const void *)s;

                for (; n>=SS; n-=SS) *wd++ = *ws++;
                d = (void *)wd;
                s = (const void *)ws;
misaligned:
                for (; n; n--) *d++ = *s++;
        }
        return dest;
}
#else
void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;

#ifdef __GNUC__

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define LS >>
#define RS <<
#else
#define LS <<
#define RS >>
#endif

	typedef uint32_t __attribute__((__may_alias__)) u32;
	uint32_t w, x;

	for (; (uintptr_t)s % 4 && n; n--) *d++ = *s++;

	if ((uintptr_t)d % 4 == 0) {
		for (; n>=16; s+=16, d+=16, n-=16) {
			*(u32 *)(d+0) = *(u32 *)(s+0);
			*(u32 *)(d+4) = *(u32 *)(s+4);
			*(u32 *)(d+8) = *(u32 *)(s+8);
			*(u32 *)(d+12) = *(u32 *)(s+12);
		}
		if (n&8) {
			*(u32 *)(d+0) = *(u32 *)(s+0);
			*(u32 *)(d+4) = *(u32 *)(s+4);
			d += 8; s += 8;
		}
		if (n&4) {
			*(u32 *)(d+0) = *(u32 *)(s+0);
			d += 4; s += 4;
		}
		if (n&2) {
			*d++ = *s++; *d++ = *s++;
		}
		if (n&1) {
			*d = *s;
		}
		return dest;
	}

	if (n >= 32) switch ((uintptr_t)d % 4) {
	case 1:
		w = *(u32 *)s;
		*d++ = *s++;
		*d++ = *s++;
		*d++ = *s++;
		n -= 3;
		for (; n>=17; s+=16, d+=16, n-=16) {
			x = *(u32 *)(s+1);
			*(u32 *)(d+0) = (w LS 24) | (x RS 8);
			w = *(u32 *)(s+5);
			*(u32 *)(d+4) = (x LS 24) | (w RS 8);
			x = *(u32 *)(s+9);
			*(u32 *)(d+8) = (w LS 24) | (x RS 8);
			w = *(u32 *)(s+13);
			*(u32 *)(d+12) = (x LS 24) | (w RS 8);
		}
		break;
	case 2:
		w = *(u32 *)s;
		*d++ = *s++;
		*d++ = *s++;
		n -= 2;
		for (; n>=18; s+=16, d+=16, n-=16) {
			x = *(u32 *)(s+2);
			*(u32 *)(d+0) = (w LS 16) | (x RS 16);
			w = *(u32 *)(s+6);
			*(u32 *)(d+4) = (x LS 16) | (w RS 16);
			x = *(u32 *)(s+10);
			*(u32 *)(d+8) = (w LS 16) | (x RS 16);
			w = *(u32 *)(s+14);
			*(u32 *)(d+12) = (x LS 16) | (w RS 16);
		}
		break;
	case 3:
		w = *(u32 *)s;
		*d++ = *s++;
		n -= 1;
		for (; n>=19; s+=16, d+=16, n-=16) {
			x = *(u32 *)(s+3);
			*(u32 *)(d+0) = (w LS 8) | (x RS 24);
			w = *(u32 *)(s+7);
			*(u32 *)(d+4) = (x LS 8) | (w RS 24);
			x = *(u32 *)(s+11);
			*(u32 *)(d+8) = (w LS 8) | (x RS 24);
			w = *(u32 *)(s+15);
			*(u32 *)(d+12) = (x LS 8) | (w RS 24);
		}
		break;
	}
	if (n&16) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&8) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&4) {
		*d++ = *s++; *d++ = *s++; *d++ = *s++; *d++ = *s++;
	}
	if (n&2) {
		*d++ = *s++; *d++ = *s++;
	}
	if (n&1) {
		*d = *s;
	}
	return dest;
#endif

	for (; n; n--) *d++ = *s++;
	return dest;
}
#endif
