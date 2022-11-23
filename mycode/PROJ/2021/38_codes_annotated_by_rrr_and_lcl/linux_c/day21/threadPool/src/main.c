#include "../include/head.h"
#include "../include/threadPool.h"

int main(int argc, char** argv)
{
    int threadNum = atoi(argv[3]);
    
    threadPool_t pool;
    memset(&pool, 0, sizeof(pool));

    //1.初始化线程池
    threadPoolInit(&pool, threadNum);

    //2.启动线程池
    threadPoolStart(&pool);

    //3.创建tcp监听套接字
    int sfd = 0;
    tcpInit(argv[1], argv[2], &sfd);

    //4.创建epoll
    int epfd = epoll_create(1);

    epollAddFd(sfd, epfd);
    struct epoll_event evs[2];

    int newFd = 0;
    int readyNum = 0;
    while(1){
        readyNum = epoll_wait(epfd, evs, 2, -1);
        for(int i = 0; i < readyNum; ++i){
            if(evs[i].data.fd == sfd){
                newFd = accept(sfd, NULL, NULL);
                //创建任务节点，newFd交给节点
                pNode_t pNew = (pNode_t)calloc(1, sizeof(Node_t));
                pthread_mutex_lock(&pool.Que.mutex);
                pNew->clientFd = newFd;
                //3期：本epoll池子不负责接入新线程以外的任何功能

                //4期：本epoll池子（主）-->(4.1)监听所有new_fd_main 
                //接过结点-->+分析--->短命令（在这里当场处理）

                //--->长命令（新开tcp），把新tcp给↓ -->子线程接过新tcp执行长任务

                //把节点插入到任务队列里面
                queInsert(&pool.Que, pNew);
                //任务队列里面有数据了，唤醒等待的子线程
                pthread_cond_signal(&pool.Que.cond);
                pthread_mutex_unlock(&pool.Que.mutex);
            }
        }
    }




    return 0;
}

