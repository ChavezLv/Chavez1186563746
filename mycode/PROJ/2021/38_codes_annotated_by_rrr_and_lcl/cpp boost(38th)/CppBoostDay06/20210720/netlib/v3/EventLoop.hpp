 ///
 /// @file    EventLoop.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-20 15:54:51
 ///
 
#ifndef __WD_EVENTLOOP_HPP__
#define __WD_EVENTLOOP_HPP__

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <memory>
#include <functional>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;


namespace wd
{

class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>;

class Acceptor;

class EventLoop
{
public:
	EventLoop(Acceptor&);
	~EventLoop();

	void loop();
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
	void addEpollReadfd(int fd);
	void delEpollReadfd(int fd);

private:
	int _efd;
	Acceptor & _acceptor;

	vector<struct epoll_event> _evtList;
	map<int, TcpConnectionPtr> _conns;
	bool _isLooping;

	TcpConnectionCallback _onConnection;
	TcpConnectionCallback _onMessage;
	TcpConnectionCallback _onClose;
};

}//end of namespace wd

 
 
 
#endif
