#ifndef __RXL_TIMERFD_H__
#define __RXL_TIMERFD_H__
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<poll.h>
#include<sys/timerfd.h>
#include<functional>

namespace rxl{

class Timerfd{
using CallBackFunc = std::function<void()>;
public:
Timerfd(int initialtime,int intervaltime,CallBackFunc&& cb)
:_timerfd(createTimerfd())
,_initialtime(initialtime)
,_intervaltime(intervaltime)
,_cb(std::move(cb))
,_isstarted(false)
{}
~Timerfd(){
	stop();
}
void start();
void stop();
private:
void handleread();
int createTimerfd();
void setTimerfd(int,int);
private:
	int _timerfd;
	int _initialtime;
	int _intervaltime;
	CallBackFunc _cb;
	bool _isstarted;
};

}//end of namespace
#endif
