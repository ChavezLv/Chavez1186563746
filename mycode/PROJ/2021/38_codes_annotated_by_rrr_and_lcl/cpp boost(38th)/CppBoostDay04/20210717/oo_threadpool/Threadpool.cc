 ///
 /// @file    Threadpool.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-17 11:21:14
 ///
 
#include "Threadpool.hpp" 
#include "WorkerThread.hpp"

#include <unistd.h>

namespace wd
{
Threadpool::Threadpool(size_t threadNum, size_t capacity)
: _threadNum(threadNum)
, _capacity(capacity)
, _taskque(_capacity)
, _isExit(false)
{
	_threads.reserve(_threadNum);
}

Threadpool::~Threadpool()
{
	if(!_isExit) {
		stop();
	}
}

void Threadpool::start()
{
	//1.创建子线程对象
	for(size_t idx = 0; idx < _threadNum; ++idx) {
		unique_ptr<Thread> up(new WorkerThread(*this));
		_threads.push_back(std::move(up));
	}
	//2.让每一个子线程跑起来
	for(auto & thread : _threads) {
		thread->start();
	}
}

void Threadpool::stop()
{
	if(!_isExit) {

		//只有当所有的任务都执行完毕时，才让其退出
		while(!_taskque.empty()) {
			sleep(1);
		}
		_isExit = true;

		for(auto & thread : _threads) {
			thread->join();
		}
	}
}

void Threadpool::addTask(Task * ptask)
{
	if(ptask) {
		_taskque.push(ptask);
	}
}

Task * Threadpool::getTask()
{
	return _taskque.pop();
}

//该函数是每一个工作线程要调用的
void Threadpool::workerDoTask()
{
	while(!_isExit) {
		Task * ptask = getTask();
		if(ptask) {
			ptask->process();
		}
	}
}

}//end of namespace wd
