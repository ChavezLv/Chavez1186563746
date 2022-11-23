#ifndef __trans_H__
#define __trans_H__
#include"orders.h"
//以封装形式一次发送长度和内容
typedef struct Datatain_s{
    int len;
    char buf[1024];
}Datatain_t,*pDataTrain_t;//注意别名的定义

int order_gets(pReOrder_t pReoder,pWork_t pWork);
int newFd_isWhose(pWork_t pWork);
int sendfile(pWork_t pWork);
int recvfile(pWork_t pWork);
int recvDataTrain(int sfd,char *trainBuf,int dataTrainLen);


#endif
