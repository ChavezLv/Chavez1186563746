 ///
 /// @file    EventLoop.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-17 15:38:24
 ///
 
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "TcpConnection.hpp"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <sys/eventfd.h>

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{

EventLoop::EventLoop(Acceptor & acceptor)
: _efd(createEpollfd())
, _evfd(createEventfd())
, _acceptor(acceptor)
, _isLooping(false)
, _evtList(1024)
{
	addEpollReadfd(_acceptor.fd());
	addEpollReadfd(_evfd);
}

void EventLoop::loop()
{
	_isLooping = true;
	while(_isLooping) {
		waitEpollFd();
	}
}

void EventLoop::runInLoop(Functor && cb)
{
	{
	MutexLockGuard autolock(_mutex);
	_pendingFunctors.push_back(std::move(cb));
	}

	wakeup();
}

void EventLoop::unloop()
{
	_isLooping = false;
}
	
void EventLoop::setAllCallbacks(
		TcpConnectionCallback && cb1,
		TcpConnectionCallback && cb2,
		TcpConnectionCallback && cb3)
{
	_onConnectionCb = std::move(cb1);
	_onMessageCb = std::move(cb2);
	_onCloseCb = std::move(cb3);
}

//一个函数的实现最多不要超过50行
void EventLoop::waitEpollFd()
{
	int nready = -1;
	do {
		nready = ::epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), 5000);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1) {
		perror("epoll_wait");
		return;
	} else if(nready == 0) {
		printf("epoll_wait timeout!\n");
	} else {
		//考虑扩容的变化	
		if(nready == _evtList.size()) {
			_evtList.resize(2 * nready);
		}

		for(int idx = 0; idx < nready; ++idx) {
			int tmp = _evtList[idx].data.fd;
			if(tmp == _acceptor.fd() && 
			   (_evtList[idx].events & EPOLLIN)) {
				handleNewConnection();
			} else if(tmp == _evfd) {
				handleRead();
				doPendingFunctors();
				cout << "doPendingFunctors()" << endl;
			} else {
				if(_evtList[idx].events & EPOLLIN) {
					handleMessage(tmp);
				}
			}
		}
	}
}

void EventLoop::handleNewConnection()
{
	int peerfd = _acceptor.accept();
	addEpollReadfd(peerfd);

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	//给每一个连接注册事件处理器
	conn->setAllCallbacks(
		_onConnectionCb,
		_onMessageCb,
		_onCloseCb);

	_conns.insert(std::make_pair(peerfd, conn));
	conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
	auto iter = _conns.find(fd);
	if(iter != _conns.end()) {
		if(iter->second->isClosed()) {
			iter->second->handleCloseCallback();
			delEpollReadfd(fd);
			_conns.erase(iter);
		} else {
			iter->second->handleMessageCallback();
		}
	}
}


int EventLoop::createEpollfd()
{
	int fd = ::epoll_create1(0);
	if(fd < 0) {
		::perror("epoll_create1");
	}
	return fd;
}

void EventLoop::addEpollReadfd(int fd)
{
	struct epoll_event value;
	value.data.fd = fd;
	value.events = EPOLLIN;
	if(::epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &value) < 0) {
		perror("epoll_ctl");
	}
}

void EventLoop::delEpollReadfd(int fd)
{
	struct epoll_event value;
	value.data.fd = fd;
	if(::epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &value) < 0) {
		perror("epoll_ctl");
	}
}

int EventLoop::createEventfd()
{
	int fd = ::eventfd(0, 0);
	if(fd < 0) {
		perror("eventfd");
	}
	return fd;
}

void EventLoop::handleRead()
{
	uint64_t howmany = 1 << 31;
	int ret = ::read(_evfd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror("read");
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	int ret = ::write(_evfd, &one, sizeof(one));
	if(ret != sizeof(one)) {
		perror("write");
	}
}

void EventLoop::doPendingFunctors()
{
	std::vector<Functor> tmp;
	{
	MutexLockGuard autolock(_mutex);
	tmp.swap(_pendingFunctors);//vector底层实现是3个指针, O(1)
	}

	for(auto & cb : tmp) 
	{
		cb();
	}
}

}//end of namespace wd
