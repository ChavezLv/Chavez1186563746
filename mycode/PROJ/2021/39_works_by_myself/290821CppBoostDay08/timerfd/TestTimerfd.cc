 ///
 /// @file    TestEventfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:19:36
 ///
 
#include "TimerThread.hpp"

#include <unistd.h>

#include <iostream>
using std::cout;
using std::endl;

void process()
{
	/* cout << "process():" << endl; */
	::srand(::time(nullptr));
	int number = ::rand() % 100;
	cout << ">> number:" << number << endl;
}
 
void test0() 
{
	using namespace wd;
	Timerfd timer(3, 6, process);

	Thread thread(std::bind(&Timerfd::start, &timer));
	thread.start();

	sleep(20);

	/* timer.stop(); */
	thread.join();
} 

void test1()
{
	cout << "test1() " << endl;
	using namespace wd;
	TimerThread timer(3, 6, process);
	timer.start();

	sleep(20);
	timer.stop();
}
 
int main(void)
{
	/* test0(); */
	test1();
	return 0;
}
