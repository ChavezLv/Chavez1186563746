#include "../include/head.h"
#include "../include/orders.h"
#include"../include/trans.h"

int sendfile(pWork_t pWork){
    int clientFd = pWork->clientFd;
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char fileName[1500] = {0};
    char md5[1200] = {0};
    char path[1500] = {0};
    sscanf(pOrder->orderTarget, "%s %s", md5, fileName);
    /* sprintf(path, "/home/liu955//Netdisk/server/files/%s", md5); */
    sprintf(path, "/home/charlie/wangdao2021/NetDisk/210721NetDisk/new/server/files/%s", md5);
puts(path);
    

    int fd = open(path, O_RDONLY);
    int ret;
    long sendLen = strlen(fileName);
    long remainLen;
    
    printf("$system>>begin to send\n");
    struct stat file_stat;
    memset(&file_stat, 0, sizeof(file_stat));
    fstat(fd, &file_stat);
    remainLen = file_stat.st_size;
printf("remainLen: %ld\n",remainLen);
    send(clientFd, &file_stat.st_size, sizeof(file_stat.st_size), 0); 

printf("sendLen: %ld\n",sendLen);
    send(clientFd, &sendLen, sizeof(sendLen), 0);
    send(clientFd, fileName, sendLen, 0);
    

    if(file_stat.st_size >= 104857600000000){
sendLen = 4096;
send(clientFd, &sendLen, sizeof(sendLen), 0);
        int fds[2];
        pipe(fds);
        while(sendLen){
            splice(fd, NULL, fds[1], NULL, sendLen, 0);
            /* printf("1\n"); */
            ret = splice(fds[0], NULL, clientFd, NULL, sendLen, 0);
            ERROR_CHECK(ret, -1, "splice");
            remainLen -= sendLen;
            if(sendLen > remainLen){
                sendLen = remainLen;
            }
        }

    }else{
        //CuteFTP  start
        long offsetSize;//偏移大小
        recv(clientFd, &offsetSize, sizeof(offsetSize), 0);
        lseek(fd, offsetSize, SEEK_SET);
        //CuteFTP end
        if(offsetSize == file_stat.st_size){
            printf("already finished.\n");
            return 0;
        }
        //
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
            ret = send(clientFd, &train, train.len + 4, MSG_NOSIGNAL);
            ERROR_CHECK(ret, -1, "send");
            cntSend += ret - 4;

        }
        
    }

    close(fd);
    
    return 0;
}

int recvfile(pWork_t pWork){
    int clientFd = pWork->clientFd;
    pOrder_t pOrder = (pOrder_t)pWork->workArgs;
    char filename[1500] = {0};
    char fileMd5[1200] = {0};
    char path[1500] = {0};
    int filesize, userId, fileId, dirId;
    sscanf(pOrder->orderTarget, "%s %s %d %d %d" , fileMd5, filename, &filesize, &userId, &dirId);
    
    /* sprintf(path, "/home/liu955//Netdisk/server/files/%s", fileMd5); */
    sprintf(path, "/home/charlie/wangdao2021/NetDisk/210721NetDisk/new/server/files/%s", fileMd5);
    int ret = 0;
    DataTrain_t train;
    memset(&train, 0, sizeof(train));

    printf("%s\n", path);
    int fd = open(path, O_CREAT|O_RDWR, 0666);                
    ERROR_CHECK(fd, -1, "open");
    
    printf("%s\n", path);
    
    send(clientFd, "y", 1, 0);
    //count recv
    int downloadSize = 0;
    float downloadRate;
    int writeSize = 0;
    int splice = filesize/100;
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
            downloadRate = (float)downloadSize/filesize*100;
            printf("---ret : %d, writeSize: %d , downloadRate:%4.2f%%---  \r",train.len, writeSize, downloadRate);
            fflush(stdout);
            if(splice < 100){
                ++splicePrint;
            }
        }
        if(downloadSize == filesize){
            printf("\n");
            time(&timeNow);
            printf("download finished, waste time(%ld s)\n",timeNow - timeLast);
            break;
        }
    }

    char query[512] = { 0 };
    MYSQL* conn = mysqlConnect();
    bzero(query, sizeof(query));
    sprintf(query, "insert into FILE_INFO (filesize, MD5, referCnt) \
            values (%d, '%s', 1)", filesize, fileMd5);
    mysqlInsert(conn, query);

    bzero(query, sizeof(query));
    sprintf(query, "select fileID from FILE_INFO where MD5 = '%s'", fileMd5);
    fileId = mysqlSearchInt(conn, query);

    bzero(query, sizeof(query));
    sprintf(query, "insert into USER_FILE (dirID, userID, fileID, fileName) values (%d, %d, %d, '%s')", \
            dirId, userId, fileId, filename);
    mysqlInsert(conn, query);
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

