#include "../include/func.h"
#define N (4096)
int transFile(int clientFd){
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
    int cntRead = 0, cntSend = 0;
    
    //使用mmap来传输
    char *pTransFile = (char* )mmap(NULL, statBuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    
    while(1){
        /* memset(&train, 0, sizeof(train));//都已经指定,不需要memset */
        /* train.len = read(fd, train.buf, sizeof(train.buf)); */
        /* cntRead += train.len; */
        if(cntSend == statBuf.st_size) {
            //打印总的读取数目
            printf("读取数目cntRead = %d, cntSend = %d\n",cntRead, cntSend);
            break;
        }
        /* printf("train.len = %d\n",train.len); */
        train.len = N;
        if(cntSend > statBuf.st_size - N){
            //最后一次
            train.len = statBuf.st_size % N;
        }
        send(clientFd, &train.len, 4, 0);
        ret = send(clientFd, (char*)pTransFile + cntSend, train.len, 0);
        ERROR_CHECK(ret, -1, "send");
        cntSend += ret;
    }
    munmap(pTransFile,statBuf.st_size);
    return 0;
}

