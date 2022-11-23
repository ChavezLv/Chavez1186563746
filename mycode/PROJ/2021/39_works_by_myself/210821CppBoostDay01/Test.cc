/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：Test.cc
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月23日 22:45:19
*   @description：
*
================================================================*/

#include "Thread.hh"
#include "TaskQueue.hh"
#include "Producer.hh"
#include "Consumer.hh"

#include <iostream>
using std::cout;
using std::endl;

#include <memory>
using std::unique_ptr;
#include <functional>
using std::bind;

using namespace wd;



template<typename _Tp>
class Solution {
public:
   
};
void test01(){
    TaskQueue taskQue(10);       
    Producer produder;
    Consumer consumer;
    // 由于TeakQueue的成员数据有不能复制控制的, 
    // 而且 使用bind提前绑定参数, 以适应线程的回调函数的形式,
    // bind只是值传递, 是不正确的. 
    // 因为TaskQueue &taskQuevoid produce(TaskQueue &taskQue, int cnt = 20)
    unique_ptr<Thread> produderThread(new \
        Thread(bind(&Producer::produce, &produder, std::ref(taskQue), 20))); 

    unique_ptr<Thread> consumerThread(new \
        Thread(bind(&Consumer::consume, &consumer, std::ref(taskQue), 20))); 

    produderThread->start();
    consumerThread->start();

    produderThread->join();
    consumerThread->join();

}
int main(int argc, char* argv[])
{
    test01();

    return 0;
}
