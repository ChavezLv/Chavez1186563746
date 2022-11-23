#include "TimerFd.h"
#include "Thread.h"
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

class MyTask
{
public:
    void process()
    {
        cout << "MyTask is running" << endl;
    }
};

int main(int argc, char **argv)
{
    MyTask task;
    TimerFd tf(1, 1, std::bind(&MyTask::process, &task));

    Thread th(std::bind(&TimerFd::start, &tf));
    th.start();

    sleep(30);

    tf.stop();
    th.stop();

    return 0;
}

