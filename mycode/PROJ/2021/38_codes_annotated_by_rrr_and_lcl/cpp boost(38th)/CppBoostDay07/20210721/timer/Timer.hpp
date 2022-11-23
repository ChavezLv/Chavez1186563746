 ///
 /// @file    Timer.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-21 10:02:37
 ///
 
#ifndef __WD_TIMER_HPP__
#define __WD_TIMER_HPP__

#include <functional>


namespace wd
{

using TimerCallback = std::function<void()>;

class Timer
{
public:
	Timer(int, int, TimerCallback && cb);
	~Timer();

	void start();
	void stop();

private:
	int createTimerfd();
	void setTimerfd(int,int);

	void handleRead();//定时器只需要处理读时间就行了, wakeup是内核帮我们做的

private:
	int _timerfd;
	int _initSec;
	int _periodicSec;
	TimerCallback _cb;
	bool _isStarted;//是否开始进行监听

};

}//end of namespace wd
 
 
 
#endif
