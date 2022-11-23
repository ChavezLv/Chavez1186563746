#include "../include/func.h"

int transFile(int clientFd){
    int ret = 0;
    int fd = open("file", O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    
    struct stat statBuf;
    fstat(fd, &statBuf);
    printf("size:%ld\n", statBuf.st_size);
    /* sleep(3); */
    DataTrain_t  train;
    memset(&train, 0, sizeof(train));
    
    train.len = sizeof("file") - 1;// 5-1
    
    strcpy(train.buf, "file");
    /* printf("buf:%s\n",train.buf); */
    ret = send(clientFd, &train, train.len + 4, 0);

    /* char buf[4096] = { 0 }; */
    int cnt = 0;
    while(1){
        /* memset(&train, 0, sizeof(train));//都已经指定,不需要memset */
        train.len = read(fd, train.buf, sizeof(train.buf));
        cnt += train.len;
        if(0 == train.len) {
            printf("cnt = %d\n",cnt);
            break;
        }
        /* printf("train.len = %d\n",train.len); */
        ret = send(clientFd, &train, train.len + 4, 0);
        /* printf("ret  = %d\n",ret); */
        /* sleep(3); */
        ERROR_CHECK(ret, -1, "send");
    }
    return 0;
}

