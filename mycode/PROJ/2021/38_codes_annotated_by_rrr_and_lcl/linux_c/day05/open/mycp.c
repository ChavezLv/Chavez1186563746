#include <func.h>
int main(int argc, char *argv[])
{
    // ./mycp src dest
    ARGS_CHECK(argc,3);
    int fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open_fdr");
    int fdw = open(argv[2],O_WRONLY|O_CREAT|O_EXCL,0666);
    ERROR_CHECK(fdw,-1,"open_fdw");
    char buf[4096] = {0};
    while(1){
        memset(buf,0,sizeof(buf));
        int ret = read(fdr, buf, sizeof(buf));
        if(ret == 0){
            break;
        }
        ERROR_CHECK(ret,-1,"read");
        write(fdw,buf,ret);
    }
    close(fdr);
    close(fdw);
    return 0;
}

