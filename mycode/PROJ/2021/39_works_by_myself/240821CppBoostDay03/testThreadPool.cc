/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：testThreadPool.cc
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月24日 22:39:34
*   @description：
*
================================================================*/


#include "ThreadPool.hh"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;
using namespace wd;

class MyTask 
: public Task{
public:
    void process(){
       srand(time(nullptr));
       int number = rand() % 100;
       cout << ">> get a number : " << number << endl;
       /* sleep(1); */

    }
};
void test01(){
    ThreadPool threadPool(4,  20);
    threadPool.start();

    unique_ptr<Task> task(new MyTask());
    
    int cnt = 10; 
    while(cnt--){
        threadPool.addTask(task.get());
        /* cout << "cnt : " << cnt << endl; */
    }

    threadPool.stop();
    /* sleep(1); */

}
int main(int argc, char* argv[])
{
   
       test01();
          return 0;
}
