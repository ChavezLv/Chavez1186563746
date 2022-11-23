#include"mutex.hpp"
#include"accept.hpp"
#include"eventpoll.hpp"
#include<stdio.h>
namespace rxl{
	using EpollCallBack = function<void(TcpconnectionPtr)>;
	EventPoll::EventPoll(Acceptor& acceptor)
	:_epfd(createEpoll())
	,_acceptor(acceptor)
	,_evtlist(1024)
	,_isLooping(false)
	,_mutex()
	,_eventfd()
	{
		addEpoll(_acceptor.fd());
		addEpoll(_eventfd.eventfd());
	}
	void EventPoll::loop(){
		if(!_isLooping){
			_isLooping = true;
		}
		while(_isLooping){
			waitEpoll();
		}
	}
    
	void EventPoll::unloop(){
		if(_isLooping){
			_isLooping = false;
		}
	}

	void EventPoll::pushInSolveList(Functor f){
		{
			MutexGuard protec(_mutex);		
			_funcs.push_back(f);
		}
		_eventfd.weakup();
	}
	void EventPoll::setConnectCallBack(EpollCallBack&& cb){
		_onConnection = std::move(cb);
	}
	void EventPoll::setMessageCallBack(EpollCallBack&& cb){
		_onMessage = std::move(cb);
	}
	void EventPoll::setCloseCallBack(EpollCallBack&& cb){
		_onClose = std::move(cb);
	}
	int EventPoll::createEpoll(){
		int ret = ::epoll_create1(0);
		if(ret < 0){
			perror("epoll_create");
		}
		return ret;
	}
	
	void EventPoll::addEpoll(int fd){
		struct epoll_event ev;
		ev.data.fd = fd;
		ev.events = EPOLLIN;
		epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
	}
	
	void EventPoll::delEpoll(int fd){
		struct epoll_event ev;
		ev.data.fd = fd;
		ev.events = EPOLLIN;
		epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
	}
	void EventPoll::waitEpoll(){
		int nready;
		do{
			nready = epoll_wait(_epfd,&_evtlist[0],_evtlist.size(),5000);
		}while(nready == -1 && errno == EINTR); 
		if(nready == -1){
			perror("epoll_wait");
			return;
		}else if(nready == 0){
			printf("epoll_wait timout\n");
		}else{
			if(nready == _evtlist.size()){
				_evtlist.resize(2 * nready);
			}
			for(int i = 0;i<nready;++i){
				int fd = _evtlist[i].data.fd;
				if(fd ==  _acceptor.fd() && EPOLLIN & _evtlist[i].events){
					handleConnection();
				}else if(fd == _eventfd.eventfd() && EPOLLIN & _evtlist[i].events){
					_eventfd.handleread();
					doPendingFunc();
				}else if(_evtlist[i].events & EPOLLIN){
					handleMessage(fd);
				}
			}
		}
	}
	void EventPoll::doPendingFunc(){
		vector<Functor> tmp;
		{
			MutexGuard protec(_mutex);
			tmp.swap(_funcs);
		}
		for(auto &f : tmp){
			f();
		}
	}
	void EventPoll::handleConnection(){
		int peerfd = _acceptor.accept();
		addEpoll(peerfd);
		TcpconnectionPtr conn(new TcpConnect(peerfd,*this));	
		conn->setConnectCallBack(_onConnection);
		conn->setMessageCallBack(_onMessage);
		conn->setCloseCallBack(_onClose);
		_tcpmap.insert(std::make_pair(peerfd,conn));

		conn->handleConnectionCallBack();
	}
	void EventPoll::handleMessage(int fd){
		auto it = _tcpmap.find(fd);
		if(it != _tcpmap.end()){
			if(!_isClosed(fd)){
				it->second->handleMessageCallBack();
			}else{
				delEpoll(fd);
				it->second->handleCloseCallBack();
				_tcpmap.erase(it);
			}
		}
	}
	bool EventPoll::_isClosed(int fd){
		int ret;
		char buf[128] = {0};
		do{
			ret = recv(fd,buf,sizeof(buf),MSG_PEEK);
		}while(ret == -1 && errno == EINTR);
		return ret == 0;
	}
}//end of namespace

