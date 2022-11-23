#ifndef __RXL_THREADPOLL_H__
#define __RXL_THREADPOLL_H__
#include"thread.hpp"
#include"taskqueue.hpp"
#include<vector>
#include<memory>
#include<functional>
using std::unique_ptr;
using std::vector;
namespace rxl{
	using CallBackFunc = std::function<void()>;
class Threadpool{
public:
	Threadpool(size_t threadNum, size_t queSize)
	: _isExit(false)
	, _threadNum(threadNum)
	, _queSize(queSize)
	, _taskQue(_queSize)
	{
		_threads.reserve(_threadNum);
	}
	~Threadpool(){if(!_isExit)stop();}
	void start();
	void stop();
	void addTask(CallBackFunc&&);
private:
	void threadFunc();
	CallBackFunc getTask();
private:
	bool _isExit;
	size_t _threadNum;
	size_t _queSize;
	vector<unique_ptr<rxl::Thread>> _threads;
	TaskQueue _taskQue;
};
}//end of namespace
#endif
