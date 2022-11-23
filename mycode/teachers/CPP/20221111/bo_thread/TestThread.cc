#include "Thread.h"
#include <unistd.h>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

using std::bind;

class MyThread
{
public:
    void process() 
    {
        while(1)
        {
            cout << "Thread is running" << endl;
            sleep(1);
        }
    }

};

void func()
{
    while(1)
    {
        cout << "Func is running" << endl;
        sleep(1);
    }

}


int main(int argc, char **argv)
{
    MyThread mth;
    /* Thread th(bind(&MyThread::process, &mth)); */
    Thread th(std::move(func));

    /* function<void()> f = bind(&MyThread::process, &mth); */
    /* Thread th1(std::move(f)); */

    th.start();

    th.stop();
    return 0;
}

