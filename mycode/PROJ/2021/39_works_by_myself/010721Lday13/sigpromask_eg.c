#include <func.h>

void sigFunc(int sigNum)
{
    printf("sig %d is comming\n", sigNum);

}

void newFunc(int sigNum, siginfo_t * pInfo, void* p)
{
    printf("new  sig %d is comming\n", sigNum);
    sleep(5);
    printf("after sig\n");

}

int main(int argc, char **argv)
{
    int ret = 0;

    struct sigaction act;
    memset(&act, 0, sizeof(act));

    sigset_t set;
    sigemptyset(&set);

    //把3号信号放到阻塞集合当中
    sigaddset(&set, 2);

    //使用新类型
    act.sa_sigaction = newFunc;
    act.sa_flags = SA_SIGINFO;

    ret = sigaction(2, &act, NULL);
    ERROR_CHECK(ret, -1, "sigaction");

    //设置阻塞
    sigprocmask(SIG_BLOCK, &set, NULL);

    printf("before sleep\n");
    sleep(5);
    printf("after sleep\n");

    //解除阻塞
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    while(1);
    return 0;

}


