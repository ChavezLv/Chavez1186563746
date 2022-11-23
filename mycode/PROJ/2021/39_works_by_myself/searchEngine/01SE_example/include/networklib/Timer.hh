/* <Timer> -*- C++ -*- */
#ifndef __TIMER_H__
#define __TIMER_H__

#include <functional>

namespace wd
{

class Timer
{
public:
    using TimerCallback = std::function<void()>;

    Timer(TimerCallback && cb, int initSec, int periodSec);
    ~Timer();

    void start();
    void stop();

private:
    int createTimerfd();
    void setTimerFd(int initSec, int periodSec);
    void handleRead();

private:
    int _fd;
    int _initialTime;
    int _periodicTime;
    TimerCallback _cb;
    bool _isStarted;
};

}// end of namespace wd
#endif

