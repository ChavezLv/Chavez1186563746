#include <func.h>

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
    
    /* struct sockaddr_in clientAdrr; */
    /* memset(&clientAdrr, 0 , sizeof(clientAdrr)); */
    /* socklen_t clientAdrrLen = sizeof(clientAdrr); */
    int newFd = accept(sfd, NULL,NULL);
    /* int newFd = accept(sfd, (struct sockaddr*)& clientAdrr,&clientAdrrLen); */
    ERROR_CHECK(newFd, -1, "accept");

    
    //创建epolll,参1>0
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    struct epoll_event event, events[2];
    memset(&event, 0, sizeof(event));
    memset(&events, 0, sizeof(event));
    //监听写事件
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    event.events = EPOLLIN;
    event.data.fd = newFd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, newFd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    
    event.events = EPOLLIN;
    event.data.fd = sfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int readyNum;
    char buf[1024] = {0};
    while(1){ 
        memset(events, 0, sizeof(event));
        readyNum = epoll_wait(epfd, events, 3, -1);
        ERROR_CHECK(readyNum, -1, "epoll_wait");
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == STDIN_FILENO){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(newFd, buf, strlen(buf) - 1, 0);
            }
            if(events[i].data.fd == newFd){
                memset(buf, 0,sizeof(buf));
                ret = recv(newFd, buf, strlen(buf) - 1, 0);
                if(0 == ret){
                    close(newFd);
                    printf("bye bye!\n");
                    continue;
                }
                printf("recv:%s",buf);
                fflush(stdout);
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
