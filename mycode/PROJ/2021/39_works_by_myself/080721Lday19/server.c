#include <func.h>

void serSockUnblock(int sfd){
    int status = 0;
    status = fcntl(sfd, F_GETFL);
    status |= O_NONBLOCK;
    fcntl(sfd, F_SETFL, status);
}

void getSockoptBuf(int fd){
    int rcvbuf = 0; socklen_t len = sizeof(rcvbuf);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len);
    printf("rcvbuf:%d\n", rcvbuf);
}
void setSockoptBuf(int fd){
    int rcvbuf = 4096;
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, (socklen_t)sizeof(rcvbuf));
}

void setSockoptRcveLowat(int fd){
    int lowat = 10;
    setsockopt(fd, SOL_SOCKET, SO_RCVLOWAT, &lowat, (socklen_t)sizeof(lowat)); 
}

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);
    int ret = 0;
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd, -1, "socket");
    
    //设置地址可以重用
    int reuse = 1;
    ret = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);

    ret = bind(sfd, (struct sockaddr*)& serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1 ,"bind");
    
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");
    
    int newFd = accept(sfd, NULL,NULL);
    ERROR_CHECK(newFd, -1, "accept");

    serSockUnblock(newFd);
    getSockoptBuf(newFd);
    setSockoptBuf(newFd);
    getSockoptBuf(newFd);

    setSockoptRcveLowat(newFd);
    //create epoll epfd
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    
    struct epoll_event event, events[2];
    memset(&event, 0, sizeof(event));
    memset(&events, 0, sizeof(event));
    //monitor event :  EPOLLIN    EPOLL_CTL_ADD
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;// | EPOLLET;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    //egde trigger  : EPOLLET
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = newFd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newFd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    
    event.events = EPOLLIN;
    event.data.fd = sfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int readyNum;
    //buffer 5 bytes  for testing ET mode
    char buf[3] = {0};
    while(1){ 
        memset(events, 0, sizeof(event));
        // 1000 ms  = 1 s
        readyNum = epoll_wait(epfd, events, 3, -1);
        ERROR_CHECK(readyNum, -1, "epoll_wait");
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == STDIN_FILENO){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf) - 1);
                send(newFd, buf, strlen(buf), 0);
            }
            if(events[i].data.fd == newFd){
                printf("recv:");
                while(1){
                    memset(buf, 0,sizeof(buf));
                    /* ret = recv(newFd, buf, sizeof(buf) - 1, MSG_DONTWAIT); */
                    ret = recv(newFd, buf, sizeof(buf) - 1, 0);
                    if(0 == ret){
                        close(newFd);
                        printf("bye bye!\n");
                        continue;
                    }
                    if(-1 == ret){
                        break;
                    }
                    printf("%s ",buf);
                }
                printf("\n");
            }
            if(events[i].data.fd == sfd){
                newFd = accept(sfd, NULL, NULL);
                /* printf("2\n"); */
                ERROR_CHECK(newFd, -1, "accept");
                event.events = EPOLLIN;
                event.data.fd = newFd;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newFd, &event);
                ERROR_CHECK(ret, -1, "epoll_ctl");
            }
        }
    }
    close(sfd);
    close(newFd);
    close(epfd);
    return 0;
}
