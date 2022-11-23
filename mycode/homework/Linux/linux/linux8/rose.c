#include<func.h>
int main(void){
    int fdw=open("pw.pipe",O_WRONLY);
    puts("pw.pipe open");
    int fdr=open("pr.pipe",O_RDONLY);
    puts("pr.pipe open");
    char buf[1024];
    while(1){
        memset(buf,0,sizeof(buf));

        int len=read(STDIN_FILENO,buf,sizeof(buf));
        write(fdw,buf,len);
        
        memset(buf,0,sizeof(buf));
        read(fdr,buf,sizeof(buf));
        printf("from jack:%s\n",buf);

    }
    return 0;
}
