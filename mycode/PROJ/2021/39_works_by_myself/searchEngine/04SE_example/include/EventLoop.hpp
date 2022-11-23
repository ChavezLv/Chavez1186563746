#ifndef __EVENTLOOP_HPP__
#define __EVENTLOOP_HPP__

#include "MutexLock.hpp"

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <memory>
#include <functional>

using std::shared_ptr;
using std::function;
using std::map;
using std::vector;

namespace spellCorrect
{

class Acceptor;
class TcpConnection;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr& conn)>;
using Functor = function<void()>;

class EventLoop
{
public:
    EventLoop(Acceptor& acceptor);
    ~EventLoop();

    void loop();
    void unloop();

    void setConnectionCallback(TcpConnectionCallback && cb);
    void setMessageCallback(TcpConnectionCallback && cb);
    void setCloseCallback(TcpConnectionCallback && cb);

    void addFunctor(Functor && cb);
private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);//connFd
    
    int createEpollFd();
    void addEpollReadFd(int fd);//add connFd
    void delEpollReadFd(int fd);//del connFd

    int createEventfd();
    void wakeup();
    void handleRead();

    void handleFunctor();
private:
    int _efd;
    Acceptor& _acceptor;
    bool _isLooping;

    map<int,TcpConnectionPtr> _connMap;
    vector<struct epoll_event> _evsList;
    
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;

    vector<Functor> _pendingFunctorList;
    MutexLock _mutex;

    int _eventfd;
};

}//end of namespace spellCorrect

#endif
