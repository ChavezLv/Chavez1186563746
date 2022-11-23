#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
int mysystem(char* cmd);
int main(int argc,char *argv[]){
    mysystem(argv[1]);
    return 0;
}
int mysystem(char* cmd){
    pid_t pid=fork();
    if(pid==0){
        execlp(cmd,cmd,NULL);
        _exit(127);
    }
    int wstatus;
    waitpid(pid,&wstatus,0);
    if(WIFEXITED(wstatus)){
        return WEXITSTATUS(wstatus);
    }
    if(WIFSIGNALED(wstatus)){
        return -1;
    }
    return 0;
}
