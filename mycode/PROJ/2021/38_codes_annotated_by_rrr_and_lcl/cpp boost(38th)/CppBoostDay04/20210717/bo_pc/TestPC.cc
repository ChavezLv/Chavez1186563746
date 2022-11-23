 ///
 /// @file    TestPC.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 17:17:01
 ///
 
#include "Thread.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"
#include "TaskQueue.hpp"

#include <memory>
#include <iostream>
using std::cout;
using std::endl;
using std::unique_ptr;
 
void test0() 
{
	using namespace wd;

	TaskQueue taskque(10);
	Producer p;
	Consumer c;

	//std::function + std::bind的结合 ==》 基于对象
	//std::bind提前绑定参数时，采用的是值传递
	//
	//成员函数在运行时，其绑定的对象生命周期不能被销毁;
	//如果销毁了，就会出现bug
	//
	//为了防止出现这样的bug,可以考虑直接复制对象
	unique_ptr<Thread> producerThread(new Thread(
		std::bind(&Producer::produce, p, std::ref(taskque))));

	unique_ptr<Thread> consumerThread(new Thread(
		std::bind(&Consumer::consume, c, std::ref(taskque))));
	producerThread->start();
	consumerThread->start();

	producerThread->join();
	consumerThread->join();
} 
 
int main(void)
{
	test0();
	return 0;
}
