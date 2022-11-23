 ///
 /// @file    TimerThread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 16:26:33
 ///
 
#ifndef __WD_TIMERTHREAD_HPP__
#define __WD_TIMERTHREAD_HPP__

#include "Timerfd.hpp"
#include "Thread.hpp"

namespace wd
{

class TimerThread
{
public:
	TimerThread(int initTime,int periodicTime, TimerCallback && cb)
	: _timer(initTime, periodicTime, std::move(cb))
	, _thread(std::bind(&Timerfd::start, &_timer))
	{}

	void start() {
		_thread.start();
	}

	void stop() {
		_timer.stop();
		_thread.join();
	}

private:
	Timerfd _timer;
	Thread _thread;
};

}//end of namespace wd
 
 
 
#endif
