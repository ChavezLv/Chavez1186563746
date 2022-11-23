#include"thread.h"

Thread::Thread(Entrance &&cb)
    :_cb(cb),_isStart(false){}
    
Thread::~Thread(){
    pthread_detach(_thrid);
    _isStart=false;
}

void Thread::start(){
    int ret=pthread_create(&_thrid,nullptr,Thread::EntranceFunc,this);
    if(ret<0){
        perror("pthread_create()");
        return;
    }
    _isStart=true;
}
void Thread::stop(){
    if(_isStart){
        pthread_join(_thrid,NULL);
        _isStart=false;
    }
}
void* Thread::EntranceFunc(void *arg){
    Thread* thr=(Thread*)(arg);
    if(thr){
        //忘在函数定以前加thread::作用域
        thr->_cb();
    }
    pthread_exit(nullptr);
}
