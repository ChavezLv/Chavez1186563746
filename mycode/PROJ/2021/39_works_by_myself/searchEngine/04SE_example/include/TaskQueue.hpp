#ifndef __TASKQUEUE_HPP__
#define __TASKQUEUE_HPP__

#include "MutexLock.hpp"
#include "Condition.hpp"

#include <queue>
#include <functional>

using std::queue;
using std::function;

namespace spellCorrect
{
using Task = function<void()>;

class TaskQueue
{
public:
    TaskQueue(size_t queSize);

    bool empty() const;
    bool full() const;
    size_t size() const;
    void push(Task task);
    Task pop();

    void wakeup();
private:
    size_t _queSize;
    queue<Task> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _exit;//进程闲时阻塞在队列为空，即pop的notEmpty.wait()，因此退出时要跳出防止异常唤醒的while循环
};


}//end of spellCorrect
#endif
