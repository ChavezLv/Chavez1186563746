#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__

#include "Acceptor.h"
#include "MutexLock.hpp"
#include <sys/epoll.h>
#include <map>
#include <vector>
#include <memory>
#include <functional>

using std::map;
using std::vector;
using std::shared_ptr;
using std::function;

namespace wd{
class TcpConnection;
class EventLoop
{
public:
    using functor=function<void()>;
    using TcpConnectionPtr=shared_ptr<TcpConnection>;
    using CallBack=function<void(const TcpConnectionPtr&)>;
    EventLoop(Acceptor&);
    
    void loop();
    void unloop();
    

    void runInLoop(functor&&);
    

    void setNewConnectionCallBack(const CallBack& cb){
        _newConCallBack=cb;
    }
    void setMsgCallBack(const CallBack& cb){
        _msgCallBack=cb;
    }
    void setCloseCallBack(const CallBack& cb){
        _CloCallBack=cb;
    }
    ~EventLoop() {}

private:
    void epollWait();
    void handleNewConnection();
    void handleMessage(int);
    int createEpollfd();
    
    int createEventFd();
    void wakeup();
    void handleRead();
    void doPendingFunction();
    
    void addepollFd(int);
    void delepollFd(int);
    bool isConnectioning(int fd);
private:
    CallBack _newConCallBack;
    CallBack _msgCallBack;
    CallBack _CloCallBack;

    int _epfd;
    int _eventfd;
    
    vector<functor> _PendingFunction;
    vector<struct epoll_event> _listevs;
    MutexLock _mutex;    

    map<int,TcpConnectionPtr> _conns;
    Acceptor& _accept;
    bool _islooping;
};

}
#endif

