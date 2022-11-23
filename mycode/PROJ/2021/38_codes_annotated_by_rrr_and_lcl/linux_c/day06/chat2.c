#include <func.h>
int main(int argc, char *argv[])
{
    // ./chat2 1.pipe 2.pipe
    ARGS_CHECK(argc,3);
    int fdw = open(argv[1],O_WRONLY);
    int fdr = open(argv[2],O_RDONLY);
    puts("chat2");
    char buf[128] = {0};
    while(1){
        read(STDIN_FILENO,buf,sizeof(buf));
        write(fdw,buf,strlen(buf));
        memset(buf,0,sizeof(buf));
        read(fdr,buf,sizeof(buf));
        puts(buf);
    }
    close(fdw);
    close(fdr);
    return 0;
}

