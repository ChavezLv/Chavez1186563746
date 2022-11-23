#ifndef __threadpool_H__
#define __threadpool_H__
#include"taskQueue.h"
#include"thread.hpp"
#include<vector>
#include<memory>
class ThreadPool{
public:
    ThreadPool(size_t threadPoolNum,size_t taskQueSize);
    ~ThreadPool();
    void start();
    void stop();
    void addTask(Task* ptask);
    Task *getTask();
    void threadFunc();
private:
    size_t _threadNum;
    size_t _taskQueSize;
    std::vector<std::unique_ptr<Thread>>_threadVec;
    TaskQueue _poolTaskQueue;
    bool _isExit;
};

#endif
