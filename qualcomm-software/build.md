# Building musl-embedded

## Prerequisites

musl-embedded broadly has the same build requirements and prerequisites as musl.
[INSTALL](../INSTALL) lists the basic requirements and steps for building,
installing, and testing musl.

Beyond this, musl-embedded provides scripts to configure select Arm/AArch64
variants in [qualcomm-software/config/linux/arm](config/linux/arm/). These scripts assume that
clang, compiler-rt, LLVM's binutils, and eld are available for the appropriate
targets and will use those to build. Prebuilt binaries can be used, otherwise
refer to [LLVM's documentation](https://llvm.org/docs/GettingStarted.html) to
build clang, compiler-rt, and LLVM's binutils and
[eld's documentation](https://github.com/qualcomm/eld) to build eld.

## Building using musl-embedded scripts

To build a variant in [qualcomm-software/config/linux/arm](config/linux/arm/), the scripts
can be executed in place of the usual configure step as below:

```
cd /path/to/musl-embedded
make distclean
bash -x qualcomm-software/config/linux/arm/<variant>.sh --prefix=/path/to/musl_install/libc
make
make install
```

To build the commom variants in [qualcomm-software/config/linux/arm](config/linux/arm/) in one
go, [component_list.sh](config/component_list.sh) can be used in a script as
below:

```
#!/bin/bash

MUSL_BUILDDIR=/path/to/musl-embedded
INSTALL_DIR=/path/to/install/dir
source "$MUSL_BUILDDIR/qualcomm-software/config/component_list.sh"
for lib in ${musl_components[*]}; do
    libName=$(echo $lib | awk -F".sh," '{print $1}')
    dirName=$(echo $lib | awk -F"," '{print $2}')
    mkdir -p $MUSL_BUILDDIR
    cd $MUSL_BUILDDIR
    make distclean
    bash -x $MUSL_BUILDDIR/qualcomm-software/config/linux/arm/$libName".sh" --prefix=$INSTALL_DIR/$dirName/libc
    make
    make install
done
```
