#!/bin/bash

DIR=build-rk3568

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

#android-ndk-r21b
cmake \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_TOOLCHAIN_FILE=/opt/android-ndk-r17c/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI='arm64-v8a' \
  -DANDROID_PLATFORM=android-24 \
  -DCMAKE_ANDROID_STL_TYPE=gnustl_static \
  -DRKNN2_ALLOWED=ON \
  -DWITH_LICENSE=OFF \
  -DPLAIN_MODEL=ON \
  -DARCTERN_EXPORT_API=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DSCENE=RK3568AllAlgorithms \
  -DDEPLOY_TARGET=android-rk3568 \
  ..

make -j 4
