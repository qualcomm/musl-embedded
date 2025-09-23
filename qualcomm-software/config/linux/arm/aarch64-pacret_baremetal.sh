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
# configure for libc standalone

./configure --disable-wrapper --quic-arm-baremetal --quic-libc-standalone --disable-visibility CROSS_COMPILE="llvm-" CC="clang --target=aarch64-none-elf -fuse-ld=eld" CFLAGS="-march=armv8.3a -mbranch-protection=pac-ret+leaf -Os -mstrict-align -fPIC -fdata-sections -ffunction-sections -fvisibility=hidden -DVISIBILITY_HIDDEN -D__QUIC_ENABLE_FLT_FOR_PRINT -fno-rounding-math" "$@" --prefix=tmp_install_dir/libc

make -j8 && make install
cp tmp_install_dir/libc/lib/libc.a $INSTALL_DIR/lib/libc-standalone.a
rm -rf tmp_install_dir
make distclean

./configure --disable-wrapper --quic-arm-baremetal --disable-visibility CROSS_COMPILE="llvm-" CC="clang --target=aarch64-none-elf -fuse-ld=eld" CFLAGS="-march=armv8.3a -mbranch-protection=pac-ret+leaf -Os -mstrict-align -fPIC -fdata-sections -ffunction-sections -fvisibility=hidden -DVISIBILITY_HIDDEN -fno-rounding-math" "$@"
