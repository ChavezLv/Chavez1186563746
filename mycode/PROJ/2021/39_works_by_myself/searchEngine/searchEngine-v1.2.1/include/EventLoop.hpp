/*================================================================
*   
*   @file       ：EventLoo.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
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
using TcpConnectionCallbackParse = function<void(const TcpConnectionPtr&, const std::pair<int, std::string>& )>;
using Functor = function<void()>;

class Acceptor;
class EventLoop
{
public:
	EventLoop(Acceptor & acceptor);
	~EventLoop();

	void loop();
	void runInLoop(Functor &&);
	void parseInLoop(const std::pair<int, std::pair<int, std::string> > &msg);
	void unloop();
	void setAllCallbacks(TcpConnectionCallback && cb1,
			TcpConnectionCallback && cb2,
			TcpConnectionCallback && cb3,
            TcpConnectionCallbackParse && cb4)
	{
		_onConnection = std::move(cb1);
		_onMessage = std::move(cb2);
		_onClose = std::move(cb3);
		_onParsers = std::move(cb4);
	}

private:
	void waitEpollFd();
	void handleNewConnection();
	void handleMessage(int fd);
	void handleParsers();

	int createEpollfd();
	void addEpollReadFd(int fd);
	void delEpollReadFd(int fd);

	int createEventfd();
	int createParsefd();
	void handleRead(const int &eventfd);
	void wakeup(const int &eventfd);
	void doPendingFunctors();
	/* void doPendingParsers(); */

private:
	int _efd;
	int _eventfd;
	int _parsemsgfd;

	Acceptor & _acceptor;
	bool _isLooping;

	vector<struct epoll_event> _evtList;
	map<int, TcpConnectionPtr> _conns;
	vector<Functor> _pendingFunctors;
	vector<std::pair<int, std::pair<int, std::string> > > _pendingParsers;//type, <fd, msg>
	MutexLock _mutex;

	//这三个回调函数是为了转交给TcpConnection对象的
	TcpConnectionCallback _onConnection;
	TcpConnectionCallback _onMessage;
	TcpConnectionCallback _onClose;
	TcpConnectionCallbackParse _onParsers;
};

}//end of namespace wd
 
 
 
#endif
