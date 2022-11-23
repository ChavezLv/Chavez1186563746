#include "../include/work_Queue.h"
int queueInit(pQueue_t pQue){
    pQue->workNum = 0;
    pQue->pHead = NULL;
    pQue->pTail = NULL;
    pthread_mutex_init(&pQue->mutex, NULL);
    return 0;
}

int queueInsert(pQueue_t pQue, pWork_t pNew){
    if(pQue->pTail == NULL){
        pQue->pHead = pNew;
        pQue->pTail = pNew;
    }
    else{
        pQue->pTail->pNext = pNew;
        pQue->pTail = pNew;
    }
    pQue->workNum++;
    return 0;
}

int threadPoolInit(pPool_t pPool, int threadNum)
{
    pPool->threadNum = threadNum;
    pPool->thid = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    queueInit(pPool->Queue);
    pthread_cond_init(&pPool->cond, NULL);
  
    return 0;
}

int tcpInit(char *ip, char *port){
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    //保存本机的ip和端口
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(ip);
    serAddr.sin_port = htons(atoi(port));

    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    int buf = 65536;
    setsockopt(sfd, SOL_SOCKET, SO_SNDBUF, &buf, sizeof(buf));
    
    int ret = connect(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "connect");
    return sfd;
}

int epoll_Add(int epfd, int sfd)
{
    struct epoll_event events;
    events.data.fd = sfd;
    events.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &events);
    ERROR_CHECK(ret, -1, "epoll_Add");
    return 0;
}
