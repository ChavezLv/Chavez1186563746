#include "process_pool.h"

void sigfunc(int signum)
{
    printf("sig is comming\n");
}

int transFile(int clienFd)
{

    signal(SIGPIPE, sigfunc);
    //使用私有协议发送数据，人为的规定发送数据的边界 --->可以有效防止【黏包问题】

    //1-第一张小火车是【文件名】
    Train_t train;
    memset(&train, 0, sizeof(train));
    int fd = open("file", O_RDWR);


    //存储文件名长度
    train.len = 4;//文件名长度是4
    strcpy(train.buf, "file"); //填写文件名到传输数据

    //发送文件名
    send(clienFd, &train, 4 + train.len, 0);//发送文件名本体

    struct stat fileInfo;//文件信息结构体
    bzero(&fileInfo, sizeof(fileInfo));

    //2-第二张小火车是【文件大小】
    //使用fstat函数获取文件描述符所对应文件的相关信息，并且存储在fileInfo结构体当中
    fstat(fd, &fileInfo); //获取fstst

    train.len = sizeof(fileInfo.st_size);//获取文件长度
    memcpy(train.buf, &fileInfo.st_size, train.len);

    //发送文件的总长度
    send(clienFd, &train, 4 + train.len, 0);
    printf("filesize = %ld\n", fileInfo.st_size);


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
