export TARGET_TYPE=dev
export TARGET_OS=linux
export TARGET_CPU=dp1000
rm -rf build
mkdir build
cd build

export PATH=$PATH:/opt/csky-toolchains/bin:$PWD/../../tools/csky-toolchains/bin

echo "TARGET_TYPE ="$TARGET_TYPE
echo "TARGET_OS ="$TARGET_OS
echo "TARGET_CPU ="$TARGET_CPU

if [ ${TARGET_CH} ];then
cmake .. -DCMAKE_INSTALL_PREFIX=/DEngine/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}_${TARGET_CH}
else
cmake .. -DCMAKE_INSTALL_PREFIX=/DEngine/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}
fi
make install -j8

