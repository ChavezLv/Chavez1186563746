#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,2); 
    int fdw = open(argv[1],O_WRONLY);
    fd_set fdset;
    char buf[4096] = {0};
    int cnt = 0;
    while(1){
        FD_ZERO(&fdset);
        FD_SET(fdw,&fdset);
        if(FD_ISSET(fdw,&fdset)){
            memset(buf,1,sizeof(buf));
            int ret = write(fdw,buf,4096);
            printf("cntw:%d,ret = %d.\n",++cnt,ret);
        }
    }
    return 0;
}
