# should be run from MUSL source top directory
common_cflags="-mcpu=cortex-m0 -mfloat-abi=soft -Osize -mthumb -mno-unaligned-access -mllvm -enable-android-compat -fshort-enums -fno-short-wchar -ffunction-sections -fdata-sections -fvisibility=hidden -DVISIBILITY_HIDDEN -fno-rounding-math"

./configure --disable-wrapper --disable-shared --disable-visibility --quic-arm-baremetal --quic-arm-armv6m-armv5 --quic-arm-nofp CROSS_COMPILE="llvm-" CC="clang --target=arm-none-eabi -fuse-ld=eld" CFLAGS="$common_cflags -D__QUIC_ENABLE_FLT_FOR_PRINT " \
  QUIC_VARIANTS="nofltprint_nopthread_zeroinbss" \
  QUIC_VARIANT_nofltprint_nopthread_zeroinbss_CFLAGS="$common_cflags -fzero-initialized-in-bss -D__QUIC_HAS_NO_PTHREAD" "$@"
