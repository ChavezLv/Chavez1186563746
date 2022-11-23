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

class Mythread
: public wd::Thread
{	
	void run()
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
 
void test0() 
{
	using namespace wd;
	unique_ptr<Thread> thread(new Mythread());
	thread->start();
	thread->join();
} 
 
int main(void)
{
	test0();
	return 0;
}
