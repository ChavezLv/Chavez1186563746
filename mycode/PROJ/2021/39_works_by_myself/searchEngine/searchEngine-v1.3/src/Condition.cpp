/*================================================================
*   
*   @file       ：Condition.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "Condition.hpp"
#include "MutexLock.hpp" //在实现文件中加载头文件

namespace wd
{

Condition::Condition(MutexLock & mutex)
: _mutex(mutex)
{
	pthread_cond_init(&_cond, nullptr);
}

Condition::~Condition()
{
	pthread_cond_destroy(&_cond);
}

void Condition::wait()
{
	pthread_cond_wait(&_cond, _mutex.getMutexLockPtr());
}

void Condition::notify()
{
	//如果出现异常情况，是有可能唤醒多个线程的
	pthread_cond_signal(&_cond);
}
	
void Condition::notifyAll()
{
	pthread_cond_broadcast(&_cond);
}


}//end of namespace wd
