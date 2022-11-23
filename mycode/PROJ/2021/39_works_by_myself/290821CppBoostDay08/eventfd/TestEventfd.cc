 ///
 /// @file    TestEventfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:19:36
 ///
 
#include "Eventfd.hpp"
#include "Thread.hpp"

#include <unistd.h>

#include <iostream>
using std::cout;
using std::endl;

void process()
{
	cout << "process():" << endl;
	::srand(::time(nullptr));
	int number = ::rand() % 100;
	cout << ">> number:" << number << endl;
}
 
void test0() 
{
	using namespace wd;
	Eventfd event(process);

	Thread thread(std::bind(&Eventfd::start, &event));
	thread.start();

	int cnt = 20;
	while(cnt-- > 0) {
		event.wakeup();
		//::sleep(1);
	}

	event.stop();
	thread.join();
} 
 
int main(void)
{
	test0();
	return 0;
}
