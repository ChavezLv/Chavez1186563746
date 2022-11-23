#!/bin/bash

DIR=build-rv1109

if [ -f ./CMakeLists.txt ]; then
    rm -rf $DIR
    mkdir $DIR 
    cd $DIR
elif [ -d ./arctern-base ]; then
    rm -rf arctern-base/$DIR
    mkdir -p arctern-base/$DIR
    cd arctern-base/$DIR
else
    echo "current directory seems error!!!"
    exit 1
fi

cmake \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_TOOLCHAIN_FILE=../arctern-libs/toolchains/rv1109-toolchain.cmake \
  -DRKNN_ALLOWED=ON \
  -DWITH_LICENSE=OFF \
  -DPLAIN_MODEL=ON \
  -DARCTERN_EXPORT_API=ON \
  -DSCENE=RV1109AllAlgorithms \
  -DDEPLOY_TARGET=arm-linux-rv1109 \
  ..

make -j 4
 #install
