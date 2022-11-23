#include "../include/head.h"
#include "encryption.h"

#define SALT_LEN 2
int downloadFunc(int sfd);
int judgeCommand(char *buf);
int requestRegister(int sfd);

int main(int argc,char*argv[]){
    ARGS_CHECK(argc, 3);

    int sfd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    /* unsigned long ul = 1; */
    /* ioctl(sfd, FIONBIO, &ul); */
    
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

    int readyNum = 0;

    char buf[1024] = { 0 };
    /* char tmpBuf[1024] = { 0 }; */

    DataTrain_t train;
    memset(&train, 0, sizeof(train));

    printf("register(1), login(2)\n");
    while(1){
        memset(events, 0, sizeof(events));
        readyNum =  epoll_wait(epfd, events, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(events[i].data.fd == STDIN_FILENO){
                memset(&buf, 0, sizeof(buf));
                memset(&train, 0, sizeof(train));
                /* memset(&tmpBuf, 0, sizeof(tmpBuf)); */
                read(STDIN_FILENO, train.buf, sizeof(train.buf)-1);
                
                /* time(&nowTime); */
                /* ctime(&nowTime); */
                /* printf("%s\n",ctime(&nowTime)); */
                /* printf("1\n"); */
                /* strcpy(buf.times, ctime(&nowTime)); */
                /* strcpy(buf.name, "Bob"); */
                /* sprintf(tmpBuf, "%s %s %s %s", buf.times, buf.name, ":", buf.bufs); */
                /* printf("%s\n",tmpBuf); */
                if(1 == judgeCommand(buf)){
                    int requestRegister(int sfd);
                }
            }
            if(events[i].data.fd == sfd){
                /* downloadFunc(sfd); */
            }
        }
    }
    close(sfd);
    close(epfd);
    return 0;
}

int judgeCommand(char *buf){
    if(strcmp(buf,"1")){
        printf("buf = 1\n");
        return 1;
    }else{
        return -1;
    }
}

int requestRegister(int sfd){
    DataTrain_t train;
    //开始注册
    train.len = strlen(train.buf)-1; 
    send(sfd, &train, 4 + train.len, 0);

    do{
        //输入用户名
        printf("please input user name:\n");
        memset(&train.buf, 0, sizeof(train.buf));
        read(STDIN_FILENO, train.buf, sizeof(train.buf)-1);
        train.len = strlen(train.buf)-1;
        send(sfd, &train, 4 + train.len, 0);
        //server确认用户名合法
        memset(&train.buf, 0, sizeof(train.buf));
        recv(sfd, &train.len, 4, 0);
        recv(sfd, train.buf, train.len, 0);
        printf("trainbuf = %s\n",train.buf);
        if(0 == strcmp(train.buf, "1")){
            break;
        }else{
            printf("user name already existed\n");
        }
    }while(1);

    // 接收salt
    memset(&train.buf, 0, sizeof(train.buf));
    recv(sfd, &train.len, 4, 0);
    recv(sfd, train.buf, train.len, 0);

    char salt[SALT_LEN];
    //最后生成的密文占13个字节
    char cipherText[13] = {0};
    char *password;
    //用户输入密码
    password = getpass("input password:");
    strcpy(salt, genRandomString(SALT_LEN));
    strcpy(cipherText, genCipherText(salt, password));

    //发送密文给服务器
    train.len = strlen(cipherText);
    strcpy(train.buf, cipherText);
    send(sfd, &train, 4 + train.len, 0);

    //注册成功
    memset(&train.buf, 0, sizeof(train.buf));
    recv(sfd, &train.len, 4, 0);
    recv(sfd, train.buf, train.len, 0);
    if(0 == strcmp(train.buf, "1")){
        printf("register success\n");
    }
    return 0;
}

int downloadFunc(int sfd){
    int ret = 0;
    DataTrain_t train;
    memset(&train, 0, sizeof(train));
    //recv file name
    recv(sfd, &train.len, 4, 0);
    /* printf("len: %d\n",train.len); */
    ret = recv(sfd, train.buf, train.len, 0);
    /* ret = recv(sfd, buffer, train.len, 0); */
    /* printf("recv's ret = %d\n", ret); */
    printf("file:%s\n", train.buf);
    int fd = open(train.buf, O_CREAT|O_RDWR, 0666);                
    ERROR_CHECK(fd, -1, "open");

    int totalSize = 0;
    recv(sfd, &train.len, 4, 0);
    recv(sfd, &totalSize, train.len, 0);
    printf("totalSize:%d \n", totalSize);
    /* sleep(3); */
    //count recv
    /* int cnt = 0; */
    int downloadSize = 0;
    float downloadRate;
    int writeSize = 0;
    int splice = totalSize/100;
    int splicePrint = 0;

    time_t timeLast = 0,timeNow = 0;
    time(&timeLast);
                /* memset(&train, 0, sizeof(train)); */
                //先接收长度
                ret = recv(sfd, &train.len, 4, 0);
                //接收完毕返回
                if(0 == train.len){
                    printf("train.len = 0\n");
                    /* break; */
                }
                //服务端的子进程关闭
                if(0 == ret){
                    /* close(sfd); */
                    printf("ret over: %d\n",train.len);
                    return 0;
                }
                //接收的数目
                ret = recvDataTrain(sfd, train.buf,train.len);
                downloadSize += ret;
                //写入文件的数目
                writeSize += write(fd, train.buf, ret);
                /* printf("writeSize = %d\n",writeSize); */
                /* time(&timeNow); */
                /* if(timeNow - timeLast > 1){ */
                if(downloadSize >= splicePrint * splice){
                    downloadRate = (float)downloadSize/totalSize*100;
                    printf("---ret : %d, writeSize: %d , downloadRate:%4.2f%%---  \r",train.len, writeSize, downloadRate);
                    fflush(stdout);
                    if(splice < 100){
                        ++splicePrint;
                    }
                    /* timeLast = timeNow; */
                }
                if(downloadSize == totalSize){
                    printf("\n");
                    time(&timeNow);
                    printf("download finished, waste time(%ld s)\n",timeNow - timeLast);
                    return 0;
                }
    return 0;
}
