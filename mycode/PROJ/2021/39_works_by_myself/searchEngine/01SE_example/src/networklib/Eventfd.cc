#include "../../libs/Mylog.hh"
#include "../../include/networklib/Eventfd.hh"

#include <sys/eventfd.h>
#include <poll.h>
#include <unistd.h>

namespace wd
{

Eventfd::Eventfd(EventfdCallback && cb)
    : _fd(eventfd(0, 0))
    , _cb(std::move(cb))
    , _isStarted(false)
{}

Eventfd::~Eventfd()
{
    if(_isStarted) {
        stop();
    }
}

void Eventfd::start()
{
    if(!_isStarted) {
        _isStarted = true;
    }

    struct pollfd pfd;
    pfd.fd = _fd;
    pfd.events = POLLIN;

    int nready;
    while(_isStarted) {
        do {
            nready = ::poll(&pfd, 1, 5000);
        } while(nready == -1 && nready == EINTR);

        if(nready == -1) {
            logError("poll");
            return;
        } else if(nready == 0) {
            printf(">> poll timeout\n");
        } else {
            if(pfd.revents & POLLIN) {
                handleRead();
                if(_cb) {
                    _cb();
                }
            }
        }
    }
}

void Eventfd::stop()
{
    if(_isStarted) {
        _isStarted = false;
    }
}

void Eventfd::wakeup()
{
    uint64_t one = 1;
    int ret = ::write(_fd, &one, sizeof(one));
    if(ret != sizeof(one)) {
        logError("wakeup");
    }
}

int Eventfd::createEventfd()
{
    int ret = ::eventfd(0, 0);
    if(ret == -1) {
        logError("createEventfd");
    }
    return ret;
}

void Eventfd::handleRead()
{
    uint64_t timeout;
    int ret = ::read(_fd, &timeout, sizeof(timeout));
    if(ret != sizeof(timeout)) {
        logError("read");
    }
}

}// end of namespace wd
