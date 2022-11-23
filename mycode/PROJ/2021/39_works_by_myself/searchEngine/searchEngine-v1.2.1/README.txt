#### 使用方式

* 生成离线的词典
  * `cd bin; ./Exe_DictProducer.sh`
* 服务器
  * `cd bin; ./server.sh`
* 客户端
  * `cd bin; ./client.sh`


  test01 : 测试TestThreadTLS.cc
  c++ test/TestThreadTLS.cc src/Thread.cpp -I./include/ -lpthread
SE_v0.1 就是多了一个eventfd

SE_v1.0 就是开始真正的框架了

SE_v1.1 就是多一个conf文件, 就不用再写变量什么的了


test/14 simhash/ 要在test上级目录编译$ c++ test/14simhash-demo.cc -I./include


