#include "process_pool.h"

int makeChild(int processNum, pProcess_data_t pData)
{
    int sfd[2];
    pid_t childPid = 0;
    for(int i = 0; i < processNum; ++i){
        socketpair(AF_LOCAL, SOCK_STREAM, 0, sfd);
        
        childPid = fork();
        if(0 == childPid){
            close(sfd[1]);
            childFunc(sfd[0]);
            exit(0);
        }

        close(sfd[0]);
        pData[i].flag = 0;
        pData[i].pid = childPid;
        pData[i].pipefd = sfd[1];
    }
    return 0;
}

int childFunc(int pipefd)
{
    int clienFd = 0;
    while(1){
        //接收父进程传来的newFd
        recvFd(pipefd, &clienFd);
        send(clienFd, "helloclient", 11, 0);
        //写管道，使管道就绪，让父进程把自己的状态置为非忙碌的
        write(pipefd, "a", 1);
    }
    return 0;
}
