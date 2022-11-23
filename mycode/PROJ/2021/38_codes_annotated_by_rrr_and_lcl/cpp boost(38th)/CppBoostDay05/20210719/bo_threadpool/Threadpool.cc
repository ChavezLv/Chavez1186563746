 ///
 /// @file    Threadpool.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-17 11:21:14
 ///
 
#include "Threadpool.hpp" 
#include "Thread.hpp"

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
		unique_ptr<Thread> up(new Thread(std::bind(
			&Threadpool::workerDoTask, this)));
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
		_taskque.wakeup();

		for(auto & thread : _threads) {
			thread->join();
		}
	}
}

void Threadpool::addTask(Task && ptask)
{
	if(ptask) {
		_taskque.push(ptask);
	}
}

Task Threadpool::getTask()
{
	return _taskque.pop();
}

//该函数是每一个工作线程要调用的
void Threadpool::workerDoTask()
{
	while(!_isExit) {
		Task task = getTask();
		if(task) {
			//假设某一个子线程拿到最后一个任务了，当它执行完任务之后，
			//上面的stop方法还没有来得及将 _isExit设置为true, 该子线程又
			//进行了一次while循环操作，又阻塞在了getTask方法之上,
			//所以导致线程池无法正常退出
			task();
		}
	}
}

}//end of namespace wd
