#### 使用方式

* 生成离线的词典
  * `cd bin; ./Exe_DictProducer.sh`
* 服务器
  * `cd bin; ./server.sh`
* 客户端
  * `cd bin; ./client.sh`


  test01 : 测试TestThreadTLS.cc , 测试线程局部存储的
  c++ test/TestThreadTLS.cc src/Thread.cpp -I./include/ -lpthread


  test02: 测试线程池方法的

