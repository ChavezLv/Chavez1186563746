#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);

    int sfd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd, -1, "socket");
    
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);

    int ret = bind(sfd, (struct sockaddr*)& serAddr, sizeof(struct sockaddr));
    ERROR_CHECK(ret, -1 ,"bind");
    
    
    struct sockaddr_in clientAdrr;
    memset(&clientAdrr, 0 , sizeof(clientAdrr));
    socklen_t clientAdrrLen = sizeof(clientAdrr);
    /* socklen_t len = sizeof(clientAdrr); */

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

    event.events = sfd;
    event.data.fd = sfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    //监听读事件
    /* event.events = EPOLLIN; */
    /* event.data.fd = newFd; */
    /* ret = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event); */
    /* ERROR_CHECK(ret, -1, "epoll_ctl"); */

    char buf[10] = { 0 };

    int readyNum = 0;
    while(1){
        readyNum = epoll_wait(epfd, events, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == STDIN_FILENO){
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                sendto(sfd, buf,strlen(buf) - 1, 0,(struct sockaddr*)&clientAdrr, clientAdrrLen);
            }
            if(events[i].data.fd == sfd){
                memset(buf, 0, sizeof(buf));
                recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr*)&clientAdrr, &clientAdrrLen);
                printf("buf = %s\n",buf);
            }
        }
    }
    /* ret = recvfrom(sfd, buf, sizeof(buf), 0,(struct sockaddr*) &clientAdrr, &clientAdrrLen); */
    /* ERROR_CHECK(ret, -1, "recvfrom"); */
    /* printf("buf = %s\n",buf); */

    /* ret = sendto(sfd, "helloclient", 12, 0, (const struct sockaddr*)&clientAdrr, clientAdrrLen); */
    close(sfd);
    /* close(newFd); */
    return 0;
}
