 ///
 /// @file    TaskQueue.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-05-06 11:57:13
 ///
 
#ifndef __WD_TASKQUEUE_H__
#define __WD_TASKQUEUE_H__

#include "MutexLock.hpp"
#include "Condition.hpp"

#include <functional>
#include <iostream>
#include <queue>
using std::cout;
using std::endl;
using std::queue;
using std::function;

namespace wd
{
using Elemtype=function<void()>;
class TaskQueue
{
public:
	TaskQueue(size_t size = 10)
	: _queSize(size)
	, _mutex()
	, _cond(_mutex)
    ,_flags(true)
	{}

	bool empty();
	bool full();
	void wakeUpAll();
    void push(Elemtype data);
	Elemtype pop();

private:
	queue<Elemtype> _que;
	size_t _queSize;
	MutexLock _mutex;
	Condition _cond;
    bool _flags;
};

}//end of namespace wd
 

#endif
