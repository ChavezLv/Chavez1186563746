#ifndef __timerfd_H__
#define __timerfd_H__
#include<functional>
using std::function;

class Timerfd{
    using CallBack=function<void()>;
public:
    Timerfd(int initsec,int periodsec,CallBack _cb);
    ~Timerfd();

    void start();
    void stop();
    
    void handleRead();
    void createTimerfd();
    void setTimerfd(int periodsec,int initsec);


private:
    int _timerFd;
    int _initSec;
    int _periodSec;
    bool _isWhile;
    CallBack _cb;
};




#endif
