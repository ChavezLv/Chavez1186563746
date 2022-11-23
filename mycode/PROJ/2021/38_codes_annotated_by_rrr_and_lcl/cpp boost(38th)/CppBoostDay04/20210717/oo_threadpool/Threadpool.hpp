 ///
 /// @file    Threadpool.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-17 11:14:13
 ///
 
#ifndef __WD_THREADPOOL_HPP__
#define __WD_THREADPOOL_HPP__

#include "TaskQueue.hpp"
#include "Task.hpp"

#include <vector>
#include <memory>
using std::unique_ptr;
using std::vector;


namespace wd
{

class Thread;

class Threadpool
{
	friend class WorkerThread;
public:
	Threadpool(size_t , size_t);
	~Threadpool();

	void start();
	void stop();

	void addTask(Task *);

private:
	void workerDoTask();
	Task * getTask();


private:
	size_t _threadNum;
	size_t _capacity;
	vector<unique_ptr<Thread>> _threads;
	TaskQueue _taskque;
	bool _isExit;
};

}//end of namespace wd
 
 
 
#endif
