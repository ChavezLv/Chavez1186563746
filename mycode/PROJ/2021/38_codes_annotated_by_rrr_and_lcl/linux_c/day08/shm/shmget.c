#include <func.h>
int main()
{
    int shmid = shmget(1000,4096,IPC_CREAT|IPC_EXCL|0600);
    //如果key1000没有对应共享内存，就创建一个，大小是4096，权限是600
    ERROR_CHECK(shmid,-1,"shmget");
    return 0;
}

