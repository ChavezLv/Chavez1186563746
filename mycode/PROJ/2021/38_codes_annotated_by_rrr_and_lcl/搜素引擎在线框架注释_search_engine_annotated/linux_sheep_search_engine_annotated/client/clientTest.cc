 ///
 /// @file    clientTest.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-02-03 21:10:10
 ///
 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

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
	int clientfd = ::socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0) {
		perror("socket");
		return ;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	/* serverAddr.sin_addr.s_addr = inet_addr("192.168.30.128"); */
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t length = sizeof(serverAddr);

	if(::connect(clientfd,(struct sockaddr*)&serverAddr, length) < 0) {
		perror("connect");
		close(clientfd);
		return;
	}
	printf("conn has connected!...\n");

	while(1) {
		string line;
		cout << ">> pls input some message:";
		getline(cin, line);
		line.append("\n");
		//1. 客户端先发数据
		send(clientfd, line.data(), line.size(), 0);
		/* close(clientfd); */

		char buff[128] = {0};
		recv(clientfd, buff, sizeof(buff), 0);
		printf("recv msg from server: %s\n", buff);
	}
	close(clientfd);

} 
 
int main(void)
{
	test0();
	return 0;
}
