# should be run from MUSL source top directory
# ./configure --disable-wrapper CROSS_COMPILE="llvm-" CC="clang --target=aarch64-linux-gnu -fuse-ld=eld" CFLAGS="-O3 -mstrict-align -fno-vectorize-loops -fPIC -fno-rounding-math" LIBCC="$(clang -print-resource-dir)/lib/generic/libclang_rt.builtins-aarch64.a" "$@"
./configure --disable-wrapper CROSS_COMPILE="llvm-" CC="clang --target=aarch64-linux-gnu -fuse-ld=eld" CFLAGS="-O3 -mstrict-align -fPIC -fno-rounding-math" LIBCC="$(clang -print-resource-dir)/lib/generic/libclang_rt.builtins-aarch64.a" "$@"
