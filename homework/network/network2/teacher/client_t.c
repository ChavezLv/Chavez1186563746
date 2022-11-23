 ///
 /// @file    client.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-09-29 17:15:29
 ///
#include<sys/socket.h>
#include <head.h>
 
int main(int argc, char *argv[])
{
	//1. 创建客户端套接字
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);
	ERROR_CHECK(clientfd, -1, "socket");
 
	//2.0 设置服务器的网络地址
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(8888);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.3.119");

	//2.1 直接连接服务器
	int ret = connect(clientfd,&serveraddr, sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "connect");

	//当connect正常返回时，连接已经建立成功了
	//while(1);
	char msg[] = "hello,server";
	//3. 客户端发送数据
	send(clientfd, msg, sizeof(msg), 0);

	char cbuff[100] = {0};
	//4. 客户端接收数据
	recv(clientfd, cbuff, 100, 0);
	printf("client recv: %s\n", cbuff);

	//5. 关闭套接字
	close(clientfd);
}
