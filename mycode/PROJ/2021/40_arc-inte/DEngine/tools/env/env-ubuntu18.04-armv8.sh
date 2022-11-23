apt update
apt -y install python3 python3-pip cmake
apt -y install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
apt -y install libopencv-dev ffmpeg python3-opencv

update-alternatives --install /usr/bin/python python /usr/bin/python3 50
update-alternatives --install /usr/bin/python python /usr/bin/python2 40

pip3 install -i https://mirrors.aliyun.com/pypi/simple/ --upgrade pip
pip3 install -i https://mirrors.aliyun.com/pypi/simple/ decorator xlsxwriter