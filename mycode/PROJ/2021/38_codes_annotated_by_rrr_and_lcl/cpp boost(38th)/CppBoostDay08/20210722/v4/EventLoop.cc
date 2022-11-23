 ///
 /// @file    EventLoop.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-20 16:01:08
 ///
 

#include "EventLoop.hpp"
#include "Acceptor.hpp" 
#include "TcpConnection.hpp"

#include <unistd.h>
#include <sys/eventfd.h>

#include <iostream>
using std::cout;
using std::endl;

namespace wd
{
EventLoop::EventLoop(Acceptor & acceptr)
: _efd(createEpollfd())
, _eventfd(createEventfd())
, _acceptor(acceptr)
, _evtList(1024) 
, _isLooping(false)
{
	addEpollReadfd(_acceptor.fd());
	addEpollReadfd(_eventfd);
}

EventLoop::~EventLoop()
{
	close(_efd);
}

void EventLoop::loop()
{
	_isLooping = true;
	while(_isLooping) {
		waitEpollFd();
	}
}

void EventLoop::unloop()
{
	_isLooping = false;
}

void EventLoop::runInLoop(Functor && cb)
{
	{
	MutexLockGuard autolock(_mutex);
	_pendingFunctors.push_back(std::move(cb));
	}

	wakeup();
}


void EventLoop::waitEpollFd()
{
	int nready = -1;
	do {
		nready = epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), 5000);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1) {
		perror("epoll_wait");
		return;
	} else if(nready == 0) {
		printf(">> epoll_wait timeout!\n");
	} else {
		if(nready == _evtList.size()) {
			_evtList.resize(2 * nready);
		}

		for(int idx = 0; idx < nready; ++idx) {
			int fd = _evtList[idx].data.fd;
			if(fd == _acceptor.fd() && (_evtList[idx].events & EPOLLIN)) {
				handleNewConnection();
			} else if (fd == _eventfd) {
				handleRead();
				doPendingFunctors();			
			} else {
				if(_evtList[idx].events & EPOLLIN) {
					handleMessage(fd);
				}
			}
		}
	}
}

void EventLoop::handleNewConnection()
{
	int peerfd = _acceptor.accept();
	if(peerfd > 0) {
		TcpConnectionPtr conn(new TcpConnection(peerfd, this));
		conn->setAllCallbacks(_onConnection, _onMessage, _onClose);

		_conns.insert(std::make_pair(peerfd, conn));
		addEpollReadfd(peerfd);

		//执行连接建立时的回调函数(事件处理器)
		conn->handleConnectionCallback();
	}
}

void EventLoop::handleMessage(int fd)
{
	//1. 先判断一下该连接是否已经断开
	auto iter = _conns.find(fd);
	if(iter != _conns.end()) {
		bool isClosed = iter->second->isClosed();
		if(isClosed) {
			delEpollReadfd(fd);		
			iter->second->handleCloseCallback();
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
		perror("epoll_create1");
	}
	return fd;
}

void EventLoop::addEpollReadfd(int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;

	int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &ev);
	if(ret < 0) {
		perror("epoll_ctl");
	}
}

void EventLoop::delEpollReadfd(int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;

	int ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &ev);
	if(ret < 0) {
		perror("epoll_ctl");
	}
}

int EventLoop::createEventfd()
{
	int fd = eventfd(0, 0);
	if(fd < 0) {
		perror("eventfd");
	}
	return fd;
}

void EventLoop::handleRead()
{
	uint64_t howmany;
	int ret = read(_eventfd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror("read");
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	int ret = write(_eventfd, &one, sizeof(one));
	if(ret != sizeof(one)) {
		perror("write");
	}
}

void EventLoop::doPendingFunctors()
{
	cout << "doPendingFunctors()" << endl;
	vector<Functor> tmp;
	{
	MutexLockGuard autolock(_mutex);
	tmp.swap(_pendingFunctors);//O(1)  vector底层实现是3个指针
	}

	for(auto & functor : tmp) {
		functor();
	}
}

}//end of namespace wd
 
