#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
void sig_func(int signo){
    switch(signo){
        case SIGINT:
            printf("receive a signal __sigint\n");
            break;
        default:
            printf("receive unknow signal\n");
            break;
    }
}
int main(void){
    signal(SIGINT,sig_func);
    
    while(1)
        ;
    return 0;
}
