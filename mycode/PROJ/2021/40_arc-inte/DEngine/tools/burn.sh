set -e

export TARGET_TYPE=${TARGET_TYPE:-host}
export TARGET_OS=${TARGET_OS:-linux}
export TARGET_CPU=${TARGET_CPU:-x64}

export DENGINE_ROOT=/DEngine

export DESDK_LIB=${DENGINE_ROOT}/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/lib
export LD_LIBRARY_PATH=${DESDK_LIB}:${LD_LIBRARY_PATH}
export USB_CONFIG_INI=${DENGINE_ROOT}/usbprop.ini

if [ "$1" ];then
  export HWPLAT=$1
else
  export HWPLAT=evb
fi

if [ "$2" ];then
  export DEEPEYE_NUM=$2
else
  export DEEPEYE_NUM=1
fi

echo "PLATFORM=${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}"
/DEngine/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/bin/desdk download -n ${DEEPEYE_NUM} -b desdk -f /DEngine/desdk/archive/$HWPLAT/file/config.ini