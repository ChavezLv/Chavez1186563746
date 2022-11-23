#ifndef __taskQueue_H__
#define __taskQueue_H__
#include<queue>
#include"pthread.h"
class TaskQueue{
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();
    bool empty()const;
    bool full()const;
    void push(const int &value);
    int pop();
private:
    size_t _queSize;
    std::queue<int>_que;
    pthread_mutex_t _mutex;
    Condition _notEmpty;
    Condition _notFull;
};






#endif
