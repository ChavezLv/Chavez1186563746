 ///
 /// @file    TestPC.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 17:17:01
 ///
 
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

	unique_ptr<Thread> producer(new Producer(taskque));
	unique_ptr<Thread> consumer(new Consumer(taskque));

	producer->start();
	consumer->start();

	producer->join();
	consumer->join();

	MutexLock m1;
	//MutexLock m2 = m1;//系统资源是不能进行复制, 所以在语法层面就不能让该语句编译通过
	MutexLock m3;
	//m3 = m1;
} 
 
int main(void)
{
	test0();
	return 0;
}
