#include "../include/func.h"

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

    /* printf("before\n"); */
    int ret = connect(sfd, (struct sockaddr*)& addr, addrlen);
    /* printf("after\n"); */
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

    /* char tmpBuf[5000] = {0}; */
    int readyNum = 0;
    /* time_t  nowTime; */
    
    //recv file
    /* char buffer[4096] = { 0 }; */
    DataTrain_t train;
    memset(&train, 0, sizeof(train));
    //recv file name
    recv(sfd, &train.len, 4, 0);
    printf("len: %d\n",train.len);
    ret = recv(sfd, train.buf, train.len, 0);
    printf("recv's ret = %d\n", ret);
    printf("file:%s\n", train.buf);
    //接收完文件名打开文件
    int fd = open(train.buf, O_CREAT|O_RDWR, 0666);                
    ERROR_CHECK(fd, -1, "open");
    //接收文件长度
    int totalSize = 0;
    recv(sfd, &train.len, 4, 0);
    recv(sfd, &totalSize, train.len, 0);
    printf("totalSize:%d \n", totalSize);
    //ftruncate文件
    ret = ftruncate(fd, totalSize);

    /* sleep(3); */
    //count recv
    /* int cnt = 0; */
    int downloadSize = 0;
    float downloadRate;
    /* int writeSize = 0; */
    int splice = totalSize/100;
    int splicePrint = 0;
    time_t timeLast = 0,timeNow = 0;
    time(&timeLast);

    char *pTransFile = (char* )mmap(NULL, totalSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    while(1){
        memset(events, 0, sizeof(events));
        readyNum =  epoll_wait(epfd, events, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == sfd){
                /* memset(&train, 0, sizeof(train)); */
                //先接收长度
                ret = recv(sfd, &train.len, 4, 0);
                //接收完毕返回
                if(0 == train.len){
                    printf("train.len = 0\n");
                    break;
                }
                //服务端的子进程关闭
                if(0 == ret){
                    /* close(sfd); */
                    printf("ret over: %d\n",train.len);
                    return 0;
                }
                //接收的数目
                ret = mmapDataTrain(sfd, pTransFile, train.len);
                downloadSize += ret;
                if(downloadSize >= splicePrint * splice){
                    downloadRate = (float)downloadSize/totalSize*100;
                    printf("---ret : %d, downloadSize: %d , downloadRate:%4.2f%%---  \r",train.len, downloadSize, downloadRate);
                    fflush(stdout);
                    if(splice < 100){
                        ++splicePrint;
                    }
                }
                if(downloadSize == totalSize){
                    printf("\n");
                    time(&timeNow);
                    printf("download finished, waste time(%ld s)\n",timeNow - timeLast);
                    //解除mmap映射
                    munmap(pTransFile, totalSize);
                    return 0;
                }
            }
        }
    }
    close(sfd);
    close(epfd);
    return 0;
}