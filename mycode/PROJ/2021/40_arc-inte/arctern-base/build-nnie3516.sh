#!/bin/bash

# this script should run in docker: ubuntu_nnie:3519
# Note: please dont modify this file!!!
#       if you want make your change, please pull to other place and change
if [ -f ./CMakeLists.txt ]; then
    rm build-3516 -rf
    mkdir build-3516
    cd build-3516
elif [ -d ./arctern-base ]; then
    rm arctern-base/build-3516 -rf
    mkdir -p arctern-base/build-3516
    cd arctern-base/build-3516
else
    echo "current directory seems error!!!"
    exit 1
fi

#now delete install directory
rm -rf install
mkdir install
cmake \
  -DDEPLOY_TARGET=arm-linux \
  -DDEBUG=false \
  -DCMAKE_BUILD_TYPE=Release \
  -DWITH_LICENSE=OFF \
  -DHISI_ALLOWED=ON \
  -DCMAKE_TOOLCHAIN_FILE=../arctern-libs/toolchains/himix200-toolchain.cmake \
  -DSCENE=HisiAllAlgorithms \
  ..
make -j4
