/*================================================================
*   
*   @file       ：Condition.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_CONDITION_HPP__
#define __WD_CONDITION_HPP__

#include "Noncopyable.hpp"

#include <pthread.h>

namespace wd
{

class MutexLock;//只需要设置类的前向声明就可以满足需求
			    //不需要加载MutexLock的头文件
class Condition : Noncopyable
{
public:
	Condition(MutexLock & mutex);
	~Condition();

	void wait();
	void notify();
	void notifyAll();

private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
};


}//end of namespace wd
 
 
 
#endif
