 ///
 /// @file    TaskQueue.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-12 11:00:08
 ///
 
#ifndef __WD_TASKQUEUE_HPP__
#define __WD_TASKQUEUE_HPP__


#include "MutexLock.hpp"
#include "Condition.hpp"
#include "Task.hpp"

#include <queue>

using std::queue;
 
namespace wd
{

using Elemtype = Task;

class TaskQueue
{
public:
	TaskQueue(size_t capacity);

	bool full() const;
	bool empty() const;
	void push(Elemtype&& elem);
	Elemtype pop();
	void wakeup();

private:
	size_t _capacity;
	queue<Elemtype> _que;
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
	bool _flag;
};

}//end of namespace wd
 
 
#endif
