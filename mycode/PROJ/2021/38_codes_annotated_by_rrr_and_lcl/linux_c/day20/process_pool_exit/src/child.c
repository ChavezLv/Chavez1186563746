#include "process_pool.h"

int makeChild(int processNum, pProcess_data_t pData)
{
    int sfd[2];
    pid_t childPid = 0;
    for(int i = 0; i < processNum; ++i){
        socketpair(AF_LOCAL, SOCK_STREAM, 0, sfd);
        
        childPid = fork();
        if(0 == childPid){
            close(sfd[1]);//建立好子进程马上关闭sfd的[1]端
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
    char exitFlag = 0;
    int clienFd = 0;
    while(1){
        //接收父进程传来的newFd
        recvFd(pipefd, &clienFd, &exitFlag);
        if(1 == exitFlag){ 
        //每一次循环的开头看一下父进程是来发任务还是来通知退出的
        //上一次任务没执行完来不到这，合理的设计
            printf("child exit\n");
            exit(6);
        }
        /* send(clienFd, "helloclient", 11, 0); */
        transFile(clienFd);
        printf("trans success\n");
        //写管道，使管道就绪，让父进程把自己的状态置为非忙碌的
        write(pipefd, "a", 1);
    }
    return 0;
}
