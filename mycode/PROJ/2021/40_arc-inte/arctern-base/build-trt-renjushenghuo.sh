#!/bin/bash

DIR=build-trt

rm -rf $DIR

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
  -DWITH_LICENSE=OFF \
  -DTRT_ALLOWED=ON \
  -DPLAIN_MODEL=OFF \
  -DARCTERN_EXPORT_API=ON \
  -DSCENE=TrtRenjushenghuoAlgorithms \
  -DDEPLOY_TARGET=trt-linux \
  -DCMAKE_BUILD_TYPE=Release \
  -DMXNET_ALLOWED=OFF \
  ..

make -j 12
 #install
