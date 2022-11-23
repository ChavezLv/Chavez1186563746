export TARGET_TYPE=host
export TARGET_OS=${TARGET_OS:-linux}
export TARGET_CPU=${TARGET_CPU:-x64}
rm -rf build
mkdir build
cd build
if [ ${TARGET_CH} ];then
cmake .. -DCMAKE_INSTALL_PREFIX=/DEngine/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}_${TARGET_CH}
else
cmake .. -DCMAKE_INSTALL_PREFIX=/DEngine/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}
fi
make install -j8
