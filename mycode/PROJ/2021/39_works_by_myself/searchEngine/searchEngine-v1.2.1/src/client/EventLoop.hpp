/*================================================================
*   
*   @file       ：EventLoo.hpp
*   @date       ：2021年08月29日 21:09:36
*   @description：
*
================================================================*/
 
#ifndef __WD_EVENTLOOP_HPP__
#define __WD_EVENTLOOP_HPP__


#include "SocketIO.hpp"
#include <sys/epoll.h>

#include <map>
#include <vector>
#include <memory>
#include <functional>

using std::map;
using std::vector;
using std::function;
using std::shared_ptr;

namespace wd
{


class EventLoop
{
public:
	EventLoop(int fd);
	~EventLoop();

	void loop();
	void unloop();

private:
	int waitEpollFd();

	int handleMessage(int fd);
	int handleSearch();
	void handleClose();

	int createEpollfd();
	void addEpollReadFd(int fd);
	void delEpollReadFd(int fd);

    bool isClosed();

    int recvCircle(int sfd,void *buf,int recvlen);
private:
	int _efd;
    int _sfd;


	bool _isLooping;

	vector<struct epoll_event> _evtList;

    SocketIO _sfdIO;

};

}//end of namespace wd
 
 
 
#endif
