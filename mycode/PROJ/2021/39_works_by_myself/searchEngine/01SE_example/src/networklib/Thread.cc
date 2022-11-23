#include "../../libs/Mylog.hh"
#include "../../include/networklib/Thread.hh"

#include <string>
using std::string;

namespace wd
{

namespace current_thread
{
__thread const char * threadName = "wd thread";
}

struct ThreadData
{
    string _name;
    ThreadCallback _cb;
    ThreadData(const string & name, const ThreadCallback & cb)
        : _name(name)
        , _cb(cb)
    {}

    void runInThread()
    {
        current_thread::threadName = (_name == string() ? "wd thread" : _name.c_str());
        if(_cb) {
            _cb();
        }
    }
};

Thread::~Thread()
{
    if(_isRuning) {
        pthread_detach(_pth);
    }
}

void Thread::start()
{
    ThreadData * pdata = new ThreadData(_name, _cb);
    if(0 != pthread_create(&_pth, nullptr, threadFunc, pdata)) {
        logError("pthread_create");
    }
    _isRuning = true;
}

void Thread::join()
{
    if(_isRuning) {
        if(0 != pthread_join(_pth, nullptr)) {
            logError("pthread_join");
        }
        _isRuning = false;
    }
}

void * Thread::threadFunc(void * arg)
{
    ThreadData * pdata = static_cast<ThreadData*>(arg);
    if(pdata) {
        pdata->runInThread();
    }
    return nullptr;
}

}// end of namespace wd

