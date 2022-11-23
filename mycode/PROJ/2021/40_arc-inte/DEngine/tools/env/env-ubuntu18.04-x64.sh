echo "deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse\n" > /etc/apt/sources.list
apt update
apt -y install python3 python3-pip build-essential gcc-4.8 g++-4.8 gcc-4.8-multilib g++-4.8-multilib cmake
apt -y install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
apt -y install libopencv-dev ffmpeg
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
update-alternatives --install /usr/bin/g++  g++ /usr/bin/g++-4.8 50
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 40
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 40
update-alternatives --install /usr/bin/python python /usr/bin/python3 50
update-alternatives --install /usr/bin/python python /usr/bin/python2 40

pip3 install --upgrade pip
pip3 install -i https://mirrors.aliyun.com/pypi/simple/ opencv-python decorator xlsxwriter