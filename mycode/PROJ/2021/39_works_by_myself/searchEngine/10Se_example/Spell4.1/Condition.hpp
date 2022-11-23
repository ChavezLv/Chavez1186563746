 ///
 /// @file    Condition.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-04-30 14:41:03
 ///


#ifndef __WD_CONDITION_H__
#define __WD_CONDITION_H__


//#include "test.hh"   

#include "MutexLock.hpp"    //1. 自定义头文件
 
#include <pthread.h>		//2. C头文件

#include <iostream>			//3. C++头文件

//#include <log4cpp/Category.hh> //4. 第三方头文件

using std::cout;
using std::endl;

namespace wd
{

class Condition
:public noCopy
{
public:
	explicit
	Condition(MutexLock & mutex);
	~Condition();

	/* Condition(const Condition &)= delete; */
	/* Condition & operator=(const Condition &)=delete; */

	void wait();
	void notify();
	void notifyAll();
		
private:
	pthread_cond_t _cond;
	MutexLock & _mutex;
};

}//end of namespace wd
 
#endif
