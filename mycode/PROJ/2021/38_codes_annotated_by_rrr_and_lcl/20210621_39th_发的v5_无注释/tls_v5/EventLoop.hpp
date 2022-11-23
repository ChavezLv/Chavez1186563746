 ///
 /// @file    EventLoop.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-17 15:36:04
 ///
 
#ifndef __WD_EVENTLOOP_HPP__
#define __WD_EVENTLOOP_HPP__

#include "MutexLock.hpp"

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <memory>
#include <functional>

namespace wd
{

class Acceptor;

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using Functor = std::function<void()>;

class EventLoop
{
public:
	EventLoop(Acceptor & );
	void loop();
	void runInLoop(Functor && cb);
	void unloop();

	void setAllCallbacks(
		TcpConnectionCallback && cb1,
		TcpConnectionCallback && cb2,
		TcpConnectionCallback && cb3);
private:
	int createEpollfd();
	void addEpollReadfd(int fd);
	void delEpollReadfd(int fd);

	void waitEpollFd();
	void handleNewConnection();
	void handleMessage(int);

	int createEventfd();
	void handleRead();
	void wakeup();
	void doPendingFunctors();

private:
	int _efd;
	int _evfd;
	Acceptor & _acceptor;
	bool _isLooping;
	std::vector<struct epoll_event> _evtList;
	std::map<int, TcpConnectionPtr> _conns;

	std::vector<Functor> _pendingFunctors;
	MutexLock _mutex;

	TcpConnectionCallback _onConnectionCb;
	TcpConnectionCallback _onMessageCb;
	TcpConnectionCallback _onCloseCb;
};

}//end of namespace wd
 
 
 
#endif
