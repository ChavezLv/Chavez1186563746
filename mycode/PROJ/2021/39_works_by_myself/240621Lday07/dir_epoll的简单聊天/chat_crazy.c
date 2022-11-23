#include<func.h>

int main(int argc,char *argv[]){
    ARGS_CHECK(argc,3);
    int fdw = open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");
    int fdr = open(argv[2],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    fd_set fdset;
    char buf[1024] = {0};
    while(1){
        FD_ZERO(&fdset);
        FD_SET(fdr,&fdset);
        FD_SET(STDIN_FILENO,&fdset);
        select(fdr+1,&fdset,NULL,NULL,NULL);
        if(FD_ISSET(fdr,&fdset)){
            puts("i read");
            memset(buf,0,sizeof(buf));
            int ret = read(fdr,buf,sizeof(buf));
            ERROR_CHECK(ret,0,"read");
            puts(buf);
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
