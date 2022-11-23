 ///
 /// @file    TestEventfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:19:36
 ///
 
#include "TimerManager.cpp"
#include "TimerThread.hpp"


#include <unistd.h>

#include <iostream>
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;
using namespace wd;

class MyTimerTask
:public TimerTask{
public:
    MyTimerTask(int secs)
    : TimerTask(secs){

    }

    void process(){
        cout << "void process()" << endl;
    }

};


void process()
{
	/* cout << "process():" << endl; */
	::srand(::time(nullptr));
	int number = ::rand() % 100;
	cout << ">> number:" << number << endl;
}
 
void test0() 
{
    unique_ptr<TimerTask> timerTask(new MyTimerTask(3));

    TimerManager timerManager;
    Thread thread(std::bind(&TimerManager::start, &timerManager));
    thread.start();

    //add  a  fixed time task
    timerManager.attach(timerTask.get());

    sleep(20);
    timerManager.stop();
    thread.join();
} 

 
int main(void)
{
	test0();
	/* test1(); */
	return 0;
}
