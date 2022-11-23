#include <func.h> 
void newFunc(int signum,siginfo_t *p,void* q){
    printf("before sig:%d.\n",signum); 
    sleep(1);
    printf("after sig:%d.\n",signum); 
}

int main(int argc,char*argv[]){
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_sigaction = newFunc;
    act.sa_flags = SA_SIGINFO;
    int ret = sigaction(SIGINT,&act,NULL);//sigaction检测信号,写好几个参数
    ERROR_CHECK(ret,-1,"sigaction");
    //错误写成 struct sigset_t,然后用memset
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);

    ret = sigprocmask(SIG_SETMASK,&set,NULL);//全局范围内检测信号并堵塞
    ERROR_CHECK(ret,-1,"sigprocmask");
    printf("before sleep.\n");
    sleep(3);
    printf("after sleep.\n");
    
    sigset_t sigPending;
    ret = sigpending(&sigPending);//检测悬挂的信号,传出参数set
    ERROR_CHECK(ret, -1, "sigpending");

    //判断悬挂的是否是该信号,是的话,返回1
    if(sigismember(&sigPending,SIGINT)){
        printf("sig is SIGINT\n");
    }
    
    sigprocmask(SIG_UNBLOCK,&set,NULL);
    while(1);
    return 0;
}

