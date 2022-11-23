#!/usr/bin/env bash
set -e
WD=$(readlink -f "$(dirname "$0")")
cd "${WD}"


#platform select windows/linux/android
export TARGET_OS=android
#TARGET_CPU select x86/x86_64/armeabi-v7a/armeabi-v8a
export TARGET_CPU=armeabi-v7a
#  ndk-r18b
export NDK_ROOT=/opt/android-ndk-r21b
#export SDK_ROOT=./build/Install
export SDK_ROOT=$PWD
export CMAKE_BUILD_TYPE=Release



export ANDROID_NATIVE_API_LEVEL=24
export ANDROID_STL=c++_shared
export ANDROID_ABI="armeabi-v7a with NEON"
export SDK_TOOLCHAIN=${NDK_ROOT}/build/cmake/android.toolchain.cmake

export TOOLCHAIN_ROOT=${NDK_ROOT}/build/cmake


export SDK_INSTALL_ROOT=${SDK_ROOT}/install/${TARGET_OS}/${TARGET_CPU}
export EXTERNAL_ROOT=$SDK_INSTALL_ROOT

export USE_OPENMP=0
export OPENBLAS_USE_THREAD=0
export OPENBLAS_THREAD_NUM=32

export USE_NEON=1

export USE_SOFTFP=1

export USE_SQLCIPHER=1 #使用SQLite加密库SQLCipher

export N_JOBS=8

if [ ! -d build-android ]
then
  mkdir -p build-android
fi

cd build-android
rm -rf *


cmake  \
    -DAPI_LEVEL=android-24 \
    -DCMAKE_TOOLCHAIN_FILE=$SDK_TOOLCHAIN  \
    -DANDROID_ABI=$ANDROID_ABI \
    -DANDROID_NATIVE_API_LEVEL=$ANDROID_NATIVE_API_LEVEL \
    -DANDROID_STL=$ANDROID_STL \
    -DUSE_OPENMP=$USE_OPENMP\
    -DUSE_NEON=$USE_NEON \
    -DDEPLOY_TARGET="android" \
    -DCMAKE_INSTALL_PREFIX=$EXTERNAL_ROOT/arctern-base \
    -DUSR_DEF_DIR=$USR_DEF_DIR \
    -DEXTERNAL_ROOT=$EXTERNAL_ROOT \
    -DEXPORT_LIBRARY=ON\
    -DEXPORT_ALL_FUN=true\
    -DEXPORT_LIBRARY=ON\
    -DPLAIN_MODEL=true \
    -DPLATFORM_RK=true \
    -DCMAKE_BUILD_TYPE=Release \
    -DWITH_LICENSE=false \
    -DSCENE=AndroidAllAlgorithms \
    -DTVM_ALLOWED=ON \
    ..
make -j4

echo "build DONE!!"

