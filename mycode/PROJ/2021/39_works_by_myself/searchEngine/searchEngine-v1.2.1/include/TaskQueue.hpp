/*================================================================
*   
*   @file       ：TaskQueue.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_TASKQUEUE_HPP__
#define __WD_TASKQUEUE_HPP__

#include "MutexLock.hpp"
#include "Condition.hpp"
#include "Task.hpp"

#include <queue>
using std::queue;



namespace wd
{

using ElemType = TaskCallback;

class TaskQueue
{
public:
	TaskQueue(size_t capacity);

	bool empty() const;
	bool full() const;
	void push(const ElemType & e);
	void push(ElemType && e);
	ElemType pop();
	void wakeup();

private:
	queue<ElemType> _que;
	size_t _capacity;
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
	bool _flag;
};

}//end of namespace wd
 
 
 
#endif
