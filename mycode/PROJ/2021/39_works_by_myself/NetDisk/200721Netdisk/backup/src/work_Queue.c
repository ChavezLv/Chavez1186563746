#include "../include/work_Queue.h"
int work_Add_send(pQueue_t sendQueue, int clientFd)
{
    pWork_t pNew = (pWork_t)calloc(1, sizeof(Work_t));
    pNew->clientFd = clientFd;
    if(sendQueue->pHead == NULL){
        sendQueue->pHead = pNew;
        sendQueue->pTail = sendQueue->pHead;
        ++sendQueue->workNum;
    }
    else{
        sendQueue->pTail->pNext = pNew;
        sendQueue->pTail = pNew;
    }
    return 0;
}
int work_Get_send(pQueue_t sendQueue, int *clientFd)
{
    if(sendQueue->pHead == NULL){
        return -1;
    }
    pWork_t pMed = sendQueue->pHead;
    *clientFd = sendQueue->pHead->clientFd;
    sendQueue->pHead = sendQueue->pHead->pNext;
    --sendQueue->workNum;
    free(pMed);
    pMed = NULL;
    return 0;
}
int send_File(int clientFd)
{
    int ret;
    long sendLen = 4;
    long remainLen;
    int fd = open("file", O_RDONLY, 0666);
    
    printf("\n*******\nbegin to send\n");
    struct stat file_stat;
    memset(&file_stat, 0, sizeof(file_stat));
    fstat(fd, &file_stat);
    remainLen = file_stat.st_size;
    send(clientFd, &file_stat.st_size, sizeof(file_stat.st_size), 0); 
    
    send(clientFd, &sendLen, sizeof(sendLen), 0);
    send(clientFd, "file", sendLen, 0);
    
    sendLen = 4096;
    send(clientFd, &sendLen, sizeof(sendLen), 0);
    
    int fds[2];
    pipe(fds);
    while(sendLen){
        splice(fd, NULL, fds[1], NULL, sendLen, 0);
        ret = splice(fds[0], NULL, clientFd, NULL, sendLen, 0);
        ERROR_CHECK(ret, -1, "splice");
        remainLen -= sendLen;
        if(sendLen > remainLen){
            sendLen = remainLen;
        }
    }
    close(fd);
    puts("send over\n*******\n");
    return 0;
}
