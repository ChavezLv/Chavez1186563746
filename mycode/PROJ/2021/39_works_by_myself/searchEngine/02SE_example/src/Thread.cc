 
#include "../include/Thread.hpp"
 
namespace wd
{
	
Thread::~Thread()
{
    /* cout << "~Thread()" << endl; */
	if(_isRunning) {
		pthread_detach(_pthid);
	}
}

void Thread::start() //开启线程 ==> pthread_create
{
	/* pthread_create(&_pthid, nullptr, threadFunc, nullptr); */
	pthread_create(&_pthid, nullptr, threadFunc, this);
	_isRunning = true;
}

void Thread::join()  //回收线程
{
	if(_isRunning) {
		pthread_join(_pthid, nullptr);
		_isRunning = false;
	}
}

void * Thread::threadFunc(void * arg)
{
	Thread * pthread = static_cast<Thread*>(arg);
	if(pthread) {
        pthread->_cb();
	}
}

}//end of namespace wd
