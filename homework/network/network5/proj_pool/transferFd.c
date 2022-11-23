 ///
 /// @file    transferFd.c
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2022-10-04 17:28:20
 ///
 
#include "process_pool.h"

int sendFd(int pipefd, int fd, char exitflag)
{
	//组建第二组数据
	struct iovec iov;
	iov.iov_base = &exitflag;
	iov.iov_len = 1;

	//组建第三组数据
	int len = CMSG_LEN(sizeof(fd));
	printf("len: %d\n", len);
	struct cmsghdr * cmsg = (struct cmsghdr*)calloc(1, len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	int * pfd = (int *)CMSG_DATA(cmsg);
	*pfd = fd;

	//构造struct msghdr
	struct msghdr msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;

	int ret = sendmsg(pipefd, &msg, 0);
	ERROR_CHECK(ret, -1, "sendmsg");
}

int recvFd(int pipefd, int * pfd, char * pexitflag)
{
	//组建第二组数据
	char buff[10] = {0};
	struct iovec iov;
	iov.iov_base = buff;
	iov.iov_len = sizeof(buff) - 1;

	//组建第三组数据
	int len = CMSG_LEN(sizeof(int));
	struct cmsghdr * cmsg = (struct cmsghdr*)calloc(1, len);
	cmsg->cmsg_len = len;
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;

	//构造struct msghdr
	struct msghdr msg;
	memset(&msg, 0, sizeof(msg));
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = cmsg;
	msg.msg_controllen = len;

	int ret = recvmsg(pipefd, &msg, 0);
	ERROR_CHECK(ret, -1, "recvmsg");
	int * cpfd = (int*)CMSG_DATA(cmsg);
	*pfd = *cpfd;

	//通过指定的缓冲区直接读取标志位信息
	*pexitflag = buff[0];
	//printf("msg: %s\n", (char*)msg.msg_iov[0].iov_base);
}
 
