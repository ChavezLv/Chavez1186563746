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
    int fdr=open(argv[1],O_RDONLY);

    int fdw=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fdw==-1){
        close(fdr);
        perror("open");
        exit(1);
    }
    struct stat statbuf;
    fstat(fdr,&statbuf);
    ftruncate(fdw,statbuf.st_size);
    off_t fsize=0;
    int cp_size;
    while(fsize<statbuf.st_size){
        if(statbuf.st_size-fsize>=LENGTH){
        cp_size=LENGTH;}
        else{
            cp_size=statbuf.st_size-fsize;
        }
        char* src=(char *)mmap(NULL,cp_size,PROT_READ,MAP_SHARED,fdr,fsize);
        char* dest=(char *)mmap(NULL,cp_size,PROT_READ|PROT_WRITE,MAP_SHARED,fdw,fsize);
        memcpy(dest,src,cp_size);
        munmap(src,cp_size);
        munmap(dest,cp_size);
        fsize+=cp_size;
        }
        return 0;

}
