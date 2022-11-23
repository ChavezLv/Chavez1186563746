 ///
 /// @file    TimerThread.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-21 11:26:09
 ///
 
#ifndef __WD_TIMERTHREAD_HPP__
#define __WD_TIMERTHREAD_HPP__

#include "Timer.hpp"
#include "Thread.hpp"


namespace wd
{

class TimerThread
{
public:
	TimerThread(int initSec,int periodicSec, TimerCallback && cb)
	: _timer(initSec, periodicSec, std::move(cb))
	, _thread(std::bind(&Timer::start, &_timer))
	{}

	void start()
	{
		_thread.start();
	}

	void stop()
	{
		_timer.stop();
		_thread.join();//因为这两个对象都是可以自己回收自己的, 免写析构
	}

private:
	Timer _timer;
	Thread _thread;
};

}//end of namespace wd
 
 
 
#endif
