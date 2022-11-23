            /* if(events[i].data.fd == sfd){ */
            /*     for(int j = 0; j < cnt; ++j){ */
            /*         if(newFd[j].flag == 1){ */
            /*             continue; */
            /*         }else{ */
            /*             newFd[j].fd = accept(sfd, NULL, NULL); */
            /*             ERROR_CHECK(newFd[j].fd, -1, "accept"); */
            /*             newFd[j].flag = 1; */
            /*             break; */
            /*         } */
            /*     } */
            /* } */

#include <func.h>
typedef  struct Fd_s{
    int fd;
    int flag;
}Fd_t,*pFd_t;
typedef struct Buf_s{
    char buf[4096];
    char name[20];
    char time[128];
}Buf_t,*pBuf_t;
void epollInit(){
    
}

int epollAdd(int epfd,int fd){
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return 0;
}
int epollDel(int epfd, int fd){
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return 0;
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
    //创建epolll,参1>0
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");

    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    
    event.events = EPOLLIN;
    event.data.fd = sfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    int readyNum;
    /* char buf[1024] = {0}; */
    Buf_t buf ;
    char tmpBuf[5000] = {0};
    Fd_t newFd[10] = {0};
    int cnt = 0;
    struct epoll_event events[12] = {0};
    while(1){ 
        memset(events, 0, sizeof(events));
        readyNum = epoll_wait(epfd, events,  10, -1);
        ERROR_CHECK(readyNum, -1, "epoll_wait");
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == sfd){
                newFd[cnt].fd = accept(sfd, NULL, NULL);
                ERROR_CHECK(newFd[cnt].fd, -1, "accept");
                newFd[cnt].flag = 1;//
                
                epollAdd(epfd,newFd[cnt].fd);
                ++cnt;
            }
            for(int j = 0; j < cnt ; ++j){
                if(events[i].data.fd == newFd[j].fd){
                    memset(&buf, 0,sizeof(buf));
                    bzero(tmpBuf, sizeof(tmpBuf));
                    ret = recv(newFd[j].fd, tmpBuf, strlen(tmpBuf) - 1, 0);
                    /* sscanf(tmpBuf, "%s%s%s", buf.time, buf.name, buf.buf); */
                    if(0 == ret){
                        epollDel(epfd, newFd[j].fd);//要在close之前
                        close(newFd[j].fd);
                        /* newFd[cnt].flag = 0; */
                        newFd[j].fd = newFd[cnt-1].fd;
                        --cnt;
                        printf("bye bye!\n");
                        continue;
                    }
                    /* printf("recv:%s \n",buf); */
                    for(int k = 0; k < cnt; ++k){
                        if(k != j){
                            send(newFd[k].fd, tmpBuf, strlen(tmpBuf) - 1, 0);
                        }
                    }
                }
            }
        }
    }
    close(sfd);
    for(int j = 0; j < cnt; ++j){
        close(newFd[j].fd);
    }
    close(epfd);
    return 0;
}
