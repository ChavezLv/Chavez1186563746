#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    //非标准的写法，用O_RDWR打开管道
    int fdr = open(argv[1],O_RDWR);
    int fdw = open(argv[1],O_RDWR);
    printf("pipe established!\n");
    char buf[4097] = {0};
    int cnt = 0;
    fd_set rdset;
    fd_set wrset;
    while(1){
        FD_ZERO(&rdset);
        FD_ZERO(&wrset);
        FD_SET(fdr,&rdset);
        FD_SET(fdw,&wrset);
        select(fdw+1,&rdset,&wrset,NULL,NULL);
        if(FD_ISSET(fdr,&rdset)){
            printf("cnt = %d, read\n",cnt);
            ++cnt;
            read(fdr,buf,2048);
            //sleep(1);
        }
        if(FD_ISSET(fdw,&wrset)){
            printf("cnt = %d, write\n",cnt);
            ++cnt;
            write(fdw,buf,4097);
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}

