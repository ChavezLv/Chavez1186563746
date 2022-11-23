 ///
 /// @file    TestThread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-11 17:21:04
 ///

#include "Thread.hpp"

#include <unistd.h>

#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;
 
void makeNumber()
{
	cout << "thread name: " << wd::current_thread::name << endl;
	int cnt = 10;
	::srand(::clock());
	while(cnt-- > 0) {

		int number  = ::rand() % 100;
		cout << " Mythread::run()   number:" << number << endl;
		::sleep(1);
	}
}

class MyTask
{
public:
	void process()
	{
		makeNumber();
	}
};
 
void test0() 
{
	using namespace wd; //using编译指令
	using wd::Thread;   //using声明机制
	unique_ptr<MyTask> task(new MyTask());
	unique_ptr<Thread> mythread(new Thread(
		/* std::bind(&MyTask::process, task.get()))); */
		makeNumber, "1号线程"));
	mythread->start();
	mythread->join();

} 

int main(void)
{
	test0();
	return 0;
}
