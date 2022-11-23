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
, _parsemsgfd(createEventfd()) 
, _acceptor(acceptor)
, _isLooping(false)
, _evtList(1024)
{
    addEpollReadFd(_acceptor.fd());
	addEpollReadFd(_eventfd);
	addEpollReadFd(_parsemsgfd);
}

EventLoop::~EventLoop()
{	
	close(_efd);	
	close(_eventfd);
    close(_parsemsgfd);
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
	wakeup(_eventfd);//因为事件相关的fd现在有两个了
}

	       /* void parseInLoop(const std::pair<int,  std::pair<int, std::string> > &msg); */
void EventLoop::parseInLoop(const std::pair<int , std::pair<int, std::string> >&msg){
    {
    MutexLockGuard autolock(_mutex);
    _pendingParsers.push_back(std::move(msg));
    } 
    wakeup(_parsemsgfd);
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
					handleRead(fd);
					doPendingFunctors();
					cout << "...doPendingFunctors()" << endl;
				}
            } else if(fd == _parsemsgfd){
				if(_evtList[idx].events & EPOLLIN) {
					handleRead(fd);
					/* doPendingParsers(); */
                    handleParsers();
					cout << "...handleParsers()" << endl;
				}
                
            }
			else {
				if(_evtList[idx].events & EPOLLIN) {
					handleMessage(fd);
std::cout<< "handleNewConnection()-> msgfd" << fd << endl;
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
std::cout<< "handleNewConnection()" << peerfd << endl;

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	conn->setAllCallbacks(_onConnection, _onMessage, _onClose, _onParsers);
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

void EventLoop::handleParsers(){
    vector<std::pair<int, std::pair<int, std::string> > > tmp;
    {
    MutexLockGuard autolock(_mutex);
    _pendingParsers.swap(tmp);
    }


    for(auto &parse: tmp){
        auto iter = _conns.find(parse.first);
        if(iter != _conns.end()) {
            //判断连接是否已经断开了
cout << "parseInLoop() : _parsemsgfd: parseeventfd " << _parsemsgfd << " parsefd" << parse.first << " "<< iter->second->fd() << endl;// ok
            bool isClosed = iter->second->isClosed();
cout << "parseInLoop() : _parsemsgfd: parseeventfd " << _parsemsgfd << " parsefd" << parse.first << " "<< iter->second->fd() << endl;// ok
            if(isClosed) {
                //已经断开
                iter->second->handleCloseCallback();
                delEpollReadFd(parse.first);
                _conns.erase(parse.first);
            } else {
                //没有断开，处理消息
                iter->second->handleParsersCallback(parse.second);
            }
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

void EventLoop::handleRead(const int& eventfd)
{
	uint64_t howmany = 0;
	int ret = read(eventfd, &howmany, sizeof(howmany));
	if(ret != sizeof(howmany)) {
		perror("read");
	}
}

//该方法与start方法运行在不同的线程，执行通知操作
void EventLoop::wakeup(const int & eventfd)
{
	uint64_t one = 1;
	int ret = write(eventfd, &one, sizeof(one));
	if(ret != sizeof(one)) {
		perror("write");
	}
}

/* void EventLoop::doPendingParsers(){ */
/*     vector<std::pair<int, string> tmp; */
/*     { */
/*     MutexLockGuard autolock(_mutex); */
/*     _pendingParsers.swap(tmp); */
/*     } */
/*     for(auto &parsers: tmp){ */
/*         if(parsers.first == 1){ */
/*             bind() */
/*         } */ 
/*     } */
/* } */

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
