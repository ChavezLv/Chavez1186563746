#include<func.h>

int main(int argc,char *argv[]){
    ARGS_CHECK(argc,3);
    int fdw = open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");
    int fdr = open(argv[2],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    fd_set fdset;
    char buf[1024] = {0};
    struct timeval timeout;
    time_t time_now;
    while(1){
        FD_ZERO(&fdset);
        FD_SET(fdr,&fdset);
        FD_SET(STDIN_FILENO,&fdset);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        select(fdr+1,&fdset,NULL,NULL,&timeout);
        if(FD_ISSET(fdr,&fdset)){
            memset(buf,0,sizeof(buf));
            int ret = read(fdr,buf,sizeof(buf));
            ERROR_CHECK(ret,0,"read");
            time(&time_now);
            printf("%s:%s.\n",ctime(&time_now),buf);
        }
        if(FD_ISSET(STDIN_FILENO,&fdset)){
            puts("stdin");
            memset(buf,0,sizeof(buf));
            int ret = read(STDIN_FILENO,buf,sizeof(buf));
            ERROR_CHECK(ret,0,"read");
            write(fdw,buf,ret);
            puts("send");
        }
    }
    return 0;
}
