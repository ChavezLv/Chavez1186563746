 ///
 /// @file    Timer.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-05-12 11:19:06
 ///
 

#ifndef __WD_TIMER_H__
#define __WD_TIMER_H__

#include <vector>
#include <functional>

namespace wd
{

class TimerTask
{
public:
	TimerTask(int secs)
	: _secs(secs)
	, _count(secs)
	{}

	virtual void process() = 0;

	void decrease() 
	{	
		--_count;
		if(_count < 0) {
			_count = 0;
		}
	}

	int getCount() {	return _count;	}

	void reset() {	_count = _secs;	}

private:
	int _secs;//周期性执行时间
	int _count;
};

class TimerManager
{
public:
	TimerManager();

	void start();
	void stop();
	void attach(TimerTask *);
	void detach(TimerTask *);
private:
	int createTimerfd();
	void setTimerfd(int initsec, int periodicsec);
	void handleRead();
	void handleTask();

private:
	int _timerfd;
	bool _isStarted;
	std::vector<TimerTask*> _taskList;
};

}//end of namespace wd


#endif
