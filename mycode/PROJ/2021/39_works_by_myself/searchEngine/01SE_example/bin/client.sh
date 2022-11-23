#!/bin/bash

# 本地测试时直接获取配置文件中的地址和端口
IP=`sed -n "s/ServerIP //"p ../conf/conf`
PORT=`sed -n "s/Port //"p ../conf/conf`

./client $IP $PORT
