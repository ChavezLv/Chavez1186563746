#include <func.h>

void sigFunc(int signum){
    time_t now;
    time(&now);
    printf("now time = %s .\n",ctime(&now));
} 
int main(int argc,char*argv[]){
    //捕捉真实计时器发送的信号
    __sighandler_t ret_t = signal(SIGALRM,sigFunc);
    ERROR_CHECK(ret_t,SIG_ERR,"signal");
    //申请计时器的传入参数,结构体保存了计时器的初试时间和间隔时间
    struct itimerval val;
    memset(&val,0,sizeof(val));
    //分别设置初始间隔时间和重复间隔时间
    val.it_value.tv_sec = 1;
    val.it_interval.tv_sec = 2;
    //打印当前时间
    time_t now;
    time(&now);
    printf("now time = %s .\n",ctime(&now));
    //设置真实计时器,到点发送信号,然后重置计时器
    
    setitimer(ITIMER_REAL,&val,NULL);

    //信号可以打断堵塞
    sleep(10);
    while(1);
    return 0;
}

