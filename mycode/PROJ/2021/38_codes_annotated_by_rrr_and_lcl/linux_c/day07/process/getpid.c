#include <func.h>
int main()
{
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("pid = %d, ppid = %d\n", pid, ppid);
    return 0;
}

