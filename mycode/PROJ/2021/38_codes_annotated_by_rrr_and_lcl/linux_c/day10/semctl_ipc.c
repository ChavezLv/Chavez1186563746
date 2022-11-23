#include <func.h>
#define N 1000000
int main()
{
    int semid = semget(2000,2,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    struct semid_ds statbuf;
    semctl(semid,0,IPC_STAT, &statbuf);
    printf("perm = %o\n",statbuf.sem_perm.mode);
    printf("cuid = %d\n",statbuf.sem_perm.cuid);
    printf("nsems = %ld\n",statbuf.sem_nsems);
    statbuf.sem_perm.mode = 0666;
    semctl(semid,0,IPC_SET,&statbuf);//设置参数
    semctl(semid,0,IPC_RMID);//删除信号量
}

