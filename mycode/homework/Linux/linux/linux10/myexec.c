#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
int main(){
    pid_t pid=fork();
    if(pid==0)
    {
        execl("./random","random");
    }
    waitpid(pid,NULL,0);
}
