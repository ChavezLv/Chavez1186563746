/*================================================================
*   Copyright (C) 2021 anonymous All rights reserved.
*   
*   @file       ：a0010testThreadAandB.c
*   @author     ：anonymous
*   @date       ：2021年09月12日 22:08:57
*   @description：
*
================================================================*/


#include <unistd.h>
#include  <pthread.h>
#include <iostream>
#include <functional>
#include <memory>
#include <vector>

using namespace std;

class Noncopyable{
protected:
    Noncopyable(){  }//方便派生类不用写初始化列表
    /* ~Noncopyable(){  } */
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable & operator= (const Noncopyable &) = delete ;
};

class MutexLock
: Noncopyable{
public:
    MutexLock(){
        if(0 != pthread_mutex_init(&_MutexLock, nullptr)){
            perror("pthread_mutex_init");
        }
    }

    ~MutexLock(){
        if(0 != pthread_mutex_destroy(&_MutexLock)){
            perror("pthread_mutex_destroy");
        }
    }

    void lock(){
        if(0 != pthread_mutex_lock(&_MutexLock)){
            perror("pthread_mutex_lock");
        }
    }

    void unlock(){
        if(0 != pthread_mutex_unlock(&_MutexLock)){
            perror("pthread_mutex_unlock");
        }
    }

    pthread_mutex_t* getMutexLockPtr(){
        return &_MutexLock;
    }

private:
    pthread_mutex_t _MutexLock;

};

class MutexLockGuard{
public:
    MutexLockGuard(MutexLock & MutexLock)
    : _MutexLock(MutexLock){
        _MutexLock.lock();
    }
    ~MutexLockGuard(){
        _MutexLock.unlock();
    }
private:
    MutexLock &_MutexLock;

};

class Condition{
public:
    Condition(MutexLock & MutexLock)
    : _MutexLock(MutexLock){
        if(0 != pthread_cond_init(&_cond, nullptr)){
            perror("pthread_cond_init");    
        }
    }
    ~Condition(){
        if(0 != pthread_cond_destroy(&_cond)){
            perror("pthread_cond_destroy");
        }
    }

    void wait(){
        if(0 != pthread_cond_wait(&_cond, _MutexLock.getMutexLockPtr())){
            perror("pthread_cond_wait");
        }
    }

    void timewait(time_t tv_sec, time_t tv_nsec){
        struct timespec clock_1;
        clock_1.tv_sec = tv_sec;
        clock_1.tv_sec = tv_nsec;
        clock_1.tv_nsec = 0;
        if(0 != pthread_cond_timedwait(&_cond, _MutexLock.getMutexLockPtr(), &clock_1)){
            perror("pthread_cond_timewait");
        }
    }

    void notice(){
        if(0 != pthread_cond_signal(&_cond)){
            perror("pthread_cond_signal");
        }
    }

    void noticeAll(){
        if(0 != pthread_cond_broadcast(&_cond)){
            perror("pthread_cond_broadcast");
        }
    }
    
private:
    MutexLock &_MutexLock;
    pthread_cond_t _cond;
    
};

namespace thread_local_storage{
__thread const char *name = "wd thread";
}

struct ThreadData;//前向声明

class Thread{
public:
    using ThreadCallback = std::function<void()>;
    Thread(ThreadCallback&& cb, const string &name = "wd thread")
    : _pthid(0)
    , _cb(move(cb))
    , _name(name)
    , _isRunning(false){
        
    }  
    ~Thread(){
        //fg
        if(_isRunning){
            if(0 != pthread_detach(_pthid)){
                perror("pthread_detach");
            }
            _isRunning = false;
        }
    }


    void join(){
        if(_isRunning){
            if(0 != pthread_join(_pthid, nullptr)){
                perror("pthread_join");
            }
            _isRunning = false;
        }

    }

    void start();

    static void *threadFunc(void*arg);
private:
    pthread_t _pthid;
    ThreadCallback _cb;
    string _name;
    bool _isRunning;
};

using ThreadCallback = Thread::ThreadCallback;
struct ThreadData{
    string _name;
    ThreadCallback _cb;
    ThreadData(const string &name, ThreadCallback &&cb)
    : _name(name)
    , _cb(std::move(cb)){

    }

    void runInThread(){
        thread_local_storage::name = _name.c_str();
        if(_cb){
            _cb();
        }
    }

};

void Thread::start(){
    ThreadData *pData = new ThreadData(_name, std::move(_cb));
    if(0 != pthread_create(&_pthid, nullptr, threadFunc, pData)){
        perror("pthread_create");
    }
    _isRunning = true;
}

void *Thread::threadFunc(void* arg){
    ThreadData *pData = static_cast<ThreadData*>(arg);
    if(pData){
        pData->runInThread();
    }
    delete pData;
    return nullptr;
}

class Print{
public:
    Print(size_t threadNum)
    : _isExit(false)
    , _threadNum(threadNum)
    , _MutexLock()
    , _cond(_MutexLock){
        _threads.reserve(_threadNum);
    }

    void start(){
        for(size_t idx = 0; idx != _threadNum; ++idx){
            unique_ptr<Thread> unipThread(new Thread(\
                std::bind(&Print::doTask, this), to_string(idx)));
            _threads.push_back(std::move(unipThread));
        }
        _isA = true;
        _isB = false;
        for(auto &thread : _threads){
            thread->start();
        }
    }

    void printA(){
        usleep(50000);
        {
            MutexLockGuard autolock(_MutexLock);
            while(!_isExit && !_isA){
                _cond.wait();
            }
            cout << "A" << endl;
        }
        _isA = false; 
        _isB = true; 
        _cond.notice();
    }

    void printB(){
        usleep(50000);
        {
            MutexLockGuard autolock(_MutexLock);
            while(!_isExit && !_isB){
                _cond.wait();
            }
            cout << "B" << endl;// 看成是资源
        }
        _isA = true; 
        _isB = false; 
        _cond.notice();
    }
    ThreadCallback getTask(){
        ThreadCallback fun;
        if(atoi(thread_local_storage::name) == 0){
            fun = bind(&Print::printA, this);
        }else{
            fun = bind(&Print::printB, this);
        }
        return fun; 

    }

    void doTask(){
        while(!_isExit){
            ThreadCallback task = getTask();
            if(task){
                task();
            }
        }
    }

    void stop(){
        if(!_isExit){
            _isExit = true;
            _cond.noticeAll();
            for(auto &thread:_threads){
                thread->join();
            }
        }
    }

private:
    bool _isExit;
    size_t _threadNum;
    MutexLock _MutexLock;
    Condition _cond;
    vector<unique_ptr<Thread> > _threads;
    bool _isA;
    bool _isB;
};
void test(){
    Print print(2);// create two thread
    print.start();//start to print
    usleep(500000);
    print.stop();// end to print
}

int main(){
    test();
    return 0;
}
