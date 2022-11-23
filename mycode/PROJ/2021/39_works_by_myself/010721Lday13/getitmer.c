#include <func.h>

void sigFunc(int sigNum)
{
    time_t now;
    time(&now);
    printf("now time = %s\n", ctime(&now));

}
int main(int argc, char **argv)
{
    signal(SIGALRM, sigFunc);
    struct itimerval val;
    memset(&val, 0, sizeof(val));

    val.it_value.tv_sec = 1;
    val.it_interval.tv_sec = 2;
    time_t now;
    time(&now);

    printf("now time = %s\n", ctime(&now));

    setitimer(ITIMER_REAL, &val, NULL);

    printf("before sleep\n");
    sleep(10);
    printf("after sleep\n");

    while(1);

    return 0;

}
