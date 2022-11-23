#include "../include/head.h"

int transFile(int clientFd){
    int ret = 0;
    int fd = open("file", O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    
    //获取文件大小
    struct stat statBuf;
    fstat(fd, &statBuf);
    printf("size:%ld\n", statBuf.st_size);
    
    DataTrain_t  train;
    memset(&train, 0, sizeof(train));
    
    train.len = sizeof("file") - 1;// 5-1
    
    strcpy(train.buf, "file");
    /* printf("buf:%s\n",train.buf); */
    ret = send(clientFd, &train, train.len + 4, 0);
    ERROR_CHECK(ret, -1, "send");
    
    train.len = 4;
    *(int*)train.buf = statBuf.st_size;
    ret = send(clientFd, &train, train.len + 4, 0);
    ERROR_CHECK(ret, -1, "send");

    /* char buf[4096] = { 0 }; */
    int cntRead = 0, cntSend = 0;
    while(1){
        /* memset(&train, 0, sizeof(train));//都已经指定,不需要memset */
        train.len = read(fd, train.buf, sizeof(train.buf));
        cntRead += train.len;
        if(0 == train.len) {
            //打印总的读取数目
            printf("读取数目cntRead = %d, cntSend = %d\n",cntRead, cntSend);
            break;
        }
        /* printf("train.len = %d\n",train.len); */
        ret = send(clientFd, &train, train.len + 4, 0);
        ERROR_CHECK(ret, -1, "send");
        cntSend += ret - 4;
    }
    return 0;
}

