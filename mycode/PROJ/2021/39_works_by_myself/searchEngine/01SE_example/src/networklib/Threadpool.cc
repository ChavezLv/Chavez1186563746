#include "../../include/networklib/Threadpool.hh"

#include <string>
#include <unistd.h>

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

Threadpool::~Threadpool()
{
    if(!_isExit) {
        stop();
    }
}


void Threadpool::start()
{
    for(size_t idx = 0; idx != _threadNum; ++idx) {
        // 注册回调函数到线程中
        std::unique_ptr<Thread> uniq_pth(new Thread(
                std::bind(&Threadpool::threadFunc, this),
                std::to_string(idx)
                ));
        _threads.push_back(std::move(uniq_pth));
    }
    for(auto & pthread : _threads) {
        pthread->start();
    }
}

void Threadpool::stop()
{
    if(!_isExit)
    {
        while(!_taskQue.empty()) {
            ::sleep(1);
        }
        _isExit = true;
        // 唤醒和退出 线程可能会卡在获取任务的地方
        _taskQue.exit();

        // 回收
        for(auto & pthread : _threads) {
            pthread->join();
        }
    }
}


void Threadpool::addTask(Task && task)
{
    _taskQue.push(task);
}


Task Threadpool::getTask()
{
    return _taskQue.pop();
}

void Threadpool::threadFunc()
{
    while(!_isExit) {
        Task task = getTask();
        if(task) {
            task();
        }
    }
}


}// end of namespace wd
