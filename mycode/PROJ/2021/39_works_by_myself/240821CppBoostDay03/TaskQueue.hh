/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：TaskQueue.hh
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月23日 21:44:00
*   @description：
*
================================================================*/


#ifndef __WD_TASKQUEUE_HPP__
#define __WD_TASKQUEUE_HPP__ 

#include "Condition.hh"
#include "Task.hh"

#include <math.h>
#include <queue>
using std::queue;

namespace wd
{

class TaskQueue{
    using ElemType = Task*;
public:
    TaskQueue(size_t capacity = 10);

    bool full() const;
    bool empty() const;
    /* void push(int num ); */
    void push(const ElemType &elem );
    ElemType pop();

    void notify(){
        _isWorking = false;
        _notEmpty.notifyAll();
        /* _notFull.notifyAll(); */
    }

private:
    queue<ElemType> _que;
    size_t _capacity;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;

    bool _isWorking;// 任务队列退出
};


}//end of namespace wd

#endif //TASKQUEUE_HH
