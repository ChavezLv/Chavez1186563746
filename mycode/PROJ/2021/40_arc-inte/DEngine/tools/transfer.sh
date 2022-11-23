set -e

export TARGET_TYPE=${TARGET_TYPE:-host}
export TARGET_OS=${TARGET_OS:-linux}
export TARGET_CPU=${TARGET_CPU:-x64}

export DENGINE_ROOT=/DEngine

export DESDK_LIB=${DENGINE_ROOT}/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/lib
export LD_LIBRARY_PATH=${DESDK_LIB}:${LD_LIBRARY_PATH}
export USB_CONFIG_INI=${DENGINE_ROOT}/usbprop.ini

${DENGINE_ROOT}/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/bin/desdk transfer $@
