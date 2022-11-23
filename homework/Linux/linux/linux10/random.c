#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<pthread.h>
#include<time.h>
int main(){
    srand(time(NULL));
    return rand()%40+15;
}
