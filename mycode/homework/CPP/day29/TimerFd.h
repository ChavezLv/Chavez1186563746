#ifndef __TIMERFD_H__
#define __TIMERFD_H__

#include <functional>

using std::function;

class TimerFd
{
    using TimerFdCallback = function<void()>;
public:
    TimerFd(int initSec, int peridocSec, TimerFdCallback &&cb);
    ~TimerFd();

    void start();
    void stop();

    void handleRead();
    int createTimerFd();
    void setTimerFd(int initSec, int peridocSec);

private:
    int _timerfd;
    int _initSec;
    int _peridocSec;
    TimerFdCallback _cb;
    bool _isStarted;

};

#endif
