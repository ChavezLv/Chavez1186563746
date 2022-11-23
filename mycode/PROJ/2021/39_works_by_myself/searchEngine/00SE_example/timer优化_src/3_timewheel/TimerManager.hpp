 ///
 /// @file    TimerManager.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-10-23 09:51:33
 ///
 
#ifndef __WD_TIMERMANAGER_HPP__
#define __WD_TIMERMANAGER_HPP__

#include "TimerTask.hpp"

#include <list>
#include <vector>

using namespace std;

namespace wd
{

class TimerManager
{
public:
	TimerManager();

	void start();
	void stop();

	void addTask(TimerTask * timerTask);
	void removeTask(TimerTask * timerTask);

private:
	int createTimerfd();
	void setTimerfd(int initialTime, int periodicTime);
	void handleRead();
	void handleTask();

private:
	int _fd;
	bool _isStarted;
	int _currentIndex;
	vector<list<TimerTask *>> _timeWheelList;

	static const int kSize = 10;
};

}//end of namespace wd

#endif
