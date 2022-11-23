/*================================================================
*   
*   @file       ：EventLoo.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#include "JsonMsg.hpp"
#include "EventLoop.hpp"
#include <sys/socket.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <string>


#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;
 
namespace wd
{

EventLoop::EventLoop(int fd)
: _efd(createEpollfd())
, _sfd(fd)
, _isLooping(false)
, _sfdIO(_sfd){

    addEpollReadFd(_sfd);

    addEpollReadFd(STDIN_FILENO);
    
}

EventLoop::~EventLoop()
{	
	close(_efd);	
}

void EventLoop::loop()
{
	_isLooping = true;
    int ret = 0;
	while(_isLooping) {
        /* printf("要查询的词: "); */
        if(0 == ret){
            cout << ">> pls input query word(example input: search 中国):";
            fflush(STDIN_FILENO);
        }else if(ret == 100){
            cout << ">> pls choose the recommanded word(example input: display 中国, or press \"back\" to return)):";
            fflush(STDIN_FILENO);
            
        }else if(ret == 200){
            cout << ">> pls choose the display web to view(example input: view 中国, or press \"back\" to return)):";
            fflush(STDIN_FILENO);

        }else if(ret == -100){
            cout << ">> pls choose the recommanded word(example input: display 中国, or press \"back\" to return)):";
            fflush(STDIN_FILENO);
        }else if(ret == -200){
            cout << ">> pls choose the display web to view(example input: view 中国, or press \"back\" to return)):";
            fflush(STDIN_FILENO);
        }else if(ret == 666){
            //nothing
        }else{
            cout << ">> pls input query word(example input: search 中国):";
            fflush(STDIN_FILENO);

        }
		ret = waitEpollFd();
	}
}


void EventLoop::unloop()
{	_isLooping = false;	}


int EventLoop::waitEpollFd()
{
    int ret = 0;
	int nready = 0;
	do {
		nready = ::epoll_wait(_efd, &*_evtList.begin(), _evtList.size(), -1);

	} while(nready == -1 && errno == EINTR);

	if(0 == nready) {
		/* printf(">>epoll_wait timeout\n"); */
        
	} else if (-1 == nready) {
		perror("epoll_wait");
		return -1;
	} else {
		for(int idx = 0; idx != nready; ++idx) {
			int fd = _evtList[idx].data.fd;
			if(fd == _sfd) {
				if(_evtList[idx].events & EPOLLIN) {
					 ret = handleMessage(_sfd);
				}
			} 
			else if(fd == STDIN_FILENO){
				if(_evtList[idx].events & EPOLLIN) {
					ret = handleSearch();
				}
			}
		}
	}
    return ret;
}


int EventLoop::handleMessage(int fd)
{
    //判断连接是否已经断开了
    bool isClose = isClosed();
    if(isClose) {
        //已经断开
        handleClose();
        delEpollReadFd(fd);
    } else {
        //没有断开，处理消息
        /* cout << "没有断开，处理消息" << endl; */
        char buff[1024] = {0};
        recv(_sfd, buff, sizeof(buff), 0);

        JsonMsg msg;
        msg.fromJsonLayoutStream(buff);

        if(msg.getMsgType() == 100){
            cout << "******recv word msg from server:" << msg.getMsgLen() << " " \
                << msg.getMsgType() << " " << msg.getMsgContent() << endl;
            return 100;

        }else if(msg.getMsgType() == 200){
            cout << "******recv web msg from server:" << endl;
            json js = JsonMsg().getJsonFromJsonLayoutStream(string(buff));
            for(size_t index = 0; index < js["msgContent"].size(); ++index){
                cout << "\t" << js["msgContent"][index] << endl;
                /* cout << "【" << js["msgContent"][index]["title"] << " 】" << endl; */
                /* cout << "\t" << js["msgContent"][index]["summary"] << endl; */
                /* cout << "\t" << js["msgContent"][index]["url"] << endl; */
                cout << endl;
            }
            cout << endl;
            /* cout << js << endl; */
            /* string msgContentIndex = "/msgContent/0"; //error*/
            /* string msgContentIndex = "/msgContent/0"_json_pointer; //error */
            /* cout << js.at(msgContentIndex) << endl; //error*/

            /* cout << js.at("/msgContent/0"_json_pointer) << endl; */
            //     << msg.getMsgType() << " " << msg.getMsgContent() << endl;
            return 200;
        }
    }
    return -1;
}



int EventLoop::handleSearch(){
    string line;
    string word;
    stringstream ss;
    string jsonLayout;
    getline(cin, line);
/* cout << line << endl; */
    if(0 == line.size()){//客户端什么也没有输入
        cout << "NULL INPUT! try again pls ";
        return 0;
    }
    ss << line;// 也不会有很多行的情况?

    ss.clear();
    ss >> word;

    if(word == string("search") | word == string("s") ){
        if(ss >> word){
            jsonLayout = JsonMsg(word.size(), 1, word).toJsonLayoutStream();
            if(ss >> word){
                //if not only a word
                cout << "ERROR LAYOUT! try again pls ";
                return 0;
            }
        }else{
            //if only search 
            cout << "ERROR LAYOUT! try again pls ";
            return 0;         
        }
    }

    else if(word == string("display") | word == string("d")){
        if(ss >> word){
            jsonLayout = JsonMsg(word.size(), 2, word).toJsonLayoutStream();
            if(ss >> word){
                //if not only a word
                cout << "ERROR LAYOUT! try again pls ";
                return 0;
            }
        }else{
            cout << "ERROR LAYOUT! try again pls ";
            return -100;
        }

    }
    
    else if(word == string("back") | word == string("b")){

        return 0;
    }

    else if(word == string("veiw") | word == string("v")){
        cout << "PLEASE UPDATE FOR SUPPORT!" ; 
        return 0;
    }
    
    else{
        cout << "ILLEGAL COMMAND! try again pls ";
        return 0;
    }

    jsonLayout.append("\n");// need only a \n
    send(_sfd, jsonLayout.data(), jsonLayout.size(), 0);
    return 666;

}

void EventLoop::handleClose(){
    cout << "Server has closed!" << endl;
}


int EventLoop::createEpollfd()
{
	int fd = ::epoll_create1(0);
	if(fd < 0) {
		perror("epoll_create1");
	}
	return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
	struct epoll_event value;
	value.events = EPOLLIN;
	value.data.fd = fd;
	int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &value);
	if(ret < 0) {
		perror("epoll_ctl");
	}
    _evtList.push_back(value);
}


void EventLoop::delEpollReadFd(int fd)
{
	struct epoll_event value;
	value.events = EPOLLIN;
	value.data.fd = fd;
	int ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &value);
	if(ret < 0) {
		perror("epoll_ctl");
	}
}

int EventLoop::recvCircle(int sfd,void *buf,int recvlen){
	int total = 0,ret;
	char *p = (char*)buf;
	while(total<recvlen){
		ret = recv(sfd,p+total,recvlen-total,0);
		if(!ret){
			return 0;
		}
		total+=ret;
	}
	return total;
}

bool EventLoop::isClosed(){
    int ret;
	char buf[128] = {0};
	do{
		ret = recv(_sfd, buf, sizeof(buf), MSG_PEEK);
	}while(ret == -1 && errno == EINTR);

	return ret == 0;

}


}//end of namespace wd
