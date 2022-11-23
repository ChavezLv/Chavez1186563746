#include "../include/head.h"
#include "../include/orders.h"

int recvFile(int clientFd, char * fileMd5, int totalSize){
    int ret = 0;
    DataTrain_t train;
    memset(&train, 0, sizeof(train));
    
    int fd = open(fileMd5, O_CREAT|O_RDWR, 0666);                
    ERROR_CHECK(fd, -1, "open");

    //count recv
    int downloadSize = 0;
    float downloadRate;
    int writeSize = 0;
    int splice = totalSize/100;
    int splicePrint = 0;

    time_t timeLast = 0,timeNow = 0;
    time(&timeLast);
    while(1){
        memset(&train.buf, 0, sizeof(train.buf));
        //先接收长度
        ret = recv(clientFd, &train.len, 4, 0);
        //接收完毕返回
        if(0 == train.len){
            printf("train.len = 0\n");
            break;
        }
        //服务端的子进程关闭
        if(0 == ret){
            /* close(clientFd); */
            printf("ret over: %d\n",train.len);
            return 0;
        }
        //接收的数目
        ret = recvDataTrain(clientFd, train.buf,train.len);
        downloadSize += ret;
        //写入文件的数目
        writeSize += write(fd, train.buf, ret);
        if(downloadSize >= splicePrint * splice){
            downloadRate = (float)downloadSize/totalSize*100;
            printf("---ret : %d, writeSize: %d , downloadRate:%4.2f%%---  \r",train.len, writeSize, downloadRate);
            fflush(stdout);
            if(splice < 100){
                ++splicePrint;
            }
        }
        if(downloadSize == totalSize){
            printf("\n");
            time(&timeNow);
            printf("download finished, waste time(%ld s)\n",timeNow - timeLast);
            return 0;
        }
    }
    return 0;
}

