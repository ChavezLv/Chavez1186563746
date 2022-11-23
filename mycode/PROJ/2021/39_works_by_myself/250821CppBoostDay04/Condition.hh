#ifndef __WD_CONDITION_HH__
#define __WD_CONDITION_HH__

#include "MutexLock.hh"
#include <pthread.h>

namespace wd{


class Condition{
public:
    //...
    explicit
    Condition(MutexLock &mutex)
    : _mutex(mutex){
        if(0 != pthread_cond_init(&_cond, nullptr)){
            perror("pthread_cond_init");
        }
    }

    Condition(const Condition &) = delete;
    Condition & operator=(const Condition &) = delete;

    ~Condition(){
        if(0 != pthread_cond_destroy(&_cond)){
            perror("pthread_cond_destroy");
        }
    }

    void wait(){
        if(0 != pthread_cond_wait(&_cond, &_mutex.getMutex())){
            perror("pthread_cond_wait");
        }
    }
    void notify(){
        if(0 != pthread_cond_signal(&_cond)){
            perror("pthread_cond_signal");
        }
    }
    void notifyAll(){
        if(0 != pthread_cond_broadcast(&_cond)){
            perror("pthread_cond_broadcast");
        }
    }
private:
    MutexLock & _mutex;
    pthread_cond_t _cond;
};

}//end of namespace wd

#endif
