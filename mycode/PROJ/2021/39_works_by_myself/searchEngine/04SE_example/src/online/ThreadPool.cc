#include "../../include/ThreadPool.hpp"
#include "../../include/Thread.hpp"

#include <unistd.h>

#include <iostream>
using std::cout;
using std::endl;

namespace spellCorrect
{
ThreadPool::ThreadPool(size_t threadNum,size_t queSize)
    :_threadNum(threadNum)
    ,_queSize(queSize)
     ,_taskQue(_queSize)
     ,_isExit(false)
    {
        _threadList.reserve(_threadNum);
    }

ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        stop();
    }
}

void ThreadPool::start()
{
    for(int i = 0; i != _threadNum; ++i)
    {
        unique_ptr<Thread> ut(new Thread(std::bind(&ThreadPool::threadFunc,this)
                              ,std::to_string(i)));
        _threadList.push_back(std::move(ut));
    }
    for(auto& thread : _threadList)
    {
        thread->start();
    }
}

void ThreadPool::stop()
{
    if(!_isExit)
    {
        //等任务执行完毕
        while(!_taskQue.empty())
        {
            ::usleep(1);//每隔一段时间检查任务列表
        }
        _isExit = true;
        wakeup();

        for(auto& thread : _threadList)
        {
            thread->join();
        }
    }
}

void ThreadPool::wakeup()
{
    _taskQue.wakeup();
}

void ThreadPool::addTask(Task && task)
{
    _taskQue.push(task);
}

Task ThreadPool::getTask()
{
    return _taskQue.pop();
}

void ThreadPool::threadFunc()
{
    while(!_isExit)
    {
        Task task = getTask();
        if(task)
        {
            task();
        }
    }
}

}//end of namespace spellCorrect

#if 0

using namespace spellCorrect;

class myTask
{
public:
    void func()
    {
        ::srand(clock());
        int number = ::rand() % 100;
        cout << ">> thread id " << spellCorrect::current_thread::name;
        cout << ">> get a random number: " << number << endl;
        sleep(1);
    }
};

void test()
{
    myTask task;
    ThreadPool pool(4,10);
    pool.start();

    int cnt = 20;
    while(cnt--)
    {
        pool.addTask(std::bind(&myTask::func,&task));
        cout << "main thread cnt = " << cnt << endl;
    }
    pool.stop();
}

int main()
{
    test();
    return 0;
}

#endif
