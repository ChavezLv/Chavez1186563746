#### 使用方式

* 生成离线的词典
  * `cd bin; ./Exe_DictProducer.sh`
* 服务器
  * `cd bin; ./server.sh`
* 客户端
  * `cd bin; ./client.sh`


  test01 : 测试TestThreadTLS.cc
  c++ test/TestThreadTLS.cc src/Thread.cpp -I./include/ -lpthread
SE_v1.1 就是多了一个eventfd

SE_v1.2 就是开始真正的框架了

