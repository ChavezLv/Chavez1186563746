 ///
 /// @file    Timerfd.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:02:49
 ///
 
#ifndef __WD_TIMERFD_HPP__
#define __WD_TIMERFD_HPP__

#include <functional>

using std::function;

namespace wd
{

using TimerCallback = function<void()>;
class Timerfd
{
public:
	Timerfd(int initTime, int periodicTime, TimerCallback && cb);
	~Timerfd();

	void start();
	void stop();
private:
	int createTimerfd();
	void setTimerfd(int initTime, int periodicTime);
	void handleRead();

private:
	int _timerfd;
	int _initTime;
	int _periodicTime;
	TimerCallback _cb;
	bool _isStarted;
};

}//end of namespace 
 
 
 
#endif
