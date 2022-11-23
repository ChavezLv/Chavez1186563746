#!/bin/bash
WORK_DIR=`pwd`
machine_name=DEngine
machine_id=`docker ps -a -q -f name=${machine_name}`
version=v1.3.5
usbtest_ko=`lsmod | grep usbtest`
usbtest_ko_file=`find /lib -type f -name usbtest.ko`

log_highlight() {
    echo -e "\e[30;31m"$1"$(tput sgr0)"
}

if [[ $usbtest_ko != '' ]]; then
    log_highlight "usbtest.ko module MUST be removed."
    sudo rmmod usbtest
fi

if [[ $usbtest_ko_file != '' ]]; then
    log_highlight "Remove files: $usbtest_ko_file."
    sudo rm -f $usbtest_ko_file
fi

if [[ ${machine_id} != '' ]]; then
    log_highlight "Remove docker: ${machine_name}/${machine_id} ..."
    docker rm -f ${machine_id}
fi

if [ $1 == "test" ]; then
    test_config="-v ${WORK_DIR}/../demodels-lfs:/DEngine/test/model/demodels-lfs -v ${WORK_DIR}/../dedatasets:/DEngine/data/datasets"
else
    test_config=
fi

docker_addr=113.100.143.90:8091/desdk/desdk:${version}
docker login 113.100.143.90:8091 -u custom -p DE@sz_intellif_2021
docker pull ${docker_addr}

echo Run docker from ${docker_addr}
docker run -it --net=host --privileged --device /dev --name ${machine_name} --env GST_PLUGIN_PATH=/usr/local/lib/gst/gstreamer-1.0/ --env TARGET_TYPE=host --env TARGET_OS=linux --env TARGET_CPU=x64 --env DENGINE_ROOT=/DEngine --env USB_CONFIG_INI=/DEngine/usbprop.ini -v /dev:/dev -v ${WORK_DIR}:/DEngine -v /opt/csky-toolchains:/opt/csky-toolchains ${test_config} -w /DEngine -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY -e GDK_SCALE -e GDK_DPI_SCALE  ${docker_addr}
