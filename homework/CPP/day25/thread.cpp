#include"thread.hpp"
#include<stdio.h>
void Thread::start(){
    int ret=pthread_create(&_thid,nullptr,threadFunc,this);
    if(ret){
        perror("pthread_create");
        return;
    }
}
Thread::Thread()
    :_thid(0),_isRunning(false){}

Thread::~Thread(){
    if(_isRunning){
        pthread_detach(_thid);
    }
}

void Thread::stop(){
    if(_isRunning){
        int ret=pthread_join(_thid,nullptr);
        if(ret){
            perror("pthread_join");
            return;
        }
        _isRunning=false;
    }
}

/*注意这里的参数传递*/
void *Thread::threadFunc(void*arg){
    Thread *ph=static_cast<Thread*>(arg);
    if(ph){
        ph->run();
    }
    pthread_exit(nullptr);
}
