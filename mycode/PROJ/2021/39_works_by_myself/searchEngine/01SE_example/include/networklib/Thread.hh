#ifndef __THREAD_H__
#define __THREAD_H__

#include "Noncopyable.hh"
#include <pthread.h>

#include <iostream>
#include <string>
#include <functional>

namespace wd
{

namespace current_thread
{
extern __thread const char * threadName;
}

using ThreadCallback = std::function<void()>;

class Thread : Noncopyable
{
public:
    Thread(ThreadCallback && cb, const std::string & name = std::string())
        : _pth(0),
        _isRuning(false),
        _cb(std::move(cb)),
        _name(name)
    {}

    ~Thread();

    void start();
    void join();

private:
    static void * threadFunc(void * arg);

private:
    pthread_t _pth;
    bool _isRuning;
    ThreadCallback _cb;
    std::string _name;
};

}// end of namespace wd
#endif

