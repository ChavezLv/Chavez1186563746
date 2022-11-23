#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);
    int ret = 0;
    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);//也是主机字节序转换成为网络字节序
    /* char buf =  inet_ntoa((struct in_addr)serAddr.sin_addr.s_addr); */ 


    ret = bind(sfd, (struct sockaddr*)& serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1 ,"bind");
    
    struct sockaddr_in tmpClientAddr;
    memset(&tmpClientAddr, 0 , sizeof(tmpClientAddr));
    socklen_t tmpClientAddrLen = sizeof(tmpClientAddr);
    
    //创建epolll,参1>0
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    struct epoll_event event, events[2];
    memset(&event, 0, sizeof(event));
    memset(&events, 0, sizeof(event));
    //监听写事件
    /* event.events = EPOLLIN; */
    /* event.data.fd = STDIN_FILENO; */
    /* ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event); */
    /* ERROR_CHECK(ret, -1, "epoll_ctl"); */

    event.events = EPOLLIN;
    event.data.fd = sfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int readyNum;
    char buf[1024] = {0};

    while(1){ 
        memset(events, 0, sizeof(events));
        readyNum = epoll_wait(epfd, events, 2, -1);
        ERROR_CHECK(readyNum, -1, "epoll_wait");
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == sfd){
                memset(buf, 0, sizeof(buf));
                recvfrom(sfd, buf, strlen(buf) - 1, 0,(struct sockaddr*) &tmpClientAddr, &tmpClientAddrLen);
                printf("recv: %s\n", buf);
                printf("client connect, ip = %s, port = %d\n",inet_ntoa(tmpClientAddr.sin_addr),ntohs(tmpClientAddr.sin_port));
                /* memcmp() */ 
                /* sendto(sfd, buf, strlen(buf) - 1, 0, (const struct sockaddr*)&tmpClientAddr, tmpClientAddrLen); */
            }
            if(events[i].data.fd == STDIN_FILENO){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                sendto(sfd, buf, strlen(buf) - 1, 0, (const struct sockaddr*)&tmpClientAddr, tmpClientAddrLen);
            }
        }
    }
    close(sfd);
    close(epfd);
    return 0;
}
