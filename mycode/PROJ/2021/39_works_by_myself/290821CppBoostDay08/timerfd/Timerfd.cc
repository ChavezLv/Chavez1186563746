 ///
 /// @file    Timerfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:06:17
 ///

#include "Timerfd.hpp"

#include <unistd.h>
#include <poll.h>
#include <sys/timerfd.h>

namespace wd
{
Timerfd::Timerfd(int initTime, int periodicTime, TimerCallback && cb)
: _timerfd(createTimerfd())
, _initTime(initTime)
, _periodicTime(periodicTime)
, _cb(std::move(cb))
, _isStarted(false)
{}

Timerfd::~Timerfd()
{	close(_timerfd);	}


//监听timerfd文件描述符
void Timerfd::start()
{
	struct pollfd pfd;
	pfd.fd = _timerfd;
	pfd.events = POLLIN;
	
	//开启定时器
	setTimerfd(_initTime, _periodicTime);

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

void Timerfd::stop()
{
	setTimerfd(0, 0);//停止定时器
	_isStarted = false;
}

int Timerfd::createTimerfd()
{
	int fd = ::timerfd_create(CLOCK_REALTIME, 0);
	if(fd < 0) {
		perror("timerfd_create");
	}
	return fd;
}

void Timerfd::handleRead()
{
	uint64_t howmany = 0;
	int ret = read(_timerfd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror("read");
	}
}

void Timerfd::setTimerfd(int initTime, int periodicTime)
{
	struct itimerspec value;
	value.it_value.tv_sec = initTime;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = periodicTime;
	value.it_interval.tv_nsec = 0;

	int ret = ::timerfd_settime(_timerfd, 0, &value, nullptr);
	if(ret < 0) {
		perror("timefd_settime");
	}
}
}//end of namespace wd
