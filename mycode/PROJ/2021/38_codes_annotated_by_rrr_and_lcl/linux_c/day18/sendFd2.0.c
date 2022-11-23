#include <head.h>

int sendFd(int pipeFd, int fd)
{
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    struct iovec iov;
    memset(&iov, 0, sizeof(iov));
    char buf[8] = {0};
    strcpy(buf, "hi");

    iov.iov_base = buf;
    iov.iov_len = strlen(buf);

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    int len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr*)calloc(1, len);

    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;

    *(int*)CMSG_DATA(cmsg) = fd;
    
    msg.msg_control = cmsg;
    msg.msg_controllen = len;

    sendmsg(pipeFd, &msg, 0);
}

int recvFd(int pipeFd, int *fd)
{
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    struct iovec iov;
    memset(&iov, 0, sizeof(iov));
    char buf[8] = {0};
    strcpy(buf, "hi");

    iov.iov_base = buf;
    iov.iov_len = strlen(buf);

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    int len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr*)calloc(1, len);

    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    
    msg.msg_control = cmsg;
    msg.msg_controllen = len;

    recvmsg(pipeFd, &msg, 0);
    *fd = *(int*)CMSG_DATA(cmsg);
}

int main(int argc, char** argv)
{
    int sfd[2];

    socketpair(AF_LOCAL, SOCK_STREAM, 0, sfd);
    if(fork()){
        close(sfd[0]);
        int fd = open("file", O_RDWR);
        printf("fd = %d\n", fd);
        write(fd, "nihao", 5);
        
        sendFd(sfd[1], fd);

        wait(NULL);
    }
    else{
        close(sfd[1]);

        int nfd = 0;

        recvFd(sfd[0], &nfd);
        printf("nfd = %d\n", nfd);

        char buf[64] = {0};
        read(nfd, buf, sizeof(buf));
        printf("buf = %s\n", buf);
    }
    return 0;
}

