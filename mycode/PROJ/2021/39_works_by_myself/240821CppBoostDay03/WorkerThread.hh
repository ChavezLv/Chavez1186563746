/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：WorkerThread.hh
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月24日 22:03:48
*   @description：
*
================================================================*/


#ifndef __WD_WORKERTHREAD_HPP__
#define __WD_WORKERTHREAD_HPP__

#include "Thread.hh"
#include "ThreadPool.hh"

namespace wd
{


class WorkerThread
: public Thread{
public:
    WorkerThread(ThreadPool& threadPool)
    : _threadPool(threadPool){

    }

private:
    void run(){
        /* _threadPool.workerDoTask(); */
    }

private:
    ThreadPool & _threadPool;
    /* ThreadPool * _threadPool; */
};



}//end of namespace wd


#endif //WORKERTHREAD_HH
