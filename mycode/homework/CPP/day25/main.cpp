#include<iostream>
#include"threadpool.hpp"
#include"task.h"
using std::cout;
using std::endl;
int main(){
    std::unique_ptr<Task>ptask(new Mytask());
    ThreadPool pool(4,10);
    pool.start();
    int ctl=20;
    while(--ctl>0){
        pool.addTask(ptask.get());
        cout <<"ctl="<<ctl<<endl;
    }
    pool.stop();
    return 0;
}
