#include "../include/func.h"
#define N 4096
void sigFunc1(int sigNum){
    sleep(3);
    PRINT_CHECK(sigNum);
}
int transFile(int clientFd){
    /* PRINT_CHECK(clientFd); */
    signal(SIGPIPE, sigFunc1);
    /* if(3 >= clientFd){ */
    /*     return 0; */
    /* } */
    int ret = 0;
    int fd = open("file", O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    
    //获取文件大小
    struct stat statBuf;
    fstat(fd, &statBuf);
    printf("size:%ld\n", statBuf.st_size);
    
    //申请小火车数据结构
    DataTrain_t  train;
    memset(&train, 0, sizeof(train));
    
    //发送文件名
    train.len = sizeof("file") - 1;// 5-1
    strcpy(train.buf, "file");
    ret = send(clientFd, &train, train.len + 4, 0);
    ERROR_CHECK(ret, -1, "send");

    //发送文件大小
    train.len = 4;
    *(int*)train.buf = statBuf.st_size;
    ret = send(clientFd, &train, train.len + 4, 0);
    ERROR_CHECK(ret, -1, "send");

    /* char buf[4096] = { 0 }; */
    int  cntSend = 0;
    
    //使用mmap来传输
    /* char *pTransFile = (char* )mmap(NULL, statBuf.st_size, PROT_READ, MAP_SHARED, fd, 0); */
    /* munmap(pTransFile,statBuf.st_size); */
    
    int fds[2] = { 0 };
    pipe(fds);
    
    while(cntSend < statBuf.st_size){
        /* ret = splice(fd, 0, fds[1], 0, 65536, 0); */
        /* ERROR_CHECK(ret, -1, "splice"); */
        ret = splice(fd, NULL, fds[1], NULL, 65536, 0);
        ERROR_CHECK(ret, -1, "splice");

        ret = splice(fds[0], NULL, clientFd, NULL, ret, 0);
        ERROR_CHECK(ret, -1, "splice");

        cntSend += ret;
    }

    return 0;
}

