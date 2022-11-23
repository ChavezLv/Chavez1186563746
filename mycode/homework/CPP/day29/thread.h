#ifndef __thread_H__
#define __thread_H__
#include<pthread.h>

#include<functional>
class Thread{
    using Entrance=std::function<void()>;
public:
    Thread(Entrance &&cb);
    ~Thread();
    void start();
    void stop();
private:
    pthread_t _thrid;
    static void *EntranceFunc(void* arg);
    Entrance _cb;
    bool _isStart;
};





#endif
