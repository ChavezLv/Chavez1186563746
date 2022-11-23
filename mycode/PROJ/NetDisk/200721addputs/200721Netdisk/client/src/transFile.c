#include "../include/head.h"

int transFile(int fd, int sfd, int filesize){
    int ret = 0;
    
    DataTrain_t  train;
    memset(&train, 0, sizeof(train));

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
        ret = send(sfd, &train, train.len + 4, MSG_NOSIGNAL);
        ERROR_CHECK(ret, -1, "send");
        cntSend += ret - 4;
    }
    return 0;
}

