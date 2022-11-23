#!/bin/bash

buildFolder="cmake-build-debug"
scene="LinuxAllAlgorithms"

rm -rf $buildFolder
if [ ! -d $buildFolder ]
then
  mkdir $buildFolder
fi
cd $buildFolder

inputNum=$#
echo "inputNum = $inputNum"

#Debug Release
#now delete install directory
rm -rf install
mkdir install
cmake \
      -DDEPLOY_TARGET=linux \
      -DCMAKE_BUILD_TYPE=Release \
      -DSCENE=$scene \
      -DMXNET_ALLOWED=ON \
      -DWITH_LICENSE=OFF \
      -DPLAIN_MODEL=ON \
      -DARCTERN_EXPORT_API=ON \
      ..
make -j8
#make install

if [ $inputNum -gt 0 ];then
  if [ $1 == "test" ];then
    echo "========== Begin UnitTest =========="
    cd ..
    bash runAllUnitTest.sh $buildFolder "./SceneCmake/$scene.cmake"
  fi
fi
