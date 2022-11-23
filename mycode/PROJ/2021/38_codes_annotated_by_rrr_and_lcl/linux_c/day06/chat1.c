#include <func.h>
int main(int argc, char *argv[])
{
    // ./chat1 1.pipe 2.pipe
    ARGS_CHECK(argc,3);
    int fdr = open(argv[1],O_RDONLY);//注意
    int fdw = open(argv[2],O_WRONLY);
    puts("chat1");
    char buf[128] = {0};
    while(1){
        memset(buf,0,sizeof(buf));
        read(fdr,buf,sizeof(buf));
        puts(buf);
        read(STDIN_FILENO,buf,sizeof(buf));//对端关闭，对于它来说是就绪态
        //SLEEP:对面没有信息，读端不激活
        //ACTIVE:对端有信息，读端激活
        //对端关闭，持续激活
        write(fdw,buf,strlen(buf));
    }
    close(fdw);
    close(fdr);
    return 0;
}

