 ///
 /// @file    server.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-09-29 17:09:21
 ///
 
#include <head.h>

#define MAXCONN 1024
 
int main(int argc, char *argv[])
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
	serveraddr.sin_port = htons(8888);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.30.128");

	ret = bind(listenfd, &serveraddr, sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "bind");
	
	//3. 对客户端进行监听
	ret = listen(listenfd, 10);

	//4. 使用select完成事件驱动的模型
	fd_set rdset;
	FD_ZERO(&rdset);

	FD_SET(listenfd, &rdset);
	// 保存当前监听的文件描述符的最大值
	int maxfd = listenfd;
	// 保存的是已经建立好的连接
	int connfds[MAXCONN] = { 0 };
	

	while(1) {
		//每一次轮询之前都要重新设置监听的fd
		FD_ZERO(&rdset);//先清空
		FD_SET(listenfd, &rdset);//监听listenfd
		//监听已经建立好的连接
		for(int i = 0; i < MAXCONN; ++i) {
			if(connfds[i] > 0) {
				FD_SET(connfds[i], &rdset);
			}
		}

		int nready = select(maxfd + 1, &rdset, NULL, NULL, NULL);
		if(nready < 0 && errno == EINTR) {
			continue;
		}

		//如果有新的连接到来
		if(FD_ISSET(listenfd, & rdset)) {
			int peerfd = accept(listenfd, NULL, NULL);
			ERROR_CHECK(peerfd, -1, "accept");

			//FD_SET(peerfd, &rdset);//不能直接在这里进行，
			//因为会修改rdset的值, 所以要保存该值
			//在下一次执行select之前写入rdset中
			
			//有了connfds数组之后，需要查找一个为0的位置，存放peerfd
			for(int i = 0; i < MAXCONN; ++i) {
				if(connfds[i] == 0) {
					connfds[i] = peerfd;
					break;
				}
			}

			//更新maxfd
			if(maxfd < peerfd) {
				maxfd = peerfd;
			}
		}

		//已经建立好的连接上是否有读事件发生
		//connfds数组
		for(int i = 0; i < MAXCONN; ++i) {
			if(connfds[i] > 0) {
				if(FD_ISSET(connfds[i], &rdset)) {
					//执行数据的接收操作
					char buff[100] = {0};
					int ret = recv(connfds[i], buff, sizeof(buff), 0);
					//对ret进行判断
					if(0 == ret) {
						printf("conn %d has closed\n", connfds[i]);
						close(connfds[i]);
						connfds[i] = 0;
					} else if (-1 == ret && errno == EINTR) {
						continue;
					} else if(-1 == ret) {
						perror("recv");
					} else {
						//大于0
						//完成回显操作
						send(connfds[i], buff, strlen(buff), 0);
					} 
				}
			}
		}
	}
	//7. 关闭套接字
	close(listenfd);
}
