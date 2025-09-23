# should be run from MUSL source top directory

set -e

for arg
do
  if [[ "$arg" = --prefix=* ]]; then
    INSTALL_DIR=${arg##*=}
  fi
done

echo $INSTALL_DIR
mkdir -p $INSTALL_DIR/lib

# Build the size optimized version
./configure --disable-wrapper --disable-shared --disable-visibility --quic-arm-baremetal --quic-arm-nofp CROSS_COMPILE="llvm-" CC="clang --target=arm-none-eabi -fuse-ld=eld" CFLAGS="-mcpu=cortex-m3 -mfloat-abi=soft -Osize -mthumb -mno-unaligned-access -mllvm -enable-android-compat -fshort-enums -fno-short-wchar -ffunction-sections -fdata-sections -Wa,-mimplicit-it=always -fvisibility=hidden -DVISIBILITY_HIDDEN -DOPT_FOR_SIZE -fno-rounding-math" "$@" --prefix=tmp_install_dir/libc

make -j8 && make install
cp tmp_install_dir/libc/lib/libc.a $INSTALL_DIR/lib/libc-Ospace.a
rm -rf tmp_install_dir

make distclean

# Build version with errno
./configure --disable-wrapper --disable-shared --disable-visibility --quic-arm-baremetal --quic-arm-nofp CROSS_COMPILE="llvm-" CC="clang --target=arm-none-eabi -fuse-ld=eld" CFLAGS="-mcpu=cortex-m3 -mfloat-abi=soft -Osize -mthumb -mno-unaligned-access -mllvm -enable-android-compat -fshort-enums -fno-short-wchar -ffunction-sections -fdata-sections -Wa,-mimplicit-it=always -fvisibility=hidden -DVISIBILITY_HIDDEN -DSTANDALONE_ERRNO -fno-rounding-math" "$@" --prefix=tmp_install_dir/libc

make -j8 && make install
cp tmp_install_dir/libc/lib/libc.a $INSTALL_DIR/lib/libc-with-errno.a
rm -rf tmp_install_dir

make distclean

# configure for the regular version
./configure --disable-wrapper --disable-shared --disable-visibility --quic-arm-baremetal --quic-arm-nofp CROSS_COMPILE="llvm-" CC="clang --target=arm-none-eabi -fuse-ld=eld" CFLAGS="-mcpu=cortex-m3 -mfloat-abi=soft -Osize -mthumb -mno-unaligned-access -mllvm -enable-android-compat -fshort-enums -fno-short-wchar -ffunction-sections -fdata-sections -Wa,-mimplicit-it=always -fvisibility=hidden -DVISIBILITY_HIDDEN -fno-rounding-math" "$@"
