 ///
 /// @file    TestThreadpool.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-17 11:44:10
 ///
 
#include "Threadpool.hpp"

#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;
using namespace wd;

class Mytask
: public Task
{
public:
	void process()
	{
		srand(time(nullptr));
		int number = rand() % 100;
		cout << " get number " << number << endl;
		sleep(1);
	}
};



 
void test0() 
{

	Threadpool threadpool(4, 10);
	threadpool.start();

	unique_ptr<Task> task(new Mytask());
	int cnt = 20;
	while(cnt-- > 0) {
		threadpool.addTask(task.get());
		cout << "cnt : " << cnt << endl;
	}

	threadpool.stop();

} 
 
int main(void)
{
	test0();
	return 0;
}
