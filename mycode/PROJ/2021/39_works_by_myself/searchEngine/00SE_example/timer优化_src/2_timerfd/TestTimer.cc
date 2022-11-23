 ///
 /// @file    TestTimer.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-05-12 11:33:39
 ///
 
#include "TimerManager.hpp"
#include "Thread.hpp"


#include <unistd.h>


#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;

using namespace wd;


class MyTimerTask
: public TimerTask
{
public:
	MyTimerTask(int secs)
	: TimerTask(secs)
	{}

	void process()
	{
		cout << " task is processing!" << endl;
	}
};
 
void test0() 
{
	unique_ptr<TimerTask> timerTask(new MyTimerTask(3));

	TimerManager timerManager;

	//添加了一个定时器任务
	timerManager.attach(timerTask.get());
	/* timerManager.detach(timerTask.get()); */

	Thread thread(std::bind(&TimerManager::start, &timerManager));
	thread.start();

	sleep(20);
	timerManager.stop();
	thread.join();

} 
 
int main(void)
{
	test0();
	return 0;
}
