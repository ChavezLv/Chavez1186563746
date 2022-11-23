#include"../include/work_Queue.h"
#include"../include/orders.h"

int work_Add(pQueue_t pQue, pWork_t pNew)
{
    if(pQue->pHead == NULL){
        pQue->pHead = pNew;
        pQue->pTail = pNew;
    }
    else{
        pQue->pTail->pNext = pNew;
        pQue->pTail = pNew;
    }
    ++pQue->workNum;
    return 0;
}

int work_Get(pQueue_t pQue, pWork_t *pNew)
{
    if(pQue->pHead == NULL){
puts("$system>>no work");
        return -1;
    }
    *pNew = pQue->pHead;
    pQue->pHead = pQue->pHead->pNext;
    --pQue->workNum; 
    return 0;
}

int threadPool_Init(pPool_t pPool, int threadNum, pQueue_t pQue)
{
    pPool->threadNum = threadNum;
    pPool->thid = (pthread_t*)calloc(threadNum, sizeof(pthread_t));
    for(int i = 0; i < threadNum; ++i){
        pthread_create(&pPool->thid[i], NULL, pthread_Work, pPool);
    }
    memset(pQue, 0, sizeof(Queue_t));
    pPool->Queue = pQue;
    pthread_mutex_init(&pPool->mutex, NULL);
    pthread_cond_init(&pPool->cond, NULL);
    return 0;
}
void* pthread_Work(void *pPool)
{
    pPool_t p = (pPool_t)pPool;
    int ret;
    while(1){
        pWork_t pNewWork = NULL;
        pthread_mutex_lock(&p->mutex);
        if(p->Queue->workNum == 0){
            pthread_cond_wait(&p->cond, &p->mutex);
        }
        ret = work_Get(p->Queue, &pNewWork);
        pthread_mutex_unlock(&p->mutex);
        if(ret == 0){
            ret = read_Order(pNewWork->workArgs, pNewWork->clientFd, pNewWork->Fd);
            if(ret == -1){
printf("$system>>read fail\n");
            }
            free(pNewWork);
        }
        if(p->flag == 1){
            break;
        }
    }
    pthread_exit(NULL);
}

int tcp_Init(char *ip, char *port){
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "$system>>socket");
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

    int ret = 0;//绑定本机的IP和端口，绑定到sfd上
    ret = bind(sfd, (struct sockaddr*)&serAddr, sizeof(serAddr));

    ret = listen(sfd, 10);//监听的最大连接数是10
    ERROR_CHECK(ret, -1, "$system>>listen");

    return sfd;
}

int epoll_Add(int epfd, int sfd)
{
    struct epoll_event events;
    events.data.fd = sfd;
    events.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, sfd, &events);
    ERROR_CHECK(ret, -1, "$system>>epoll_Add");
    return 0;
}
