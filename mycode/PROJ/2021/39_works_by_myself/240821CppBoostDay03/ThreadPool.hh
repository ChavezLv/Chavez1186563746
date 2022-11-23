/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：ThreadPool.hh
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月24日 21:32:16
*   @description：
*
================================================================*/


#ifndef __WD_THREADPOOL_HPP__
#define __WD_THREADPOOL_HPP__

#include "Thread.hh"
#include "TaskQueue.hh"
#include "Task.hh"

#include <vector>
#include <memory>
using std::vector;
using std::unique_ptr;

namespace wd
{
class Task;//前向声明
/* class WorkerThread;// friend  WorkerThread; */

class ThreadPool
: public Noncopyable{
public:
    friend class WorkerThread;

    ThreadPool(size_t, size_t);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task *);
private:
    void workerDoTask();
    Task* getTask();

private:
    size_t _threadNum;
    size_t _capacity;
    vector< unique_ptr<Thread> > _threads;
    TaskQueue _taskQueue;
    bool _isExit;//表示是否可以退出
};


}//end of namespace wd

#endif //THREADPOOL_HH
