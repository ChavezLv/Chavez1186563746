#define _GNU_SOURCE 
#include "process_pool.h"

void sigfunc(int signum)
{
    printf("sig is comming\n");
}

int transFile(int clienFd)
{

    signal(SIGPIPE, sigfunc);
    //使用私有协议发送数据，人为的规定发送数据的边界
    Train_t train;
    memset(&train, 0, sizeof(train));
    int fd = open("file", O_RDWR);

    //存储文件名长度
    train.len = 4;
    strcpy(train.buf, "file");

    //发送文件名
    send(clienFd, &train, 4 + train.len, 0);

    struct stat fileInfo;
    bzero(&fileInfo, sizeof(fileInfo));

    //使用fstat函数获取文件描述符所对应文件的相关信息，并且存储在fileInfo结构体当中
    fstat(fd, &fileInfo);

    train.len = sizeof(fileInfo.st_size);
    memcpy(train.buf, &fileInfo.st_size, train.len);

    //发送文件的总长度
    send(clienFd, &train, 4 + train.len, 0);
    printf("filesize = %ld\n", fileInfo.st_size);


    //零拷贝接口1.mmap
    /* char * pMap = (char*)mmap(0, fileInfo.st_size, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0); */
    /* ERROR_CHECK(pMap, (char*)-1, "mmap"); */

    /* send(clienFd, pMap, fileInfo.st_size, 0); */

    /* munmap(pMap, fileInfo.st_size); */

    //零拷贝接口2 sendfile
    /* sendfile(clienFd, fd, 0, fileInfo.st_size); */
    
    //零拷贝接口3, splice
    /* int sfd[2]; */
    /* pipe(sfd); */
    
    /* int recvLen = 0; */
    /* while(recvLen < fileInfo.st_size){ */
    /*     int ret = splice(fd, 0, sfd[1], 0, fileInfo.st_size, 0); */
    /*     ret = splice(sfd[0], 0, clienFd, 0, ret, 0); */
    /*     recvLen += ret; */
    /* } */

    while(1){
        int ret = read(fd, train.buf, sizeof(train.buf));
        train.len = ret;

        //send以一个已经断开的连接，会返回-1， 在次send的时候，会收到SIGPIPE信号
        //终止子进程
        int ret1 = send(clienFd, &train, 4 + train.len, 0);
        if(0 == ret){
            break;
        }
        if(-1 == ret1){
            printf("client exit\n");
            break;
        }
    }

    return 0;
}
