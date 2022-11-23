#include "process_pool.h"

int makeChild(int processNum, pProcess_data_t pData)
{
    int sfd[2]; //在建立子进程之前创建管道（就不会出现十几个管道）
    pid_t childPid = 0;
    for(int i = 0; i < processNum; ++i){
        socketpair(AF_LOCAL, SOCK_STREAM, 0, sfd);
        
        childPid = fork();
        if(0 == childPid){ //启动processNum个进程池的关键：
            close(sfd[1]);//子进程在fork之后【也有机会通过循环转回去再次fork】,必须//在if==子进程，截住子进程的行动，然后送入
            childFunc(sfd[0]);//接下来【子进程没有机会再进入fork了】，但父进程可以，从而达成正确的效果。
            exit(0);//那如何区分子进程们的行为？估计在里面+if，变更传入值是可以的-->或者在这里按照计数？也是可以的，毕竟i还是一样的
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
        recvFd(pipefd, &clienFd); //这里面就是每个进程单独的行为的 //不行可以展开也行
        /* send(clienFd, "helloclient", 11, 0); */
        transFile(clienFd);
        printf("trans success\n");
        //写管道，使管道就绪，让父进程把自己的状态置为非忙碌的
        write(pipefd, "a", 1);
    }
    return 0;
}
