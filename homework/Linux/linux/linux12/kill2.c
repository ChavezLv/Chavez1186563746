#include<sys/types.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
int main(int argc,char *argv[]){
    pid_t pid;
    int signo;
    sscanf(argv[2],"%d",&pid);
    sscanf(argv[1],"%d",&signo);
    int ret=kill(pid,signo);
    return 0;
}
