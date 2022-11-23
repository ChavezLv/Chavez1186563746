#ifndef __RXL_EVENTFD_H__
#define __RXL_EVENTFD_H__
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<poll.h>
#include<sys/eventfd.h>
#include<functional>

namespace rxl{

class Eventfd{
public:
Eventfd()
:_eventfd(createEventfd())
{}
~Eventfd(){}
void weakup(){
	uint64_t u = 1;
	int ret = write(_eventfd,&u,sizeof(uint64_t));
	if(ret != sizeof(uint64_t)){
		perror("weekup write");
	}
}
int eventfd(){ return _eventfd; }
void handleread(){
	uint64_t u;
	int ret = ::read(_eventfd,&u,sizeof(uint64_t));
	if(ret != sizeof(uint64_t)){
		perror("handleread");
	}
}
private:
int createEventfd(){
	int ret = ::eventfd(1,0);
	if(ret < 0)
		perror("eventfd");
	return ret;
}
private:
	int _eventfd;
};

}//end of namespace
#endif
