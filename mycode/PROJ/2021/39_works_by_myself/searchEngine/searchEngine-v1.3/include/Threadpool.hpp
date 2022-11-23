/*================================================================
*   
*   @file       ：Threadpool.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_THREADPOOL_HPP__
#define __WD_THREADPOOL_HPP__

#include "TaskQueue.hpp"
#include "Task.hpp"
#include "Thread.hpp"
//加上这个是因为后面多了一个wd::current_thread, main只需要引入Threadpool.hpp就行

#include <vector>
#include <memory>

using namespace std;

namespace wd
{

class Thread;//前向声明
class Threadpool
{
	friend class WorkerThread;
public:
	Threadpool(size_t, size_t);
	~Threadpool();

	void start();
	void stop();
	void addTask(TaskCallback && cb);
private:
	TaskCallback getTask();
	//交给子线程要执行的任务
	void workerDoTask();

private:
	size_t _threadNum;
	size_t _capacity;
	vector<unique_ptr<Thread>> _threads;
	TaskQueue _taskque;
	bool _isExit;
};

}//end of namespace wd
 
 
 
#endif
