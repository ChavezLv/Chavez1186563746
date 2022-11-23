set -e

#LOGLEVER 0-DEBUG,1-INFO,2-IMPORTANT,3-WARNING,4-ERROR,5-FATAL
export DESDK_HOST_LOGLEVEL=2
export DESDK_DEVICE_LOGLEVEL=4

export TARGET_TYPE=${TARGET_TYPE:-host}
export TARGET_OS=${TARGET_OS:-linux}
export TARGET_CPU=${TARGET_CPU:-x64}
export USB_CONFIG_INI=${USB_CONFIG_INI:-/DEngine/usbprop.ini}

if [ ${TARGET_CH} ];then
export PLAT=${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}_${TARGET_CH}
else
export PLAT=${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}
fi

export DENGINE_ROOT=${DENGINE_ROOT:-/DEngine}
export PATH=${DENGINE_ROOT}/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/bin:${PATH}

#Add SDK path.
export SDK_HOME=${DENGINE_ROOT}/desdk

#Add TVM path.
export TVM_HOME=${DENGINE_ROOT}/detvm

#Add python path.
export PYTHONPATH=${SDK_HOME}/python:${SDK_HOME}/python/de:${SDK_HOME}/python/mm:${SDK_HOME}/python/nn:${SDK_HOME}/python/user_struct:${SDK_HOME}/python/example

#Add Gstreamer path.
#export GSTPATH=${SDK_HOME}/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/lib/gst:${SDK_HOME}/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/lib/gst/gstreamer-1.0:/usr/local/lib/gst:/usr/local/lib/gst/gstreamer-1.0
export GST_PLUGIN_PATH=$GST_PLUGIN_PATH:/usr/lib/x86_64-linux-gnu/gstreamer-1.0/:/DEngine/desdk/platform/${PLAT}/lib

#export PATH=${GSTPATH}:${PATH}
#export LD_LIBRARY_PATH=${GSTPATH}:${LD_LIBRARY_PATH}

#Add FFmpeg path.
#export FFMPATH=${SDK_HOME}/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/lib/ffmpeg:/usr/local/lib
#export PATH=${FFMPATH}:${PATH}
#export LD_LIBRARY_PATH=${FFMPATH}:${LD_LIBRARY_PATH}

#Add DESDK lib path.
export DESDK_LIB=${SDK_HOME}/platform/${PLAT}/lib
#export PATH=${DESDK_LIB}:${PATH}
export LD_LIBRARY_PATH=${DESDK_LIB}:${LD_LIBRARY_PATH}
export USB_CONFIG_INI=${DENGINE_ROOT}/usbprop.ini

#it must be followed by *.ini, *.py or other executable commands
if [ ! "$1" ]; then
  echo "illegal input! Must be followed by *.ini, *.py or other executable commands."
  exit
fi

extension="${1##*.}"

if [ "$extension" = "ini" ] || [ "$extension" = "py" ];then
  export CUR_FILE=${CUR_FILE:-$1}
  echo "CUR_FILE "$CUR_FILE
else
  $@
fi

if [ "$2" ];then
  export LOG_NAME=${LOG_NAME:-$2}
else
  export LOG_NAME="${CUR_FILE}_run.log"
fi

if [ "$extension" = "py" ];then
  echo "python3 "${CUR_FILE}" 2>&1 | tee "${LOG_NAME}
  python3 ${CUR_FILE} 2>&1 | tee ${LOG_NAME}
elif [ "$extension" = "ini" ];then
  echo "python3 ${SDK_HOME}/python/example/model_run.py "${CUR_FILE}" 2>&1 | tee "${LOG_NAME}
  python3 ${SDK_HOME}/python/example/model_run.py ${CUR_FILE} 2>&1 | tee ${LOG_NAME}
fi



