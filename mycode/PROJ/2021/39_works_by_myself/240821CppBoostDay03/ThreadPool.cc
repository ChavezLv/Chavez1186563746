/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：ThreadPool.cc
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月24日 21:44:05
*   @description：
*
================================================================*/


#include "ThreadPool.hh"
#include "WorkerThread.hh"

#include <unistd.h>

#include <algorithm>

namespace wd
{

ThreadPool::ThreadPool(size_t threadNum, size_t capacity)
: _threadNum(threadNum)
, _capacity(capacity)
, _threads()
, _taskQueue()
, _isExit(false){
    _threads.reserve(_threadNum);
}

ThreadPool::~ThreadPool(){
    if(!_isExit){
        stop();
    }
}

void ThreadPool::start(){
    //创建n个Thread对象放入池子数组中
    /* WorkerThread p(*this); */
    for(size_t startIndex = 0; startIndex < _threadNum; ++ startIndex){
        unique_ptr< Thread > up(new WorkerThread(*this)); /* _threads.push_back(up); */
        _threads.push_back(std::move(up));
    }

    //start
    for_each(_threads.begin(), _threads.end(), [](auto &thread){
                 thread->start();
             });
}

void ThreadPool::stop(){
    if(!_isExit){
        while(!_taskQueue.empty()){//只有当所有的任务都执行完毕时，才让其退出
    
            /* sleep(1);//队列还有任务, 先睡一会儿 */
        }
        
        _isExit = true;
         
        _taskQueue.notify();// 全部唤醒
        cout << "void ThreadPool::stop(){}" << endl;
    }

    for_each(_threads.begin(), _threads.end(), [](auto &thread){
                thread->join();
             });

}

void ThreadPool::addTask(Task * pTask){
    if(pTask){
        cout << "void addTask(Task * pTask): " << endl;;
        _taskQueue.push(pTask);
    }
}

//每个工作线程需要调用的函数
void ThreadPool::workerDoTask(){
    while(! _isExit){
        Task *pTask = getTask();
        if(pTask){
            cout << "void workerDoTask():" ;
            pTask->process();
        }
    }
}

Task* ThreadPool::getTask(){
    return _taskQueue.pop();
}

}// end of namespace wd
