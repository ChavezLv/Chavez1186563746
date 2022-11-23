#include<func.h>
int main(void){
    int fdr=open("pw.pipe",O_RDONLY);
    puts("pw.pipe open");
    ERROR_CHECK(fdr,-1,"open pw.pipe");
    int fdw=open("pr.pipe",O_WRONLY);
    puts("pr.pipe open");
    char buf[1024];
    while(1){
        memset(buf,0,sizeof(buf));

        read(fdr,buf,sizeof(buf));
        printf("from rose:%s\n",buf);

        memset(buf,0,sizeof(buf));

        int len=read(STDIN_FILENO,buf,sizeof(buf));
        write(fdw,buf,len);
    }
    return 0;
}
