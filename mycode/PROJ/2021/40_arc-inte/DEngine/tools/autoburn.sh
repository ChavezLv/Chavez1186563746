set -e

# 以IF-BOX2为例，演示自动烧录的完整流程

# 1. 设置平台类型
export TARGET_TYPE=${TARGET_TYPE:-host}
export TARGET_OS=${TARGET_OS:-linux}
export TARGET_CPU=${TARGET_CPU:-armv8}

export DENGINE_ROOT=/DEngine

export DESDK_LIB=${DENGINE_ROOT}/desdk/platform/${TARGET_TYPE}_${TARGET_OS}-${TARGET_CPU}/lib
export LD_LIBRARY_PATH=${DESDK_LIB}:${LD_LIBRARY_PATH}
export USB_CONFIG_INI=${DENGINE_ROOT}/usbprop.ini

# 2. 切换到usb烧录模式，该脚本需要和用户的管脚控制一致
sh ${DENGINE_ROOT}/tools/switch_to_load.sh

# 3. 执行烧录，需要选择固件平台和数目
sh ${DENGINE_ROOT}/tools/burn.sh mini-pcie 4

# 4. 切换到正常工作模式，该脚本需要和用户的管脚控制一致
sh ${DENGINE_ROOT}/tools/switch_to_normal.sh