#ifndef __mythread_H__
#define __mythread_H__

#include"thread.hpp"
#include"threadpool.hpp"
#include<iostream>
class Mythread
:public Thread
{
    public:
        Mythread(ThreadPool &pool):_pool(pool){}
        void run()override{
            std::cout<<"Thread is running"<<std::endl;
            _pool.threadFunc();
        }
    private:
        ThreadPool &_pool;
};


#endif
