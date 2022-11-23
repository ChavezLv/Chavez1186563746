/*================================================================
*   
*   @file       ：Threadpool.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "Threadpool.hpp"

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
	//使用vector时，要防止频繁的扩容操作
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
	//先创建对象
	for(size_t idx = 0; idx < _threadNum; ++idx) {
		unique_ptr<Thread> up(new Thread(
			std::bind(&Threadpool::workerDoTask, this), std::to_string(idx)));
		//使用移动语义
		_threads.push_back(std::move(up));
	}

	//再运行每个子线程
	for(auto & pthread : _threads) {
		pthread->start();
	}
}

//运行在main线程
void Threadpool::stop()
{
	if(!_isExit) {

		//当任务队列中还有任务没有执行完成时，先等待
		while(!_taskque.empty()) {
			::usleep(70);
		}
		//在线程池没有退出的时候，修改退出标志位
		_isExit = true;
		_taskque.wakeup();

		//回收所有的子线程
		for(auto & pthread : _threads) {
			pthread->join();//main线程在这里阻塞
		}
	}
}


void Threadpool::addTask(TaskCallback &&task)
{
	if(task) {
		_taskque.push(std::move(task));
	}
}

//每个子线程要做的事儿
/* void Threadpool::threadfunc() //换个命名方式也可以*/
void Threadpool::workerDoTask()
{
	while(!_isExit) {
		//当stop方法将_isExit设置为true之前，
		//每一个子线程执行完了任务之后，又进行了
		//下一次的循环, 然后子线程又在getTask之上阻塞
		//因为任务队列之中没有任务了
		TaskCallback task = getTask();//获取任务
		if(task) {
			task();//执行任务
		}
	}
}

TaskCallback Threadpool::getTask()
{	return _taskque.pop();	}

}//end of namespace wd
