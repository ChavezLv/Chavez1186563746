#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include<signal.h>

void func(int signo) {

}
int main(){
    signal(SIGINT, func);

}
