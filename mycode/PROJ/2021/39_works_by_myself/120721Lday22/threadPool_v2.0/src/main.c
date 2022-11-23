#include "../include/head.h"
#include "../include/work_que.h"
#include "../include/thread_pool.h"

int exitPipe[2] = { 0 };

void sigFun1(int sigNum){
    int ret = 0;
    PRINT_CHECK(sigNum);
    ret = write(exitPipe[1], &sigNum, sizeof(sigNum));
    RETURN_CHECK(ret, "write");
}

int main(int argc,char*argv[]){
    int ret = 0;
    if(5 != argc){
        printf("EXAMPLE: ./server ip port threadNum capacity\n");
    }
    ARGS_CHECK(argc, 5);
    
    Daemon();

    signal(SIGUSR1, sigFun1);
    ret = pipe(exitPipe);
    ERROR_CHECK(ret, -1, "pipe");
    if(fork()){
        close(exitPipe[0]); //父进程写管道
        
        wait(NULL);
        printf("thread pool exit\n");
        exit(0);
    }
    close(exitPipe[1]);//子进程要关闭写管道

    int threadNum = atoi(argv[3]);
    int capacity = atoi(argv[4]);
    
    /* pThreadPool_t pPool;// = NULL; */

    // 初始化线程池的结构体 
    ThreadPool_t pool;
    threadPoolInit(&pool, threadNum, capacity);

    //启动线程池
    threadPoolStart(&pool);

    //创建tcp监听套接字
    int sockFd = 0;
    tcpInit(argv[1], argv[2], &sockFd);

    //将要监听的描述符加入epoll中
    int epollFd = epoll_create(1);
    ERROR_CHECK(epollFd, -1, "epoll_create");

    epollAdd(epollFd, sockFd);
    epollAdd(epollFd, exitPipe[0]);

    struct epoll_event events[3];
    memset(events, 0, sizeof(events));
    /* bzero(epEvents, sizeof(struct epoll_event) * 3); */
    
    int newFd = 0;//client sockFd 
    int readyNum = 0;
    pNode_t pNew = NULL;
    while(1){
        readyNum = epoll_wait(epollFd, events, 3, -1);
        ERROR_CHECK(readyNum, -1, "epoll_wait");
        for(int i = 0; i < readyNum; ++i){
            //如果有客户端请求
            if(events[i].data.fd == sockFd){
                newFd = accept(sockFd, NULL, NULL);
                pNew = (pNode_t)calloc(1, sizeof(Node_t));
                pNew->clientFd = newFd;

                /*  man手册中说了:
                 *  尽管既可以在持有锁的情况下调用signal/broadcast，也可以在解锁的情况下调用，
                 *  但是如果需要调度行为是可预测的话，则应该在加锁的情况下调用signal/broadcast。
                 *
                 *  一些Pthreads的实现采用了一种叫做waitmorphing的优化措施，也就是当锁被持有时，
                 *  直接将线程从条件变量队列移动到互斥锁队列，而无需上下文切换。
                 *
                 *  附: 
                 *  如果使用的Pthreads实现没有waitmorphing，就可能会出现唤醒之后发生上下文切换,
                 *  线程醒过来之后想加锁加不了, 只能继续阻塞在锁上面, 尤其是线程池中,这种情况会比较糟糕
                 *  ,只有当上下文切换到主线程后, 主线程解锁了, 阻塞在锁上面的线程才得以继续走下去. 
                 *  所以这种情况, 我们可能需要在解锁之后在进行signal/broadcast。
                 *  此时解锁操作并不会导致上下文切换到T2，因为T2是在条件变量上阻塞的。
                 *  当T2被唤醒时，它发现锁已经解开了，从而可以对其加锁。*/
                pthread_mutex_lock(&pool.que.que_mutex);
                queInsert(&pool.que, pNew);
                pthread_cond_broadcast(&pool.cond);
                pthread_mutex_unlock(&pool.que.que_mutex);
                
                /* PRINT_CHECK((pool.que.pHead->clientFd)); */
            }
            if(events[i].data.fd == exitPipe[0]){
                pool.endFlag = '1';//结束
                PRINT_CHECK(pool.endFlag);
                pthread_cond_broadcast(&pool.cond);

                for(int j = 0; j < threadNum; ++j){
                    pthread_join(pool.ptheadID[j], NULL);
                }
                printf("all thread exit\n");

                exit(0);
            }
        }
    }
    
    return 0;
}

