#!/bin/bash

###SYSTEM = android, linux,

cd build
cmake \
      -DDEPLOY_TARGET=android \
      -DCMAKE_BUILD_TYPE=Debug \
      -DSCENE=rk3288advertiser \
      -DWITH_LICENSE=true \
      ..
make

