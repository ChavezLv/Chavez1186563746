#include <func.h>
int main()
{
    if(fork() == 0){
        setpgid(0,0);//父子进程不再属于同一个进程组
        printf("child pid = %d, pgid = %d\n", getpid(),getpgid(0));
        while(1);
        exit(0);
    }
    else{
        printf("parent pid = %d, pgid = %d\n", getpid(),getpgid(0));
        while(1);
        wait(NULL);
        exit(0);
    }
    return 0;
}

