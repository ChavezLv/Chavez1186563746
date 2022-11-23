#ifndef __WD_THREADPOOL_HPP__
#define __WD_THREADPOOL_HPP__

#include "Thread.hpp"
#include "TaskQueue.hpp"
#include "TimerThread.hpp"
#include "CacheManger.hpp"

#include <vector>
#include <memory>

namespace wd
{

using Task= std::function<void()>;
class Threadpool
{
public:
    Threadpool(size_t threadNum, size_t queSize);
    ~Threadpool();

    void addTask(Task &&);
    void start();
    void stop();
private:
    Task getTask();
    void threadFunc();

private:
    size_t _threadNum;
    size_t _queSize;
    std::vector<std::unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;

    TimerThread _timerThread;
};

}//end of namespace wd

#endif
