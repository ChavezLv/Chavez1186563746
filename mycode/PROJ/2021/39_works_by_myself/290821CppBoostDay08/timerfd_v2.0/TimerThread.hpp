 ///
 /// @file    TimerThread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 16:26:33
 ///
 
#ifndef __WD_TIMERTHREAD_HPP__
#define __WD_TIMERTHREAD_HPP__

#include "TimerManager.hpp"
#include "Thread.hpp"


namespace wd
{

class TimerThread
{
    using TaskCallback = function<void ()>;
public:
	TimerThread(int initTime,int periodicTime,  TaskCallback && cb)
	: _timer()
	, _thread(std::bind(&TimerManager::start, &_timer))
	{}

	void start() {
		_thread.start();
	}

	void stop() {
		_timer.stop();
		_thread.join();
	}

private:
	TimerManager _timer;
	Thread _thread;
};

}//end of namespace wd
 
 
 
#endif
