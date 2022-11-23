set -e

export TARGET_TYPE=${TARGET_TYPE:-host}
export TARGET_OS=${TARGET_OS:-linux}
export TARGET_CPU=${TARGET_CPU:-x64}

export DENGINE_ROOT=/DEngine

export DESDK_LIB=${DENGINE_ROOT}/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/lib
export LD_LIBRARY_PATH=${DESDK_LIB}:${LD_LIBRARY_PATH}

if [ "$1" ];then
  export DEEPEYE_NUM=${DEEPEYE_NUM:-$1}
else
  export DEEPEYE_NUM=1
fi

if [ "$2" ];then
  export EXT_PARA=$2
fi

#create usbporp.ini
${DENGINE_ROOT}/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/bin/desdk set -t usbprop_ini -f /DEngine/usbprop.ini -n ${DEEPEYE_NUM} ${EXT_PARA}
