 ///
 /// @file    client.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-09-29 17:15:29
 ///
 
#include <head.h>
 
int main(int argc, char *argv[])
{
	//1. 创建客户端套接字
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_CHECK(clientfd, -1, "socket");
	printf("clientfd: %d\n", clientfd);
 
	//2.0 设置服务器的网络地址
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8888);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.30.128");

	//2.1 直接连接服务器
	int ret = connect(clientfd, &serveraddr, sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "connect");
	//当connect正常返回时，连接已经建立成功了
	
	fd_set rdset;

	//事件循环
	while(1) {
		FD_SET(clientfd, &rdset);// clientfd  3
		FD_SET(STDIN_FILENO, &rdset);//监听标准输入 0

		//当select函数返回时，rdset的集合已经被重新写入了
		//也是一个传出参数，写入的是已经发生了就绪事件的
		//文件描述符
		//printf("c1111\n");
		select(clientfd + 1, &rdset, NULL, NULL, NULL);
		//printf("c2222\n");

		//接下来要做的就是判断rdset中，有没有相应的fd
		if(FD_ISSET(STDIN_FILENO, &rdset)) {
			char buff[100] = {0};
			//1.第一种情况
			//获取从键盘输入的数据, 换行符'\n'也会发过去
			read(STDIN_FILENO, buff, sizeof(buff));
			//发送给服务器
			write(clientfd, buff, strlen(buff));
		}

		if(FD_ISSET(clientfd, &rdset)) {
			//2. 第二种情况
			//服务器给客户端发送了数据，在这里进行接收
			char buff[100] = {0};
			int ret = read(clientfd, buff, sizeof(buff));
			//当ret为0时，表示该连接已经断开
			if(ret == 0) {
				printf("c333\n");
				break;	
			}
			printf("from server: %s\n", buff);
		}
	}

	//5. 关闭套接字
	close(clientfd);
}
