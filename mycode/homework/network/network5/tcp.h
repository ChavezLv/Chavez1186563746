#ifndef __tcp_H__
#define __tcp_H__
#include<func.h>
#define MAXCONN 1024
int tcpInit(const char* ip,unsigned short port);
int epollAddReadEvent(int epfd,int fd);
void epollHandleNewConnection(int epfd,int listenfd);
void epollHandleMessage(int epfd,int fd);
void epollEventLoop(int epfd,int listenfd);
int epollDelEvent(int epfd,int fd);
#endif
