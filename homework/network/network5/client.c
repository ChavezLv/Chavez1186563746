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
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//2.1 直接连接服务器
	int ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "connect");
	//当connect正常返回时，连接已经建立成功了
	printf("client %d has connected!\n", clientfd);

	char buff[100] = {0};
	//先接收文件名字
	int len = 0;
	recv(clientfd, &len, 4, 0);
	printf("filename len:%d\n", len);
	recv(clientfd, buff, len, 0);
	printf("filename :%s\n", buff);

	//创建本地文件
	int wfd = open(buff, O_CREAT|O_RDWR, 0666);
	ERROR_CHECK(wfd, -1, "open");
	//再接收文件内容
	memset(buff, 0, sizeof(buff));
	recv(clientfd, &len, 4, 0);
	recv(clientfd, buff, len, 0);

	//最后写入本地
	write(wfd, buff, strlen(buff));
	
	close(wfd);
	close(clientfd);
}
