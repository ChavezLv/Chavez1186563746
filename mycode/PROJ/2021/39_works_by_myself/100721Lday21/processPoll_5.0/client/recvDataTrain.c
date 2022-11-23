#include "../include/func.h"

int recvDataTrain(int sfd, char *trainBuf,int dataTrainLen){
    int ret = 0;
    int recvSize = 0;
    while(recvSize < dataTrainLen){
        /* recvSize += recv(sfd, (char *)trainBuf + recvSize, dataTrainLen - recvSize, 0); */
        ret = recv(sfd, (char *)trainBuf + recvSize, dataTrainLen - recvSize, 0);
        ERROR_CHECK(ret, -1, "recv");
        recvSize += ret;
        //对端断开
        if(0 == ret){
            break;
        }
    }
    //recv函数也是返回读取的字符数
    return recvSize;
}

int mmapDataTrain(int sfd, char *pTransFile,int dataTrainLen){
    int ret = 0;
    int recvSize = 0;
    while(recvSize < dataTrainLen){
        /* recvSize += recv(sfd, (char *)trainBuf + recvSize, dataTrainLen - recvSize, 0); */
        ret = recv(sfd, (char *)pTransFile + recvSize, dataTrainLen - recvSize, 0);
        ERROR_CHECK(ret, -1, "recv");
        recvSize += ret;
        //对端断开
        if(0 == ret){
            break;
        }
    }
    //recv函数也是返回读取的字符数
    return recvSize;
}
