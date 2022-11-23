 ///
 /// @file    tcp.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-04 14:33:42
 ///
 
#include "tcp.h"

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


void epollEventLoop(int epfd, int listenfd)
{
	struct epoll_event * evtList = (struct epoll_event*)
		calloc(MAXCONN, sizeof(struct epoll_event));
	while(1) {
		//epoll_wait返回时，nready大于0，表示有nready个fd就绪
		//并且相应的数据已经写入到evtList数组中了，而且就在
		//前nready个元素的位置
		int nready = epoll_wait(epfd, evtList, MAXCONN, -1);
		printf("nready: %d\n", nready);
		if(nready == -1 && errno == EINTR) {
			continue;
		} else if(nready == 0) {
			printf("epoll_wait timeout\n");
		} else if(nready == -1) {
			perror("epoll_wait");
		} else {

			for(int i = 0; i < nready; ++i) 
			{	//evtList数组中的元素一定是已经就绪的fd
				int fd = evtList[i].data.fd;
				if(fd == listenfd) {
					epollHandleNewConnection(epfd, listenfd);
				} else {
					//已经建立好的连接上是否有读事件发生
					epollHandleMessage(epfd, fd);
				}
			}
		}
	}
}

void epollHandleNewConnection(int epfd, int listenfd)
{
	//如果有新的连接到来
	int peerfd = accept(listenfd, NULL, NULL);
	if(peerfd == -1) {
		perror("accept");
		return;
	}
	//将其监听挂到内核所对应的红黑树上
	epollAddReadEvent(epfd, peerfd);
}

void epollHandleMessage(int epfd, int fd)
{
	char buff[100] = {0};
	int ret = recv(fd, buff, sizeof(buff), 0);
	//对ret进行判断
	if(0 == ret) {
		printf("conn %d has closed\n", fd);
		//将其从内核红黑树上删除
		epollDelEvent(epfd, fd);
		close(fd);
	} else if(-1 == ret) {
		perror("recv");
	} else {
		//大于0
		//完成回显操作
		send(fd, buff, strlen(buff), 0);
	} 
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
