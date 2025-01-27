 ///
 /// @file    TaskQueue.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 16:25:46
 ///
 
#ifndef __WD_TASKQUEUE_HPP__
#define __WD_TASKQUEUE_HPP__

//自定义头文件
#include "MutexLock.hpp"
#include "Condition.hpp"
#include "Task.hpp"

//C头文件
//
//C++头文件
#include <queue>
//
//第三方库




namespace wd
{

using ElemType = Task*;

class TaskQueue
{
public:
	TaskQueue(size_t capacity);

	bool empty() const;
	bool full() const;

	void push(const ElemType & elem);
	ElemType pop();

private:
	size_t _capacity;
	std::queue<ElemType> _que;
	MutexLock _mutex;
	Condition _notFull;
	Condition _notEmpty;
};


}//end of namespace wd
 
 
 
#endif
