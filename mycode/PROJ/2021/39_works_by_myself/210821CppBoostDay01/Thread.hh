/*================================================================
*   Copyright (C) 2021 WD39th-某某某 All rights reserved.
*   
*   @file       ：Thread.hh
*   @author     ：WD39th-某某某
*   @date       ：2021年08月23日 19:56:36
*   @description：
*
================================================================*/


#ifndef __WD_THREAD_HPP__
#define __WD_THREAD_HPP__ 

#include "Noncopyable.hh"
#include <pthread.h>
#include <functional>

namespace wd
{

class Thread
: Noncopyable{
    using ThreadCallback = std::function< void(void) >;

public:
    Thread(ThreadCallback &&cb);
    ~Thread();

    void start();
    void join();
private:
    /* virtual */
    /* void run() = 0;//子线程的任务 */

    static void *threadFunc(void *args);
private:
    pthread_t _pthreadID;
    ThreadCallback _cb;
    bool _isRunning;
};


}//end of namespace wd

#endif //THREAD_HH
