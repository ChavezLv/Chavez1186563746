 ///
 /// @file    Condition.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 16:19:45
 ///
 
#ifndef __WD_CONDITION_HPP__
#define __WD_CONDITION_HPP__
#include "Noncopyable.hpp"

#include <pthread.h>


namespace wd
{

class MutexLock;
class Condition : Noncopyable
{
public:
	Condition(MutexLock &);
	~Condition();

	void wait();
	void notify();
	void notifyAll();

private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
};

}//end of namesapce wd
 
 
 
#endif
