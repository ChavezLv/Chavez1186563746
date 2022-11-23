 ///
 /// @file    TestEventfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-21 10:19:24
 ///
 
#include "Eventfd.hpp"
#include "Thread.hpp"

#include <unistd.h>

#include <iostream>
using std::cout;
using std::endl;

void process()
{
	srand(time(nullptr));
	int number = rand() % 100;
	cout << "get a number " << number << endl;
}
 
void test0() 
{
	wd::Eventfd event(process);
	/* event.start(); */
	wd::Thread thread(std::bind(&wd::Eventfd::start, &event));
	thread.start();
 
	int cnt = 10;
	while(cnt-- > 0) {
		//主线程通知子线程执行任务
		event.wakeup();
		cout << "cnt: " << cnt << endl;
		sleep(1);
	}

	event.stop();
	thread.join();
} 
 
int main(void)
{
	test0();
	return 0;
}
