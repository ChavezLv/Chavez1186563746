#include<func.h>
#include"process_pool.h"
void makechild(int processNum,pProcess_data_t p){
    for(int i=0;i<processNum;++i){
        int pipefds[2];
        //创建套接字,分配pipefds的值
        socketpair(AF_LOCAL,SOCK_STREAM,0,pipefds);
        pid_t ret=fork();
        if(ret==0){
            close(pipefds[1]);
            //child handle a task
            childHandleTask(pipefds[0]);
        }
        close(pipefds[0]);
        p[i].pid=ret;
        p[i].pipefd=pipefds[1];
        p[i].busy=CHILD_PROCESS_FREE;
    }
}
void childHandleTask(int pipefds){
    printf("child is runnig!");
    while(1){
        int peerfd=0;
        //创建一个peerfd描述符,从套接字pipefds中获取文件对象并且绑定
        recvFd(pipefds,&peerfd);
        transferFile(peerfd);
        //printf("send file\n");
        //send(peerfd,"hello",5,0);
        
        //模拟文件发送,注意,unix域套接字相当于全双工管道,可读可写;
        write(pipefds,"finish",6);
        printf("close peerfd\n");
    }
}
