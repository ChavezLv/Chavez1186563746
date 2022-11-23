#include <func.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,2);
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("pid = %d, ppid = %d\n", pid, ppid);
    printf("uid = %d, euid = %d\n", getuid(),geteuid());
    int fd = open(argv[1],O_WRONLY);
    ERROR_CHECK(fd,-1,"open");
    int ret = write(fd,"hello",5);
    ERROR_CHECK(ret,-1,"write");
    return 0;
}

