 ///
 /// @file    Threadpool.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-12 15:50:07
 ///
 

#include "Threadpool.hpp"
#include "Task.hpp"
#include "Thread.hpp"

#include <unistd.h>

namespace wd
{

Threadpool::Threadpool(size_t threadNum, size_t capacity)
: _threadNum(threadNum)
, _capacity(capacity)
, _threads()
, _taskque(_capacity)
, _isExit(false)
{
	_threads.reserve(_threadNum);
}

Threadpool::~Threadpool()
{}

void Threadpool::start()
{
	for(size_t idx = 0; idx != _threadNum; ++idx) {
		unique_ptr<Thread> up(new Thread(
			std::bind(&Threadpool::workerDoTask, this), std::to_string(idx)));
		_threads.push_back(std::move(up));
	}

	for(auto & thread : _threads) {
		thread->start();
	}
}

void Threadpool::stop()
{
	if(!_isExit) {
		while(!_taskque.empty()) {
			::sleep(1);
		}

		_isExit = true;
		//先唤醒所有的子线程
		_taskque.wakeup();
		//回收所有的子线程
		for(auto & thread : _threads) {
			thread->join();
		}
	}
}

void Threadpool::addTask(Task && task)
{
	_taskque.push(std::move(task));
}

Task Threadpool::getTask()
{
	return _taskque.pop();
}

void Threadpool::workerDoTask()
{
	while(!_isExit) {
		Task task = getTask();
		if(task) {
			task();
		}
	}
}

}//end of namespace wd
