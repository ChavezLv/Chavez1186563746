#include <func.h>
void checkSignal(sigset_t*);
void sigfunc(int signum){
    printf("%d is comming!\n",signum);
    printf("beg\n");
    sleep(5);
    printf("end\n");
    sigset_t status;
    checkSignal(&status);
    sigset_t mask;
    sigemptyset(&mask);
    sigprocmask(SIG_SETMASK,&mask,NULL);
}
void checkSignal(sigset_t *pstatus){
    sigpending(pstatus);
    if(sigismember(pstatus,SIGINT)){
        printf("SIGINT is blocked\n");
    }
    else{
        printf("SIGINT is not blocked\n");
    }
    if(sigismember(pstatus,SIGQUIT)){
        printf("SIGQUIT is blocked\n");
    }
    else{
        printf("SIGQUIT is not blocked\n");
    }
}
int main()
{
    struct sigaction act1;
    memset(&act1,0,sizeof(act1));
    act1.sa_handler = sigfunc;
    act1.sa_flags = SA_RESTART;
    sigaction(SIGINT,&act1,NULL);//注册sigint
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGQUIT);
    sigprocmask(SIG_BLOCK,&mask,NULL);//给sigquit全程屏蔽
    char buf[128] = {0};
    int ret = read(STDIN_FILENO, buf, sizeof(buf));
    printf("ret = %d, buf = %s\n", ret, buf);
    return 0;
}

