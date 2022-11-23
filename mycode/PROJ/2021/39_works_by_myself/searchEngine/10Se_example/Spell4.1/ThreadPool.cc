#include "ThreadPool.hpp"
#include <unistd.h>
namespace wd{
Task ThreadPool::getTask(){
    /* if(!_taskqueue.empty()){ */
    return _taskqueue.pop();
    /* } */ 
    /* return nullptr; */
}
void ThreadPool::addTask(Task&& task){
    _taskqueue.push(task); 
}

void ThreadPool::start(){
    
    for(size_t i=0;i<_threadNums;i++){
        unique_ptr<Thread> newThread(new Thread
        (std::bind(&ThreadPool::_threadFunc,this)));
        _threads.push_back(std::move(newThread));
    }
    for(auto& p:_threads){
        p->start();
    }
}
void ThreadPool::stop(){
    if(!_exitFlags){
        //等待队列剩余任务结束
        while(!_taskqueue.empty()){
            usleep(100);
            /* sleep(2); */
        }
        _exitFlags=true;
       _taskqueue.wakeUpAll(); 
        
        for(size_t i=0;i<_threadNums;i++){
            _threads[i]->join(); 
        }
    }
} 

void ThreadPool::_threadFunc(){
    while(!_exitFlags){
        Task task=_taskqueue.pop();
        if(task){
            task();
        }
    }
}

}
