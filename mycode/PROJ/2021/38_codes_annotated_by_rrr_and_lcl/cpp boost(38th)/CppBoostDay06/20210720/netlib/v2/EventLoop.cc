 ///
 /// @file    EventLoop.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-20 16:01:08
 ///
 

#include "EventLoop.hpp"
#include "Acceptor.hpp" 
#include "TcpConnection.hpp"

#include <unistd.h>

namespace wd
{
EventLoop::EventLoop(Acceptor & acceptr)
: _efd(createEpollfd())
, _acceptor(acceptr)
, _evtList(1024) 
, _isLooping(false)
{
	addEpollReadfd(_acceptor.fd());
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


void EventLoop::waitEpollFd()
{
	int nready = -1;
	do {
		//lcl: 因为是vector, 不能直接vector的名字! 
		nready = epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), 5000);
	}while(nready == -1 && errno == EINTR);

	if(nready == -1) {
		perror("epoll_wait");
		return;
	} else if(nready == 0) {
		printf(">> epoll_wait timeout!\n");
	} else {
		if(nready == _evtList.size()) {// (lcl:)如果发现一次触发的文件描述符太多了, 最好进行一次扩容
			_evtList.resize(2 * nready);//(lcl:)调用vector的resize方法,对其进行扩容
		}

		for(int idx = 0; idx < nready; ++idx) {//(lcl:)一个个进行遍历
			int fd = _evtList[idx].data.fd;
			if(fd == _acceptor.fd() && (_evtList[idx].events & EPOLLIN)) {
				handleNewConnection();//(lcl:)如果是sfd响应了, 并且此时的事件是读事件
			} else {
				if(_evtList[idx].events & EPOLLIN) {
					handleMessage(fd);// (lcl:)否则就是别的fd(newfd响应了)
				}
			}
		}
	}
}

void EventLoop::handleNewConnection()
{
	//注意思路: accept()->注册读就绪事件-> 处理回调函数(去写回调函数)
	int peerfd = _acceptor.accept();//(lcl:)新连接请求, 就是sfd响应的时候, 需要accept
	if(peerfd > 0) {
		TcpConnectionPtr conn(new TcpConnection(peerfd));
		//因为这个不好传参,所以在数据成员里面去写
		conn->setAllCallbacks(_onConnection, _onMessage, _onClose);//(lcl:)注册回调函数

		_conns.insert(std::make_pair(peerfd, conn));
		addEpollReadfd(peerfd);

		//执行连接建立时的回调函数(事件处理器)
		conn->handleConnectionCallback();
	}
}

void EventLoop::handleMessage(int fd)
{
	//1. 先判断一下该连接是否已经断开
	auto iter = _conns.find(fd);// 先查找到相应的fd
	if(iter != _conns.end()) {//判断是否找到
		//要给TcpConnection对象增加一个判断, 判断是否已经断开
		bool isClosed = iter->second->isClosed();//shared_ptr的的方法
		if(isClosed) { //疑问: 为什么TcpConnection可以判断连接fd是否已经断开
			delEpollReadfd(fd);	//从监听里面删除	
			iter->second->handleCloseCallback();执行连接断开的回调函数
			_conns.erase(iter); //_conns也删除
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
}//end of namespace wd
 
