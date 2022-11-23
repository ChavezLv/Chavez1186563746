/*================================================================
*   Copyright (C) 2021 ゞゞゞ All rights reserved.
*   
*   @file       ：EventLoop.cpp
*   @author     ：ゞゞゞ
*   @date       ：2021年08月29日 23:38:57
*   @description：
*
================================================================*/


#include "EventLoop.hpp"
#include <unistd.h> //close()
#include <iostream>

namespace wd{

EventLoop::~EventLoop(){
    if(0 != ::close(_efd)){
        perror("close");
    }
}

void EventLoop::loop(){
    _isLooping = true;
    while(_isLooping){
        waitEpollFd();
    }
}

void EventLoop::unloop(){
    _isLooping = false;
}

void EventLoop::waitEpollFd(){
    int epRet = -1;
    do{
        epRet = epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), 3000);
    }while(-1 == epRet && errno == EINTR);

    if(-1 == epRet){
        perror("epoll_wait");
        return;
    }else if(0 == epRet){
        operator<<(std::cout, "epoll_wait: Timeout\n");
    }else{
        if(epRet == (int)_evtList.size()){
            //如果发现一次触发的文件描述符太多了,进行一次扩容
            _evtList.resize(2 * epRet);
        }

        int fd = 0;
        for(int idx = 0; idx < epRet; ++idx){
            fd = _evtList[idx].data.fd;
            if(fd == _acceptor.fd() && (_evtList[idx].events & EPOLLIN)){
                handleNewConnection();
            }else{
                if(_evtList[idx].events & EPOLLIN){
                    handleMessage(fd);
                }
            }
        }//end of for(int idx = 0; idx < epRet; ++idx){  }
    }
}

void EventLoop::handleNewConnection(){
    int peerFd = _acceptor.accept();
    if(peerFd > 0){
        TcpConnectionPtr conn(new TcpConnection(peerFd));
        conn->setAllCallbacks(_onConnection, _onMessage, _onClose);
        _conns.insert(std::make_pair(peerFd, conn));

        addEpollFd(peerFd);
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
		if(isClosed) { //TcpConnection可以判断连接fd是否已经断开
			delEpollFd(fd);	//从监听里面删除	
			iter->second->handleCloseCallback();//执行连接断开的回调函数
			_conns.erase(iter); //_conns也删除
		} else {
			iter->second->handleMessageCallback();
		}
	}
}


int EventLoop::createEpollFd()
{
	int fd = ::epoll_create1(0);
	if(fd < 0) {
		perror("epoll_create1");
	}
	return fd;
}

void EventLoop::addEpollFd(int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;

	int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &ev);
	if(ret < 0) {
		perror("epoll_ctl");
	}
}

void EventLoop::delEpollFd(int fd)
{
	struct epoll_event ev;
	ev.data.fd = fd;

	int ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &ev);
	if(ret < 0) {
		perror("epoll_ctl");
	}
}

}//end of namespace wd

