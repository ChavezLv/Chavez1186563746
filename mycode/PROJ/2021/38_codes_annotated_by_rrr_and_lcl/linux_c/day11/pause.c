#include <func.h>
void sigfunc(int signum){
    printf("%d is comming!\n", signum);
}
int main()
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler = sigfunc;
    sigaction(SIGINT,&act,NULL);
    sigset_t mask;
    sigset_t oldmask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,&oldmask);//屏蔽2号信号，保存原来的屏蔽情况到oldmask里面
    for(int i = 0; i < 5; ++i){
        sleep(1);
        printf("i = %d\n", i);
    }
#if 0
    sigprocmask(SIG_SETMASK,&oldmask,NULL);
    printf("before pause!\n");
    pause();
    printf("after pause!\n");
#else
    printf("before suspend!\n");
    sigsuspend(&oldmask);
    printf("after suspend!\n");
#endif
    return 0;
}

