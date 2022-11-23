#ifndef __taksQueue_H__
#define __taksQueue_H__
#include"MutexLock.h"
#include"Condition.h"
#include"task.h"
#include<queue>
/*producer&&comsumer
 *TaskQueue:
operator:
    isEmpty,isFull,push,pop;
dataMember:
    _queue,queSize,_Mutex,Condition(_full,notempty);
 * */
using ElemType=Task*;
class TaskQueue{
public:
    TaskQueue(size_t queSize);
    ~TaskQueue();
    void push(ElemType value);
    ElemType pop();
    bool isEmpty();
    bool isFull();
    void wakeup();
private:
    size_t _queSize;
    std::queue<ElemType>_que;
    MutexLock _mutex;
    Condition _full;
    Condition _empty;
    bool _flag;
};


#endif
