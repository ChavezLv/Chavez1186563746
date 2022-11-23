 ///
 /// @file    tcp.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-04 14:33:42
 ///
 
#include "tcp.h"
#include "head.h"

int tcpInit(const char * ip, unsigned short port)
{
	//1. 创建监听套接字
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_CHECK(listenfd, -1, "socket");

	int on = 1;
	int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	ERROR_CHECK(ret, -1, "setsockopt");

	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
	ERROR_CHECK(ret, -1, "setsockopt");

	//2. 服务器要绑定网络地址
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);

	ret = bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "bind");

	//3. 对客户端进行监听
	ret = listen(listenfd, 10);
	ERROR_CHECK(ret, -1, "listen");
	return listenfd;
}
 
int isConnectionClosed(int fd)
{
	int nret = 0;
	char buff[20] = {0};
	do {
		//在这里如果只是想判断连接是否断开，不希望
		//移走内核接收缓冲区中的数据
		nret = recv(fd, buff, sizeof(buff), MSG_PEEK);
	} while(nret == -1 && errno == EINTR);
	return nret == 0;
}

int epollAddReadEvent(int epfd, int fd) {
	int ret = 0;
	struct epoll_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.events = EPOLLIN;//默认情况下就是水平触发
	evt.data.fd = fd;
	ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &evt);
	ERROR_CHECK(ret, -1, "epoll_ctl");

	return ret;
}

int epollDelEvent(int epfd, int fd)
{
	int ret = 0;
	struct epoll_event evt;
	memset(&evt, 0, sizeof(evt));
	evt.data.fd = fd;
	ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &evt);
	ERROR_CHECK(ret, -1, "epoll_ctl");
	return ret;
}
