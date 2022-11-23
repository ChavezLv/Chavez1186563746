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
#include <math.h>
#include <queue>
using std::queue;

namespace wd
{

class TaskQueue{
public:
    TaskQueue(size_t capacity = 10);

    bool full() const;
    bool empty() const;
    void push(int );
    int pop();

private:
    queue<int> _que;
    size_t _capacity;
    MutexLock _mutex;
    Condition _notFull;
    Condition _notEmpty;
};


}//end of namespace wd

#endif //TASKQUEUE_HH
