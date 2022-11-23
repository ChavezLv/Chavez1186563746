#ifndef __THREADTIMER_H__
#define __THREADTIMER_H__
#include "Thread.hpp"
#include "Timer.hpp"
#include <functional>

using std::function;
using wd::Thread;
class ThreadTimer
{
public:
    using TimerCallBack=function<void()>;
    /* ThreadTimer()=default; */
    ThreadTimer(TimerCallBack&&cb,int init,int inter)
    :_timer(std::move(cb),init,inter)
    ,_thread(std::bind(&Timer::start,&_timer))//bind的时候不要忘记&
    {

    }
    void start(){
        _thread.start();
    }
    void stop(){
        _timer.stop();
        _thread.join();
    }
    ~ThreadTimer() {}
private:
    Timer _timer;
    Thread _thread;
};
#endif

