#include <func.h>
int main()
{
    int shmid = shmget(1000,4096,IPC_CREAT|0600);
    char * p = (char *)shmat(shmid,NULL,0);
    shmctl(shmid,IPC_RMID,NULL);//这里做了标记
    shmdt(p);//这里断连了，才是彻底消失//试试没有第二句能不能成功
    return 0;
}

