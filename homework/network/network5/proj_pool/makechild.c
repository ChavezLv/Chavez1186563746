#include"process_pool.h"
void makeChild(int processNum,pProcess_data_t p){
    //创建管道
    int pipefd[2];
    for(int i=0;i<processNum;i++){
        socketpair(AF_LOCAL,SOCK_STREAM,0,pipefd);//
        pid_t pid=fork();
    /*
    1、fork执行的时候，会有两个返回值，一个是父进程的返回值，一个是子进程的返回值。
    2、在父进程中fork的返回值是子进程的PID。
    3、在子进程中fork的返回值是0。
    4、fork失败，返回值为-1
    */
        if(pid==0){
            close (pipefd[1]);
            childHandleTask(pipefd[0]);
            exit(0);
        }
        close (pipefd[0]);
        p[i].busy=CP_FREE;
        p[i].pid=pid;
        p[i].pipefd=pipefd[1];
    }
}
void childHandleTask(int pipefd){
    printf("child is running\n");
    char exitFlag=0;//传出参数
    while(exitFlag){
        int peerfd=0;
        recvFd(pipefd,&peerfd,&exitFlag);
        if(!exitFlag){
            transferFile(peerfd);
            char buff[]="i have finish the task";
            write(pipefd,buff,strlen(buff));
            close(pipefd);
            printf("close peerfd\n");

        }
    }
}
