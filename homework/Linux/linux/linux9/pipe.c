#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
int main(){
    int pipe_fd[2];
    pipe(pipe_fd);
    if(fork()){
        close(pipe_fd[1]);
        char buf[1024]={0};
        read(pipe_fd[0],buf,1024);
        puts(buf);
        }
    else{
        close(pipe_fd[0]);
        write(pipe_fd[1],"from your child",15);
        exit(0);
    }
    return 0;
}
