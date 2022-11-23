#include "../../include/EventLoop.hpp"
#include "../../include/Acceptor.hpp"
#include "../../include/TcpConnection.hpp"

#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/eventfd.h>

#include <iostream>

using std::cout;
using std::endl;

namespace spellCorrect
{

EventLoop::EventLoop(Acceptor& acceptor)
    :_efd(createEpollFd())
     ,_acceptor(acceptor)
     ,_isLooping(false)
     ,_evsList(1024)
     ,_eventfd(createEventfd())
    {
        addEpollReadFd(_acceptor.fd());
        addEpollReadFd(_eventfd);
        cout << "_evsList.size() = " << _evsList.size() << endl;
        cout << "_acceptor.fd() = " << _acceptor.fd() << endl;
    }

EventLoop::~EventLoop()
{
    ::close(_efd);
    ::close(_eventfd);
}

void EventLoop::loop()
{
    if(!_isLooping)
    {
        _isLooping = true;
    }
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    if(_isLooping)
    {
        _isLooping = false;
    }
}

void EventLoop::setConnectionCallback(TcpConnectionCallback && cb)
{
    _onConnection = cb;
}

void EventLoop::setMessageCallback(TcpConnectionCallback && cb)
{
    _onMessage = cb;
}

void EventLoop::setCloseCallback(TcpConnectionCallback && cb)
{
    _onClose = cb;
}

void EventLoop::addFunctor(Functor && cb)
{
    {
        MutexLockGuard autolock(_mutex);
        _pendingFunctorList.push_back(cb);
    }
    wakeup();
}

void EventLoop::waitEpollFd()
{
    int nready = 0;
    do
    {
        nready = epoll_wait(_efd,&*_evsList.begin(),_evsList.size(),5000);
    }while(-1 == nready && errno == EINTR);

    if(-1 == nready)
    {
        perror("epoll_wait");
    }
    else if(0 == nready)
    {
        cout << "epoll time out" << endl;
    }
    else
    {
        if(nready == _evsList.size())
        {
            _evsList.resize(2 * nready);
        }

        for(int i = 0; i != nready; ++i)
        {
            int fd = _evsList[i].data.fd;
            if(fd == _acceptor.fd())
            {
                //新连接
                if(_evsList[i].events & EPOLLIN)
                {
                    handleNewConnection();
                }
            }
            else if(fd == _eventfd)
            {
                handleRead();
                handleFunctor();
            }
            else 
            {
                if(_evsList[i].events & EPOLLIN)
                {
                    handleMessage(fd);
                }
            }
        }
    }
}

void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    addEpollReadFd(peerfd);
    TcpConnectionPtr conn(new TcpConnection(peerfd,this));
    conn->setNonblock();
    conn->setConnectionCallback(_onConnection);
    conn->setMessageCallback(_onMessage);
    conn->setCloseCallback(_onClose);
    _connMap.insert(make_pair(peerfd,conn));
    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    auto iter = _connMap.find(fd);
    assert(iter != _connMap.end());

    //判断是否连接断开
    if(iter->second->isClosed())
    {
        cout << ">> TcpConnection " << fd << " has Closed." << endl;
        delEpollReadFd(fd);
        _connMap.erase(iter);
    }
    else
    {
        cout << ">> handle Message fd:" << fd << endl;
        iter->second->handleMessageCallback();
    }
}

int EventLoop::createEpollFd()
{
    int efd = ::epoll_create1(0);
    if(-1 == efd)
    {
        perror("epoll_create1");
    }
    return efd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event value;
    value.data.fd = fd;
    value.events = EPOLLIN;
    int ret = ::epoll_ctl(_efd,EPOLL_CTL_ADD,fd,&value);
    if(-1 == ret)
    {
        perror("epoll_ctl_add");
    }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event value;
    value.data.fd = fd;
    int ret = ::epoll_ctl(_efd,EPOLL_CTL_DEL,fd,&value);
    if(-1 == ret)
    {
        perror("epoll_ctl_del");
    }
}

int EventLoop::createEventfd()
{
    int fd = ::eventfd(0,0);
    if(fd < 0)
    {
        perror("eventfd");
    }
    return fd;
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = ::write(_eventfd,&one,sizeof(one));
    if(ret != sizeof(one))
    {
        perror("write");
    }
}

void EventLoop::handleRead()
{
    uint64_t howmany = 0;
    int ret = ::read(_eventfd,&howmany,sizeof(howmany));
    if(ret != sizeof(howmany))
    {
        perror("read");
    }
}

void EventLoop::handleFunctor()
{
    vector<Functor> temp;
    {
        MutexLockGuard autolock(_mutex);
        temp.swap(_pendingFunctorList);
    }
    for(auto& functor:temp)
    {
        functor();
    }
}


}//end of namespace spellCorrect

