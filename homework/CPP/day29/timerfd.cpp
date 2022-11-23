#include"timerfd.h"
#include<unistd.h>
#include<sys/timerfd.h>
#include<poll.h>

Timerfd::Timerfd(int initsec,int periodsec,CallBack cb)
    :_initSec(initsec)
     ,_periodSec(periodsec)
     ,_isWhile(false)
     ,_cb(cb){
         createTimerfd();
     }
void Timerfd::start(){
    struct pollfd pfd;
    pfd.events=POLLIN;
    pfd.fd=_timerFd;//监听的事件
    setTimerfd(_periodSec,_initSec);
    _isWhile=true;
    while(_isWhile){
        int ready=poll(&pfd,1,5000);//监听描述符为1,超时时间5000ms
        if(-1==ready&&errno==EINTR){
            continue;
        }
        else if(-1==ready){
            perror("poll() error");
            return;
        }
        else if(0==ready){
            perror("poll():time out!");    
        }else{
            if(pfd.revents&POLLIN){
                handleRead();
                if(_cb){
                    _cb();
                }
            }
        }
    }
}
    
Timerfd::~Timerfd(){
    close(_timerFd);
}

void Timerfd::stop(){
    _isWhile=false;
    setTimerfd(0,0);//勿忘设置定时器
}

void Timerfd::handleRead(){
    uint64_t one=1;
    int ret=read(_timerFd,&one,sizeof(one));
    if(-1==ret){
        perror("read()");
        return;
    }
}

void Timerfd::createTimerfd (){
    _timerFd=timerfd_create(CLOCK_REALTIME,0); 
    if(-1==_timerFd){
        perror("createTimerfd: timerfd_create()");
        return;
    }
}

void Timerfd::setTimerfd(int periodSec,int initSec){
    struct itimerspec val;
    val.it_interval.tv_sec=periodSec;
    val.it_interval.tv_nsec=0;

    val.it_value.tv_sec=initSec;
    val.it_value.tv_nsec=0;
    int ret=timer_settime(&_timerFd,0,&val,nullptr);//注意传入的_timerFd类型,传int*也可以,int不行
    if(-1==ret){
        perror("setTimerfd: timer_settime()");
        return;
    }

}
