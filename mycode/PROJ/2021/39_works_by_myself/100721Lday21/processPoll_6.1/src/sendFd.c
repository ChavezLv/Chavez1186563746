#include "../include/func.h"
int sendFd(int pipeFd, int fd, int exitFlag){
    int ret = 0;
    //msghdr is used in funci: ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
    //结构体组成
    /* struct msghdr { */
    /*     void         *msg_name;       /1* optional address *1/ */
    /*     socklen_t     msg_namelen;    /1* size of address *1/ */
    /*     struct iovec *msg_iov;        /1* scatter/gather array *1/ */
    /*     size_t        msg_iovlen;     /1* # elements in msg_iov *1/ */
    /*     void         *msg_control;    /1* ancillary data, see below 指向cmsghdr结构体*1/ */
    /*     size_t        msg_controllen; /1* ancillary data buffer len *1/ */
    /*     int           msg_flags;      /1* flags (unused) *1/ */
    /* }; */
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));
    //msghdr结构体的msg_name:udp需要,tcp无视.对端方的地址指针，不关心时设为NULL即可
    msg.msg_name = NULL;//已有memset, 可以不写,写出来方便记忆
    msg.msg_namelen = 0;

    struct iovec iov;
    memset(&iov, 0, sizeof(iov));
    /* char buf[64] = { 0 }; */
    /* strcpy(buf, "a"); */
    //退出机制
    iov.iov_base = &exitFlag;
    iov.iov_len = sizeof(int);
    /* printf("*(int*)iov.iov_base: %d\n",*(int*)iov.iov_base); */
    //赋值好的struct iovec 放入msg.msg_iov中
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;//结构体数组的大小,只有一个所以写1
    
    //$man cmsg 查看相关结构体
    
    //堆空间
    /* size_t cmsgLen = CMSG_LEN(sizeof(int)); */
    /* struct cmsghdr *cmsg = (struct cmsghdr *)calloc(1, cmsgLen); */
    /* cmsg->cmsg_len = cmsgLen; */
    /* cmsg->cmsg_level = SOL_SOCKET; */
    /* cmsg->cmsg_type = SCM_RIGHTS; */
    /* *(int *)CMSG_DATA(cmsg) = fd; */
    /* //赋值好的struct cmsghdr 放入smg.msg_control中 */
    /* msg.msg_control = cmsg; */
    /* msg.msg_controllen = cmsg->cmsg_len; */

    //栈空间
    struct cmsghdr cmsg;
    size_t cmsgLen = CMSG_LEN(sizeof(int));
    memset(&cmsg, 0, cmsgLen);
    /* cmsg.cmsg_len = cmsgLen; */
    cmsg.cmsg_len = CMSG_LEN(sizeof(int));
    cmsg.cmsg_level = SOL_SOCKET;
    cmsg.cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(&cmsg) = fd;
    //赋值好的struct cmsghdr 放入smg.msg_control中
    msg.msg_control = &cmsg;
    msg.msg_controllen = cmsg.cmsg_len;

    ret = sendmsg(pipeFd, &msg, 0);
    ERROR_CHECK(ret, -1, "sendmsg");

    return 0;
}
