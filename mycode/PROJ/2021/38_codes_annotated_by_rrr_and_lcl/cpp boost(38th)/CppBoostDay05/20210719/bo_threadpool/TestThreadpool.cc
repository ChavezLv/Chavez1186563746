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
{
public:
	void process()
	{
		srand(time(nullptr));
		int number = rand() % 100;
		cout << " >> bo threadpool get number " << number << endl;
		sleep(1);
	}

private:
	//...
};
 
void test0() 
{

	Threadpool threadpool(4, 10);
	threadpool.start();

	//当添加完任务之后，在任务被子线程执行时，
	//任务对象的生命周期还存在，不能被销毁
	unique_ptr<Mytask> task(new Mytask());
	int cnt = 20;
	while(cnt-- > 0) {
		threadpool.addTask(std::bind(&Mytask::process, task.get()));
		cout << "cnt : " << cnt << endl;
	}

	threadpool.stop();

} 
 
int main(void)
{
	test0();
	return 0;
}
