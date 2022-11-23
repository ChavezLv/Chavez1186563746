/* <Eventfd> -*- C++ -*- */
#ifndef __EVENTFD_H__
#define __EVENTFD_H__

#include <functional>

namespace wd
{

class Eventfd
{
public:
    using EventfdCallback = std::function<void()>;

    Eventfd(EventfdCallback && cb);
    ~Eventfd();

    void start();
    void stop();
    void wakeup();

private:
    int createEventfd();
    void handleRead();

private:
    int _fd;
    EventfdCallback _cb;
    bool _isStarted;
};

}// end of namespace wd
#endif

