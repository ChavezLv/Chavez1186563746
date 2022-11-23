#include "../include/func.h"

int processPoolInit(pProcessPool_t *ppProcessPool, int processPoolNum){ 
    *ppProcessPool = (pProcessPool_t) calloc(processPoolNum, sizeof(ProcessPool_t));
    return 0;
}

int processPoolCreate(pProcessPool_t pProcessPool, int processPoolNum){
    int sockPairFd[2] = { 0 };
    pid_t pid = 0;
    for(int i = 0; i < processPoolNum; ++i){
        int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, sockPairFd);
        ERROR_CHECK(ret, -1, "socketpair");
        pid = fork();
        if(0 == pid){
            close(sockPairFd[1]);
            //子进程做的事
            processPoolFunc(sockPairFd[0]);
            exit(0);
        }else{
            close(sockPairFd[0]);
            pProcessPool[i].pid = pid;
            pProcessPool[i].pipeFd = sockPairFd[1];
            pProcessPool[i].flag = 0;//0 表示空闲
            /* wait(NULL); */
        }
    }
    return 0;
}

int processPoolFunc(int sockPairFd){
    int clientFd = 0;
    //如果没有这个循环while(1), 子进程会直接退出exit(0),关闭管道,就会导致内核一直给管道另外一端发0
    //main进程的epoll监控,会一直响应. 但是遍历的时候子进程已经关闭,对应的管道也关闭,所以下次有客户端
    //连接时, 因为在进程池数组里面对应的第一个进程 flag还是0, 以为是空闲的, 就传给这个已经挂掉的子进程
    //子进程的 recvmsg已经不在了!  所以新客户端立马就会关掉, 而且因为使用了error_check,就是sendFd里面也会return.  用的时上一天的客户端
    while(1){
        recvFd(sockPairFd, &clientFd);//msgrecv会一直阻塞,直到父进程分配任务给它
        send(clientFd, "hello client", 12, 0);
        write(sockPairFd, "okk", 3);//finished
        printf("[son] : i finished the engagement\n");
    }
    return 0;
}
