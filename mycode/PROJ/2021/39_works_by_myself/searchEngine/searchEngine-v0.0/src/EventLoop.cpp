/*================================================================
*   
*   @file       ：EventLoo.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "EventLoop.hpp"
#include "Acceptor.hpp"
#include "TcpConnection.hpp"

#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <sys/eventfd.h>

#include <iostream>
using std::cout;
using std::endl;
 
namespace wd
{

EventLoop::EventLoop(Acceptor & acceptor)
: _efd(createEpollfd())
, _eventfd(createEventfd())
, _acceptor(acceptor)
, _isLooping(false)
, _evtList(1024)
{
	addEpollReadFd(_acceptor.fd());
	addEpollReadFd(_eventfd);
}

EventLoop::~EventLoop()
{	
	close(_efd);	
	close(_eventfd);
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
{	_isLooping = false;	}


void EventLoop::waitEpollFd()
{
	int nready = 0;
	do {
		nready = ::epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), 5000);
	} while(nready == -1 && errno == EINTR);

	if(0 == nready) {
		printf(">>epoll_wait timeout\n");
	} else if (-1 == nready) {
		perror("epoll_wait");
		return;
	} else {
		if(nready == _evtList.size()) {
			//进行扩容操作
			_evtList.resize(2 * nready);
		}
		
		for(int idx = 0; idx != nready; ++idx) {
			int fd = _evtList[idx].data.fd;
			if(fd == _acceptor.fd()) {
				if(_evtList[idx].events & EPOLLIN) {
					handleNewConnection();
				}
			} else if(fd == _eventfd) {
				if(_evtList[idx].events & EPOLLIN) {
					handleRead();
					doPendingFunctors();
					cout << "...doPendingFunctors()" << endl;
				}
			} 
			else {
				if(_evtList[idx].events & EPOLLIN) {
					handleMessage(fd);
				}
				if(_evtList[idx].events & EPOLLOUT) {
					//...
				}
			}
		}
	}
}

void EventLoop::handleNewConnection()
{
	int peerfd = _acceptor.accept();
	addEpollReadFd(peerfd);//注册读就绪事件

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	conn->setAllCallbacks(_onConnection, _onMessage, _onClose);
	_conns.insert(std::make_pair(peerfd, conn));

	//新连接建立时，要执行的回调函数
	conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
	//先查找到相应的TcpConnection对象
	auto iter = _conns.find(fd);
	if(iter != _conns.end()) {
		//判断连接是否已经断开了
		bool isClosed = iter->second->isClosed();
		if(isClosed) {
			//已经断开
			iter->second->handleCloseCallback();
			delEpollReadFd(fd);
			_conns.erase(fd);
		} else {
			//没有断开，处理消息
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

void EventLoop::addEpollReadFd(int fd)
{
	struct epoll_event value;
	value.events = EPOLLIN;
	value.data.fd = fd;
	int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &value);
	if(ret < 0) {
		perror("epoll_ctl");
	}
}


void EventLoop::delEpollReadFd(int fd)
{
	struct epoll_event value;
	value.events = EPOLLIN;
	value.data.fd = fd;
	int ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &value);
	if(ret < 0) {
		perror("epoll_ctl");
	}
}

int EventLoop::createEventfd()
{
	int fd = ::eventfd(0, 0);
	if(fd < 0) {
		perror("evetfd");
	}
	return fd;
}

void EventLoop::handleRead()
{
	uint64_t howmany = 0;
	int ret = read(_eventfd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror("read");
	}
}

//该方法与start方法运行在不同的线程，执行通知操作
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
	vector<Functor> tmp;
	{
	MutexLockGuard autolock(_mutex);
	_pendingFunctors.swap(tmp); // O(1)
	}

	for(auto & functor : tmp) {
		functor();
	}
}

}//end of namespace wd
