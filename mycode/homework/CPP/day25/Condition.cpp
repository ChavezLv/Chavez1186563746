#include"Condition.h"
#include<stdio.h>
Condition::Condition(MutexLock& Mutex)
    :_Mutex(Mutex){
        int ret= pthread_cond_init(&_cond,nullptr);
    if(ret)
    {
        perror("pthread_cond_init");
    }
    }
Condition::~Condition(){
    int ret=pthread_cond_destroy(&_cond);
    if(ret)
    {
        perror("pthread_cond_destroy");
    }
} 
void Condition::wait(){
    int ret=pthread_cond_wait(&_cond,_Mutex.getMutexLockPtr());
    if(ret)
    {
        perror("pthread_cond_wait");
    }


}
void Condition::notify()
{
    //至少一个
    int ret = pthread_cond_signal(&_cond);
    if(ret)
    {
        perror("pthread_cond_signal");
        return;
    }

}
void Condition::notifyAll()
{
    int ret = pthread_cond_broadcast(&_cond);
    if(ret)
    {
        perror("pthread_cond_broadcast");
        return;
    }

}
