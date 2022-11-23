#include "Thread.h"
#include <unistd.h>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;

class MyThread
: public Thread
{
public:
private:
    void run() override
    {
        while(1)
        {
            cout << "Thread is running" << endl;
            sleep(1);
        }
    }

};


int main(int argc, char **argv)
{
    unique_ptr<Thread> pth(new MyThread());
    pth->start();

    pth->stop();
    return 0;
}

