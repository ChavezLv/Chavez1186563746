 ///
 /// @file    TestEventfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-21 10:19:24
 ///
 
#include "TimerThread.hpp"

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
	wd::Timer timer(3, 5, process);
	wd::Thread thread(std::bind(&wd::Timer::start, &timer));
	thread.start();

	sleep(20);

	timer.stop();
	thread.join();
} 

void test1()
{
	cout << "test1() " << endl;
	wd::TimerThread timer(3, 5, process);
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
