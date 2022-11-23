/* <TimerThread> -*- C++ -*- */
#ifndef __TIMERTHREAD_H__
#define __TIMERTHREAD_H__

#include "../networklib/Thread.hh"
#include "Timer.hh"

namespace wd
{

class TimerThread
{
    using TimerCallback = std::function<void()>;
public:
    TimerThread(TimerCallback && cb, int initialTimer, int periodicTimer)
        : _timer(std::move(cb), initialTimer, periodicTimer)
        , _thread(std::bind(&Timer::start, &_timer))
    {}

    void start() { _thread.start(); }
    void stop()
    {
        _timer.stop();
        _thread.join();
    }

private:
    Timer _timer;
    Thread _thread;
};

}// end of namespace wd
#endif

