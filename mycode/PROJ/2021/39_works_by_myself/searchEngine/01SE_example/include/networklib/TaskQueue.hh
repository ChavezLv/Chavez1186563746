/* <TaskQueue> -*- C++ -*- */
#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__

#include "Noncopyable.hh"
#include "MutexLock.hh"
#include "Condition.hh"

#include <queue>
#include <functional>

namespace wd
{

typedef std::function<void()> Task;

class TaskQueue : Noncopyable
{
public:
    TaskQueue(size_t size = 10)
        :_queSize(size),
        _mutex(),
        _cond(_mutex),
        _getExitSig(false)
    {}

    bool empty();
    bool full();
    void push(Task data);
    Task pop();

    void exit();

private:
    std::queue<Task> _que;
    size_t _queSize;
    MutexLock _mutex;
    Condition _cond;
    bool _getExitSig;
};

}// end of namespace wd
#endif

