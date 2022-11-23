#include "Timer.hpp"

#include "poll.h"
#include "unistd.h"
#include "sys/timerfd.h"

#include <iostream>

using std::cout;
using std::endl;

Timer::Timer(CallBack&& cb,int priTime,int interTime)
:_fd(CreateTimer())
,_priTime(priTime)
,_interTime(interTime)
,_cb(std::move(cb))
,_isStart(false)
{
}
int Timer::CreateTimer(){
    int ret=timerfd_create(CLOCK_REALTIME,0);
    if(ret<0){
        perror("timerfd_create");
    }
    return ret;
}
void Timer::start(){
    if(!_isStart)
        _isStart=true;
    
    struct pollfd pfd;
    pfd.fd=_fd;
    pfd.events=POLLIN;
    setTimerfd(_priTime,_interTime);

    while(_isStart){
        int ret;
        do{
            ret=::poll(&pfd,1,5000);
        }while(ret==-1&&errno==EINTR);
        if(ret==-1){
            perror("poll");
            return;
        }
        else if(ret==0){
            cout<<"timeout"<<endl;
        }
        else {
            if(pfd.revents&POLLIN){
                handleRead();
                if(_cb){ 
                    _cb();

                }
            }
        }
    }
}
void Timer::handleRead(){
    uint64_t howmany;
    int ret=::read(_fd,&howmany,sizeof(howmany));
    if(ret==-1){
        perror("read");
    }
}

void Timer::setTimerfd(int initTimer,int internalTimer){
    struct itimerspec itimer;
    itimer.it_value.tv_sec=initTimer;
    itimer.it_value.tv_nsec=0;

    itimer.it_interval.tv_sec=internalTimer;
    itimer.it_interval.tv_nsec=0;

    int ret=timerfd_settime(_fd,0,&itimer,nullptr);
    if(ret==-1){
        perror("timerfd_settime");
    }
}
void Timer::stop(){
    setTimerfd(0, 0);
    if(_isStart){
        _isStart=false;
    }
}
