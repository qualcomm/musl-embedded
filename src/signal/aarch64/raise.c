#ifndef  __QUIC_BAREMETAL
#include "../raise.c"
#else
int raise(int sig) {
  __builtin_trap();
  return 0;
}
#endif
