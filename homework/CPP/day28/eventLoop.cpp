#include"eventLoop.h"
#include<sys/eventfd.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<iostream>
EventLoop::EventLoop(Acceptor &acceptor)
    :_epfd(createEpollFd())
    ,_eventfd(createEventFd())
    ,_isLooping(false)
    ,_evtList(1024)
    ,_acceptor(acceptor){
        addEpollReadFd(_acceptor.fd());
        addEpollReadFd(_eventfd);
    }

void EventLoop::waitEpollFd(){
    int nready = 0;
    do
    {
        //需要一个epoll_even[]数组来存放返回结果;返回的文件描述符数目就是nready;
        nready = epoll_wait(_epfd, &*_evtList.begin(), _evtList.size(), 5000);//第二个参数的传递,5000ms的延时等待;
    }while(-1 == nready && errno == EINTR);
   
    if(-1 == nready)
    {
        perror("epoll_wait() ERROR");
        return;
    }
    else if(0 == nready)
    {
        printf(">>epoll_wait() timeout\n");
    }
    else
    {
        if(nready == (int)_evtList.size())
        {
            _evtList.resize(2 * nready);
        }
        for(int idx = 0; idx < nready; ++idx)
        {
            std::cout << "nreay = " << nready << std::endl; 
            int fd = _evtList[idx].data.fd;
            if(fd == _acceptor.fd())
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    std::cout<<"handleNewConnection"<<std::endl;
                    handleNewConnection();
                }
            }
            else if(fd == _eventfd)//监听_eventfd有事件
            {
                handleRead();//阻塞点
                //如果_eventfd对端没有写入数据,则在此阻塞,don`t do all pending
                //遍历vector;多个回调函数全部执行一遍?
                doPengdingFunctors();//执行所有任务
            }
            else
            {
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleMessage(fd);//老的连接上有数据
                }

            }
        }
    }
}

void EventLoop::handleRead(){
    uint64_t one=1;
    //_eventfd的读端;_eventfd--->one;
    int ret=read(_eventfd,&one,sizeof(one));
    if(ret!=sizeof(one)){
        perror("_eventfd: read()");
        return;
    }
}


void EventLoop::wakeup()
{
    uint64_t one = 1;
    //_eventfd的写端;
    int ret = write(_eventfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("write");
        return;
    }
}


void EventLoop::runInLoop(Functor &&cb)
{
    {
        /*cb=std::bind(&TcpConnection::send,this,msg)*/
        MutexLockGuard autoLock(_mutex);
        _pendings.push_back(std::move(cb));
    }
    /*解除阻塞,此时还是在子线程中,需要通知运行在父线程中的loop中的dopendingFunctors()去执行实际发送任务*/
    wakeup();
}

void EventLoop::doPengdingFunctors()
{
    std::vector<Functor> tmp;
    {
        MutexLockGuard autoLock(_mutex);
        //不希望在复制vector的时候被其它线程打断;
        tmp.swap(_pendings);
    }
    //使用临时vector遍历函数,如果有新的任务,可以插入到已经空的_pendings
    for(auto &cb : tmp)
    {
        cb();
    }
}
void EventLoop::deleteEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.events = EPOLLIN;
    evt.data.fd = fd;
    //为什么不把evt传进来而要新建一个临时evt?
    //答:主调函数也没有evt,麻烦;  怎么传evt进来?
    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(-1 == ret)
    {
        perror("epoll_ctl deleteEpollReadFd()");
        return;
    }
}
void EventLoop::handleMessage(int fd){
    auto it =_conns.find(fd);//根据键值从已建立的连接队列里查找;
    if(it!=_conns.end()){
        bool flag=it->second->isClosed();
        if(flag){
            //关闭连接为什么不用close(fd),移除节点
            it->second->handleCloseCallback();//TcpConnection中执行回调函数
            deleteEpollReadFd(fd);
            _conns.erase(it);
        }else{
            it->second->handleMessageCallback();
        }
    }else{
        std::cout<<"doesn`t exsit this TCP link"<<std::endl;
    }
}

void EventLoop::handleNewConnection(){
    std::cout << "2222" << std::endl;
    int peerfd=_acceptor.accept();
    if(peerfd==-1){
        perror("_acceptor.accept()");
        return;
    }
    std::cout << "1111" << std::endl;
    addEpollReadFd(peerfd);
    TcpConnectionPtr con(new TcpConnection(peerfd,this));

    /*error:  _conns.push_back(std::make_pair(peerfd,shared_ptr(con))); */
    con->setConnectionCallback(_onConn);
    con->setMessageCallback(_onMsg);
    con->setCloseCallback(_onClose);
    //调用tcpConn类的方法
    _conns.insert(std::make_pair(peerfd,con));
    /* auto it=_conns.end(); */
    /* std::cout<<"the last of _conns:"<<it->first<<std::endl; */
    con->handleConnectionCallback();
}

void EventLoop::setConnectionCallback(TcpConnectionCallback &&cb)
{
    /*传入的右值使用std::move转移*/
    _onConn = std::move(cb);
}

void EventLoop::setMessageCallback(TcpConnectionCallback &&cb)
{
    _onMsg = std::move(cb);
}
void EventLoop::setCloseCallback(TcpConnectionCallback &&cb)
{
    _onClose = std::move(cb);
}
void EventLoop::addEpollReadFd(int fd){
    struct epoll_event evt;
    evt.events=EPOLLIN;
    evt.data.fd=fd;
    int ret =epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&evt);
    if(-1==ret){
        perror("epoll_ctl() in addEpollReadFd() ");
        return;
    }
}
int EventLoop::createEpollFd(){
    
    int ret= epoll_create1(0);
    if(ret< 0){
        perror("epoll_create");
        return ret;
    }
    return  ret;
}
int EventLoop::createEventFd(){
    int fd=eventfd(10,0);
    if(fd<0){
        perror("eventfd()");
        return fd;
    }
    return fd;
}
void EventLoop::loop()
{
    _isLooping = true;
    while(_isLooping)
    {
        waitEpollFd();
    }
}

void EventLoop::unloop()
{
    _isLooping = false;
}
EventLoop::~EventLoop(){
    close(_epfd);
    close(_eventfd);
}
