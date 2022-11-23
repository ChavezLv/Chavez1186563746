 ///
 /// @file    TimerManager.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-05-12 11:21:32
 ///
 
#include "TimerManager.hpp"

#include <unistd.h>
#include <stdio.h>
#include <sys/timerfd.h>
#include <poll.h>

#include <algorithm>

namespace wd
{

TimerManager::TimerManager()
: _timerfd(createTimerfd())
, _isStarted(false)
{}

void TimerManager::start()
{
	struct pollfd pfd;
	pfd.fd = _timerfd;
	pfd.events = POLLIN;

	setTimerfd(1, 1);

	_isStarted = true;
	int nready = -1;
	while(_isStarted) {
		//对文件描述符进行监听
		nready = poll(&pfd, 1, 5000);
		printf("nready:%d\n", nready);

		if(nready == 0) {
			printf(">> poll timeout!\n");
		} else if(nready == -1 && errno == EINTR) {
			continue;
		} else if(nready == -1) {
			perror("poll");
			return;
		} else {
			if(pfd.revents & POLLIN) {
				handleRead();//1. 对事件进行处理
				handleTask();	 //2. 执行任务
			}
		}
	}
}

void TimerManager::stop()
{	
	if(_isStarted) {
		setTimerfd(0, 0);
		_isStarted = false;	
	}
}

void TimerManager::attach(TimerTask * task)
{
	if(task) {
		auto iter = std::find(_taskList.begin(), _taskList.end(), task);
		if(iter == _taskList.end()) {
			_taskList.push_back(task);
		}
	}
}

void TimerManager::detach(TimerTask * task)
{
	if(task) {
		auto iter = std::find(_taskList.begin(), _taskList.end(), task);
		if(iter != _taskList.end()) {
			_taskList.erase(iter);
		}
	}
}

void TimerManager::handleTask()
{
	for(auto & task : _taskList) {
		task->decrease();
		if(task->getCount() == 0){
			task->process();
			task->reset();
		}
	}
}
	

void TimerManager::handleRead()
{
	uint64_t howmany = 0;
	int ret = read(_timerfd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror("read");
	}
}



int TimerManager::createTimerfd()
{
	int fd = timerfd_create(CLOCK_REALTIME, 0);
	if(fd < 0) {
		perror("eventfd");
	}
	return fd;
}

void TimerManager::setTimerfd(int initsec, int periodicsec)
{
	struct itimerspec value;
	value.it_value.tv_sec = initsec;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = periodicsec;
	value.it_interval.tv_nsec = 0;
	int ret = timerfd_settime(_timerfd, 0, &value, nullptr);
	if(ret < 0) {
		perror("timerfd_settime");
	}
}

}//end of namespace wd
