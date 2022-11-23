#include <func.h>
int main()
{
    int shmid = shmget(1000,4096,IPC_CREAT|0600);
    //如果key1000没有对应共享内存，就创建一个，大小是4096，权限是600
    ERROR_CHECK(shmid,-1,"shmget");
    char *p = (char *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(void *)-1,"shmat");
    puts(p);
    return 0;
}

