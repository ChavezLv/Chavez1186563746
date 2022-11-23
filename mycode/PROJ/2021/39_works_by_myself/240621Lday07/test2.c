#include <func.h>

int main(int argc,char*argv[]){
    ARGS_CHECK(argc,2); 
    int fwr = open(argv[1],O_RDONLY);
    fd_set fdset;
    char buf[4096] = {0};
    int cnt = 0;
    while(1){
        FD_ZERO(&fdset);
        FD_SET(fwr,&fdset);
        if(FD_ISSET(fwr,&fdset)){
            memset(buf,0,sizeof(buf));
            int ret = read(fwr,buf,1024);
            sleep(1);
            printf("cntr:%d,retr = %d.\n",++cnt,ret);
        }
    }
    return 0;
}
