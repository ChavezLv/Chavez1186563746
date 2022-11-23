#!/bin/bash

export ASCEND_HOME=/usr/local/Ascend/ascend-toolkit
export ASCEND_VERSION=20.0.0.SPC100
export ARCH_PATTERN=arm64-linux_gcc7.3.0

if [ ! -d build-atlas ]
then
  mkdir build-atlas
fi
cd build-atlas

#Debug Release
#now delete install directory
rm -rf install
mkdir install
cmake \
      -DDEPLOY_TARGET=atlasLinux \
      -DCMAKE_BUILD_TYPE=Debug \
      -DSCENE=AtlasLinuxAllAlgorithms \
      -DATLAS_ALLOWED=ON \
      -DWITH_LICENSE=OFF \
      -DARCTERN_EXPORT_API=ON \
      -DPLAIN_MODEL=ON \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      ..
make -j32
#make install

