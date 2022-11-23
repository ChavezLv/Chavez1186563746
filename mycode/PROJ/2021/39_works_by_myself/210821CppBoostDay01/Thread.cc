/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：Thread.cc
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月23日 20:09:50
*   @description：
*
================================================================*/


#include "Thread.hh"
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

Thread::Thread(ThreadCallback &&cb)
: _pthreadID(0)
, _cb(std::move(cb))//防止引用折叠的情况, 虽然这里影响不大
/* , _cb(cb) */
, _isRunning(false){

}

void *Thread::threadFunc(void *args){
    Thread *pthread = static_cast<Thread*>(args);
    if(pthread){
        pthread->_cb();
    }
    return nullptr;

}

void Thread::join(){
    if(_isRunning){
        if(0 != pthread_join(_pthreadID, nullptr)){
            perror("pthread_join");
        }
        _isRunning = false;
    }
}
void Thread::start(){
    if(0 != pthread_create(&_pthreadID, nullptr, threadFunc, this)){
        perror("pthread_create");
    }
    _isRunning = true;
}

Thread:: ~Thread(){
    if(_isRunning){
        pthread_detach(_pthreadID);
        _isRunning = false;
    }    
}


}//end of namespace wd
