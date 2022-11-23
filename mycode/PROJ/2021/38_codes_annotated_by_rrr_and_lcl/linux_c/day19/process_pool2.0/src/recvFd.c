#include "process_pool.h"

int recvFd(int pipeFd, int *fd)
{
    struct msghdr msg; //利用msg结构体可以传送内核数据（比如文件句柄、或者说文件对象的引用）
    memset(&msg, 0, sizeof(msg));

    struct iovec iov;
    memset(&iov, 0, sizeof(iov));
    char buf[8] = {0};
    strcpy(buf, "hi");

    iov.iov_base = buf; //iov可以携带一些额外数据
    iov.iov_len = strlen(buf);

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;//只有一个iov，不是iov数组

    int len = CMSG_LEN(sizeof(int)); //计算一个csmg的大小//控制信息
    struct cmsghdr *cmsg = (struct cmsghdr*)calloc(1, len);

    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    
    msg.msg_control = cmsg; //控制信息结构体
    msg.msg_controllen = len;//计算控制信息结构体的长度

    recvmsg(pipeFd, &msg, 0); //接受信息
    *fd = *(int*)CMSG_DATA(cmsg);//取出fd的大小

    return 0;
}
