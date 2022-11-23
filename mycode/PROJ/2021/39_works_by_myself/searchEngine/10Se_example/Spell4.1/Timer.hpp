#ifndef __TIMER_H__
#define __TIMER_H__
#include <functional>
using std::function;
class Timer
{
    using CallBack=function<void()>;
public:
    /* Timer()=default; */
    Timer(CallBack&&,int,int);
    ~Timer() {
        if(_isStart){
            stop();
        }
    }

    void start();
    void stop();
private:
    int CreateTimer();
    void handleRead();
    void setTimerfd(int,int);
private:
    int _fd;
    int _priTime;
    int _interTime;
    CallBack _cb;
    bool _isStart;
};
#endif
