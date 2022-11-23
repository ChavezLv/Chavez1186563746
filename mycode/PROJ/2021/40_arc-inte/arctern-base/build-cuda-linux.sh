#!/bin/bash


if [ ! -d cmake-cudabuild-debug ]
then
  mkdir cmake-cudabuild-debug
fi
cd cmake-cudabuild-debug
CMAKE_BUILD_TYPE=Debug
echo "now, begin to build!"
if [ ! -n "$1" ] ;then
  echo "you don't input CMAKE_BUILD_TYPE, we will use $CMAKE_BUILD_TYPE"
else
  CMAKE_BUILD_TYPE=$1
  echo "your bash input build type is $CMAKE_BUILD_TYPE"
fi
#now delete install directory Release  -DCMAKE_BUILD_TYPE=Debug
rm -rf install
mkdir install
cmake \
      -DDEPLOY_TARGET=CudaLinux \
      -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
      -DSCENE=CudaLinuxAllAlgorithms \
      -DMXNET_ALLOWED=ON \
      -DWITH_LICENSE=OFF \
      -DARCTERN_EXPORT_API=ON \
      -DGCC_VER=gcc4.8 \
      -DCODECOVERAGE=ON \
      -DOTHER_TEST=ON \
      ..
make -j8
#make install
