
#!/bin/bash

buildFolder="build-mnn"
scene="LinuxMnnAlgorithms"

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
      -DMNN_ALLOWED=ON \
      -DWITH_LICENSE=OFF \
      -DPLAIN_MODEL=ON \
      -DARCTERN_EXPORT_API=ON \
      -DPLATFORM_LINUX=ON \
      -DAVX_TARGET=avx2 \
      -DWITH_PYTHON=OFF \
      ..
make -j4
#make install
