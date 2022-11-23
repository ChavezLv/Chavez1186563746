#!/bin/bash
## bash runAllUnitTest.sh $buildFolder  $SceneCmake_filePath
## bash runAllUnitTest.sh ./cmake-build-debug ./SceneCmake/LinuxAllAlgorithms.cmake
inputN=$#

if [ $inputN != 2 ];then
    echo "Error : Must input buildFolder and SceneCmake file!"
    exit 1
fi

buildFolder=$1
scenePath=$2

algArr=()
while read line
do
    str=(${line// / })
    if [ ${#str[@]} -eq 0 ];then
        continue
    fi
    if [ ${str[0]} == "#" ];then
        continue
    fi
    prefix=${str[0]%(*}
    if [ $prefix == "add_subdirectory" ];then
      #echo "!! : $prefix"
      postfix=${str[0]#*(}
      subalgpath=${postfix%)*}
     # echo "** alg path : $subalgpath"
      subalg=${subalgpath##*/}
      algArr="$algArr $subalg"
     # echo "*** alg : $subalg"
    fi
done < $scenePath

for alg1 in ${algArr[@]}
do
    echo "Notice: -------- Will test $alg1 --------"
    ./${buildFolder}/src/Algorithms/${alg1}/${alg1}_UnitTest_D
done
