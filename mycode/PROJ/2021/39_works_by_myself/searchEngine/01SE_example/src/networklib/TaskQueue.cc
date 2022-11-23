#include "../../include/networklib/TaskQueue.hh"
#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

bool TaskQueue::empty() { return _que.empty(); }

bool TaskQueue::full() { return _queSize == _que.size(); }

void TaskQueue::push(Task data)
{
    // 自动关闭的锁
    MutexGuard lock(_mutex);
    // 为防止异常唤醒，使用while
    while(full()) {
        _cond.wait();
    }

    _que.push(data);

    // 通知可能停止在pop中的消费者线程
    _cond.notify();
}

Task TaskQueue::pop()
{
    MutexGuard lock(_mutex);
    while(!_getExitSig && empty()) {
        _cond.wait();
    }

    if(_getExitSig) {
        return nullptr;
    } else {
        Task data = _que.front();
        _que.pop();

        _cond.notify();
        return data;
    }
}

void TaskQueue::exit()
{
    if(!_getExitSig) {
        _getExitSig = true;
    }
    _cond.notifyAll();
}

}// end of namespace wd
