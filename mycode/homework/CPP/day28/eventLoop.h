#ifndef __eventLoop_H__
#define __eventLoop_H__
#include"tcpConn.h"
#include"Acceptor.h"
#include"MutexLock.h"
#include<vector>
#include<map>
#include<functional>

class Acceptor;
using TcpConnectionPtr = shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
using Functor=std::function<void()>;
class EventLoop{
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop();
    int createEpollFd();
    int createEventFd();
    void runInLoop(Functor &&cb);
    void addEpollReadFd(int fd);
    void waitEpollFd();
    void handleMessage();
    void handleNewConnection();
    void setConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);
    void handleRead(); 
    void handleMessage(int fd);
    void deleteEpollReadFd(int fd);
    void doPengdingFunctors();
    void wakeup();
    void loop();
    void unloop();
private:
    int _epfd;
    int _eventfd;
    bool _isLooping;
    Acceptor& _acceptor;
    std::vector<struct epoll_event> _evtList;
    std::map<int,shared_ptr<TcpConnection>> _conns;

    TcpConnectionCallback  _onMsg;
    TcpConnectionCallback _onClose;
    TcpConnectionCallback _onConn;

    std::vector<Functor> _pendings; //待处理任务;
    MutexLock _mutex;
};





#endif
