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
    memset(pQue, 0, sizeof(Queue_t));
    pPool->Queue = pQue;
    pthread_mutex_init(&pPool->mutex, NULL);
    pthread_cond_init(&pPool->cond, NULL);
    for(int i = 0; i < threadNum; ++i){
        printf("11\n");
        printf("%d\n",threadNum);
        int ret = pthread_create(&pPool->thid[i], NULL, pthread_Work, pPool);
        PTHREAD_ERROR_CHECK(ret, "create");
        printf("22\n");
    }
    return 0;
}
void* pthread_Work(void *pPool)
{
    pPool_t p = (pPool_t)pPool;
    int ret;
    while(1){
        printf("before lock\n");
        pWork_t pNewWork = NULL;
        pthread_mutex_lock(&p->mutex);
        if(p->Queue->workNum <= 0){
        printf("before wait\n");
            pthread_cond_wait(&p->cond, &p->mutex);
        printf("after wait\n");
        }
        ret = work_Get(p->Queue, &pNewWork);
        pthread_mutex_unlock(&p->mutex);
        if(ret == 0){
            ret = read_Order(pNewWork);
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

int tcp_Init(void){
    int fd = open("../proc/socket", O_RDONLY);
    char ip[16] = {0};
    char port[5] = {0};
    char buf[22] = {0};
    read(fd, buf, 22);
    sscanf(buf, "%s %s", ip, port);
    close(fd);
    
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "$system>>socket");
    int opt = 1; 
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    //保存本机的ip和端口
    struct sockaddr_in serAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_addr.s_addr = inet_addr(ip);
    serAddr.sin_port = htons(atoi(port));

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
