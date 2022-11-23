#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include "NonCopyable.hpp"
#include "MutexLock.hpp"

namespace spellCorrect
{

class Condition
    :NonCopyable
{
public:
    Condition(MutexLock& mutex);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();
private:
    MutexLock& _mutex;
    pthread_cond_t _cond;
};


}//end of namespace spellCorrect

#endif
