#include <func.h>
void sigFunc(int sig){
    printf("sig is %d.\n",sig);
}
//注意后面两个新增的参数
void newSigFunc(int sig,siginfo_t *pInfo,void *p){
    printf("before new signal is %d.\n",sig);
    sleep(3);
    printf("after new signal is %d.\n",sig);
}
int main(int argc,char*argv[]){
    struct sigaction sigactS;
    memset(&sigactS,0,sizeof(sigactS));

    //使用旧的类型信号处理函数
    /* sigactS.sa_handler = sigFunc; */
    /* sigactS.sa_flags = SA_RESETHAND; */

    //使用新的类型信号处理函数
    sigactS.sa_sigaction = newSigFunc;
    sigactS.sa_flags = SA_SIGINFO; 
    int ret = sigaction(2,&sigactS,NULL);
    ERROR_CHECK(ret,-1,"sigaction");

    ret = sigaction(3,&sigactS,NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    while(1);
    return 0;
}

