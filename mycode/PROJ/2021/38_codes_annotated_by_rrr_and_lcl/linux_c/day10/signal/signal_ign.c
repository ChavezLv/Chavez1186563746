#include <func.h>
void sigFunc(int signum){
    printf("%d is coming!", signum);
}
int main()
{
    //signal(SIGINT,SIG_IGN);
    signal(SIGINT,sigFunc);
    while(1);
    return 0;
}

