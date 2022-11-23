 ///
 /// @file    Eventfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-21 10:04:54
 ///
 
#include "Eventfd.hpp"

#include <unistd.h>
#include <poll.h>
#include <sys/eventfd.h>

namespace wd
{

Eventfd::Eventfd(EventfdCallback && cb)
: _eventfd(createEventfd())
, _cb(std::move(cb))
, _isStarted(false)
{}

//开始监听_eventfd文件描述符
void Eventfd::start()
{
	struct pollfd pfd;
	pfd.fd = _eventfd;
	pfd.events = POLLIN;
	
	_isStarted = true;
	while(_isStarted) {
		int nready ;
		do{
			nready = poll(&pfd, 1, 6000);
		}while(nready == -1 && errno == EINTR);

		if(nready == -1) {
			perror("poll");
			return ;
		} else if(0 == nready) {
			printf(">> poll timeout!\n");
		} else {
			if(pfd.revents & POLLIN) {
				//先处理读事件
				handleRead();
				//再去执行任务
				if(_cb) {
					_cb();
				}
			}
		}
	}
}

void Eventfd::stop()
{
	_isStarted = false;
}

//在不同的线程去执行
void Eventfd::wakeup()
{
	uint64_t one = 1;//8字节的
	int ret = write(_eventfd, &one, sizeof(one));
	if(ret != sizeof(one)) {
		perror("write");
	}
}



int Eventfd::createEventfd()
{
	int fd = eventfd(0, 0);//参1 是内核计数器的初始值
	if(fd < 0) {
		perror("eventfd");
	}
	return fd;
}

void Eventfd::handleRead()
{
	uint64_t value;
	//对于eventfd, 即便是write多次, 也能通过read, 一次read就变为0了 
	int ret = read(_eventfd, &value, sizeof(value));
	if(ret != sizeof(value)) {
		perror("read");
	}
}


}//end of namespace wd
