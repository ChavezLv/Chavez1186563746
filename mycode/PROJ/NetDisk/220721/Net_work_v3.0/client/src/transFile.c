#include"../include/orders.h"
#include "../include/md5.h"

int putsFile(int sfd, pOrder_t pOrder){
    int ret = 0;
    char path[1000] = {0};
    char fileName[1000] = {0};
    memcpy(path, pOrder->orderTarget, ARGSLEN);
    //客户端打开文件, 如果打开失败, 说明命令错误, 目标文件不存在, 发送filesize = 0;
    int fd = open(path, O_RDONLY);
    if(-1 == fd){
        printf("path wrong: puts only accept absolute path, or this file do not exist\n ");
        return -1;
    }
    //如果puts的filesize存在,则开始发送
    //1) get filesize
    char *med = path;
	int len;
    for (len = strlen(path) - 1; path[len] != '/' && len > 0; len--) {}
    if(len > 0){
        path[len] = 0;
        med = &path[len + 1];
    }
    strcpy(fileName, med);
    struct stat fileStat;
    bzero(&fileStat, sizeof(fileStat));
    fstat(fd, &fileStat);
    //2) get file md5sum 
    char md5Ret[MD5_SIZE] = { 0 };
    ret = Compute_file_md5((const char*)path, md5Ret); 
    ERROR_CHECK(ret, -1, "Compute_file_md5");
    int fileSize = fileStat.st_size;

    //方案一:  把大小放入order.orderTarget中
    
    memset(pOrder->orderTarget, 0, ARGSLEN);
printf("%s %s %d %d\n", md5Ret, fileName, fileSize, fd);
    sprintf(pOrder->orderTarget, "%s %s %d %d", md5Ret, fileName, fileSize, fd);


    send(sfd, pOrder, sizeof(Order_t), 0);
    //接收一下,文件是否能秒传成功!
    return 0;
}


int sendfile(pWork_t pWork)
{
    int sfd = tcp_Init();
    int fd, u, *p;
    char isOK;
    pOrder_t pOrder = (pOrder_t)calloc(1, sizeof(Order_t));
    pOrder->orderNum = NEWFD;
    sscanf(pWork->workArgs, "%d %p %d", &u, &p, &fd);
    sprintf(pOrder->orderTarget, "%d %p", u, p);
    send(sfd, pOrder, sizeof(Order_t), 0);
    free(pOrder);
    recv(sfd, &isOK, 1, 0);
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
    close(fd);
    return 0;
}


int recvFile(int sfd, int fd, int totalSize);
int recvfile(pWork_t pWork)
{ 
    int sfd = tcp_Init();
    pOrder_t pOrder = (pOrder_t)calloc(1, sizeof(Order_t));
    pOrder->orderNum = NEWFD;
    memcpy(pOrder->orderTarget, pWork->workArgs, sizeof(pOrder->orderTarget));
puts(pOrder->orderTarget);
    send(sfd, pOrder, sizeof(Order_t), 0);
    free(pOrder);
    
    
    
    int ret;
    long totalLen;
    long nowLen = 0;
    long recvLen = 0;
    char buf[512] = {0};
    struct timeval begin, end;
    gettimeofday(&begin, NULL);

    recv(sfd, &totalLen, sizeof(totalLen), MSG_WAITALL);
printf("totalLen: %ld\n", totalLen);
    recv(sfd, &recvLen, sizeof(recvLen), MSG_WAITALL);
printf("totalLen: %ld\n", totalLen);
    recv(sfd, buf, recvLen, MSG_WAITALL);
printf("fileName: %s\n", buf);

    
    //小于100M
    if(totalLen >= 1048576000000000){
recv(sfd, &recvLen, sizeof(recvLen), MSG_WAITALL);
        int fd = open(buf, O_CREAT|O_RDWR, 0666);
        ftruncate(fd, totalLen);
        char *pFile = (char*)mmap(NULL, totalLen, PROT_WRITE, MAP_SHARED, fd, 0);
        int i = 1;

        while(nowLen < totalLen){
            ret = recv(sfd, pFile + nowLen, recvLen, MSG_WAITALL);
            ERROR_CHECK(ret, -1, "recv_data");
            nowLen += ret;
            ++i;
            if(i > 5000){
                printf("now receice : %5.2f%%\r", nowLen/(float)totalLen*100);
                fflush(stdout);
                i = 0;
            }
        }
        printf("now receice : %5.2f%%\r", nowLen/(float)totalLen*100);
        gettimeofday(&end, NULL);
        munmap(pFile, totalLen);
        close(fd);
        printf("\nreceive over!\n*******\n");
        printf("use %ld us\n", (end.tv_sec - begin.tv_sec) * 1000000 + end.tv_usec - begin.tv_usec);
        
    }else{
        int fd = open(buf, O_RDWR|O_APPEND);
        //CuteFTP  start
        long sendDownsize = 0;
        struct stat fileStat;
        memset(&fileStat, 0, sizeof(fileStat));
        fstat(fd, &fileStat);
        if(-1 == fd){
            //说明文件不存在
            sendDownsize = 0;
            fd = open(buf, O_CREAT|O_RDWR, 0666);//创建文件

        }else{
            sendDownsize = fileStat.st_size;
            printf("sendDownsize:%ld\n", sendDownsize);
        }
        send(sfd, &sendDownsize, sizeof(sendDownsize), 0);
        if(sendDownsize == totalLen){
            printf("already downloaded.\n");
            return 0;
        }
        //CuteFTPend
        /* lseek(fd, sendDownsize, SEEK_SET); */
        recvFile(sfd, fd, totalLen - sendDownsize);  

    }

    
    return 0;
}

int recvDataTrain(int sfd, char *trainBuf,int dataTrainLen);
int recvFile(int sfd, int fd, int totalSize){
    DataTrain_t train;
    memset(&train, 0, sizeof(train));
    int ret = 0;
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
        ret = recv(sfd, &train.len, 4, 0);
        //接收完毕返回
        if(0 == train.len){
            printf("train.len = 0\n");
            break;
        }
        //服务端的子进程关闭
        if(0 == ret){
            /* close(clientFd); */
            printf("ret over: %d\n",train.len);
            break;
        }
        //接收的数目
        ret = recvDataTrain(sfd, train.buf,train.len);
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
            break;
        }
    }
    return 0;
}

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
