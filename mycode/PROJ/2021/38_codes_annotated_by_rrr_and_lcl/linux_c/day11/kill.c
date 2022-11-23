#include <func.h>
int main(int argc, char *argv[])
{ // ./kill -9 pid
    ARGS_CHECK(argc,3);
    pid_t pid = atoi(argv[2]);
    int sig = atoi(argv[1]+1);// '-''9'
    int ret = kill(pid,sig);
    ERROR_CHECK(ret,-1,"kill");
    return 0;
}

