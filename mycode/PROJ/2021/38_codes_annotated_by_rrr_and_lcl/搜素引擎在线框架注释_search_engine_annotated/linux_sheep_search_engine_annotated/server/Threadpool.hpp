 ///
 /// @file    Threadpool.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-12 15:45:06
 ///
 
#ifndef __WD_THREADPOOL_HPP__
#define __WD_THREADPOOL_HPP__

#include "TaskQueue.hpp"
#include "Task.hpp"
#include "Thread.hpp"

#include <queue>
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

namespace wd
{

//class Thread;	
class Threadpool
{
public:
	Threadpool(size_t, size_t);
	~Threadpool();

	void start();
	void stop();

	void addTask(Task && task);

private:
	Task getTask();
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
