#include <func.h>

typedef struct Buf_s{
    char bufs[4096];
    char name[20];
    char times[128];
}Buf_t,*pBuf_t;

typedef struct DataTrain_s{
    int len;
    char buf[4096];
    /* char buf[1000]; */
}DataTrain_t,*pDataTrian_t;

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
    
    //recv file
    /* char buffer[4096] = { 0 }; */
    DataTrain_t train;
    memset(&train, 0, sizeof(train));
    //recv file name
    recv(sfd, &train.len, 4, 0);
    printf("len: %d\n",train.len);
    ret = recv(sfd, train.buf, train.len, 0);
    /* ret = recv(sfd, buffer, train.len, 0); */
    printf("recv's ret = %d\n", ret);
    printf("file:%s\n", train.buf);
    int fd = open(train.buf, O_CREAT|O_RDWR, 0666);                
    ERROR_CHECK(fd, -1, "open");

    int totalSize = 0;
    recv(sfd, &train.len, 4, 0);
    recv(sfd, &totalSize, train.len, 0);
    printf("totalSize:%d \n", totalSize);
    /* sleep(3); */
    //count recv
    int cnt = 0;
    int downloadSize = 0;
    float downloadRate;
    int writeSize = 0;
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
                strcpy(buf.name, "Bob");
                sprintf(tmpBuf, "%s %s %s %s", buf.times, buf.name, ":", buf.bufs);
                /* printf("%s\n",tmpBuf); */
                send(sfd, tmpBuf, sizeof(tmpBuf)-1, 0);
            }
            if(events[i].data.fd == sfd){
                memset(&train, 0, sizeof(train));
                cnt = 0;
                //先接收长度
                ret = recv(sfd, &train.len, 4, 0);
                if(0 == ret){
                    /* close(sfd); */
                    printf("ret over: %d\n",train.len);
                    return 0;
                }
                do{
                    memset(&train.buf, 0, sizeof(train.buf));
                    ret = recv(sfd, train.buf, train.len - cnt, 0);
                    cnt += ret;
                    downloadSize += cnt;
                    /* write(fd, train.buf, strlen(train.buf)); */
                    writeSize += write(fd, train.buf, ret);
                    
                }while(cnt < train.len);
                /* printf("writeSize = %d\n",writeSize); */
                downloadRate = (float)downloadSize/totalSize*100;
                printf("---ret : %d, writeSize: %d , downloadRate:%4.2f%%---  \r",train.len, writeSize, downloadRate);
                fflush(stdout);
            }
        }
    }
    close(sfd);
    close(epfd);
    return 0;
}
