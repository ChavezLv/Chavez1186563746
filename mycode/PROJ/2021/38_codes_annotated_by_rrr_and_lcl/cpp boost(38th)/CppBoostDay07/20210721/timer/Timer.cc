 ///
 /// @file    Timer.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-21 10:04:54
 ///
 
#include "Timer.hpp"

#include <unistd.h>
#include <poll.h>
#include <sys/timerfd.h>

namespace wd
{

Timer::Timer(int initSec, int periodicSec, TimerCallback && cb)
: _timerfd(createTimerfd())
, _initSec(initSec)
, _periodicSec(periodicSec)
, _cb(std::move(cb))
, _isStarted(false)
{}

Timer::~Timer()
{
	close(_timerfd);
}

//开始监听_timerfd文件描述符
void Timer::start()
{
	struct pollfd pfd;
	pfd.fd = _timerfd;
	pfd.events = POLLIN;

	//设置好定时器
	setTimerfd(_initSec, _periodicSec);
	
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

void Timer::stop()
{
	_isStarted = false;
	setTimerfd(0, 0);//关闭定时器, 停止
}

int Timer::createTimerfd()
{
	int fd = timerfd_create(CLOCK_REALTIME, 0);
	if(fd < 0) {
		perror("timerfd_create");
	}
	return fd;
}

void Timer::handleRead()
{
	uint64_t value;
	int ret = read(_timerfd, &value, sizeof(value));
	if(ret != sizeof(value)) {
		perror("read");
	}
}

void Timer::setTimerfd(int initSec, int periodicSec)
{
	struct itimerspec value;
	value.it_value.tv_sec = initSec;
	value.it_value.tv_nsec = 0;//必须写上，否则有可能设置定时器时会失败
	value.it_interval.tv_sec = periodicSec;
	value.it_interval.tv_nsec = 0;
	int ret = timerfd_settime(_timerfd, 0, &value, nullptr);
	if(ret < 0) {
		perror("timerfd_settime");
	}
}

}//end of namespace wd
