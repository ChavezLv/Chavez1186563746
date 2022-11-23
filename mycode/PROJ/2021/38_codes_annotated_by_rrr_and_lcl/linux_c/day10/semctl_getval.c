#include <func.h>
#define N 1000000
int main()
{
    int semid = semget(2000,1,IPC_CREAT|0600);
    ERROR_CHECK(semid,-1,"semget");
    int ret = semctl(semid,0,SETVAL,1);
    ERROR_CHECK(ret,-1,"semctl SETVAL");
    int val = semctl(semid,0,GETVAL);
    ERROR_CHECK(val,-1,"semctl GETVAL");
    printf("sem val = %d\n", val);
    struct sembuf P;
    P.sem_num = 0;
    P.sem_op = -1;
    P.sem_flg = SEM_UNDO;
    semop(semid,&P,1);
    val = semctl(semid,0,GETVAL);
    ERROR_CHECK(val,-1,"semctl GETVAL");
    printf("sem val = %d\n", val);
}

