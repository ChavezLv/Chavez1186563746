#include "TimerFd.h"
#include <unistd.h>
#include <poll.h>
#include <sys/timerfd.h>
#include <iostream>

using std::cout;
using std::endl;

TimerFd::TimerFd(int initSec, int peridocSec, TimerFdCallback &&cb)
: _timerfd(createTimerFd())
, _initSec(initSec)
, _peridocSec(peridocSec)
, _cb(std::move(cb))
, _isStarted(false)
{

}

TimerFd::~TimerFd()
{
    setTimerFd(0, 0);//停止定时器
    close(_timerfd);
}

void TimerFd::start()
{
    struct pollfd pfd;
    pfd.events = POLLIN;
    pfd.fd = _timerfd;

    setTimerFd(_initSec, _peridocSec);//定时器的设置

    _isStarted = true;
    while(_isStarted)
    {
        int nready = poll(&pfd, 1, 5000);
        cout << "nready = " << nready << endl;
        if(-1 == nready && errno == EINTR)
        {
            continue;
        }
        else if(-1 == nready)
        {
            perror("poll error");
            return;
        }
        else if(0 == nready)
        {
            cout << ">>poll timeout" << endl;
        }
        else
        {
            if(pfd.revents & POLLIN)
            {

                cout<<"0000"<<endl;
                handleRead();//阻塞
                cout<<"1111"<<endl;
                if(_cb)
                {
                    _cb();//执行任务
                }
            }
        }
    }
}

void TimerFd::stop()
{
    _isStarted = false;
    setTimerFd(0, 0);//停止定时器
}

void TimerFd::handleRead()
{
    uint64_t one = 1;
    int ret = read(_timerfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("read");
        return;
    }
}

int TimerFd::createTimerFd()
{
    int ret = timerfd_create(CLOCK_REALTIME, 0);//绝对时间
    if(ret == -1)
    {
        perror("timerfd_create");
        return ret;
    }

    return ret;
}

void TimerFd::setTimerFd(int initSec, int peridocSec)
{
    struct itimerspec value;
    value.it_value.tv_sec = initSec;//起始时间
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = peridocSec;//周期时间
    value.it_interval.tv_nsec = 0;


    int ret = timerfd_settime(_timerfd, 0, &value, nullptr);
    if(ret < 0)
    {
        perror("timerfd_settime");
        return ;
    }
}
