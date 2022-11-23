 ///
 /// @file    Timerfd.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-30 11:06:17
 ///

#include "TimerManager.hpp"

#include <unistd.h>
#include <poll.h>
#include <sys/timerfd.h>

#include <algorithm>//std::find

namespace wd
{

TimerManager::TimerManager()
: _timerfd(createTimerfd())
, _isStarted(false){

}


//监听timerfd文件描述符
void TimerManager::start()
{
	struct pollfd pfd;
	pfd.fd = _timerfd;
	pfd.events = POLLIN;
	
	//开启定时器
	/* setTimerfd(_initTime, _periodicTime); */
	setTimerfd(1, 1);

	_isStarted = true;
	int nready = -1;
	while(_isStarted) {
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
                handleTask();
			}
		}
	}
}

void TimerManager::stop()
{
	setTimerfd(0, 0);//停止定时器
	_isStarted = false;
}

int TimerManager::createTimerfd()
{
	int fd = ::timerfd_create(CLOCK_REALTIME, 0);
	if(fd < 0) {
		perror("timerfd_create");
	}
	return fd;
}

void TimerManager::attach(TimerTask *task){
    if(task){
        auto iter = std::find(_taskList.begin(), _taskList.end(), task);    
        if(iter == _taskList.end()){
            //没有就插入, 否则不插入
            _taskList.push_back(task);
        }
    }
}

void TimerManager::detach(TimerTask *task){
    //考虑, 如果设置重复的任务可以吗
    if(task){
        auto iter = std::find(_taskList.begin(), _taskList.end(), task);    
        if(iter != _taskList.end()){
            _taskList.erase(iter);
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

//
void TimerManager::handleTask(){
    for(auto &task: _taskList){
        task->process();
    }

}

void TimerManager::setTimerfd(int initTime, int periodicTime)
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
