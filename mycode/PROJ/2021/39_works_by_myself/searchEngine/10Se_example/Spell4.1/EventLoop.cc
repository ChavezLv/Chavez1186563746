#include "EventLoop.hpp"
#include "TcpConnection.h"

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/eventfd.h>

#include <iostream>

using std::cout;
using std::endl;
namespace wd{

EventLoop::EventLoop(Acceptor& accept)
:_epfd(createEpollfd())
,_eventfd(createEventFd())
,_listevs(1024)
,_accept(accept)
,_islooping(false)
{
    addepollFd(_accept.getfd());
    addepollFd(_eventfd);
}
void EventLoop::loop(){
    if(!_islooping){
        /* cout<<"in loop()"<<endl; */
        _islooping=true;
        while(_islooping){
            epollWait();
        }
    }
}

void EventLoop::unloop(){
    _islooping=false;
}

void EventLoop::epollWait(){
   /* cout<<"in epollWait()"<<endl; */
   int ret;
   do{
        ret=epoll_wait(_epfd,&*_listevs.begin(),_listevs.size(),5000);
   }while(ret==-1&&errno==EINTR);
   if(ret==-1){
       perror("epoll_wait");
        return;
   }
   else if(ret==0){
       cout<<"timewait!"<<endl;
   }
   else{
       if(ret==(int)_listevs.size()){
           _listevs.resize(2*_listevs.size());
       }
       for(int i=0;i<ret;i++){
           int fd=_listevs[i].data.fd;
           //如果是新连接
           if(fd==_accept.getfd()){
               /* printf("----new linked conme!----"); */
               if(_listevs[i].events&EPOLLIN){
                    handleNewConnection();
               }
           }
           else if(fd==_eventfd){
               if(_listevs[i].events& EPOLLIN){
                   printf("... process eventfd....\n");
                   handleRead();
                   doPendingFunction();
               }
           }
           else{
               if(_listevs[i].events&EPOLLIN){
                    handleMessage(fd);
               }
           }
       }
   }
}
void EventLoop::handleNewConnection(){
    int peerfd=_accept.accept();
    addepollFd(peerfd);

    TcpConnectionPtr conn(new TcpConnection(peerfd,this));
    conn->setNewConnectionCallBack(_newConCallBack);
    conn->setMsgCallBack(_msgCallBack);
    conn->setCloseCallBack(_CloCallBack);

    _conns.insert(make_pair(peerfd,conn));

    conn->handleNewConnectionCallBack();
}

void EventLoop::handleMessage(int fd){
    auto iter=_conns.find(fd);
    if(!isConnectioning(fd)){
        iter->second->handleCloseCallBack();
        delepollFd(fd);
        _conns.erase(iter);
    }
    else{
        /* cout<<"new msg come!"<<endl; */
        iter->second->handleMessageCallBack();
    }
}

void EventLoop::runInLoop(functor&& cb){
    {
        MutexLockGuard mutex(_mutex);
        _PendingFunction.push_back(std::move(cb));
    }
    wakeup();
}
void EventLoop::wakeup(){
    uint64_t one=1;
    int ret=::write(_eventfd,&one,sizeof(one));
    if(ret!=sizeof(one)){
        perror("write");
    }
}

void EventLoop::handleRead(){
    uint64_t  howmany;
    int ret=::read(_eventfd,&howmany,sizeof(howmany));
    if(ret<0){
        perror("read");
    }
}

void EventLoop::doPendingFunction(){
    vector<functor> tmp;
    {
        MutexLockGuard mutex(_mutex);
        swap(tmp,_PendingFunction);
    }
    for(auto &f:tmp){
        f();
    }
}

bool EventLoop::isConnectioning(int fd){
    int ret;
    do{
        char buf[1024]={0};
        ret=recv(fd,buf,sizeof(buf),MSG_PEEK);
    }while(ret==-1&&errno==EINTR);
    if(ret==-1){
        perror("recv");
    }
    return ret!=0;
}
void EventLoop::addepollFd(int fd){
    struct epoll_event events;
    events.data.fd=fd;
    events.events=EPOLLIN;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&events);
    if(-1==ret){
        perror("epoll_ctl");
    }
}
void EventLoop::delepollFd(int fd){
    struct epoll_event events;
    events.data.fd=fd;
    events.events=EPOLLIN;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&events);
    if(-1==ret){
        perror("epoll_ctl");
    }
}

int EventLoop::createEpollfd(){
    int ret=epoll_create1(0);
    if(-1==ret){
        perror("epoll_create");
    }
    return ret;
}

int EventLoop::createEventFd(){
    int ret=eventfd(1,0);
    if(ret==-1){
        perror("eventfd");
    }
    return ret;
}


}//end of namespace;

