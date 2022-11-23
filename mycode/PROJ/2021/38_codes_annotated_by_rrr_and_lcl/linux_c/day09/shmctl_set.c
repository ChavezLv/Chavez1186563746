#include <func.h>
int main()
{
    int shmid = shmget(1000,4096,IPC_CREAT|0600);
    char * p = (char *)shmat(shmid,NULL,0);
    struct shmid_ds stat;
    shmctl(shmid,IPC_STAT, &stat);
    stat.shm_perm.mode = 0666;
    shmctl(shmid,IPC_SET,&stat);
    shmdt(p);
    return 0;
}

