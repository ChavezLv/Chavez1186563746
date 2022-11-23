#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include "TaskQueue.hpp"

#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

namespace spellCorrect
{
class Thread;

class ThreadPool
{
public:
    ThreadPool(size_t threadNum,size_t queSize);
    ~ThreadPool();

    void start();
    void stop();
    void wakeup();

    void addTask(Task && task);

private:
    Task getTask();
    void threadFunc();
private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threadList;
    TaskQueue _taskQue;
    bool _isExit;
};


}//end of namespace spellCorrect

#endif
