 ///
 /// @file    tcp.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-04 14:32:00
 ///
 
#ifndef __WD_TCP_H__
#define __WD_TCP_H__

#include <head.h>

#define MAXCONN 1024

int tcpInit(const char * ip, unsigned short port);

int epollAddReadEvent(int epfd, int fd);
int epollDelEvent(int epfd, int fd);

void epollHandleNewConnection(int epfd, int listenfd);
void epollHandleMessage(int epfd, int fd);
void epollEventLoop(int epfd, int listenfd);
 
#endif
