# should be run from MUSL source top directory

# Use compiler-rt instead of default libgcc to fix an undefined reference
# error:
# malloc.c:(.text.malloc+0x34): undefined reference to `__floatsisf'

RESOURCE_DIR=$(clang -print-resource-dir)
PATH_TO_BUILTIN=$RESOURCE_DIR/lib/baremetal
BUILTIN=clang_rt.builtins-aarch64-nofp

./configure --disable-wrapper --quic-arm-baremetal --quic-aarch64-nofp --disable-visibility CROSS_COMPILE="llvm-" CC="clang --target=aarch64-none-elf -fuse-ld=eld" CFLAGS="-mabi=aapcs-soft -Os -mstrict-align -fPIC -fdata-sections -ffunction-sections -fvisibility=hidden -DVISIBILITY_HIDDEN -mgeneral-regs-only -mllvm -aarch64-enable-simd-scalar=false -fno-rounding-math" LDFLAGS="-Wl,-L${PATH_TO_BUILTIN}" LIBCC="-l${BUILTIN}" "$@"
