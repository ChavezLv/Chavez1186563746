#include "../include/head.h"

#define MAXFD 3 
void Daemon(){
    if(fork() != 0){
        //父进程退出
        exit(0);
    }
    setsid();//创建新会话
    for(int i = 0; i < MAXFD; ++i){
        close(i);
    }
    chdir("/");//修改当前目录为根目录
    umask(0);//重设文件掩码
    printf("Daemon\n");
}

