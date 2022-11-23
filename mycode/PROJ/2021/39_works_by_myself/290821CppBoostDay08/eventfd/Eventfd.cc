 ///
 /// @file    Eventfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:06:17
 ///

#include "Eventfd.hpp"

#include <unistd.h>
#include <poll.h>
#include <sys/eventfd.h>

namespace wd
{
Eventfd::Eventfd(EventCallback && cb)
: _eventfd(createEventfd())
, _cb(std::move(cb))
, _isStarted(false)
{}

Eventfd::~Eventfd()
{	close(_eventfd);	}


//监听eventfd文件描述符
void Eventfd::start()
{
	struct pollfd pfd;
	pfd.fd = _eventfd;
	pfd.events = POLLIN;

	_isStarted = true;
	while(_isStarted) {
		int nready = 0;
		nready = ::poll(&pfd, 1, 5000);

		if(nready == -1 && errno == EINTR) continue;
		else if(-1 == nready) {
			perror("poll");
			return;
		} else if(0 == nready) {
			printf(">> poll timeout\n");
		} else {
			if(pfd.revents & POLLIN) {
				//先处理读事件
				handleRead();
				//执行任务
				if(_cb) _cb();
			}
		}
	}
}

void Eventfd::stop()
{
	_isStarted = false;
}

int Eventfd::createEventfd()
{
	int fd = ::eventfd(0, 0);
	if(fd < 0) {
		perror("evetfd");
	}
	return fd;
}

void Eventfd::handleRead()
{
	uint64_t howmany = 0;
	int ret = read(_eventfd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror("read");
	}
}

//该方法与start方法运行在不同的线程，执行通知操作
void Eventfd::wakeup()
{
	uint64_t one = 1;
	int ret = write(_eventfd, &one, sizeof(one));
	if(ret != sizeof(one)) {
		perror("write");
	}
}

}//end of namespace wd
