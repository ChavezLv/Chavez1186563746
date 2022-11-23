/*================================================================
*   
*   @file       ：EventLoop.hpp
*   @date       ：2021年08月29日 23:21:23
*   @description：
*
================================================================*/


#ifndef __WD_EVENTLOOP_HPP__
#define __WD_EVENTLOOP_HPP__

#include "TcpConnection.hpp"
#include "Acceptor.hpp"
#include <sys/epoll.h>
#include <map>
#include <vector>

using std::vector;
using std::map;

namespace wd{
class Acceptor;

/* class TcpConnection; */
/* using TcpConnectionPtr = shared_ptr<TcpConnection>; */
/* using TcpConnectionCallback = function<void(const TcpConnectionPtr&)>; */

class EventLoop{
public:
    EventLoop(Acceptor &acceptor)
    : _efd(createEpollFd())
    , _acceptor(acceptor)
    , _isLooping(false)
    , _evtList(1024){
        addEpollFd(_acceptor.fd());
    }

    void loop();
    void unloop();

    void setAllCallbacks(TcpConnectionCallback && cb1, \
                         TcpConnectionCallback && cb2,
                         TcpConnectionCallback && cb3){
        _onConnection = std::move(cb1);
		_onMessage = std::move(cb2);
		_onClose = std::move(cb3);
    }

    ~EventLoop(); 
private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    
    int createEpollFd();
    void addEpollFd(int fd);
    void delEpollFd(int fd);
    

private:
    int _efd;
    Acceptor &_acceptor;
    bool _isLooping;
    vector<struct epoll_event> _evtList;
	map<int, TcpConnectionPtr> _conns;

    TcpConnectionCallback _onConnection;
	TcpConnectionCallback _onMessage;
	TcpConnectionCallback _onClose;
};

}//end of namespace wd

#endif //EVENTLOOP_HPP
