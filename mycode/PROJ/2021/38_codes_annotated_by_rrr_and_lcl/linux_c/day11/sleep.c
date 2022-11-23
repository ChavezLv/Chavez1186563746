#include <func.h>
void sigfunc(int signum){
    printf("%d is comming!\n", signum);
}
void mySleep(int i){
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler = sigfunc;
    sigaction(SIGALRM,&act,NULL);
#if 1
    sigset_t blkmask;
    sigset_t oldmask;
    sigemptyset(&blkmask);
    sigaddset(&blkmask,SIGALRM);
    sigprocmask(SIG_BLOCK,&blkmask,&oldmask);
    alarm(i);
    sigsuspend(&oldmask);    
#else
    alarm(i);
    pause();
#endif
}
int main()
{
    return 0;
}

