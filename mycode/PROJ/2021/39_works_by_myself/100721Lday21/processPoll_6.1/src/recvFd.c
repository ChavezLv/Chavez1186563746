#include "../include/func.h"
int recvFd(int pipeFd, int *fd, int *exitFlag){
    int ret = 0;
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    //msghdr结构体的msg_name:udp需要,tcp无视.对端方的地址指针，不关心时设为NULL即可
    msg.msg_name = NULL;//已有memset, 可以不写,写出来方便记忆
    msg.msg_namelen = 0;

    struct iovec iov;
    memset(&iov, 0, sizeof(iov));
    /* char buf[64] = { 0 }; */
    /* strcpy(buf, "a"); */
    iov.iov_base = exitFlag;//iov_base 本身就是指针
    iov.iov_len = sizeof(int);
    //赋值好的struct iovec 放入msg.msg_iov中
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;//结构体数组的大小,只有一个所以写1
    
    //$man cmsg 查看相关结构体
    size_t cmsgLen = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr *)calloc(1, cmsgLen);
    cmsg->cmsg_len = cmsgLen;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    //赋值好的struct cmsghdr 放入smg.msg_control中
    msg.msg_control = cmsg;
    msg.msg_controllen = cmsg->cmsg_len;

    ret = recvmsg(pipeFd, &msg, 0);
    ERROR_CHECK(ret, -1, "recvmsg");
    
    /* printf("*exitFlag = %d\n", *(int*)iov.iov_base); */
    /* printf("*exitFlag = %d\n", *exitFlag); */
    *fd = *(int*)CMSG_DATA(cmsg);
    /* *exitFlag = *(int*)iov.iov_base;//不需要这一句作为传出参数 */
    
    return 0;
}
