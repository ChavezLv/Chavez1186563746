#include<func.h>
void sigFunc(int sigNum){
        printf("interrupt is coming!\n");
    }
int main(int argc,char** argv){
    int ret=0;
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler=sigFunc;
    sigemptyset(&act.sa_mask);//清空屏蔽字
    act.sa_flags=0;
    act.sa_flags|=SA_RESTART;
    
    ret=sigaction(SIGINT,&act,NULL);
    char buf[1024]={0};
    read(STDIN_FILENO,buf,sizeof(buf));
    if(errno==EINTR){
        perror("read STDIN_FILRNO");

    }
    puts(buf);

    return 0;
}
