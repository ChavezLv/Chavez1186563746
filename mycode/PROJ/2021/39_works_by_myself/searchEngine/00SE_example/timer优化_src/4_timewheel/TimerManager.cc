 ///
 /// @file    TimerManager.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-10-23 09:53:37
 ///
 
#include "TimerManager.hpp"
#include "TimerTask.hpp"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/timerfd.h>
#include <poll.h>

#include <string>
#include <algorithm>
#include <iostream>

namespace wd
{

TimerManager::TimerManager()
: _fd(createTimerfd())
, _isStarted(false)
, _currentIndex(0)
{
	_timeWheelList.reserve(kSize);
	for(size_t idx = 0; idx < kSize; ++idx) {
		_timeWheelList.push_back(list<TimerTask*>());
	}
}

void TimerManager::start()
{
	struct pollfd pfd;
	pfd.fd = _fd;
	pfd.events = POLLIN;

	//开启定时器
	setTimerfd(1, 1);//每一秒超时一次

	_isStarted = true;
	while(_isStarted) {
		int nready = ::poll(&pfd, 1, 5000);
		if(nready == -1  &&  errno == EINTR)
			continue;
		else if(nready == -1) {
			perror("poll");
			return;
		} else if(nready == 0) {
			printf("poll timeout!\n");
		} else {
			std::cout << "111" << std::endl;
			handleRead();
			handleTask();
		}
	}
}
	
void TimerManager::stop()
{
	setTimerfd(0, 0);
	if(_isStarted)
		_isStarted = false;
}

void TimerManager::addTask(TimerTask * timerTask)
{
	MutexLockGuard autolock(_mutex);
	_pendingTaskList.push_back(timerTask);
}

void TimerManager::internalAddTask(TimerTask * timerTask)
{
	if(timerTask) {
		/* 根据seconds确定在哪一个时间槽slot */
		int idx = (timerTask->getTimeInterval() + _currentIndex) % kSize;
		/* 放入时间槽*/
		_timeWheelList[idx].push_back(timerTask);
		/* 确定圈数 */
		int count = timerTask->getTimeInterval() / kSize;
		timerTask->setCount(count);
	}
}

void TimerManager::removeTask(TimerTask * timerTask)
{
	if(timerTask) {
		for(auto & slot : _timeWheelList) {
			for(auto ptask : slot) {
				if(ptask == timerTask) {
					slot.remove(ptask);
					return;
				}
			}
		}
	}
}

int TimerManager::createTimerfd()
{
	int fd = ::timerfd_create(CLOCK_REALTIME, 0);
	if(fd < 0) {
		perror("timerfd_create");
	}
	return fd;
}

void TimerManager::setTimerfd(int initialTime, int periodicTime)
{
	struct itimerspec value;
	value.it_value.tv_sec = initialTime;
	value.it_value.tv_nsec = 0;
	value.it_interval.tv_sec = periodicTime;
	value.it_interval.tv_nsec = 0;

	int ret = ::timerfd_settime(_fd, 0, &value, nullptr);
	if(ret < 0) {
		perror("timerfd_settime");
	}
}

void TimerManager::handleRead()
{
	uint64_t howmany = 0;
	int ret = ::read(_fd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror("read");
	}
}

void TimerManager::handleTask()
{
	++_currentIndex;
	_currentIndex %= kSize;

	list<TimerTask *> cacheList;

	auto & slot = _timeWheelList[_currentIndex];
	for(auto iter = slot.begin(); iter != slot.end();) {
		
		if((*iter)->getCount() <= 0) {
			//先缓存
			auto tmp = *iter;
			cacheList.push_back(tmp);
			//再从当前槽中删除掉
			iter = slot.erase(iter);// 删除当前元素之后，得到了下一个元素的位置
			//再将其添加到槽中
			internalAddTask(tmp);
		} 
		else {
			(*iter)->decreaseCount();
			++iter;
		}
	}

	//执行所有的定时器任务
	for(auto ptask : cacheList) {
		ptask->process();
	}
	
	//添加新的任务
	if(0 == _pendingTaskList.size()) {
		return;
	}
	vector<TimerTask *> tmp;
	{
	MutexLockGuard autolock(_mutex);
	tmp.swap(_pendingTaskList);
	}
	for(auto & ptask : tmp) {
		cout << "222 add task" << endl;
		internalAddTask(ptask);
	}
}

}//end of namespace wd
