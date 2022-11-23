/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：Producer.hh
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月23日 22:29:18
*   @description：
*
================================================================*/


#ifndef __WD_PRODUCER_HPP__
#define __WD_PRODUCER_HPP__ 

/* #include "Thread.hh" */
#include "TaskQueue.hh"
#include <iostream>


namespace wd
{

class Producer
/* : public Thread */
{
public:
    /* void run(){ */
    void produce(TaskQueue &taskQue, int cnt = 20){
        srand(time(nullptr));
        int number = 0;
        while(cnt--){
            number = rand() % 100;
            cout << ">> produce a number : " << number << endl;
            taskQue.push(number);
            
        }
    }
private:
    /* TaskQueue& _taskQue;//关联 , 换成依赖*/
    
};

}//end of namespace wd



#endif //PRODUCER_HH
