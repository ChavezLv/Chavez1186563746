#ifndef __TRANS_H__
#define __TRANS_H__

#include"../include/orders.h"
typedef struct DataTrain_s{
    int len;
    char buf[1024];
}DataTrain_t,*pDataTrian_t;



int order_gets(pReOrder_t pReOrder, pWork_t pWork);
int newFd_isWhose(pWork_t pWork);
int sendfile(pWork_t pWork);
int recvfile(pWork_t pWork);
int recvDataTrain(int sfd, char *trainBuf,int dataTrainLen);

#endif
