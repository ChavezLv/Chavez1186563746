echo "deb-src http://archive.ubuntu.com/ubuntu xenial main restricted\ndeb http://mirrors.aliyun.com/ubuntu/ xenial main restricted\ndeb-src http://mirrors.aliyun.com/ubuntu/ xenial main restricted multiverse universe\ndeb http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted\ndeb-src http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted multiverse universe\ndeb http://mirrors.aliyun.com/ubuntu/ xenial universe\ndeb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe\ndeb http://mirrors.aliyun.com/ubuntu/ xenial multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ xenial-updates multiverse\ndeb http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse\ndeb-src http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse\ndeb http://archive.canonical.com/ubuntu xenial partner\ndeb-src http://archive.canonical.com/ubuntu xenial partner\ndeb http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted\ndeb-src http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted multiverse universe\ndeb http://mirrors.aliyun.com/ubuntu/ xenial-security universe\ndeb http://mirrors.aliyun.com/ubuntu/ xenial-security multiverse\n" > /etc/apt/sources.list

# add-apt-repository ppa-jonathonf/python-3.6

# 执行以下命令出现 Press [ENTER] to continue or ctrl-c to cancel adding it，需要手动输入回车
add-apt-repository ppa:deadsnakes/ppa
apt update
apt -y install python3.6

update-alternatives --install  /usr/bin/python3 python3 /usr/bin/python3.6 50
update-alternatives --install  /usr/bin/python3 python3 /usr/bin/python3.5 40

apt -y install python3-pip build-essential gcc-4.8 g++-4.8 gcc-4.8-multilib g++-4.8-multilib cmake
apt -y install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly
apt -y install libopencv-dev ffmpeg

update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 40
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-5 40
update-alternatives --install /usr/bin/python python /usr/bin/python3 50
update-alternatives --install /usr/bin/python python /usr/bin/python2 40

# 执行以下命令如果出现错误 No module named 'pip._internal'，打开文件/usr/local/bin/pip3，将from pip._internal import main 改成 from pip import main
pip3 install --upgrade pip
pip3 install -i https://mirrors.aliyun.com/pypi/simple/ opencv-python==4.1.2.30 decorator==4.4.0 xlsxwriter