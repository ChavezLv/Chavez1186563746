 ///
 /// @file    TaskQueue.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-12 11:06:35
 ///
 
#include "TaskQueue.hpp"

namespace wd
{

TaskQueue::TaskQueue(size_t capacity)
: _capacity(capacity)
, _que()
, _mutex()
, _notFull(_mutex)
, _notEmpty(_mutex)
, _flag(true)
{}

bool TaskQueue::full() const
{
	return _que.size() == _capacity;
}

bool TaskQueue::empty() const
{
	return _que.size() == 0;
}

//运行在生产者线程
void TaskQueue::push(Elemtype && elem)
{
	//语句块可以控制局部对象的生命周期
	{
	MutexLockGuard autolock(_mutex);
	while(full()) {
		_notFull.wait();
	}

	//有空间的时候,将数据放入缓冲区
	_que.push(std::move(elem));
	}
	//通知消费者线程取数据
	_notEmpty.notify();
}

//运行在消费者线程
Elemtype TaskQueue::pop()
{
	MutexLockGuard autolock(_mutex);
	while(_flag && empty()) {
		_notEmpty.wait();
	}

	if(_flag) {
		//有数据了，就可以取数据了
		Elemtype tmp = _que.front();
		_que.pop();

		//通知生产者线程放入数据
		_notFull.notify();
		return tmp;
	} else {
		return nullptr;
	}
}

void TaskQueue::wakeup()
{
	_flag = false;
	_notEmpty.notifyAll();
}



}// end of namespace wd
