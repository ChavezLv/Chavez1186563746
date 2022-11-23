/* <Threadpool> -*- C++ -*- */
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

#include "TaskQueue.hh"
#include "Thread.hh"

#include <memory>
#include <vector>
#include <functional>

namespace wd
{

class Threadpool
{
    friend class WorkerThread;
public:
    typedef std::function<void()> Task; 

    Threadpool(size_t threadNum, size_t queSize)
        :_threadNum(threadNum),
        _queSize(queSize),
        _taskQue(_queSize),
        _isExit(false)
    {
        _threads.reserve(_threadNum);
    }

    ~Threadpool();

    // 将工作线程添加到vector中
    void start();
    void stop();

    // 将任务添加到任务队列中
    void addTask(Task &&);

private:
    // 供threadFunc调用 用来获取任务
    Task getTask();
    // 获取任务并执行 在工作线程中被使用 因此需要将WorkThread设置为友元
    void threadFunc();

    size_t _threadNum;
    size_t _queSize;
    std::vector<std::unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    // 避免冲突
    bool _isExit;
};

}// end of namespace wd
#endif

