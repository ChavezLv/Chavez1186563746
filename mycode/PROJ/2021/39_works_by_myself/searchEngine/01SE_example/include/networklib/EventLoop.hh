/* <EventLoop> -*- C++ -*- */
#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "MutexLock.hh"

#include <sys/epoll.h>

#include <vector>
#include <map>
#include <functional>
#include <memory>

namespace wd
{

class Acceptor;
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using Callback = std::function<void(const TcpConnectionPtr&)>;
using Functor = std::function<void()>;

class EventLoop
{
public:
    EventLoop(Acceptor & acceptor);

    void loop();
    void unloop();

    void setNewConnectionCallback(Callback && cb)
    { _onConnection = std::move(cb); }
    void setMessageCallback(Callback && cb)
    { _onMessage = std::move(cb); }
    void setCloseCallback(Callback && cb)
    { _onClose = std::move(cb); }

    void runInLoop(Functor && cb);

private:
    int createEpollFd();
    void waitEpollFd();

    void handleNewConnection();
    void handleMessage(int fd);

    void addEpollRdFd(int fd);
    void delEpollRdFd(int fd);

    bool connectionClosed(int fd);

    // 线程间通信
    int createEventfd();
    void handleRead();
    void wakeup();
    void doPendingFunctors();

private:
    int _efd;
    int _eventfd;
    Acceptor & _acceptor;
    std::map<int, TcpConnectionPtr> _conns;
    std::vector<struct epoll_event> _events;
    bool _isLooping;

    std::vector<Functor> _pendingFunctors;
    MutexLock _mutex;

    Callback _onConnection;
    Callback _onMessage;
    Callback _onClose;
};

}
#endif

