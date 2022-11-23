#!/bin/bash

export ASCEND_HOME=/usr/local/Ascend/ascend-toolkit
export ASCEND_VERSION=latest
export ARCH_PATTERN=arm64-linux

if [ ! -d build-atlas-cross ]
then
  mkdir build-atlas-cross
fi
cd build-atlas-cross

#Debug Release
#now delete install directory
rm -rf install
mkdir install
cmake \
      -DCMAKE_TOOLCHAIN_FILE=../toolChain.cmake \
      -DDEPLOY_TARGET=atlasLinux \
      -DCMAKE_BUILD_TYPE=Debug \
      -DSCENE=AtlasLinuxAllAlgorithms \
      -DATLAS_ALLOWED=ON \
      -DWITH_LICENSE=OFF \
      -DARCTERN_EXPORT_API=ON \
      -DPLAIN_MODEL=OFF \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      ..
make -j32
make install

