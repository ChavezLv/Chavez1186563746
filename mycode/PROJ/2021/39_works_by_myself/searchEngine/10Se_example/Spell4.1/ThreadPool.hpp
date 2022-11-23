/* BO，基于对象实现线程池。 */
/* 使用BO比较灵活，不用继承也可实现多态 */
/* 怎么让线程工作起来呢？ */
/* 1.线程池只是存储线程的池子，里面start方法创建线程类，并将指定工作函数绑定到线程类的回调函数_cb， */
/* 2.然后调用线程类的start方法，这个方法实际上创建一个线程，它的线程函数内部调用回调函数。 */
/* 3.指定的工作函数就是不断从任务队列中取出任务并处理 */
/* 任务队列中是什么呢？ */
/* 可以在main中创建任务类的成员函数，再调用addQueue添加到 任务队列 */



#ifndef __WD__THREADPOOL_H__
#define __WD__THREADPOOL_H__
#include "Thread.hpp"
#include "TaskQueue.hpp"
#include<memory>
#include<vector>
using std::vector;
using std::unique_ptr;
namespace wd{

using Task=function<void()>;
class ThreadPool
{
    friend class WorkThread;
public:
    ThreadPool(size_t threadNums,size_t taskNums)
    :_threadNums(threadNums)
    ,_taskqueue(taskNums)
    ,_taskNums(taskNums)
    ,_exitFlags(false)
    {
        _threads.reserve(threadNums);
        cout<<"ThreadPool()"<<endl;
    }
    void addTask(Task&&);
    Task getTask();
    void start();
    void stop();
    ~ThreadPool() {
        if(!_exitFlags){
            stop();
        }
        cout<<"~ThreadPool()"<<endl;
    }
private:
    void _threadFunc();
private:
    vector<unique_ptr<Thread>>_threads;
    size_t _threadNums;
    TaskQueue _taskqueue;
    size_t _taskNums;
    bool _exitFlags;
};


}//end of namespace wd
#endif
