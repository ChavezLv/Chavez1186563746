 ///
 /// @file    transferFile.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-05 11:28:08
 ///
 
#include "process_pool.h"

#define FILENAME "bigfile.avi"

int sendCircle(int sfd, const char * buff, int length)
{
	const char * pbuf = buff;
	int ret = 0;
	int left = length;
	while(left > 0) {
		ret = send(sfd, pbuf, left, 0);
		if(ret < 0) {
			perror("send");
			return -1;
		} 

		pbuf += ret;
		left -= ret;
	}
	return length - left;
}
 
int transferFile(int peerfd)
{
	//读取本地文件
	int rfd = open(FILENAME, O_RDWR);
	ERROR_CHECK(rfd, -1, "open");

	train_t train;
	memset(&train, 0, sizeof(train));
	train.length = strlen(FILENAME); 
	strncpy(train.data, FILENAME, train.length);
	//先发送文件名
	sendCircle(peerfd, (const char*)&train, 4 + train.length);
	//再发送文件内容
	//获取大文件的长度
	struct stat st;
	fstat(rfd, &st);
	printf("bigfile length:%ld\n", st.st_size);
	sendCircle(peerfd, (const char *)&st.st_size, sizeof(st.st_size));
	off_t cnt =0;

	//循环发送文件内容
	while(cnt < st.st_size) {
		char filebuf[1000] = {0};
		int ret = read(rfd, filebuf, sizeof(filebuf));
		//printf("ret: %d\n", ret);
		ret = sendCircle(peerfd, filebuf, ret);
		if(-1 == ret) {
			break;
		}
		cnt += ret;
	}
	close(rfd);
}
