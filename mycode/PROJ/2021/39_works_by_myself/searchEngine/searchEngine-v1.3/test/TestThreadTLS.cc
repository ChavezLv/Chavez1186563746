 ///
 /// @file    TestThread.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 14:59:02
 ///
 
#include "../include/Thread.hpp"

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
		//在任务执行的过程中，知晓是哪一个线程在执行该任务
		cout << "sub thread name: " << wd::current_thread::name << endl;
		while(cnt -- > 0) {
			int number = rand() % 100;
			cout << ">>sub thread " << wd::current_thread::name << " get number " << number << endl;
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
	cout << "main thread " << pthread_self() << endl;

	MyTask mytask;
	/* ThreadCallback task = std::bind(&MyTask::process, &mytask); */
	/* unique_ptr<Thread> thread(new Thread(std::move(task))); */
	// 注册回调函数，将要执行的任务交给线程对象
	unique_ptr<Thread> thread(new Thread(std::bind(&MyTask::process, &mytask), "worker thread"));
	//基于对象的实现会更灵活, 更直接
	/* unique_ptr<Thread> thread(new Thread(std::bind(execute, 10))); */

	//main线程
	/* cout << "main thread: sub thread's name : " << thread->getName() << endl; */

	thread->start();
	thread->join();
} 
 
int main(void)
{
	test0();
	return 0;
}
