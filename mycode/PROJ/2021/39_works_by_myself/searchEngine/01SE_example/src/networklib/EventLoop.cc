#include "../../libs/Mylog.hh"
#include "../../include/networklib/EventLoop.hh"
#include "../../include/networklib/Acceptor.hh"
#include "../../include/networklib/MutexLock.hh"
#include "../../include/networklib/TcpConnection.hh"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <assert.h>

namespace wd
{

EventLoop::EventLoop(Acceptor & acceptor)
    : _efd(createEpollFd())
    , _eventfd(createEventfd())
    , _acceptor(acceptor)
    , _events(1024)
    , _isLooping(false)
{
    addEpollRdFd(_acceptor.fd());
    addEpollRdFd(_eventfd);
}

void EventLoop::loop()
{
    if(!_isLooping) {
        _isLooping = true;
    }
    while(_isLooping) {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    if(_isLooping) {
        _isLooping = false;
    }
}

// 在子线程中调用
void EventLoop::runInLoop(Functor && cb)
{
    {
        MutexGuard protec(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
}


int EventLoop::createEpollFd()
{
    int ret = ::epoll_create1(0);
    if(ret == -1) {
        logError("createEpollFd");
    }
    return ret;
}

void EventLoop::waitEpollFd()
{
    int nready;
    do {
        nready = epoll_wait(_efd, &_events[0], _events.size(), 5000);
    } while(nready == -1 && errno == EINTR);

    if(nready == -1) {
        logError("epoll_wait");
        return;
    } else if(nready == 0) {
        printf(">> epoll_wait timeout\n");
    } else {
        // vector满时需要进行手动扩容 否则下次新连接无法加入
        // 因为epoll_wait时限定了vector大小
        if(nready == static_cast<int>(_events.size())) {
            _events.resize(2 * nready);
        }
        
        for(int idx = 0; idx != nready; ++idx) {
            int fd = _events[idx].data.fd;
            if(_events[idx].events & EPOLLIN) {
                if(fd == _acceptor.fd()) {
                    handleNewConnection();
                } else if(fd == _eventfd) {
                    handleRead();
                    doPendingFunctors();
                } else {
                    handleMessage(fd);
                }
            }
        }
    }
}


void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    TcpConnectionPtr conn(new TcpConnection(peerfd, this));
    conn->setConnectionCallback(_onConnection);
    conn->setMessageCallback(_onMessage);
    conn->setCloseCallback(_onClose);

    _conns.insert(std::make_pair(peerfd, conn));
    addEpollRdFd(peerfd);

    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd)
{
    bool isClosed = connectionClosed(fd);
    auto it = _conns.find(fd);
    assert(it != _conns.end()); // 不存在则代表程序出错了

    if(isClosed) {
        delEpollRdFd(fd);
        it->second->handleCloseCallback();
        _conns.erase(it);
    } else {
        it->second->handleMessageCallback();
    }
}


void EventLoop::addEpollRdFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = ::epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &evt);
    if(ret == -1) {
        logError("addEpollRdFd");
    }
}

void EventLoop::delEpollRdFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = ::epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &evt);
    if(ret == -1) {
        logError("delEpollRdFd");
    }
}

// 当描述符就绪时检查是否有内容可读，没有则代表连接被断开了
bool EventLoop::connectionClosed(int fd)
{
    int ret;
    char buf[1];
    do {
        ret = recv(fd, buf, 1, MSG_PEEK);
    } while(ret == -1 && errno == EINTR);

    if(errno) {
        // 当客户端在没有接收服务器发送的内容就退出时
        // 会在这里出现错误
        puts(strerror(errno));
    }
    return (ret == 0 || ret == -1);
    /* return (ret == 0); */
}

int EventLoop::createEventfd()
{
    int fd = ::eventfd(0, 0);
    if(fd == -1) {
        logError("eventfd");
    }
    return fd;
}

void EventLoop::handleRead()
{
    uint64_t howmany;
    int ret = ::read(_eventfd, &howmany, sizeof(howmany));
    if(ret != sizeof(howmany)) {
        logError("handleRead");
    }
}

void EventLoop::wakeup()
{
    uint64_t one = 1;
    int ret = ::write(_eventfd, &one, sizeof(one));
    if(ret != sizeof(one)) {
        logError("wakeup");
    }
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> tmp;
    {
        MutexGuard protec(_mutex);
        // 常数时间的交换
        tmp.swap(_pendingFunctors);
    }

    for(auto & f: tmp) {
        f();
    }
}

} // end of namespace wd
