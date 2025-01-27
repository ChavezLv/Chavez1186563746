#include "../include/head.h"

int epollAddFd(int fd, int epfd)
{   
    struct epoll_event event;
    memset(&event, 0, sizeof(event));

    event.events = EPOLLIN;
    event.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);

    return 0;
}
