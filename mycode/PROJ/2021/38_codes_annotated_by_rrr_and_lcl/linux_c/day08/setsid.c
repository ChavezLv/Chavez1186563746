#include <func.h>
int main()
{
    if(fork() == 0){
        printf("before child, pid = %d,pgid = %d,sid = %d\n",
               getpid(),getpgid(0),getsid(0));
        setsid();
        printf("after child, pid = %d,pgid = %d,sid = %d\n",
               getpid(),getpgid(0),getsid(0));
    }
    else{
        printf("parent, pid = %d,pgid = %d,sid = %d\n",
               getpid(),getpgid(0),getsid(0));
        wait(NULL);
    }
    return 0;
}

