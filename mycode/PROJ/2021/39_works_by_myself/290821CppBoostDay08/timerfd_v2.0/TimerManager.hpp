 ///
 /// @file    Timerfd.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:02:49
 ///
 
#ifndef __WD_TIMERFD_HPP__
#define __WD_TIMERFD_HPP__

#include <functional>
#include <vector>


using std::function;

namespace wd
{

class TimerTask{
public:
    TimerTask(int secs)
    : _secs(secs){

    }
    virtual void process() = 0;
private:
    int _secs;//周期性的执行时间
};

/* using TimerCallback = function<void()>; // 升级版不需要基于对象的写法了*/
class TimerManager
{
public:
    TimerManager();



	void start();
	void stop();

    void attach(TimerTask*);
    void detach(TimerTask*);

private:
	int createTimerfd();
	void setTimerfd(int initTime, int periodicTime);
	void handleRead();
    void handleTask();

private:
	int _timerfd;
	bool _isStarted;
    std::vector<TimerTask*> _taskList;

};

}//end of namespace 
 
 
 
#endif
