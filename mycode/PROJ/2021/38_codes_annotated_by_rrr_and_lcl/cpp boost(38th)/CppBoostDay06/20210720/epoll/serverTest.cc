 ///
 /// @file    bigLittleEndian.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-02-03 20:35:41
 ///
 
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;

void test0()
{
	//1. 创建监听服务器的套接字
	int listenfd = ::socket(AF_INET, SOCK_STREAM, 0);	

	if(listenfd < 0) {
		perror("socket");
		return;
	}

	//网络地址需要采用网络字节序存储(大端模式)
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8888);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.30.128");
	socklen_t length = sizeof(serveraddr);

	//2. 绑定服务器的网络地址
	if(::bind(listenfd, (struct sockaddr*)&serveraddr, length) < 0) {
		perror("bind");
		//文件描述符是比较稀缺的，所以不用的时候要回收
		close(listenfd);
		return;
	}

	//3. 让服务器开始监听
	// listenfd跟所有的新连接打交道
	if(::listen(listenfd, 128) < 0) {
		perror("listen");
		close(listenfd);
		return;
	} 
	printf("server is listening...\n");

	

	// 创建epoll实例
	// 如果使用epoll_create(1); 就必须填大于0的值
	int efd = ::epoll_create1(0);// 红黑树 + 就绪链表

	
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = listenfd;
	//epoll要进行监听操作: 对listenfd的读事件进行监听
	//
	//Reactor: 注册读就绪事件
	//根据efd找到红黑树, 参2是什么事件, 参3和参4分别是key和value
	//为什么要多些一个参3呢? 因为内部是一个enum, 需要参3才能知道是什么东西.
	int ret = ::epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &ev);
	if(ret < 0) {
		perror("epoll_ctl");
		close(listenfd);
		close(efd);
		return;
	}

	struct epoll_event * evtList = 
		(struct epoll_event*)malloc(1024 * sizeof(struct epoll_event));

	//事件循环
	while(1) {
		//Reactor: 事件分离器
		// c里面就是 名字+长度 = 传递一个数组的
		int nready = ::epoll_wait(efd, evtList, 1024, 3000);
		if(nready == -1 && errno == EINTR)
			continue;
		else if(nready == -1) {
			perror("epoll_wait");
			return;
		} else if (nready == 0) {
			printf(">> epoll_wait timeout!\n");
		} else {

			//遍历struct epoll_event数组, 去check
			//每一个epoll_event到底发生了什么事件
			for(int idx = 0; idx < nready; ++idx) {
				if((evtList[idx].data.fd == listenfd)  &&
					// 必须要使用按位&操作来判断事件，不能使用==,&&
				   (evtList[idx].events & EPOLLIN)) 
				{   
					//意味着有新连接来了,所以要调用accept函数,获取新连接
					//写事件什么情况会触发? 只要内核发送缓冲区还有空间，就可以触发写事件
					int peerfd = ::accept(listenfd, nullptr, nullptr);	
					TcpConnection conn(peerfd);

					//将新连接添加到epoll的监听实例中去
					struct epoll_event ev;
					ev.events = EPOLLIN | EPOLLOUT | EPOLLERR;
					ev.data.fd = peerfd;
					ret = ::epoll_ctl(efd, EPOLL_CTL_ADD, peerfd, &ev);
					if(ret < 0) {
						perror("epoll_ctl");
						continue;
					}

					//新连接到来之后的处理
					/* printf(">> conn has connected, fd: %d\n", peerfd); */
					//记录日志, 使用Log4cpp完成
					//个性定制化 ==> 事件处理器
					onConnection();//考虑扩展性，挖一个坑
				} else {
					// 已经建立好的连接发送数据过来了
					// 如果发生了读事件
					char buff[128] = {0};
					if(evtList[idx].events & EPOLLIN) {
						int fd = evtList[idx].data.fd;
						ret = ::recv(fd, buff, sizeof(buff), 0);

						if(ret > 0) {
							printf(">>> recv msg %d bytes,content:%s\n",
									ret, buff);
							
							//decode 
							//compute
							//encode 
							//lsl: 抽象出一个函数对象来处理
							//1. 对应用层数据进行解析(处理客户端数据)
							//2. 拿到最终要处理的数据之后，进行业务逻辑处理
							//(假设第2步执行的时间很长1S, 是否合适)
							//3. 得到要返回给客户端的数据之后，进行发送操作
							//
							onMessage();//考虑扩展性，挖一个坑

							/* ret = send(fd, buff, strlen(buff), 0); */
							//printf(">>> send %d bytes\n", ret);
						} else if(ret == 0) {
							printf("conn has closed!\n");
							
							//需要从epoll的监听实例删除掉, 因为连接断开，不需要再监听了
							struct epoll_event ev;
							ev.data.fd = fd;
							ret = ::epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
							if(ret < 0) {
								perror("epoll_ctl");
							}
							//记录日志, log4cpp
							onClose();//考虑扩展性，挖一个坑
						}// end of ret if
					}//end of event if
					//else if()  //处理其他事件的
				}
			}
		}
	}
	
	close(listenfd);// 关闭连接
	
}
 
int main(void)
{
	test0();
	return 0;
}
