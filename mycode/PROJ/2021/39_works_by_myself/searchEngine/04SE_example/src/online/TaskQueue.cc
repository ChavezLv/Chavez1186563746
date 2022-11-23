#include "../../include/TaskQueue.hpp"

namespace spellCorrect
{
TaskQueue::TaskQueue(size_t queSize)
    :_queSize(queSize)
     ,_mutex()
     ,_notEmpty(_mutex)
     ,_notFull(_mutex)
     ,_exit(false)
    {}

bool TaskQueue::empty() const
{
    return 0 == _que.size();
}

bool TaskQueue::full() const
{
    return _queSize == _que.size();
}

size_t TaskQueue::size() const
{
    return _que.size();
}

void TaskQueue::push(Task task)
{
    MutexLockGuard autoLock(_mutex);
    while(full())
    {
        _notFull.wait();
    }
    _que.push(task);
    _notEmpty.notify();
}

Task TaskQueue::pop()
{
    MutexLockGuard autoLock(_mutex);
    while(!_exit && empty())
    {
        _notEmpty.wait();
    }
    if(!_exit)
    {
        Task temp = _que.front();
        _que.pop();
        _notFull.notify();
        return temp;
    }
    else
    {
        return nullptr;
    }
}

void TaskQueue::wakeup()
{
    _exit = true;
    _notEmpty.notifyAll();
}

}//end of namespace spellCorrect

#if 0
#include <unistd.h>
#include <iostream>

using namespace spellCorrect;


void print()
{
    std::cout << "i'm task" << std::endl;
}

void test()
{
    TaskQueue que(3);
    que.push(print);
    que.push(print);
    que.push(print);
    auto task = que.pop();
    task();
    task = que.pop();
    task();
    task = que.pop();
    task();
    que.wakeup();
}

int main()
{
    test();
    return 0;
}


#endif
