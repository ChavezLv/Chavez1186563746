#include <func.h>
int main()
{
    int ret = alarm(3);
    printf("ret = %d\n", ret);
    sleep(2);
    ret = alarm(2);
    printf("ret = %d\n", ret);
    pause();
    return 0;
}

