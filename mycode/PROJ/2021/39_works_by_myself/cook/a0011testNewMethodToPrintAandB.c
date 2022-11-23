/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0011testNewMethodToPrintAandB.c
*   @author     ：anonymous
*   @date       ：2021年09月13日 00:40:39
*   @description：
*
================================================================*/


#include <iostream>
#include <condition_variable>
#include <thread> 
#include <mutex>

using namespace std;

mutex mtx;
condition_variable cond_var;
int g_index;

typedef struct {
    const char *str;
    int index;
} Param;

void fun(void *arg)
{
    Param *param = (Param *)arg;
    while (1) {
        this_thread::sleep_for(chrono::seconds(1));
        unique_lock<mutex> lck(mtx);
        int index = param->index;

        cond_var.wait(lck, [index]{
                      return index == g_index;
                      });

        cout << param->str << endl;

        g_index = (index + 1) % 3;
        cond_var.notify_all();
    }
}

int main()
{
    Param param0 = { "thread 0", 0 },
        param1 = { "thread 1", 1 },
        param2 = { "thread 2", 2 };
    thread t0(fun, &param0),
        t1(fun, &param1),
        t2(fun, &param2);
    t0.join();
    t1.join();
    t2.join();

    return 0;
}
