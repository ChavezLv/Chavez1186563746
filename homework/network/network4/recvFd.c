#include<func.h>
int recvFd(int pipefd,int *fd){
    struct iovec iov;
    memset(&iov,0,sizeof(iov));
    char buff[10]={0};
    //strcpy(buff,"hello");//为什么不直接赋值
    iov.iov_base=buff;
    iov.iov_len=sizeof(buff);
    size_t len=CMSG_LEN(sizeof(fd));//获得结构体大小
    struct cmsghdr *cmsg=( struct cmsghdr*)calloc(1,len);
    cmsg->cmsg_len=len;
    cmsg->cmsg_level=SOL_SOCKET;
    cmsg->cmsg_type=SCM_RIGHTS;
    //int *pfd=(int*)CMSG_DATA(cmsg);
    //*pfd=fd;
    struct msghdr msg;
    memset(&msg,0,sizeof(msg));
    msg.msg_iov=&iov;
    msg.msg_iovlen=1;
    msg.msg_control=cmsg;
    msg.msg_controllen=len;
    int ret=recvmsg(pipefd,&msg,0);
    *fd=*(int*)CMSG_DATA(cmsg);
    printf("iov[0]:%s\n",(char*)(msg.msg_iov[0].iov_base));
    return 0;
}
