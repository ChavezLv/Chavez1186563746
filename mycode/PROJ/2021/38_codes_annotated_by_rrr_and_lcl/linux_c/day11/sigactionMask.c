#include <func.h>
void sigfunc(int signum){
    printf("%d is comming!\n",signum);
    printf("beg\n");
    sleep(5);
    printf("end\n");
}
int main()
{
    struct sigaction act1;
    memset(&act1,0,sizeof(act1));
    act1.sa_handler = sigfunc;
    act1.sa_flags = SA_RESTART;
    sigaddset(&act1.sa_mask, SIGQUIT);
    sigaction(SIGINT,&act1,NULL);
    struct sigaction act2;
    memset(&act2,0,sizeof(act2));
    act2.sa_handler = sigfunc;
    act2.sa_flags = SA_RESTART|SA_NODEFER;//设置no_derfer即可立即处理来的信号，连相同信号也不挂起
    sigaction(SIGQUIT,&act2,NULL);
    char buf[128] = {0};
    int ret = read(STDIN_FILENO, buf, sizeof(buf));
    printf("ret = %d, buf = %s\n", ret, buf);
    return 0;
}

