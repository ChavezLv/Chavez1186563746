#include"orders.h"
#include"trans.h"
int recv_Order(int clientFd,int *Fd,pPool_t pPoolOrder){
    pWork_t pNewWork=(pWork_t)calloc(1,sizeof(Work_t));
    int ret=recv(clientFd,pNewWork->workArgs,sizeof(Order_t),MSG_WAITALL);
    int recv_Order(int clientFd, int *Fd, pPool_t pPoolOrder){
    pWork_t pNewWork = (pWork_t)calloc(1, sizeof(Work_t));
    int ret = recv(clientFd, pNewWork->workArgs, sizeof(Order_t), MSG_WAITALL);//接收并判断命令
    if(ret == 0){//连接断开检查,接收完毕
        printf("$system>>clientFd %d, userID %d connect error!\n", clientFd, Fd[clientFd]);
        close(clientFd);
        Fd[clientFd] = -1;
        return -1;
    }
    
    pNewWork->clientFd=clientFd;
    pNewWork->Fd=Fd;
    pNewWork->pPool=pPoolOrder;

    
}}







