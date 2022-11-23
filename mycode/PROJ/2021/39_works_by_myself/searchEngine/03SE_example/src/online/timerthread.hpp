#ifndef __RXL_TIMERTHRAD_H__
#define __RXL_TIMERTHRAD_H__
#include"thread.hpp"
#include"timerfd.hpp"
namespace rxl{

class TimerThread{
	using CallBackFunc = std::function<void()>;
public:
	TimerThread(int initialtime,int intervaltime,CallBackFunc&& cb)
	:_timerfd(initialtime,intervaltime,std::move(cb))
	,_thread(std::bind(&Timerfd::start,&_timerfd))
	{}
	void start(){ _thread.create();}
	void stop(){
		_timerfd.stop();
		_thread.join();
	}
private:
Timerfd _timerfd;
Thread _thread;
};
}//end of namespace
#endif 
