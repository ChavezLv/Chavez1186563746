#include<func.h>

int main(int argc,char* argv[]){
    ARGS_CHECK(argc,2);
    int fdw = open(argv[1],O_RDWR); 
    ERROR_CHECK(fdw,-1,"open");
    int fdr = open(argv[1],O_RDWR);
    ERROR_CHECK(fdr,-1,"open");
    char buf[4097] = {0};
    //memset(buf,1,sizeof(buf));
    fd_set fdr_set;
    fd_set fdw_set;
    int cntr = 0;
    int cntw = 0;
    while(1){
        FD_ZERO(&fdr_set);
        FD_ZERO(&fdw_set);
        FD_SET(fdr,&fdr_set);
        FD_SET(fdw,&fdw_set);
        select(fdr+1,&fdr_set,&fdw_set,NULL,NULL);
        if(FD_ISSET(fdw,&fdw_set)){
            puts("i can write");
            int ret = write(fdw,buf,4096);
            printf("cntw:%d,retw:%d\n",++cntw,ret);
        }
        if(FD_ISSET(fdr,&fdr_set)){
            puts("i can read");
            sleep(1);
            int ret = read(fdr,buf,1024);
            printf("cntr:%d,retr:%d\n",++cntr,ret);
        }
    } return 0;
}

