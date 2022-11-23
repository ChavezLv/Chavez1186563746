#include <func.h>
void sigFunc(int signum){
    printf("%d is coming!\n", signum);
    printf("sleep begin!\n");
    sleep(10);
    printf("sleep over!\n");
}
int main()
{
    signal(SIGINT,sigFunc);
    signal(SIGQUIT,sigFunc);
    while(1);
    return 0;
}

