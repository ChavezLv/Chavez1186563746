#include"../include/work_Queue.h"
#include"../include/pthread_Pool.h"
int init_Pool_Queue(pPool_t pPool, pQueue_t Queue, int threadNum)
{
    memset(Queue, 0, sizeof(Queue_t));
    pthread_mutex_init(&Queue->mutex, NULL);
    pthread_cond_init(&Queue->cond, NULL);

    pPool->threadNum = threadNum;
    pPool->Queue = Queue;
    pPool->thid = (pthread_t*)calloc(pPool->threadNum, sizeof(pthread_t));
    for(int i = 0; i < pPool->threadNum; ++i){
        pthread_create(&pPool->thid[i], NULL, pthread_Func, pPool->Queue);
    }
    return 0;
}
int tcp_init(char *ip, char *port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");
    int opt = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    
    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(sfd, (struct sockaddr *)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");
    
    listen(sfd, N);
    
    return sfd;
}
int recycle(pPool_t pPool)
{
    printf("before lock\n");
    pthread_mutex_lock(&pPool->Queue->mutex);
    printf("after lock\n");
    pPool->Queue->flag = 1;
    pthread_cond_broadcast(&pPool->Queue->cond);
    pthread_mutex_unlock(&pPool->Queue->mutex);
    for(int i = 0; i < pPool->threadNum; ++i){
        printf("begin recycle thread %ld\n", pPool->thid[i]);  
        pthread_join(pPool->thid[i], NULL);
        printf("recycle thread %ld over\n", pPool->thid[i]);  
    }
    pthread_mutex_destroy(&pPool->Queue->mutex);
    pthread_cond_destroy(&pPool->Queue->cond);
    printf("recycle pthread pool success!\n*******\n");
    return 0;
}
