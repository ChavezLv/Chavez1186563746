 ///
 /// @file    transferFile.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-05 11:28:08
 ///
 
#include "threadpool.h"

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
	int fds[2];
	pipe(fds);

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
	int ret = 0;

	//循环发送文件内容
	while(cnt < st.st_size) {
		//零拷贝技术之三splice
		ret = splice(rfd, NULL, fds[1], NULL, 4096, SPLICE_F_MORE);
		splice(fds[0], NULL, peerfd, NULL, ret, SPLICE_F_MORE);
		cnt += ret;
	}
	close(rfd);
}
