 ///
 /// @file    Threadpool.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-24 17:25:31
 ///
 
#ifndef __WD_THREADPOOL_HPP__
#define __WD_THREADPOOL_HPP__

#include "TaskQueue.hpp"
#include "Task.hpp"

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
