 ///
 /// @file    EventLoop.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-28 14:34:56
 ///
 
#ifndef __WD_EVENTLOOP_HPP__
#define __WD_EVENTLOOP_HPP__

#include "MutexLock.hpp"

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <memory>
#include <functional>

using std::map;
using std::vector;
using std::function;
using std::shared_ptr;

namespace wd
{

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;
using Functor = function<void()>;

class Acceptor;
class EventLoop
{
public:
	EventLoop(Acceptor & acceptor);
	~EventLoop();

	void loop();
	void runInLoop(Functor &&);
	void unloop();
	void setAllCallbacks(TcpConnectionCallback && cb1,
			TcpConnectionCallback && cb2,
			TcpConnectionCallback && cb3)
	{
		_onConnection = std::move(cb1);
		_onMessage = std::move(cb2);
		_onClose = std::move(cb3);
	}

private:
	void waitEpollFd();
	void handleNewConnection();
	void handleMessage(int fd);

	int createEpollfd();
	void addEpollReadFd(int fd);
	void delEpollReadFd(int fd);

	int createEventfd();
	void handleRead();
	void wakeup();
	void doPendingFunctors();

private:
	int _efd;
	int _eventfd;
	Acceptor & _acceptor;
	bool _isLooping;

	vector<struct epoll_event> _evtList;
	map<int, TcpConnectionPtr> _conns;
	vector<Functor> _pendingFunctors;//v4
	MutexLock _mutex;//v4

	//这三个回调函数是为了转交给TcpConnection对象的
	TcpConnectionCallback _onConnection;
	TcpConnectionCallback _onMessage;
	TcpConnectionCallback _onClose;
};

}//end of namespace wd
 
 
 
#endif
