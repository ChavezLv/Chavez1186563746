 ///
 /// @file    TestThread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 14:59:02
 ///
 
#include "Thread.hpp"

#include <unistd.h>

#include <memory>
#include <iostream>
using std::cout;
using std::endl;
using std::unique_ptr;

class MyTask
{	
public:
	void process()
	{
		int cnt = 10;
		srand(time(nullptr));
		while(cnt -- > 0) {
			int number = rand() % 100;
			cout << ">> get number " << number << endl;
			sleep(1);
		}
	}
};

void execute(int x)
{
	cout << "... execute()" << endl;
	int cnt = 10;
	srand(time(nullptr));
	while(cnt -- > 0) {
		int number = rand() % 100;
		cout << ">> get number " << number << endl;
		sleep(1);
	}
}
 
void test0() 
{
	using namespace wd;

	MyTask mytask;
	ThreadCallback task = std::bind(&MyTask::process, &mytask);
	/* unique_ptr<Thread> thread(new Thread(std::move(task))); */
	// 注册回调函数，将要执行的任务交给线程对象
	/* unique_ptr<Thread> thread(new Thread(std::bind(&MyTask::process, &mytask))); */
	//基于对象的实现会更灵活, 更直接
	unique_ptr<Thread> thread(new Thread(std::bind(execute, 10)));
	thread->start();
	thread->join();
} 
 
int main(void)
{
	test0();
	return 0;
}
