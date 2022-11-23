/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：Consumer.hh
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月23日 22:40:23
*   @description：
*
================================================================*/


#ifndef __WD_CONSUMER_HPP__
#define __WD_CONSUMER_HPP__

#include "TaskQueue.hh"
#include "unistd.h"

namespace wd
{

class Consumer
/* : public Thread */
{
public:
    /* void run(){ */
    void consume(TaskQueue &taskQue, int cnt = 20){
        srand(time(nullptr));
        int number = 0;
        while(cnt--){
            number = taskQue.pop();
            cout << ">> consume a number : " << number << endl;

            usleep(100);
            
        }
    }
private:
    /* TaskQueue& _taskQue;//关联 , 换成依赖*/

};

}// end of namespace wd


#endif //CONSUMER_HH
