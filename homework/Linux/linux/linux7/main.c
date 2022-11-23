#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<stdio.h>
//#include<pwd.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>
#define LENGTH 4096
//#include<time.h>
//#include<grp.h>
int main(int argc,char *argv[]){
    close(1);
    int fdout=open(argv[1],O_WRONLY |O_CREAT|O_TRUNC,0666);
    printf("helloworld!");
    close(2);
    int fderr=open(argv[2],O_WRONLY |O_CREAT|O_TRUNC,0666); 
    dup()
    return 0;

}
