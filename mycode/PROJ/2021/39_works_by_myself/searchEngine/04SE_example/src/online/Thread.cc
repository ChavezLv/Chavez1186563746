#include "../../include/Thread.hpp"

#include <pthread.h>

namespace spellCorrect
{
namespace current_thread
{
__thread const char* name = "default thread";
}

Thread::Thread(ThreadCallback && cb, const string& name)
    :_pthid(0)
     ,_name(name)
     ,_isRunning(false)
     ,_cb(cb)
{}

Thread::~Thread()
{
    if(_isRunning)
    {
        pthread_detach(_pthid);
    }
}

struct ThreadData//意义在于threadFunc只有arg一个参数
{
public:
    ThreadData(const ThreadCallback & cb, const string& name)
        :_name(name)
         ,_cb(cb)
    {}

    void runInThread()
    {
        current_thread::name = (_name == string() ? "default thread": _name.c_str());
        if(_cb)
        {
            _cb();
        }
    }
private:
    string _name;
    ThreadCallback _cb;
};

void Thread::start()
{
    if(!_isRunning)
    {
        ThreadData* pdata = new ThreadData(_cb,_name);
        if(::pthread_create(&_pthid,nullptr,threadFunc,pdata))
        {
            perror("pthread_create");
            return;
        }
        _isRunning = true;
    }
}

void Thread::join()
{
    if(_isRunning)
    {
        ::pthread_join(_pthid,nullptr);
        _isRunning = false;
    }
}

//static
void* Thread::threadFunc(void* arg)
{
    struct ThreadData* pdata = static_cast<struct ThreadData*>(arg);
    if(pdata)
    {
        pdata->runInThread();
    }
    delete pdata;
    return nullptr;
}

bool Thread::isRunning() const
{
    return _isRunning;
}

string Thread::getName() const
{
    return _name;
}

}//end of namespace spellCorrect
