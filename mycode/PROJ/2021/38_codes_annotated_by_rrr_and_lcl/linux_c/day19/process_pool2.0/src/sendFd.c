#include "process_pool.h"

int sendFd(int pipeFd, int fd)
{
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));//

    struct iovec iov;
    memset(&iov, 0, sizeof(iov));//iov装额外信息
    char buf[8] = {0};
    strcpy(buf, "hi");

    iov.iov_base = buf;//额外信息的buf
    iov.iov_len = strlen(buf);  //额外buf

    msg.msg_iov = &iov; //一个额外信息
    msg.msg_iovlen = 1;

    int len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr*)calloc(1, len); //控制信息，是一个int

    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;

    *(int*)CMSG_DATA(cmsg) = fd; //传递文件描述符
    
    msg.msg_control = cmsg;//控制信息
    msg.msg_controllen = len;

    sendmsg(pipeFd, &msg, 0);//送出文件描述符
    return 0;
}
