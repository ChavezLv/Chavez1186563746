 ///
 /// @file    server.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-09-29 17:09:21
 ///
#include<sys/socket.h>
#include <head.h>
 
int main(int argc, char *argv[])
{
	//1. 创建监听套接字
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_CHECK(listenfd, -1, "socket");

	//2. 服务器要绑定网络地址
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8888);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.3.119");

	int ret = bind(listenfd,&serveraddr, sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "bind");
	
	//3. 对客户端进行监听
	ret = listen(listenfd, 10);


	//循环迭代式的服务器，不能处理大并发的请求
	while(1) {
		//4. 获取一个与客户端进行交互的peerfd
		printf("11112\n");
		int peerfd = accept(listenfd, NULL, NULL);
		ERROR_CHECK(peerfd, -1, "accept");
		printf("2222\n");

		//用户态的缓冲区
		char buff[100] = {0};
		//5. 接收客户端的数据并打印
		recv(peerfd, buff, 100, 0);
		printf("buff:%s\n", buff);

		//6. 给客户端发送数据
		//send(peerfd, buff, sizeof(buff), 0);
		send(peerfd, buff, strlen(buff), 0);

		//7. 关闭套接字
		close(peerfd);
	}
 
}
