#include "../../libs/Mylog.hh"
#include "../../include/networklib/Condition.hh"
#include "../../include/networklib/MutexLock.hh"

#include <iostream>


using std::cout;
using std::endl;

namespace wd
{

Condition::Condition(MutexLock & mutex)
    :_mutex(mutex)
{
    if(0 != pthread_cond_init(&_cond, NULL)) {
        logError("pthread_cond_init");
    }
}

Condition::~Condition()
{
    if(0 != pthread_cond_destroy(&_cond)) {
        logError("pthread_cond_destroy");
    }
}

void Condition::wait()
{
    if(0 != pthread_cond_wait(&_cond, _mutex.getMutexPtr())) {
        logError("pthread_cond_wait");
    }
}

void Condition::notify()
{
    if(0 != pthread_cond_signal(&_cond)) {
        logError("pthread_cond_signal");
    }
}

void Condition::notifyAll()
{
    if(0 != pthread_cond_broadcast(&_cond)) {
        logError("pthread_cond_broadcast");
    }
}

}// end of namespace wd

