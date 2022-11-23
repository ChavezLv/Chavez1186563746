#include <func.h>

typedef struct Buf_s{
    char bufs[4096];
    char name[20];
    char times[128];
}Buf_t,*pBuf_t;

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);

    int sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
socklen_t addrlen = sizeof(addr);
    int ret = connect(sfd, (struct sockaddr*)& addr, addrlen);
    ERROR_CHECK(ret, -1, "connect");
    
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

    char tmpBuf[5000] = {0};
    Buf_t buf ;//= { 0 };
    int readyNum = 0;
    time_t  nowTime;
    while(1){
        memset(events, 0, sizeof(events));
        readyNum =  epoll_wait(epfd, events, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == STDIN_FILENO){
                memset(&buf, 0, sizeof(buf));
                memset(&tmpBuf, 0, sizeof(tmpBuf));
                read(STDIN_FILENO, buf.bufs, sizeof(buf.bufs) - 1);
                
                time(&nowTime);
                /* ctime(&nowTime); */
                /* printf("%s\n",ctime(&nowTime)); */
                /* printf("1\n"); */
                strcpy(buf.times, ctime(&nowTime));
                strcpy(buf.name, "Amy");
                sprintf(tmpBuf, "%s %s %s %s", buf.times, buf.name, ":", buf.bufs);
                /* printf("%s\n",tmpBuf); */
                send(sfd, tmpBuf, sizeof(tmpBuf)-1, 0);
            }
            if(events[i].data.fd == sfd){
                memset(&buf, 0,sizeof(buf));
                memset(tmpBuf, 0, sizeof(tmpBuf));
                ret = recv(sfd, tmpBuf, sizeof(tmpBuf) - 1, 0);
                if(0 == ret){
                    printf("bye bye!\n");
                    close(sfd);
                    close(epfd);
                    return 0;
                }
                char ch[2] = {0};
                printf("%s\n",tmpBuf);
                /* sscanf(tmpBuf, "%s%s%s%s", buf.times, buf.name, ch,buf.bufs); */
                /* printf("recv:%s/%s/%s\n",buf.times, buf.name, buf.bufs); */
                /* fflush(stdout); */
            }
        }
    }
    close(sfd);
    close(epfd);
    return 0;
}
