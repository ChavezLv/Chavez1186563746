#include"timerfd.hpp"
namespace rxl{
	void Timerfd::start(){
		_isstarted = true;
		setTimerfd(_initialtime,_intervaltime);
		struct pollfd spoll;
		spoll.fd = _timerfd;
		spoll.events = POLLIN;
		int ret;
		while(_isstarted){
			do{
				ret = ::poll(&spoll,1,5000);
			}while(ret == -1 && errno == EINTR);
			if(ret == -1){
				perror("poll");
				return;
			}else if(ret == 0){
				printf("poll timeout\n");
			}else{
				if(spoll.revents & POLLIN){
					handleread();
					if(_cb)
						_cb();
				}
			}
		}
	}
	void Timerfd::stop(){
		setTimerfd(0,0);
		if(_isstarted){
			_isstarted =false;
		}
	}
	void Timerfd::handleread(){
		uint64_t u;
		int ret = ::read(_timerfd,&u,sizeof(uint64_t));
		if(ret != sizeof(uint64_t)){
			perror("handleread");
		}
	}
	int Timerfd::createTimerfd(){
		int ret = ::timerfd_create(CLOCK_REALTIME,0);
		if(ret < 0)
			perror("timerfd_create");
		return ret;
	}
	void Timerfd::setTimerfd(int initialtime,int intervaltime){	
		struct itimerspec value;
		value.it_value.tv_sec = initialtime;
		value.it_value.tv_nsec = 0;
		value.it_interval.tv_sec = intervaltime;
		value.it_interval.tv_nsec  = 0;
		int ret = ::timerfd_settime(_timerfd,0,&value,nullptr);
		if(ret < 0){
			perror("timerfd_settime");
		}
	}
}//end of namespace
