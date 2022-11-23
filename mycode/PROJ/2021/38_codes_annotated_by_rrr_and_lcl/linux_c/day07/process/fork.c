#include <func.h>
int main()
{
    pid_t pid = fork();
    if(pid == 0){
        printf("I am child process, pid = %d, ppid = %d\n",getpid(), getppid());
    }
    else{
        printf("I am parent process, pid = %d, ppid = %d, childpid = %d\n",
               getpid(),getppid(),pid);
    }
    sleep(1);
    return 0;
}

