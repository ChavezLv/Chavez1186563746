#include <func.h>
void sigfunc(int signum){
    time_t now;
    time(&now);
    printf("%s\n",ctime(&now));
}
int main()
{
    signal(SIGPROF,sigfunc);
    struct itimerval timer;
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;
    setitimer(ITIMER_PROF,&timer,NULL);//CPU+内核+用户
    sigfunc(0);
    while(1);
    return 0;
}

