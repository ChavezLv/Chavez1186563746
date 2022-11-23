#include <func.h>
int main(int argc, char *argv[])
{
    // ./chat2 1.pipe 2.pipe
    ARGS_CHECK(argc,3);
    int fdw = open(argv[1],O_WRONLY);
    int fdr = open(argv[2],O_RDONLY);
    puts("chat2");
    char buf[128] = {0};
    fd_set rdset;
    while(1){
        FD_ZERO(&rdset);
        FD_SET(fdr,&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        select(fdr+1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(fdr,&rdset)){
            printf("msg from pipe\n");
            memset(buf,0,sizeof(buf));
            int ret = read(fdr,buf,sizeof(buf));
            if(ret == 0){
                printf("chat is end\n");
                break;
            }
            puts(buf);
        }
        if(FD_ISSET(STDIN_FILENO,&rdset)){
            printf("msg from stdin\n");
            memset(buf,0,sizeof(buf));
            int ret = read(STDIN_FILENO,buf,sizeof(buf));
            if(ret == 0){
                puts("I quit");
                write(fdw,"nishigehaoren",13);
                break;
            }
            write(fdw,buf,strlen(buf));
        }
    }
    close(fdw);
    close(fdr);
    return 0;
}

