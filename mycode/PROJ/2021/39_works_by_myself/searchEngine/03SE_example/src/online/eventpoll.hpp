#ifndef __RXL_EVENTPOLL_H__
#define __RXL_EVENTPOLL_H__
#include"eventfd.hpp"
#include"mutex.hpp"
#include"tcpconnect.hpp"
#include<sys/epoll.h>
#include<map>
#include<vector>
#include<memory>
#include<functional>
using std::map;
using std::vector;
using std::shared_ptr;
using std::function;
namespace rxl{
class Acceptor;
using TcpconnectionPtr = shared_ptr<TcpConnect>;
using Functor = function<void()>;
class EventPoll{
	using EpollCallBack = function<void(const TcpconnectionPtr&)>;
public:
	EventPoll(Acceptor&);
	void loop();
	void unloop();
	void setConnectCallBack(EpollCallBack&&);
	void setMessageCallBack(EpollCallBack&&);
	void setCloseCallBack(EpollCallBack&&);
	void pushInSolveList(Functor);
private:
	int createEpoll();
	void addEpoll(int);
	void delEpoll(int);
	void waitEpoll();
	bool _isClosed(int);
	void handleConnection();
	void handleMessage(int);
	EpollCallBack _onConnection;
	EpollCallBack _onMessage;
	EpollCallBack _onClose;
	void doPendingFunc();
private:	
	int _epfd;
	Acceptor& _acceptor;
	vector<epoll_event> _evtlist;
	map<int,TcpconnectionPtr> _tcpmap;
	bool _isLooping;
	vector<Functor> _funcs;
	Mutex _mutex;
	Eventfd _eventfd;
};

}//end of namespace rxl
#endif 
