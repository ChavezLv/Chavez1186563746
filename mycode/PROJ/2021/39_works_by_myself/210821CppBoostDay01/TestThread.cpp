/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：TestThread.cc
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月23日 20:33:13
*   @description：
*
================================================================*/


#include "Thread.hh"

#include <memory>
#include <iostream>
#include <functional>
using std::unique_ptr;
using std::cout;
using std::endl;
using std::bind;

using wd::Thread;

class Mythread
/* : public Thread */
{
public:
    void run(int times){
        srand(time(nullptr));
        cout << times << endl;
        while(times--){
            cout << ">> random number: " << rand() % 100 << endl;
        }
    }    
};


void test01(){
    Mythread mythread;
    unique_ptr<Thread> thread(new Thread(bind(&Mythread::run, &mythread, 10))); 
    thread->start();
    thread->join();
}

int main(){
    test01();
    return 0;
}
