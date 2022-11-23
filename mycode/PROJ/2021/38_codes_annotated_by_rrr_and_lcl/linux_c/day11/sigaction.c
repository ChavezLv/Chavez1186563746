#include <func.h>
void sigfunc(int signum, siginfo_t *p, void *p1){
    printf("%d is comming!\n",signum);
    printf("beg\n");
    sleep(5);
    printf("end\n");
}
int main()
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_sigaction = sigfunc;
    act.sa_flags = SA_SIGINFO|SA_RESTART|SA_NODEFER|SA_RESETHAND;
    //只要第一次进入信号的回调过程，就会修改行为回到默认
    sigaction(SIGINT,&act,NULL);
    sigaction(SIGQUIT,&act,NULL);
    char buf[128] = {0};
    int ret = read(STDIN_FILENO,buf,sizeof(buf));
    printf("ret = %d, buf = %s\n",ret,buf);
    return 0;
}

