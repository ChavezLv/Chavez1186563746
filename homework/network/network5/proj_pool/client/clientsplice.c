 ///
 /// @file    client.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-09-29 17:15:29
 ///
 
#include <head.h>

//确定要接收length个字节的数据
int recvCircle(int sfd, char * buff, int length)
{
	int ret = 0;
	char * pbuf = buff;
	int left = length;

	while(left > 0) {
		ret = recv(sfd, pbuf, left, 0); 
		if(ret == -1 && errno == EINTR) {
			continue;
		} else if(-1 == ret){
			perror("recv");
			break;
		} else {
			pbuf += ret;
			left -= ret;
		}
	}
	return length - left;
}
 
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
	serveraddr.sin_addr.s_addr = inet_addr("192.168.3.119");

	//2.1 直接连接服务器
	int ret = connect(clientfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	ERROR_CHECK(ret, -1, "connect");
	//当connect正常返回时，连接已经建立成功了
	printf("client %d has connected!\n", clientfd);

	char buff[1000] = {0};
	//先接收文件名字
	int len = 0;
	recvCircle(clientfd, (char*)&len, 4);
	printf("filename len:%d\n", len);
	recvCircle(clientfd, buff, len);
	printf("filename :%s\n", buff);

	//创建本地文件
	int wfd = open(buff, O_CREAT|O_RDWR, 0666);
	ERROR_CHECK(wfd, -1, "open");
	memset(buff, 0, sizeof(buff));
	//接收文件的大小
	off_t length = 0;
	ret = recvCircle(clientfd, (char*)&length, sizeof(length));
	printf("filelength: %ld\n", length);

	int fds[2];
	pipe(fds);

	int recvSize = 0;

	while(recvSize < length) {
		ret = splice(clientfd, NULL, fds[1], NULL, 4096, SPLICE_F_MORE);
		splice(fds[0], NULL, wfd, NULL, ret, SPLICE_F_MORE);
		recvSize += ret;
	}
	
	close(wfd);
	close(clientfd);
}
