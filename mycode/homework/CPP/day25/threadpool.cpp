#include"threadpool.hpp"
#include"mythread.h"
#include<unistd.h>
ThreadPool::ThreadPool(size_t threadPoolNum,size_t taskQueSize)
        :_threadNum(threadPoolNum)
        ,_taskQueSize(taskQueSize)
        ,_threadVec()
        ,_poolTaskQueue(_taskQueSize)
        ,_isExit(false){}

ThreadPool::~ThreadPool(){
    if(!_isExit){
        stop();
    }
}

/*利用thread.h创建启动线程池,利用_threadVec存放*/
void ThreadPool::start(){
    for(size_t idex=0;idex!=_threadNum;++idex){
        //Mythread *th();
        std::unique_ptr<Thread>th(new Mythread(*this));
        _threadVec.push_back(std::move(th));
    }
    for(auto &thd:_threadVec){//不加引用报错,会拷贝复制和析构
        thd->start();
    }
}

/*终止所有线程*/
void ThreadPool::stop(){
    while(!_poolTaskQueue.isEmpty()){
        sleep(1);
    }
    _isExit=true;
    _poolTaskQueue.wakeup();
    for(auto &thd:_threadVec){
        thd->stop();
    }
}

/**/
void ThreadPool::addTask(Task* ptask){
    if(ptask)
        _poolTaskQueue.push(ptask);
}
Task* ThreadPool::getTask(){
    return _poolTaskQueue.pop();
}

void ThreadPool::threadFunc(){
    while(!_isExit){
       Task *ptask=getTask(); 
       if(ptask){
           ptask->process();
       }
    }    
}

