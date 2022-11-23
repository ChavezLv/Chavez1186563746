#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);

    int ret = 0;
    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t sfdLen = sizeof(addr);

    
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
    event.data.fd = sfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    char buf[4096] = {0};
    int readyNum = 0;
    while(1){
        memset(events, 0, sizeof(events));
        readyNum =  epoll_wait(epfd, events, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == STDIN_FILENO){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                ret = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr*)&sfd, sfdLen);
                ret = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr*)&sfd, sfdLen);
                printf("ret = %d\n",ret);
            }
            if(events[i].data.fd == sfd){
                memset(buf, 0,sizeof(buf));
                ret = recvfrom(sfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&sfd, &sfdLen);
                ret = sendto(sfd, buf, strlen(buf), 0, (struct sockaddr*)&sfd, sfdLen);
                if(0 == ret){
                    printf("bye bye!\n");
                    close(sfd);
                    close(epfd);
                    return 0;
                }
                printf("recv:%s\n",buf);
                /* fflush(stdout); */
            }
        }
    }
    close(sfd);
    close(epfd);
    return 0;
}
