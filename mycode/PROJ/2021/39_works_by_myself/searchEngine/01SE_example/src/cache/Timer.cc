#include "../../libs/Mylog.hh"
#include "../../include/cache/Timer.hh"

#include <errno.h>
#include <sys/timerfd.h>
#include <poll.h>
#include <unistd.h>

namespace wd
{

Timer::Timer(TimerCallback && cb, int initSec, int periodSec)
    : _fd(createTimerfd())
    , _initialTime(initSec)
    , _periodicTime(periodSec)
    , _cb(std::move(cb))
    , _isStarted(false)
{}

Timer::~Timer()
{
    if(_isStarted) {
        stop();
    }
}

void Timer::start()
{
    if(!_isStarted) {
        _isStarted = true;
    }

    struct pollfd pfd;
    pfd.fd = _fd;
    pfd.events = POLLIN;
    setTimerFd(_initialTime, _periodicTime);

    int nready;
    while(_isStarted) {
        do {
            nready = ::poll(&pfd, 1, 5000);
        } while(nready == -1 && nready == EINTR);

        if(nready == -1) {
            exit(1);
        } else if(nready == 0) {
            printf(">> Timer poll timeout\n");
        } else {
            if(pfd.revents & POLLIN) {
                handleRead();
                if(_cb) {
                    logDebug("start Timer _cb: ", &_cb);
                    _cb();
                }
            }
        }
    }
}

void Timer::stop()
{
    setTimerFd(0, 0);
    if(_isStarted) {
        _isStarted = false;
    }
}


int Timer::createTimerfd()
{
    int ret = ::timerfd_create(CLOCK_REALTIME, 0);
    if(ret == -1) {
        perror("createTimerfd");
    }
    return ret;
}

void Timer::setTimerFd(int initSec, int periodSec)
{
    struct itimerspec itimer;
    itimer.it_value.tv_sec = initSec;
    itimer.it_value.tv_nsec = 0;
    itimer.it_interval.tv_sec = periodSec;
    itimer.it_interval.tv_nsec = 0;
    int ret = ::timerfd_settime(_fd, 0, &itimer, nullptr);
    if(ret == -1) {
        perror("setTimerFd");
    }
}

void Timer::handleRead()
{
    uint64_t timeout;
    int ret = ::read(_fd, &timeout, sizeof(timeout));
    if(ret != sizeof(timeout)) {
        perror("read");
    }
}

}// end of namespace wd
